#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "TsErrorCheck_ThreeLevel.h"

#include "search.h"


static TsThreeLevelErrorCheck_Info errorInfo;

static GOSTSR_U32 nitPreTime;
static GOSTSR_U32 nitActualPreTime;
static GOSTSR_U32 nitOtherPreTime;

static GOSTSR_U32 sdtPreTime;
static GOSTSR_U32 sdtActualPreTime;
static GOSTSR_U32 sdtOtherPreTime[256];

static GOSTSR_U32 eitPreTime;
static GOSTSR_U32 eitActualPreTime;
static GOSTSR_U32 eitActualSec0PreTime;
static GOSTSR_U32 eitActualSec1PreTime;
static GOSTSR_U32 eitOtherSec0PreTime;
static GOSTSR_U32 eitOtherSec1PreTime;
static GOSTSR_U8  eitActualPairFlag;
static GOSTSR_U8  eitOtherPairFlag;

static GOSTSR_U32 rstPreTime;
static GOSTSR_U32 rstSectionPreTime;

static GOSTSR_U32 tdtPreTime;
static GOSTSR_U32 tdtSectionPreTime;

static SiRepetRateSections_Info nitActualSectionInfo;
static SiRepetRateSections_Info nitOtherSectionInfo;
static SiRepetRateSections_Info batSectionInfo;
static SiRepetRateSections_Info sdtActualSectionInfo;
static SiRepetRateSections_Info sdtOtherSectionInfo;
static SiRepetRateSections_Info eitActualSectionInfo;
static SiRepetRateSections_Info eitOtherSectionInfo;
static SiRepetRateSections_Info eitActualScheduleSectionInfo;
static SiRepetRateSections_Info eitOtherScheduleSectionInfo;
static SiRepetRateSections_Info tdtSectionInfo;
static SiRepetRateSections_Info totSectionInfo;

static UnreferencedPid_Info unreferPidInfo[MAX_UNREFERPID_NUM];
static SEARCH_INFO_S gstProgInfo;

/*nit error check functions*/
static GOSTSR_S32 TsCheck_NitSectionsTimeError(TS_SECTION_INFO sectionInfo);

/*si repetition rate error check functions*/
static GOSTSR_S32 TsCheck_SiSectionRepetitionRateError(TS_SECTION_INFO sectionInfo);

/*buffer error check functions*/
static GOSTSR_S32 TsCheck_BufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time);

/*unreferenced pid error check functions*/
static GOSTSR_S32 TsCheck_UnreferencedPidError(TS_HEAD_INFO tsHeadInfo);

/*sdt error check functions*/
static GOSTSR_S32 TsCheck_SdtActualSectionsTimeError(TS_SECTION_INFO sectionInfo);
static GOSTSR_S32 TsCheck_SdtOtherSectionsTimeError(TS_SECTION_INFO sectionInfo);

/*eit error check functions*/
static GOSTSR_S32 TsCheck_EitActualSectionsTimeError(TS_SECTION_INFO sectionInfo);
static GOSTSR_S32 TsCheck_EitOtherSectionsTimeError(TS_SECTION_INFO sectionInfo);

/*rst error check functions*/
static GOSTSR_S32 TsCheck_RstSectionsTimeError(TS_SECTION_INFO sectionInfo);

/*tdt error check functions*/
static GOSTSR_S32 TsCheck_TdtSectionsTimeError(TS_SECTION_INFO sectionInfo);

/*empty buffer error check functions*/
static GOSTSR_S32 TsCheck_EmptyBufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time);

/*date delay error check functions*/
static GOSTSR_S32 TsCheck_DataDelayError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time);

static GOSTSR_S32 TsCheck_GetCurrentSectionInfo(TS_SECTION_INFO sectionInfo, GOSTSR_U8 *sectionNum, GOSTSR_U8 *lastSectionNum)
{
	GOSTSR_U8  *pData = GOSTSR_NULL;

	if ((GOSTSR_NULL == sectionInfo.sectionData) || (GOSTSR_NULL == sectionNum) || (GOSTSR_NULL == lastSectionNum))
	{
		return GOSTSR_SUCCESS;
	}
	
	pData = sectionInfo.sectionData;
	
    pData += 6;

    /*section number*/
    *sectionNum = pData[0];
	pData += 1;

	/*last section number*/
	*lastSectionNum = pData[0];

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_TableOverTime(TS_SECTION_INFO sectionInfo_Cur, SiRepetRateSections_Info *sectionInfo_Pre, GOSTSR_U32 overTime)
{
	GOSTSR_U8  sectionNum = 0;
	GOSTSR_U8  lastSectionNum = 0;
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_S32 retVal = GOSTSR_FAILURE;
	GOSTSR_U8  maxNumber = 0;

	if (GOSTSR_NULL == sectionInfo_Pre)
	{
		return GOSTSR_FAILURE;
	}
	
	if (GOSTSR_SUCCESS != TsCheck_GetCurrentSectionInfo(sectionInfo_Cur, &sectionNum, &lastSectionNum))
	{
		return GOSTSR_FAILURE;
	}

	if (lastSectionNum == 0)
	{
		return GOSTSR_FAILURE;
	}
	
	if (sectionInfo_Pre->number == 0)
	{
		sectionInfo_Pre->startTime = sectionInfo_Cur.stErrorInfo.startTime;
		sectionInfo_Pre->lastSectionNum = lastSectionNum;
	}

	if (sectionInfo_Pre->sectionFlag[sectionNum] == 0x01)
	{
		return GOSTSR_FAILURE;
	}
	else
	{
		sectionInfo_Pre->sectionFlag[sectionNum] = 0x01;
		sectionInfo_Pre->number++;
	}

	if ((sectionInfo_Cur.tableID == EIT_TABLE_ID_ACTUAL_SHEDULE) || (sectionInfo_Cur.tableID == EIT_TABLE_ID_OTHER_SHEDULE))
	{
		maxNumber = sectionInfo_Pre->lastSectionNum / 8; 
	}
	else
	{
		maxNumber = sectionInfo_Pre->lastSectionNum;
	}
	
	if (sectionInfo_Pre->number > maxNumber)
	{
		if(sectionInfo_Cur.stErrorInfo.bCarryFlag)
		{
			timeOffset = 0xffffffff+sectionInfo_Cur.stErrorInfo.startTime - sectionInfo_Pre->startTime;
		}
		else
		{
			if(sectionInfo_Cur.stErrorInfo.startTime >=  sectionInfo_Pre->startTime)
			{
				timeOffset = sectionInfo_Cur.stErrorInfo.startTime - sectionInfo_Pre->startTime;
			}
			else
			{
				timeOffset =  sectionInfo_Pre->startTime - sectionInfo_Cur.stErrorInfo.startTime;
			}
		}
			
		if (timeOffset <= 25000)/*SI表的间隔<= 25ms*/
		{
			errorInfo.siRepetitionRateErrorInfo.siSectionLimitTimeError++;
			errorInfo.siRepetitionRateErrorInfo.totalError++;
		}

		if (timeOffset > overTime)
		{
			retVal = GOSTSR_SUCCESS;
		}

		sectionInfo_Pre->startTime = 0;
		sectionInfo_Pre->lastSectionNum = 0;
		sectionInfo_Pre->number = 0;
		memset(sectionInfo_Pre->sectionFlag,0,sizeof(sectionInfo_Pre->sectionFlag));
	}
	
	return retVal;
}

static GOSTSR_S32 TsCheck_NitSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
		
	if ((NIT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}
	
	/*check nit table ID error*/
	if ((NIT_TABLE_ID_ACTUAL != sectionInfo.tableID) && (NIT_TABLE_ID_OTHER != sectionInfo.tableID) && (SI_TABLE_ID != sectionInfo.tableID))
	{
		errorInfo.nitErrorInfo.tableIDError++;
		errorInfo.nitErrorInfo.totalError++;
	}

	/*check nit actual time error*/
	if (NIT_TABLE_ID_ACTUAL == sectionInfo.tableID)
	{
		if (nitActualPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - nitActualPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  nitActualPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - nitActualPreTime;
				}
				else
				{
					timeOffset = nitActualPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}

			/*timeoffset over 10s*/
			if (timeOffset > 10000000)
			{
				errorInfo.nitErrorInfo.actualOverTimeError++;
				errorInfo.nitErrorInfo.totalError++;
			}
			/*timeoffset within 25ms*/
			else if (timeOffset <= 25000)
			{
				errorInfo.nitErrorInfo.actualLimitTimeError++;
				errorInfo.nitErrorInfo.totalError++;
			}
		}
	}
	/*check nit other time error*/
	else if (NIT_TABLE_ID_OTHER == sectionInfo.tableID)
	{
		if (nitOtherPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - nitOtherPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  nitOtherPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - nitOtherPreTime;
				}
				else
				{
					timeOffset = nitOtherPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}

			/*timeoffset over 10s*/
			if (timeOffset > 10000000)
			{
				errorInfo.nitErrorInfo.otherOverTimeError++;
				errorInfo.nitErrorInfo.totalError++;
			}
			/*timeoffset within 25ms*/
			else if (timeOffset <= 25000)
			{
				errorInfo.nitErrorInfo.otherLimitTimeError++;
				errorInfo.nitErrorInfo.totalError++;
			}
		}
	}
	nitOtherPreTime = sectionInfo.stErrorInfo.startTime;
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_SiSectionRepetitionRateError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_U8  sectionNum = 0;
	GOSTSR_U8  lastSectionNum = 0;
	
	if ((GOSTSR_NULL == sectionInfo.sectionData) || (sectionInfo.PID < SI_PID_MIN) || (sectionInfo.PID > SI_PID_MAX))
	{
		return GOSTSR_FAILURE;
	}

	switch(sectionInfo.PID)
	{
		case NIT_PID:
			if (GOSTSR_SUCCESS == TsCheck_GetCurrentSectionInfo(sectionInfo, &sectionNum, &lastSectionNum))
			{
				GOSTSR_U32 nitTime = 0x00;
				
				if (NIT_TABLE_ID_ACTUAL == sectionInfo.tableID)
				{
					if (lastSectionNum == 0)
					{
						break;
					}
					
					if (nitActualSectionInfo.number == 0)
					{						
						if (nitOtherSectionInfo.number == 0)
						{
							nitActualSectionInfo.startTime = sectionInfo.stErrorInfo.startTime;
						}
						nitActualSectionInfo.lastSectionNum = lastSectionNum;
					}

					if (nitActualSectionInfo.sectionFlag[sectionNum] == 0x01)
					{
						break;
					}
					else
					{
						nitActualSectionInfo.sectionFlag[sectionNum] = 0x01;
						nitActualSectionInfo.number++;
					}
				}
				else if (NIT_TABLE_ID_OTHER == sectionInfo.tableID)
				{
					if (lastSectionNum == 0)
					{
						break;
					}
					
					if (nitOtherSectionInfo.number == 0)
					{		
						if (nitActualSectionInfo.number == 0)
						{
							nitOtherSectionInfo.startTime = sectionInfo.stErrorInfo.startTime;
						}
						nitOtherSectionInfo.lastSectionNum = lastSectionNum;
					}

					if (nitOtherSectionInfo.sectionFlag[sectionNum] == 0x01)
					{
						break;
					}
					else
					{
						nitOtherSectionInfo.sectionFlag[sectionNum] = 0x01;
						nitOtherSectionInfo.number++;
					}
				}

				/*over 10s*/
				if (((nitActualSectionInfo.number > nitActualSectionInfo.lastSectionNum) && (nitOtherSectionInfo.number > nitOtherSectionInfo.lastSectionNum)) ||
					  ((nitActualSectionInfo.number == 0) && (nitOtherSectionInfo.number > nitOtherSectionInfo.lastSectionNum)) ||
					  ((nitActualSectionInfo.number > nitActualSectionInfo.lastSectionNum) && (nitOtherSectionInfo.number == 0)))
				{
					if (nitOtherSectionInfo.startTime != 0)
					{
						nitTime = nitOtherSectionInfo.startTime;
					}
					else
					{
						nitTime = nitActualSectionInfo.startTime;
					}
					
					if(sectionInfo.stErrorInfo.bCarryFlag)
					{
						timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - nitTime;
					}
					else
					{
						if(sectionInfo.stErrorInfo.startTime >=  nitTime)
						{
							timeOffset = sectionInfo.stErrorInfo.startTime - nitTime;
						}
						else
						{
							timeOffset = nitTime - sectionInfo.stErrorInfo.startTime;
						}
					}
	
					if (timeOffset <= 25000)/*SI表的间隔<= 25ms*/
					{
						errorInfo.siRepetitionRateErrorInfo.siSectionLimitTimeError++;
						errorInfo.siRepetitionRateErrorInfo.totalError++;
					}
					if (timeOffset >= 10000000)
					{
						errorInfo.siRepetitionRateErrorInfo.nitOverTimeError++;
						errorInfo.siRepetitionRateErrorInfo.totalError++;
					}

					memset(&nitOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
					memset(&nitActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
				}
			}
			break;
			
		case BAT_SDT_PID:
			if (BAT_TABLE_ID == sectionInfo.tableID)
			{
				/*over 10s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &batSectionInfo, 10000000))
				{
					errorInfo.siRepetitionRateErrorInfo.batOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (SDT_TABLE_ID_ACTUAL == sectionInfo.tableID)
			{
				/*over 2s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &sdtActualSectionInfo, 2000000))
				{
					errorInfo.siRepetitionRateErrorInfo.sdtActualOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (SDT_TABLE_ID_OTEHR == sectionInfo.tableID)
			{
				/*over 10s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &sdtOtherSectionInfo, 10000000))
				{
					errorInfo.siRepetitionRateErrorInfo.sdtOtherOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			break;

		case EIT_PID:
			if (EIT_TABLE_ID_ACTUAL == sectionInfo.tableID)
			{
				/*over 2s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &batSectionInfo, 2000000))
				{
					errorInfo.siRepetitionRateErrorInfo.eitActualOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (EIT_TABLE_ID_OTHER == sectionInfo.tableID)
			{
				/*over 10s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &eitOtherSectionInfo, 10000000))
				{
					errorInfo.siRepetitionRateErrorInfo.eitOtherOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (EIT_TABLE_ID_ACTUAL_SHEDULE == sectionInfo.tableID)
			{
				/*over 10s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &eitActualScheduleSectionInfo, 10000000))
				{
					errorInfo.siRepetitionRateErrorInfo.eitActualScheduleOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (EIT_TABLE_ID_OTHER_SHEDULE == sectionInfo.tableID)
			{
				/*over 30s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &eitOtherScheduleSectionInfo, 30000000))
				{
					errorInfo.siRepetitionRateErrorInfo.eitOtherScheduleOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			break;

		case TDT_TOT_PID:
			if (TDT_TABLE_ID == sectionInfo.tableID)
			{
				/*over 30s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &tdtSectionInfo, 30000000))
				{
					errorInfo.siRepetitionRateErrorInfo.tdtOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			else if (TOT_TABLE_ID == sectionInfo.tableID)
			{
				/*over 30s*/
				if (GOSTSR_SUCCESS == TsCheck_TableOverTime(sectionInfo, &totSectionInfo, 30000000))
				{
					errorInfo.siRepetitionRateErrorInfo.totOverTimeError++;
					errorInfo.siRepetitionRateErrorInfo.totalError++;
				}
			}
			break;
			
		default:
			break;
	}

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_BufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time)
{
	if ((GOSTSR_NULL == data) || (dataLen == 0))
	{
		return GOSTSR_FAILURE;
	}

	/*TODO*/

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_UnreferencedPidError(TS_HEAD_INFO tsHeadInfo)
{
	GOSTSR_U16 index = 0x00;
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_S32 i = 0,j = 0;
	
	if ( (tsHeadInfo.ts_pid >= 0x20) && (tsHeadInfo.ts_pid != 0x1fff))
	{
		/*check PMTs,PesPID,PcrPID*/
		for(i = 0; i < gstProgInfo.u16NbProg; i++)
		{
			if((tsHeadInfo.ts_pid  == gstProgInfo.stProgInfo[i].PmtPid) || ((tsHeadInfo.ts_pid  ==  gstProgInfo.stProgInfo[i].PcrPid)))
			{
				return GOSTSR_FAILURE;
			}
			for(j = 0; j < gstProgInfo.stProgInfo[i].u8NbPes; j++)
			{
				if(tsHeadInfo.ts_pid  == gstProgInfo.stProgInfo[i].PesPid[j])
				{
					return GOSTSR_FAILURE;
				}
			}
		}

		/*check CATs*/
		for(i = 0; i < gstProgInfo.u16NbCa; i++)
		{
			if(tsHeadInfo.ts_pid == gstProgInfo.stCaInfo[i].CaPid)
			{
				return GOSTSR_FAILURE;
			}
		}
		
		/*check unferenced pid list over time status*/
		for (i = 0; i < MAX_UNREFERPID_NUM; i++)
		{
			if ((!unreferPidInfo[i].flag) && (unreferPidInfo[i].pid != tsHeadInfo.ts_pid ))
			{
				continue;
			}
			if(tsHeadInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+tsHeadInfo.stErrorInfo.startTime - unreferPidInfo[i].startTime;
			}
			else
			{
				if(tsHeadInfo.stErrorInfo.startTime >=  unreferPidInfo[i].startTime)
				{
					timeOffset = tsHeadInfo.stErrorInfo.startTime - unreferPidInfo[i].startTime;
				}
				else
				{
					timeOffset = unreferPidInfo[i].startTime - tsHeadInfo.stErrorInfo.startTime;
				}
			}
			
			/*timeoffset over 0.5s*/
			if (timeOffset > 500000)
			{
				//printf("##############----------Pid = %#x___timeOffset = %d\n",tsHeadInfo.ts_pid ,timeOffset);
				errorInfo.unreferencedPidErrorInfo.totalError++;
			}
			unreferPidInfo[i].startTime = tsHeadInfo.stErrorInfo.startTime;
			return GOSTSR_SUCCESS;
		}
		
		if(i == MAX_UNREFERPID_NUM)
		{
			/*get empty unreferenced pid position*/
			for (index = 0; index < MAX_UNREFERPID_NUM; index++)
			{
				if (!unreferPidInfo[index].flag)
				{
					break;
				}
			}

			if (index != MAX_UNREFERPID_NUM)
			{
				unreferPidInfo[index].flag = 0x01;
				unreferPidInfo[index].pid = tsHeadInfo.ts_pid ;
				unreferPidInfo[index].startTime = tsHeadInfo.stErrorInfo.startTime;
			}
		}	
	}

	return GOSTSR_FAILURE;
}

static GOSTSR_S32 TsCheck_SdtActualSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	
	if ((BAT_SDT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}
	/*check table ID error*/
	if ((SDT_TABLE_ID_ACTUAL != sectionInfo.tableID) && (SDT_TABLE_ID_OTEHR != sectionInfo.tableID) && \
		(BAT_TABLE_ID != sectionInfo.tableID) && (SI_TABLE_ID != sectionInfo.tableID))
	{
		errorInfo.sdtErrorInfo.tableIDError++;
		errorInfo.sdtErrorInfo.totalError++; 
	}
	
	if (SDT_TABLE_ID_ACTUAL == sectionInfo.tableID)
	{
		/*check nit actual time error*/
		if (sdtActualPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - sdtActualPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  sdtActualPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - sdtActualPreTime;
				}
				else
				{
					timeOffset = sdtActualPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}
			
			/*timeoffset over 2s*/
			if (timeOffset >= 2000000)
			{
				errorInfo.sdtErrorInfo.actualOverTimeError++;
				errorInfo.sdtErrorInfo.totalError++;
			}
			/*timeoffset within 25ms*/
			else if (timeOffset <= 25000)
			{
				errorInfo.sdtErrorInfo.actualLimitTimeError++;
				errorInfo.sdtErrorInfo.totalError++;
			}
		}

		sdtActualPreTime = sectionInfo.stErrorInfo.startTime;
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_SdtOtherSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_U8  sectionNum = 0;
	GOSTSR_U8  lastSectionNum = 0;
	
	if ((BAT_SDT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}

	if (SDT_TABLE_ID_OTEHR == sectionInfo.tableID)
	{
		/*get current section number*/
		if (GOSTSR_SUCCESS == TsCheck_GetCurrentSectionInfo(sectionInfo, &sectionNum, &lastSectionNum))
		{
			/*check nit other time error*/
			if (sdtOtherPreTime[sectionNum] != 0)
			{
				if(sectionInfo.stErrorInfo.bCarryFlag)
				{
					timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - sdtOtherPreTime[sectionNum];
				}
				else
				{
					if(sectionInfo.stErrorInfo.startTime >=  sdtOtherPreTime[sectionNum])
					{
						timeOffset = sectionInfo.stErrorInfo.startTime - sdtOtherPreTime[sectionNum];
					}
					else
					{
						timeOffset = sdtOtherPreTime[sectionNum] - sectionInfo.stErrorInfo.startTime;
					}
				}
				
				/*timeoffset over 10s*/
				if (timeOffset > 10000000)
				{
					errorInfo.sdtErrorInfo.otherOverTimeError++;
					errorInfo.sdtErrorInfo.totalError++;
				}
			}

			sdtOtherPreTime[sectionNum] = sectionInfo.stErrorInfo.startTime;
		}
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_EitActualSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_U8  sectionNum = 0;
	GOSTSR_U8  lastSectionNum = 0;
	
	if ((EIT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}
	/*check table ID error*/
	if ((sectionInfo.tableID < 0x4E) || ((sectionInfo.tableID > 0x6f) && (sectionInfo.tableID != 0x72)))
	{
		errorInfo.eitErrorInfo.tableIDError++;
		errorInfo.eitErrorInfo.totalError++;
	}
	
	if (EIT_TABLE_ID_ACTUAL == sectionInfo.tableID)
	{
		/*check eit actual time error*/
		if (eitActualPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - eitActualPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  eitActualPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - eitActualPreTime;
				}
				else
				{
					timeOffset = eitActualPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}

			/*timeoffset over 2s*/
			if (timeOffset > 2000000)
			{
				errorInfo.eitErrorInfo.actualOverTimeError++;
				errorInfo.eitErrorInfo.totalError++;
			}
			/*timeoffset within 25ms*/
			else if (timeOffset <= 25000)
			{
				errorInfo.eitErrorInfo.actualLimtTimeError++;
				errorInfo.eitErrorInfo.totalError++;
			}
		}
		eitActualPreTime = sectionInfo.stErrorInfo.startTime;

		if (GOSTSR_SUCCESS == TsCheck_GetCurrentSectionInfo(sectionInfo, &sectionNum, &lastSectionNum))
		{
			/*section 0 time error*/
			if (sectionNum == 0)
			{
				if (eitActualSec0PreTime != 0)
				{
					if(sectionInfo.stErrorInfo.bCarryFlag)
					{
						timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - eitActualSec0PreTime;
					}
					else
					{
						if(sectionInfo.stErrorInfo.startTime >=  eitActualSec0PreTime)
						{
							timeOffset = sectionInfo.stErrorInfo.startTime - eitActualSec0PreTime;
						}
						else
						{
							timeOffset = eitActualSec0PreTime - sectionInfo.stErrorInfo.startTime;
						}
					}

					/*timeoffset over 2s*/
					if (timeOffset > 2000000)
					{
						//printf("##############--------timeOffset = %d\n",timeOffset);
						errorInfo.eitErrorInfo.actualSec0OverTimeError++;
						errorInfo.eitErrorInfo.totalError++;
					}
				}

				/*check eit actual pair error*/
				if (eitActualPairFlag != 3)
				{
					eitActualPairFlag++;
					if (eitActualPairFlag == 2)
					{
						eitActualPairFlag = 1;
						if (errorInfo.eitErrorInfo.actualPFPairError == 0)
						{
							errorInfo.eitErrorInfo.actualPFPairError = 1;
							errorInfo.eitErrorInfo.totalError++;
						}
					}
				}
				else
				{
					if (errorInfo.eitErrorInfo.actualPFPairError == 1)
					{
						errorInfo.eitErrorInfo.actualPFPairError = 0;
						errorInfo.eitErrorInfo.totalError--;
					}
				}
				
				eitActualSec0PreTime = sectionInfo.stErrorInfo.startTime;
			}
			/*section 1 time error*/
			else if (sectionNum == 1)
			{
				if (eitActualSec1PreTime != 0)
				{
					if(sectionInfo.stErrorInfo.bCarryFlag)
					{
						timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - eitActualSec1PreTime;
					}
					else
					{
						if(sectionInfo.stErrorInfo.startTime >=  eitActualSec1PreTime)
						{
							timeOffset = sectionInfo.stErrorInfo.startTime - eitActualSec1PreTime;
						}
						else
						{
							timeOffset = eitActualSec1PreTime - sectionInfo.stErrorInfo.startTime;
						}
					}

					/*timeoffset over 2s*/
					if (timeOffset >= 2000000)
					{
						errorInfo.eitErrorInfo.actualSec1OverTimeError++;
						errorInfo.eitErrorInfo.totalError++;
					}
				}

				/*check eit actual pair error*/
				if (eitActualPairFlag != 3)
				{
					eitActualPairFlag += 2;
					if (eitActualPairFlag == 4)
					{
						eitActualPairFlag = 1;
						if (errorInfo.eitErrorInfo.actualPFPairError == 0)
						{
							errorInfo.eitErrorInfo.actualPFPairError = 1;
							errorInfo.eitErrorInfo.totalError++;
						}
					}
				}
				else
				{
					if (errorInfo.eitErrorInfo.actualPFPairError == 1)
					{
						errorInfo.eitErrorInfo.actualPFPairError = 0;
						errorInfo.eitErrorInfo.totalError--;
					}
				}
				
				eitActualSec1PreTime = sectionInfo.stErrorInfo.startTime;
			}
		}
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_EitOtherSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	GOSTSR_U8  sectionNum = 0;
	GOSTSR_U8  lastSectionNum = 0;
	
	if ((EIT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}

	if (EIT_TABLE_ID_OTHER == sectionInfo.tableID)
	{
		if (GOSTSR_SUCCESS == TsCheck_GetCurrentSectionInfo(sectionInfo, &sectionNum, &lastSectionNum))
		{
			/*section 0 time error*/
			if (sectionNum == 0)
			{
				if (eitOtherSec0PreTime != 0)
				{
					if(sectionInfo.stErrorInfo.bCarryFlag)
					{
						timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - eitOtherSec0PreTime;
					}
					else
					{
						if(sectionInfo.stErrorInfo.startTime >=  eitOtherSec0PreTime)
						{
							timeOffset = sectionInfo.stErrorInfo.startTime - eitOtherSec0PreTime;
						}
						else
						{
							timeOffset = eitOtherSec0PreTime - sectionInfo.stErrorInfo.startTime;
						}
					}
					
					/*timeoffset over 10s*/
					if (timeOffset >= 10000000)
					{
						errorInfo.eitErrorInfo.otherSec0OverTimeError++;
						errorInfo.eitErrorInfo.totalError++;
					}
				}

				/*check eit other pf pair error*/
				if (eitOtherPairFlag != 3)
				{
					eitOtherPairFlag++;
					if (eitOtherPairFlag == 2)
					{
						eitOtherPairFlag = 1;
						if (errorInfo.eitErrorInfo.otherPFPairError == 0)
						{
							errorInfo.eitErrorInfo.otherPFPairError = 1;
							errorInfo.eitErrorInfo.totalError++;
						}
					}
				}
				else
				{
					if (errorInfo.eitErrorInfo.otherPFPairError == 1)
					{
						errorInfo.eitErrorInfo.otherPFPairError = 0;
						errorInfo.eitErrorInfo.totalError--;
					}
				}
				
				eitOtherSec0PreTime = sectionInfo.stErrorInfo.startTime;
			}
			/*section 1 time error*/
			else if (sectionNum == 1)
			{
				if (eitOtherSec1PreTime != 0)
				{
					if(sectionInfo.stErrorInfo.bCarryFlag)
					{
						timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - eitOtherSec1PreTime;
					}
					else
					{
						if(sectionInfo.stErrorInfo.startTime >=  eitOtherSec1PreTime)
						{
							timeOffset = sectionInfo.stErrorInfo.startTime - eitOtherSec1PreTime;
						}
						else
						{
							timeOffset = eitOtherSec1PreTime - sectionInfo.stErrorInfo.startTime;
						}
					}
					
					/*timeoffset over 10s*/
					if (timeOffset >= 10000000)
					{
						errorInfo.eitErrorInfo.otherSec1OverTimeError++;
						errorInfo.eitErrorInfo.totalError++;
					}
				}

				/*check eit other pf pair error*/
				if (eitOtherPairFlag != 3)
				{
					eitOtherPairFlag += 2;
					if (eitOtherPairFlag == 4)
					{
						eitOtherPairFlag = 1;
						if (errorInfo.eitErrorInfo.otherPFPairError == 0)
						{
							errorInfo.eitErrorInfo.otherPFPairError = 1;
							errorInfo.eitErrorInfo.totalError++;
						}
					}
				}
				else
				{
					if (errorInfo.eitErrorInfo.otherPFPairError == 1)
					{
						errorInfo.eitErrorInfo.otherPFPairError = 0;
						errorInfo.eitErrorInfo.totalError--;
					}
				}
				
				eitOtherSec1PreTime = sectionInfo.stErrorInfo.startTime;
			}
		}
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_RstSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	
	if ((RST_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}
	/*check table ID error*/
	if ((RST_TABLE_ID != sectionInfo.tableID) && (SI_TABLE_ID != sectionInfo.tableID))
	{
		errorInfo.rstErrorInfo.tableIDError++;
		errorInfo.rstErrorInfo.totalError++;
	}
	if (RST_TABLE_ID == sectionInfo.tableID)
	{
		if (rstSectionPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - rstSectionPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  rstSectionPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - rstSectionPreTime;
				}
				else
				{
					timeOffset = rstSectionPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}

			/*timeoffset within 25ms*/
			if (timeOffset <= 25000)
			{
				errorInfo.rstErrorInfo.limtTimeError++;
				errorInfo.rstErrorInfo.totalError++;
			}
		}

		rstSectionPreTime = sectionInfo.stErrorInfo.startTime;
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_TdtSectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	GOSTSR_U32 timeOffset = 0;
	
	if ((TDT_TOT_PID != sectionInfo.PID) || (GOSTSR_NULL == sectionInfo.sectionData))
	{
		return GOSTSR_FAILURE;
	}
	/*check table ID error*/
	if ((TDT_TABLE_ID != sectionInfo.tableID) && (SI_TABLE_ID != sectionInfo.tableID) && (TOT_TABLE_ID != sectionInfo.tableID))
	{
		errorInfo.tdtErrorInfo.tableIDError++;
		errorInfo.tdtErrorInfo.totalError++;
	}
	if (TDT_TABLE_ID == sectionInfo.tableID)
	{
		/*check eit actual time error*/
		if (tdtSectionPreTime != 0)
		{
			if(sectionInfo.stErrorInfo.bCarryFlag)
			{
				timeOffset = 0xffffffff+sectionInfo.stErrorInfo.startTime - tdtSectionPreTime;
			}
			else
			{
				if(sectionInfo.stErrorInfo.startTime >=  tdtSectionPreTime)
				{
					timeOffset = sectionInfo.stErrorInfo.startTime - tdtSectionPreTime;
				}
				else
				{
					timeOffset = tdtSectionPreTime - sectionInfo.stErrorInfo.startTime;
				}
			}

			/*timeoffset over 30s*/
			if (timeOffset >= 30000000)
			{
				errorInfo.tdtErrorInfo.overTimeError++;
				errorInfo.tdtErrorInfo.totalError++;
			}
			/*timeoffset within 25ms*/
			else if (timeOffset <= 25000)
			{
				errorInfo.tdtErrorInfo.limitTimeError++;
				errorInfo.tdtErrorInfo.totalError++;
			}
		}

		tdtSectionPreTime = sectionInfo.stErrorInfo.startTime;
	}

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_EmptyBufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time)
{
	if ((GOSTSR_NULL == data) || (dataLen == 0))
	{
		return GOSTSR_FAILURE;
	}

	/*TODO*/
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsCheck_DataDelayError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time)
{
	if ((GOSTSR_NULL == data) || (dataLen == 0))
	{
		return GOSTSR_FAILURE;
	}

	/*TODO*/
	
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_ThreeLevel_Init()
{
	memset(&errorInfo, 0x00, sizeof(TsThreeLevelErrorCheck_Info));

	/*nit check parameter init*/
	nitPreTime = 0x00;
	nitActualPreTime = 0x00;
	nitOtherPreTime = 0x00;

	/*si repetition rate check parameter init*/
	memset(&nitActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&nitOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&batSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&sdtActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&sdtOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitActualScheduleSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitOtherScheduleSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&tdtSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&totSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	
	/*unreferenced check parameter init*/
	memset(unreferPidInfo, 0x00, sizeof(UnreferencedPid_Info)*MAX_UNREFERPID_NUM);
	memset(&gstProgInfo,0x00,sizeof(SEARCH_INFO_S ));

	/*sdt check parameter init*/
	sdtPreTime = 0x00;
	sdtActualPreTime = 0x00;
	memset(sdtOtherPreTime, 0x00, sizeof(sdtOtherPreTime));

	/*eit check parameter init*/
	eitPreTime = 0x00;
	eitActualPreTime = 0x00;
	eitActualSec0PreTime = 0x00;
	eitActualSec1PreTime = 0x00;
	eitOtherSec0PreTime = 0x00;
	eitOtherSec1PreTime = 0x00;
	eitActualPairFlag = 0x00;
	eitOtherPairFlag = 0x00;
	
	/*rst check parameter init*/
	rstPreTime = 0x00;
	rstSectionPreTime = 0x00;

	/*sdt check parameter init*/
	tdtPreTime = 0x00;
	tdtSectionPreTime = 0x00;
	
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_ThreeLevel_DeInit()
{
	memset(&errorInfo, 0x00, sizeof(TsThreeLevelErrorCheck_Info));

	/*nit check parameter init*/
	nitPreTime = 0x00;
	nitActualPreTime = 0x00;
	nitOtherPreTime = 0x00;

	/*si repetition rate check parameter init*/
	memset(&nitActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&nitOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&batSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&sdtActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&sdtOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitActualSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitOtherSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitActualScheduleSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&eitOtherScheduleSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&tdtSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	memset(&totSectionInfo, 0x00, sizeof(SiRepetRateSections_Info));
	
	/*unreferenced check parameter init*/
	memset(unreferPidInfo, 0x00, sizeof(UnreferencedPid_Info)*MAX_UNREFERPID_NUM);
	memset(&gstProgInfo,0x00,sizeof(SEARCH_INFO_S ));

	/*sdt check parameter init*/
	sdtPreTime = 0x00;
	sdtActualPreTime = 0x00;
	memset(sdtOtherPreTime, 0x00, sizeof(sdtOtherPreTime));

	/*eit check parameter init*/
	eitPreTime = 0x00;
	eitActualPreTime = 0x00;
	eitActualSec0PreTime = 0x00;
	eitActualSec1PreTime = 0x00;
	eitOtherSec0PreTime = 0x00;
	eitOtherSec1PreTime = 0x00;
	eitActualPairFlag = 0x00;
	eitOtherPairFlag = 0x00;
	
	/*rst check parameter init*/
	rstPreTime = 0x00;
	rstSectionPreTime = 0x00;

	/*sdt check parameter init*/
	tdtPreTime = 0x00;
	tdtSectionPreTime = 0x00;
    return GOSTSR_SUCCESS;
}
GOSTSR_S32 TsErrorCheck_ThreeLevel_GetErrorInfo(TsThreeLevelErrorCheck_Info *info)
{
	if (GOSTSR_NULL == info)
	{
		return GOSTSR_FAILURE;
	}

	memset(info, 0x00, sizeof(TsThreeLevelErrorCheck_Info));
	memcpy(info, &errorInfo, sizeof(TsThreeLevelErrorCheck_Info));

	return GOSTSR_SUCCESS;
}
GOSTSR_S32 TsErrorCheck_ThreeLevel_setPid(SEARCH_INFO_S *pstProgInfo)
{
	if (GOSTSR_NULL == pstProgInfo)
	{
		return GOSTSR_FAILURE;
	}
	memset(&gstProgInfo, 0x00, sizeof(SEARCH_INFO_S));
	memcpy(&gstProgInfo,pstProgInfo,sizeof(SEARCH_INFO_S));
	
	return GOSTSR_SUCCESS;
}
GOSTSR_S32 TsErrorCheck_ThreeLevel_UnreferencedPidError(TS_HEAD_INFO tsHeadInfo)
{
	TsCheck_UnreferencedPidError(tsHeadInfo);
	return GOSTSR_SUCCESS;
}
GOSTSR_S32 TsErrorCheck_ThreeLevel_SectionsTimeError(TS_SECTION_INFO sectionInfo)
{
	
	TsCheck_NitSectionsTimeError(sectionInfo);
	TsCheck_SiSectionRepetitionRateError(sectionInfo);
	TsCheck_SdtActualSectionsTimeError(sectionInfo);
	TsCheck_SdtOtherSectionsTimeError(sectionInfo);
	TsCheck_EitActualSectionsTimeError(sectionInfo);
	TsCheck_EitOtherSectionsTimeError(sectionInfo);
	TsCheck_RstSectionsTimeError(sectionInfo);
	TsCheck_TdtSectionsTimeError(sectionInfo);

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_ThreeLevel_BufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time)
{
	TsCheck_BufferError(data, dataLen, time);
	TsCheck_EmptyBufferError(data, dataLen, time);
	TsCheck_DataDelayError(data, dataLen, time);
	
	return GOSTSR_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
