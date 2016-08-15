#include "GosTsr_AnalysisData.h"
#include "GosTsr_Descriptor.h"

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"
#include "TsErrorCheck_ThreeLevel.h"

#include "tsparse_search.h"

static TS_PACKAGE_INFO      *SearchPackageHead[TABLE_NUMBER] = {GOSTSR_NULL};
static GOSTSR_PSISI_PAT_S   gstSearchPatInfo;
static SEARCH_INFO_S	    gstSearchProgInfo;


static GOSTSR_BOOL	bHadSearched = GOSTSR_FALSE;
static GOSTSR_U16	gu16SearchCount = 0;

GOSTSR_S32 TsParse_Search_TsErrorCheck_setPid(SEARCH_INFO_S *pstProgInfo)
{
    TsErrorCheck_OneLevel_setPid(pstProgInfo);
    TsErrorCheck_TwoLevel_setPid(pstProgInfo);
    TsErrorCheck_ThreeLevel_setPid(pstProgInfo);
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_getProgInfo(SEARCH_INFO_S *pstProgInfo)
{
    if((GOSTSR_NULL == pstProgInfo))
    {
        return  GOSTSR_FAILURE;
    }
    memcpy(pstProgInfo, &gstSearchProgInfo, sizeof(SEARCH_INFO_S));
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_StoreProg_byPat(GOSTSR_PSISI_PAT_S *patInfo)
{
    GOSTSR_S32	i = 0;
    GOSTSR_U32	index = 0;

    if((GOSTSR_NULL == patInfo))
    {
        return  GOSTSR_FAILURE;
    }

    for(i = 0; i < patInfo->u16NbElements; i++)
    {
        if((NIT_PID == patInfo->astElement[i].u16Pid) || (patInfo->astElement[i].u16Pid == 0))
        {
            continue;
        }
        gstSearchProgInfo.stProgInfo[index++].PmtPid = patInfo->astElement[i].u16Pid;
    }
    gstSearchProgInfo.u16NbProg = index;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_StoreProg_byPmt(GOSTSR_U16 Pmt_pid, GOSTSR_PSISI_PMT_S *pmtInfo)
{
    GOSTSR_S32	i = 0, j = 0;

    if((GOSTSR_NULL == pmtInfo))
    {
        return  GOSTSR_FAILURE;
    }

    for(i = 0; i < gstSearchProgInfo.u16NbProg; i++)
    {
        if((gstSearchProgInfo.stProgInfo[i].PmtPid == Pmt_pid) && (gstSearchProgInfo.stProgInfo[i].bUsed))
        {
            break;
        }
        else if((gstSearchProgInfo.stProgInfo[i].PmtPid == Pmt_pid) && (!gstSearchProgInfo.stProgInfo[i].bUsed))
        {
            gstSearchProgInfo.stProgInfo[i].bUsed = GOSTSR_TRUE;
            gstSearchProgInfo.stProgInfo[i].PcrPid = pmtInfo->u16PcrPid;
            gstSearchProgInfo.stProgInfo[i].u8NbPes = pmtInfo->u16NbElements;
            for(j = 0; j < pmtInfo->u16NbElements; j++)
            {
                gstSearchProgInfo.stProgInfo[i].PesPid[j] = pmtInfo->astElement[j].u16Pid;
            }

            gu16SearchCount++;
            if(gu16SearchCount == gstSearchProgInfo.u16NbProg)
            {
                printf("Search Prog num = %d\n",gu16SearchCount);

                TsParse_Search_TsErrorCheck_setPid(&gstSearchProgInfo);
                bHadSearched = GOSTSR_TRUE;	/*已经全部搜索完节目*/
            }
            break;
        }
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_StoreProg_byCat(GOSTSR_PSISI_CAT_S *catInfo)
{
    GOSTSR_S32	i = 0;
    GOSTSR_U8	index = 0;
    GOS_DESC_CA_S caDesInfo;

    if((GOSTSR_NULL == catInfo))
    {
        return  GOSTSR_FAILURE;
    }

    for(i = 0;i < catInfo->u16NbDescriptors; i++)
    {
        if (GOSTSR_SUCCESS == GosTsr_Descriptor_CA(&catInfo->astDescriptor[i], &caDesInfo))
        {
            gstSearchProgInfo.stCaInfo[index++].CaPid = caDesInfo.u16CaPId;
        }
    }
    gstSearchProgInfo.u16NbCa = index;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_ParseSection(GOSTSR_U8 tableID,TS_SECTION_INFO *curSectionInfo)
{
    if((GOSTSR_NULL == curSectionInfo))
    {
        return  GOSTSR_FAILURE;
    }
    switch(tableID)
    {
        case PAT_TABLE_ID:
            {
                //printf("PAT_________Analysisi\n");
                GOSTSR_PSISI_PAT_S patInfo;
                GosTsr_AnalysisData_PAT(curSectionInfo, &patInfo);
                memcpy(&gstSearchPatInfo, &patInfo, sizeof(GOSTSR_PSISI_PAT_S));
                TsParse_Search_StoreProg_byPat(&patInfo);
            }
            break;
        case PMT_TABLE_ID:
            {
                //printf("PMT: %#x_________Analysisi\n",curSectionInfo->PID);
                GOSTSR_PSISI_PMT_S pmtInfo;
                GosTsr_AnalysisData_PMT(curSectionInfo, &pmtInfo);
                TsParse_Search_StoreProg_byPmt(curSectionInfo->PID,&pmtInfo);
            }
            break;
        case CAT_TABLE_ID:
            {
                GOSTSR_PSISI_CAT_S catInfo;
                GosTsr_AnalysisData_CAT(curSectionInfo, &catInfo);
                TsParse_Search_StoreProg_byCat(&catInfo);
            }
            break;
        default:
            break;
    }
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_LinkPackage(GOSTSR_U8 tableID, TS_PACKAGE_INFO *PackageHead_Temp, GOSTSR_U8 lastDataLen, GOSTSR_U8 *lastData)
{
    GOSTSR_U8 *pSectionData = GOSTSR_NULL;
    GOSTSR_U16 curSectionLength = 0;
    GOSTSR_U16 allSectionLength = 0;
    TS_SECTION_INFO *curSectionInfo = GOSTSR_NULL;
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;

    if((GOSTSR_NULL == PackageHead_Temp) || (GOSTSR_NULL == lastData))
    {
        return  GOSTSR_FAILURE;
    }

    pCurPackage = PackageHead_Temp;
    while (GOSTSR_NULL != pCurPackage)
    {
        allSectionLength += pCurPackage->packageLen;
        pCurPackage = pCurPackage->next;
    }

    curSectionLength = allSectionLength;
    curSectionLength += lastDataLen;

    curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
    if(GOSTSR_NULL == curSectionInfo)
    {
        return  GOSTSR_FAILURE;
    }
    curSectionInfo->sectionLength = curSectionLength;
    curSectionInfo->PID = PackageHead_Temp->PID;
    curSectionInfo->tableID = PackageHead_Temp->tableID;
    memcpy(&curSectionInfo->stErrorInfo,&PackageHead_Temp->stErrorInfo,sizeof(TR101290_ERROR_S));
    curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
    if(GOSTSR_NULL == curSectionInfo->sectionData)
    {
        return  GOSTSR_FAILURE;
    }

    memset(curSectionInfo->sectionData, 0x00, curSectionLength);
    pSectionData = curSectionInfo->sectionData;
    pCurPackage = PackageHead_Temp;

    while (GOSTSR_NULL != pCurPackage)
    {
        memcpy(pSectionData, pCurPackage->packageData, pCurPackage->packageLen);
        pSectionData += pCurPackage->packageLen;
        pCurPackage = pCurPackage->next;
    }
    memcpy(pSectionData, lastData, lastDataLen);

    GOSTSR_U16 curPos = curSectionInfo->sectionLength - 4;
    GOSTSR_U8  *pData = &(curSectionInfo->sectionData[curPos]);
    GOSTSR_U32 crc32 = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    GOSTSR_U32 calCrc32 = 0;

    calCrc32 = GosTsr_AnalysisData_CRCCheck(curSectionInfo->sectionData, curSectionInfo->sectionLength - 4);
    if ((crc32 == calCrc32) || (TDT_TABLE_ID == tableID))
    {
        TsParse_Search_ParseSection(tableID,curSectionInfo);
    }

    if (GOSTSR_NULL != curSectionInfo)
    {
        if (GOSTSR_NULL != curSectionInfo->sectionData)
        {
            free(curSectionInfo->sectionData);
            curSectionInfo->sectionData = GOSTSR_NULL;
        }
        free(curSectionInfo);
        curSectionInfo = GOSTSR_NULL;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_BuildSection(GOSTSR_U8 tableID,const void *data, TS_HEAD_INFO tsHeadInfo)
{
    GOSTSR_U8 sectionFlag = GOSTSR_FALSE;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U8 *pcurData = GOSTSR_NULL;
    GOSTSR_U8 adaptLength = 0;
    GOSTSR_U8 pointFieldLength = 0;
    GOSTSR_U8 frontSectionData[256] = {0,};
    GOSTSR_U8 curTableID = BULT_TABLE_ID;
    GOSTSR_U16 curTempData = 0;
    GOSTSR_U16 curSectionLength = 0;
    GOSTSR_U16 remainLen = 0;
    TS_SECTION_INFO *curSectionInfo = GOSTSR_NULL;
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;
    TS_PACKAGE_INFO *prePackage = GOSTSR_NULL;
    GOSTSR_U16 packageAllLen = 0;
    GOSTSR_U8 *pSectionData = GOSTSR_NULL;
    GOSTSR_U16 allSectionLength = 0;

    if (GOSTSR_NULL == data)
    {
        return  GOSTSR_FAILURE;
    }

    pcurData  = (GOSTSR_U8 *)data;
    pcurData += TS_HEAD_LENGTH;

    /*check whether or no data contains adaptation data*/
    if (tsHeadInfo.adapter_control & BIT2) /*o?óDμ÷??×???*/
    {
        /*Get adaptation length*/
        adaptLength = pcurData[0];
        pcurData += 1;

        /*Jump to load data*/
        pcurData += adaptLength;
    }

    /*check payload_unit_start_indicator*/
    if (tsHeadInfo.load_indicater == BIT1)
    {
        /*Get pointer_field value*/
        pointFieldLength = pcurData[0];
        pcurData += 1;

        /*Jump to table ID*/
        memset(frontSectionData, 0x00, sizeof(frontSectionData));
        memcpy(frontSectionData, pcurData, pointFieldLength);
        pcurData += pointFieldLength;

        /*Get current table ID*/
        curTableID = pcurData[0];
        pcurData += 1;

        if (curTableID != tableID)
        {
            return GOSTSR_FAILURE;
        }

        /*when other section arrive and free all previous section packages*/
        for (i = 0; i < TABLE_NUMBER; i++)
        {
            if (SearchPackageHead[i] == GOSTSR_NULL)
            {
                continue;
            }

            if (SearchPackageHead[i]->PID == tsHeadInfo.ts_pid)
            {
                allSectionLength = 0;

                pCurPackage = SearchPackageHead[i];
                while (GOSTSR_NULL != pCurPackage)
                {
                    allSectionLength += pCurPackage->packageLen;
                    pCurPackage = pCurPackage->next;
                }

                if((allSectionLength + pointFieldLength) >= SearchPackageHead[i]->sectionLen)
                {
                    TsParse_Search_LinkPackage(tableID,SearchPackageHead[i],pointFieldLength,frontSectionData);
                }

                pCurPackage = SearchPackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    prePackage = pCurPackage;
                    pCurPackage = pCurPackage->next;
                    free(prePackage);
                    prePackage = GOSTSR_NULL;
                }

                SearchPackageHead[i] = GOSTSR_NULL;
            }
        }

        /*Get current section length*/
        curTempData = pcurData[0]*256 + pcurData[1];
        curSectionLength = curTempData & (~((BIT8 | BIT7 | BIT6 | BIT5) * 256));
        pcurData += 2;

        /*jump to section head*/
        pcurData -= 3;
        curSectionLength += 3;

        /*get ts package remain char*/
        if (tsHeadInfo.adapter_control & BIT2)/*o?óDμ÷????μ??é????*/
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                remainLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - adaptLength - 1 - pointFieldLength);
            }
            else
            {
                remainLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - adaptLength - 1 - pointFieldLength);
            }
        }
        else
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                remainLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - pointFieldLength);
            }
            else
            {
                remainLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - pointFieldLength);
            }
        }

        /*when one section length <= one package valid loader data length*/
        if (curSectionLength <= remainLen)
        {
            curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
            curSectionInfo->sectionLength = curSectionLength;
            curSectionInfo->tableID = tableID;
            curSectionInfo->PID = tsHeadInfo.ts_pid;
            curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
            memset(curSectionInfo->sectionData, 0x00, curSectionLength);
            memcpy(curSectionInfo->sectionData, pcurData, curSectionLength);

            /*one section receive over*/
            sectionFlag = GOSTSR_TRUE;
        }
        else /*create a new section package list head*/
        {
            /*find the empty package list head*/
            for (i = 0; i < TABLE_NUMBER; i++)
            {
                if (SearchPackageHead[i] == GOSTSR_NULL)
                {
                    break;
                }
            }

            if (TABLE_NUMBER != i)
            {
                SearchPackageHead[i] = (TS_PACKAGE_INFO*)malloc(sizeof(TS_PACKAGE_INFO));
                SearchPackageHead[i]->next = GOSTSR_NULL;
                SearchPackageHead[i]->packageLen = remainLen;
                memset(SearchPackageHead[i]->packageData, 0x00, sizeof(SearchPackageHead[i]->packageData));
                memcpy(SearchPackageHead[i]->packageData, pcurData, remainLen);
                SearchPackageHead[i]->PID = tsHeadInfo.ts_pid;
                SearchPackageHead[i]->number = tsHeadInfo.counter;
                SearchPackageHead[i]->sectionLen = curSectionLength;
                SearchPackageHead[i]->tableID = tableID;
                memcpy(&SearchPackageHead[i]->stErrorInfo,&tsHeadInfo.stErrorInfo,sizeof(TR101290_ERROR_S));
            }
        }
    }
    else
    {
        /*find the list head of current package*/
        for (i = 0; i < TABLE_NUMBER; i++)
        {
            if (SearchPackageHead[i] == GOSTSR_NULL)
            {
                continue;
            }

            if (SearchPackageHead[i]->PID == tsHeadInfo.ts_pid)
            {
                break;
            }
        }

        /*no find the same PID package list head*/
        if (i == TABLE_NUMBER)
        {
            return GOSTSR_FAILURE;
        }

        /*jump to the package list end*/
        packageAllLen = 0;
        pCurPackage = SearchPackageHead[i];
        prePackage = SearchPackageHead[i];
        while (pCurPackage != GOSTSR_NULL)
        {
            packageAllLen += pCurPackage->packageLen;
            prePackage = pCurPackage;
            pCurPackage = pCurPackage->next;
        }

        /*apply the memory of current package and inset current package at the end of package list end*/
        pCurPackage = (TS_PACKAGE_INFO*)malloc(sizeof(TS_PACKAGE_INFO));
        if (GOSTSR_NULL == pCurPackage)
        {
            return GOSTSR_FAILURE;
        }

        prePackage->next = pCurPackage;
        pCurPackage->next = GOSTSR_NULL;
        if (tsHeadInfo.adapter_control & BIT2)
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                pCurPackage->packageLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - adaptLength);
            }
            else
            {
                pCurPackage->packageLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - adaptLength);
            }
        }
        else
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                pCurPackage->packageLen = (TS_LENGTH_188 - TS_HEAD_LENGTH);
            }
            else
            {
                pCurPackage->packageLen = (TS_LENGTH_204 - TS_HEAD_LENGTH);
            }
        }
        memset(pCurPackage->packageData, 0x00, sizeof(pCurPackage->packageData));
        memcpy(pCurPackage->packageData, pcurData, pCurPackage->packageLen);
        pCurPackage->PID = tsHeadInfo.ts_pid;
        pCurPackage->number = tsHeadInfo.counter;
        pCurPackage->sectionLen = SearchPackageHead[i]->sectionLen;
        pCurPackage->tableID = SearchPackageHead[i]->tableID;

        /*update total received packages length*/
        packageAllLen += pCurPackage->packageLen;

        if (packageAllLen >= SearchPackageHead[i]->sectionLen)
        {
            packageAllLen = 0;
            j = SearchPackageHead[i]->number;

            /*check: whether all packages of one section have arrived or no?*/
            while(packageAllLen < SearchPackageHead[i]->sectionLen)
            {
                pCurPackage = SearchPackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    if(pCurPackage->number == j)
                    {
                        packageAllLen += pCurPackage->packageLen;
                        j++;
                        /*when j reach the max value 15 ,j return zero*/
                        if (j == 16)
                        {
                            j = 0;
                        }
                        break;
                    }
                    pCurPackage = pCurPackage->next;
                }

                if (GOSTSR_NULL == pCurPackage)
                {
                    break;
                }
            }

            /*when found any package is loss*/
            if (GOSTSR_NULL == pCurPackage)
            {
                /*wait*/
            }
            else /*while all packages of one section have arrived*/
            {
                tableID = SearchPackageHead[i]->tableID;
                curSectionLength = SearchPackageHead[i]->sectionLen;
                curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
                curSectionInfo->sectionLength = curSectionLength;
                curSectionInfo->PID = tsHeadInfo.ts_pid;
                curSectionInfo->tableID = tableID;
                curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
                memset(curSectionInfo->sectionData, 0x00, curSectionLength);
                pSectionData = curSectionInfo->sectionData;

                packageAllLen = 0;
                while (packageAllLen < curSectionLength)
                {
                    /*get package pointer ,It's number is j*/
                    pCurPackage = SearchPackageHead[i];
                    prePackage = SearchPackageHead[i];
                    j = SearchPackageHead[i]->number;

                    while (pCurPackage != GOSTSR_NULL)
                    {
                        if(pCurPackage->number == j)
                        {
                            j++;
                            /*when j reach the max value ,j return zero*/
                            if (j == 16)
                            {
                                j = 0;
                            }
                            break;
                        }
                        prePackage = pCurPackage;
                        pCurPackage = pCurPackage->next;
                    }

                    if (GOSTSR_NULL != pCurPackage)
                    {
                        /*get section data from  current package*/
                        if ((packageAllLen + pCurPackage->packageLen) > curSectionLength)
                        {
                            remainLen =  curSectionLength - packageAllLen;
                        }
                        else
                        {
                            remainLen = pCurPackage->packageLen;
                        }

                        memcpy(pSectionData, pCurPackage->packageData, remainLen);

                        pSectionData += remainLen;
                        packageAllLen += pCurPackage->packageLen;

                        /*free memory of current package*/
                        if (prePackage == SearchPackageHead[i]) /*if current package is head package*/
                        {
                            SearchPackageHead[i] = prePackage->next;
                            free(prePackage);
                            prePackage = GOSTSR_NULL;
                        }
                        else
                        {
                            prePackage->next = pCurPackage->next;
                            free(pCurPackage);
                            pCurPackage = GOSTSR_NULL;
                        }

                    }
                }

                /*free all package list member*/
                pCurPackage = SearchPackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    prePackage = pCurPackage;
                    pCurPackage = pCurPackage->next;
                    free(prePackage);
                    prePackage = GOSTSR_NULL;
                }
                SearchPackageHead[i] = GOSTSR_NULL;

                /*one section receive over*/
                sectionFlag = GOSTSR_TRUE;
            }
        }
    }

    if (GOSTSR_TRUE == sectionFlag)
    {

        GOSTSR_U16 curPos = curSectionInfo->sectionLength - 4;
        GOSTSR_U8  *pData = &(curSectionInfo->sectionData[curPos]);
        GOSTSR_U32  crc32 = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
        GOSTSR_U32  calCrc32 = 0;
        calCrc32 = GosTsr_AnalysisData_CRCCheck(curSectionInfo->sectionData, curSectionInfo->sectionLength - 4);

        if ((crc32 == calCrc32) || (TDT_TABLE_ID == tableID))
        {
            TsParse_Search_ParseSection(tableID,curSectionInfo);
        }

    }

    if (GOSTSR_NULL != curSectionInfo)
    {
        if (GOSTSR_NULL != curSectionInfo->sectionData)
        {
            free(curSectionInfo->sectionData);
            curSectionInfo->sectionData = GOSTSR_NULL;
        }
        free(curSectionInfo);
        curSectionInfo = GOSTSR_NULL;
    }
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_TSHeadInfo(const void *srcData, TS_HEAD_INFO *desData)
{
    GOSTSR_U8 *curData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;

    if ((GOSTSR_NULL == srcData) || (GOSTSR_NULL == desData))
    {
        return GOSTSR_FAILURE;
    }

    curData = (GOSTSR_U8 *)srcData;

    /*sync_byte*/
    if (SYNC_BYTE != curData[0])
    {
        return GOSTSR_FAILURE;
    }

    curData++;
    /*transport_error_indicator*/
    desData->error_indicater = (curData[0] & BIT8) / BIT8;

    /*payload_unit_start_indicator*/
    desData->load_indicater = (curData[0] & BIT7) / BIT7;

    /*transport_priority*/

    /*PID*/
    tempData = curData[0]*256 + curData[1];
    desData->ts_pid = tempData  & (~((BIT8 | BIT7 | BIT6) * 256));

    curData += 2;
    /*transport_scrambling_control*/
    desData->scramble_control = (curData[0] & (BIT8 | BIT7)) / BIT7;

    /*adaptation_field_control*/
    desData->adapter_control = (curData[0] & (BIT6 | BIT5)) / BIT5;

    /*continuity_counter*/
    desData->counter = curData[0] & (BIT4 | BIT3 | BIT2 | BIT1);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_AnalysisPacket(GOSTSR_U8 *pPacketData, GOSTSR_U32 u32DataLen)
{
    GOSTSR_S32 s32Ret = 0;
    GOSTSR_S32 i = 0;
    TS_HEAD_INFO tsHeadInfo;
    GOSTSR_U8 *pData = pPacketData;

    if(pPacketData == GOSTSR_NULL)
    {
        return GOSTSR_FAILURE;
    }

    memset(&tsHeadInfo,0,sizeof(TS_HEAD_INFO));
    s32Ret = TsParse_Search_TSHeadInfo(pData,&tsHeadInfo);
    if(s32Ret != GOSTSR_SUCCESS)
    {
        return GOSTSR_FAILURE;
    }
    switch(tsHeadInfo.ts_pid)
    {
        case PAT_PID:
            TsParse_Search_BuildSection(PAT_TABLE_ID, pData, tsHeadInfo);
            break;

        case CAT_PID:
            TsParse_Search_BuildSection(CAT_TABLE_ID, pData, tsHeadInfo);
            break;
        default:
            {
                for(i = 0; i < gstSearchPatInfo.u16NbElements; i++)
                {
                    if(gstSearchPatInfo.astElement[i].u16Pid == tsHeadInfo.ts_pid)
                    {
                        TsParse_Search_BuildSection(PMT_TABLE_ID, pData, tsHeadInfo);
                        break;
                    }	
                }
                break;
            }

    }
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_ReadStaticTs(char *filePath)
{
    FILE *sFp = GOSTSR_NULL;
    GOSTSR_U64 fileLen = 0;
    GOSTSR_U64 offset = 0;
    GOSTSR_U32 packageLen = 0;
    GOSTSR_U8 chr = '\0';
    GOSTSR_U8 *tsData = GOSTSR_NULL;
    GOSTSR_U8 *tsData_Bak = GOSTSR_NULL;

    if (GOSTSR_NULL == filePath)
    {
        return GOSTSR_FAILURE;
    }

    if(GOSTSR_FAILURE == GosTsr_AnalysisData_PacketType(&packageLen,filePath))
    {
        return GOSTSR_FAILURE;
    }
    sFp = fopen(filePath, "rb");
    if (GOSTSR_NULL == sFp)
    {
        return GOSTSR_FAILURE;
    }

    tsData = (GOSTSR_U8 *)malloc(packageLen);
    if (GOSTSR_NULL == tsData)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        return GOSTSR_FAILURE;
    }
    tsData_Bak = (GOSTSR_U8 *)malloc(packageLen+1);
    if (GOSTSR_NULL == tsData_Bak)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        return GOSTSR_FAILURE;
    }

    fseek(sFp, 0, SEEK_END);
    fileLen = ftell(sFp);
    //printf("______ TS File size = %dM_____\n\n",fileLen / 1024 /1024);
    fseek(sFp, 0, SEEK_SET);
    while(1)
    {	
        if(bHadSearched)
            break;

        offset = ftell(sFp);
        do
        {
            chr = fgetc(sFp);
            offset++;

            if (offset >= fileLen - 1)
            {
                break;
            }

        }while(chr != SYNC_BYTE);
        if (offset >= fileLen - 1)
        {
            break;
        }
        if (chr == SYNC_BYTE)
        {
            offset = offset - 1;
            fseek(sFp, offset, SEEK_SET);
        }
        memset(tsData_Bak, 0x00, packageLen+1);
        fread(tsData_Bak, 1, packageLen+1, sFp);
        if(tsData_Bak[packageLen] != SYNC_BYTE)
        {
            offset = offset + 1;
            fseek(sFp, offset, SEEK_SET);
            continue;
        }
        offset += packageLen;
        fseek(sFp, offset, SEEK_SET);

        memset(tsData, 0x00, packageLen);
        memcpy(tsData, tsData_Bak, packageLen);

        TsParse_Search_AnalysisPacket(tsData, packageLen);
    }

    free(tsData);
    tsData = GOSTSR_NULL;
    fclose(sFp);
    sFp = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_Init(char *filePath)
{
    memset(&gstSearchProgInfo,0,sizeof(SEARCH_INFO_S));
    memset(&gstSearchPatInfo,0,sizeof(GOSTSR_PSISI_PAT_S));
    bHadSearched = GOSTSR_FALSE;
    gu16SearchCount = 0;

    TsParse_Search_ReadStaticTs(filePath);
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Search_DeInit()
{
    memset(&gstSearchProgInfo,0,sizeof(SEARCH_INFO_S));
    memset(&gstSearchPatInfo,0,sizeof(GOSTSR_PSISI_PAT_S));
    bHadSearched = GOSTSR_FALSE;
    gu16SearchCount = 0;

    return GOSTSR_SUCCESS;
}
