#include "GosTsr_AnalysisData.h"
#include "GosTsr_Descriptor.h"

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"
#include "TsErrorCheck_ThreeLevel.h"

#include "tsparse_update.h"

static UPDATE_INFO_S	gstUpdateProgInfo;
static GOSTSR_U16		gu16UpdateCount = 0;
static GOSTSR_U8        staticTsRegisterIndex = 0;

static GOSTSR_S32 tsparse_update_setpid(UPDATE_INFO_S *pstProgInfo)
{
	TsErrorCheck_OneLevel_setPid(&pstProgInfo->stProgInfo);
	TsErrorCheck_TwoLevel_setPid(&pstProgInfo->stProgInfo);
	TsErrorCheck_ThreeLevel_setPid(&pstProgInfo->stProgInfo);
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_update_storeprog_bypat(GOSTSR_PSISI_PAT_S *patInfo)
{
	GOSTSR_S32	i = 0;
	GOSTSR_U32	index = 0;
	SEARCH_INFO_S *pstUpdateProgInfo = &gstUpdateProgInfo.stProgInfo;

	if((GOSTSR_NULL == patInfo))
	{
		return  GOSTSR_FAILURE;
	}
	if((patInfo->u8Version == gstUpdateProgInfo.u8PatVersion))
	{
		return  GOSTSR_FAILURE;
	}
	/*PAT 更新后，重新搜索所有节目的PID*/
	
	gstUpdateProgInfo.u8PatVersion = patInfo->u8Version;
	gstUpdateProgInfo.bUpdateFlag = GOSTSR_TRUE;

	for(i = 0; i < patInfo->u16NbElements; i++)
	{
		if((NIT_PID == patInfo->astElement[i].u16Pid) || (patInfo->astElement[i].u16Pid == 0))
		{
			continue;
		}
		pstUpdateProgInfo->stProgInfo[index++].PmtPid = patInfo->astElement[i].u16Pid;
	}
	pstUpdateProgInfo->u16NbProg = index;
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_update_storeprog_bypmt(GOSTSR_U16 Pmt_pid, GOSTSR_PSISI_PMT_S *pmtInfo)
{
	GOSTSR_S32	i = 0, j = 0;
	SEARCH_INFO_S *pstUpdateProgInfo = &gstUpdateProgInfo.stProgInfo;

	if((GOSTSR_NULL == pmtInfo) || (!gstUpdateProgInfo.bUpdateFlag))
	{
		return  GOSTSR_FAILURE;
	}

	for(i = 0; i < pstUpdateProgInfo->u16NbProg; i++)
	{
		/*PMT 的数据已经保存*/
		if((pstUpdateProgInfo->stProgInfo[i].PmtPid == Pmt_pid) && (pstUpdateProgInfo->stProgInfo[i].bUsed))
		{
			break;
		}
		else if((pstUpdateProgInfo->stProgInfo[i].PmtPid == Pmt_pid) && (!pstUpdateProgInfo->stProgInfo[i].bUsed))
		{
			pstUpdateProgInfo->stProgInfo[i].bUsed = GOSTSR_TRUE;
			pstUpdateProgInfo->stProgInfo[i].PcrPid = pmtInfo->u16PcrPid;
			pstUpdateProgInfo->stProgInfo[i].u8NbPes = pmtInfo->u16NbElements;
			for(j = 0; j < pmtInfo->u16NbElements; j++)
			{
				pstUpdateProgInfo->stProgInfo[i].PesPid[j] = pmtInfo->astElement[j].u16Pid;
			}
			gu16UpdateCount++;
			if(gu16UpdateCount == pstUpdateProgInfo->u16NbProg)
			{
				tsparse_update_setpid(&gstUpdateProgInfo);	/*将更新后的数据应用于123级错误*/
			
				gu16UpdateCount = 0;
				gstUpdateProgInfo.bUpdateFlag = GOSTSR_FALSE;
				memset(pstUpdateProgInfo, 0 ,sizeof(SEARCH_INFO_S));	/*数据清0*/
				//printf("###################--------->Update\n");
			}
			break;
		}
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_update_storeprog_bycat(GOSTSR_PSISI_CAT_S *catInfo)
{
	GOSTSR_S32	i = 0;
	GOSTSR_U8	index = 0;
	GOS_DESC_CA_S caDesInfo;
	SEARCH_INFO_S *pstUpdateProgInfo = &gstUpdateProgInfo.stProgInfo;

	if((GOSTSR_NULL == catInfo) || (!gstUpdateProgInfo.bUpdateFlag))
	{
		return	GOSTSR_FAILURE;
	}

	for(i = 0; i < catInfo->u16NbDescriptors; i++)
	{
		if (GOSTSR_SUCCESS == GosTsr_Descriptor_CA(&catInfo->astDescriptor[i], &caDesInfo))
		{
			pstUpdateProgInfo->stCaInfo[index++].CaPid = caDesInfo.u16CaPId;
		}
	}
	pstUpdateProgInfo->u16NbCa = index;
	
	return GOSTSR_SUCCESS;
}

static void tsparse_update_callback(TS_SECTION_INFO *tsSectionInfo)
{
    if (NULL == tsSectionInfo->sectionData)
    {
        return ;
    }

    switch (tsSectionInfo->tableID)
    {
        case PAT_TABLE_ID:
            {
                GOSTSR_PSISI_PAT_S patInfo;
                memset(&patInfo,0,sizeof(GOSTSR_PSISI_PAT_S));
                GosTsr_AnalysisData_PAT(tsSectionInfo, &patInfo);
                tsparse_update_storeprog_bypat(&patInfo);
            }
            break;

        case CAT_TABLE_ID:
            {
                GOSTSR_PSISI_CAT_S catInfo;
                memset(&catInfo,0,sizeof(GOSTSR_PSISI_CAT_S));
                GosTsr_AnalysisData_CAT(tsSectionInfo, &catInfo);
                tsparse_update_storeprog_bycat(&catInfo);
            }
            break;

        case PMT_TABLE_ID:
            {
                GOSTSR_PSISI_PMT_S pmtInfo;
                memset(&pmtInfo,0,sizeof(GOSTSR_PSISI_PMT_S));
                GosTsr_AnalysisData_PMT(tsSectionInfo, &pmtInfo);
                tsparse_update_storeprog_bypmt(tsSectionInfo->PID,&pmtInfo);
            }
            break;
        default:
            break;
    }

    return;
}

GOSTSR_S32 TsParse_Update_ProgSearch_Init()
{
	memset(&gstUpdateProgInfo,0,sizeof(UPDATE_INFO_S));
	
	gstUpdateProgInfo.bUpdateFlag = GOSTSR_FALSE;
	gstUpdateProgInfo.u8PatVersion = 0xff;
	
	gu16UpdateCount = 0;
    GosTsr_AnalysisData_RegisterCallBack(tsparse_update_callback,&staticTsRegisterIndex);

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Update_ProgSearch_DeInit()
{
	memset(&gstUpdateProgInfo,0,sizeof(UPDATE_INFO_S));
	
	gstUpdateProgInfo.bUpdateFlag = GOSTSR_FALSE;
	gstUpdateProgInfo.u8PatVersion = 0xff;
	
	gu16UpdateCount = 0;
    GosTsr_AnalysisData_UnRegisterCallBack(staticTsRegisterIndex);

	return GOSTSR_SUCCESS;
}

