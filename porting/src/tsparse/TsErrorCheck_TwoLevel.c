#include "TsErrorCheck_TwoLevel.h"
#include "GosTsr_AnalysisData.h"
#include "search.h"


static GOSTSR_BOOL gbTsRateisRecved = GOSTSR_FALSE;

static TSERROR_TWOLEVEL_S stTwoLevel;
static TwoLevel_TsRateDis_Info stTsRateInfo[PCRPID_NUM_MAX];
static TwoLevel_PcrDis_Info stPcrInfo[PCRPID_NUM_MAX];
static TwoLevel_PesDis_Info stPesInfo[PESPID_NUM_MAX];

GOSTSR_BOOL TsErrorCheck_TwoLevel_getEnableCheckFlag()
{
	return gbTsRateisRecved;
}

/*获取平均码率，单位:bit/s*/
GOSTSR_U32 TsErrorCheck_TwoLevel_getAvTransportRate()
{
	GOSTSR_U64 u64TransportRate_All =  0;
	GOSTSR_U32 u32AvTransportRate =  0;
	GOSTSR_U32 u32TransportRate_Max =  0;
	GOSTSR_U32 u32TransportRate_Min=  0;
	GOSTSR_U32 i = 0;
	GOSTSR_U16 u16Count = 0;
	GOSTSR_U8 u8CarryBit = 0;
	//static GOSTSR_U32 AvTransportRate = 0;

	//if(AvTransportRate != 0)
		//return AvTransportRate;
	
	for(i = 0; i < PCRPID_NUM_MAX; i++)
	{
		if(stTsRateInfo[i].u32TransportRate != 0)
		{
			u32TransportRate_Max = stTsRateInfo[i].u32TransportRate;
			u32TransportRate_Min = u32TransportRate_Max;
			break;
		}
	}

	for(i = 0; i < PCRPID_NUM_MAX; i++)
	{
		if((stTsRateInfo[i].u32TransportRate == 0) || (stTsRateInfo[i].bFirstUsed))
			continue;

		if(u32TransportRate_Max < stTsRateInfo[i].u32TransportRate)
		{
			u32TransportRate_Max = stTsRateInfo[i].u32TransportRate;
		}
		if(u32TransportRate_Min > stTsRateInfo[i].u32TransportRate)
		{
			u32TransportRate_Min = stTsRateInfo[i].u32TransportRate;
		}
		
		if((u64TransportRate_All+stTsRateInfo[i].u32TransportRate) >= TWOLEVEL_INVALID_U32)
		{
			u64TransportRate_All = stTsRateInfo[i].u32TransportRate-(TWOLEVEL_INVALID_U32-u64TransportRate_All);
			u8CarryBit++; /*进位标志*/
		}	
		else
		{
			u64TransportRate_All += stTsRateInfo[i].u32TransportRate;
		}

		u16Count++;	
		
	}
	
	if(u16Count != 0)
	{
		if(u16Count > 2)
		{
			/*除去最大值和最小值,再求平均值*/
			u32AvTransportRate = (u8CarryBit*TWOLEVEL_INVALID_U32 + u64TransportRate_All-u32TransportRate_Min-u32TransportRate_Max) / (u16Count-2);		
			//printf("u32AvTransportRate = %d\n",u32AvTransportRate);
		}
		else
		{
			u32AvTransportRate = (u8CarryBit*TWOLEVEL_INVALID_U32 + u64TransportRate_All) / (u16Count);	
		}			
	}

	//printf("f32TsRate_u16Count = %d\n",u16Count);
	//AvTransportRate = u32AvTransportRate;
	return u32AvTransportRate;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_getTwoLevelError(TSERROR_TWOLEVEL_S *pTwoLevel)
{
	if(pTwoLevel == GOSTSR_NULL)
	{
		return GOSTSR_FAILURE;
	}
	memcpy(pTwoLevel, &stTwoLevel, sizeof(TSERROR_TWOLEVEL_S));

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsErrorCheck_TwoLevel_setPcrPid(SEARCH_INFO_S *stProgInfo)
{
	GOSTSR_S32 i = 0,k = 0, count = 0;

	for(i = 0; i < stProgInfo->u16NbProg;i++)
	{
		for(k = 0; k < i; k++)
		{
			if(stPcrInfo[k].u16PcrPid == stProgInfo->stProgInfo[i].PcrPid)
			{
				//printf("u16PcrPid = %#x___k = %d\n",stPcrInfo[k].u16PcrPid ,k);
				break;
			}
		}
		if(k == i)
		{
			stPcrInfo[count++].u16PcrPid = stProgInfo->stProgInfo[i].PcrPid;
		}
	}
	//printf("PCR________Count = %d\n",count);

	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsErrorCheck_TwoLevel_setPesPid(SEARCH_INFO_S *stProgInfo)
{
	GOSTSR_S32 i = 0, j =0, k = 0;

	for(i = 0; i < stProgInfo->u16NbProg;i++)
	{
		for(j =0; j  < stProgInfo->stProgInfo[i].u8NbPes;j++)
		{	
			stPesInfo[k++].u16PesPid = stProgInfo->stProgInfo[i].PesPid[j];
		}
	}
	//printf("PES________Count = %d\n",k);
		
	return GOSTSR_SUCCESS;
}

static GOSTSR_BOOL TwoLevel_checkisPcrPid(GOSTSR_U16 Pid,GOSTSR_U32 *index)
{
	GOSTSR_S32 i = 0;

	for(i = 0; i < PCRPID_NUM_MAX; i++)
	{
		if(Pid == stPcrInfo[i].u16PcrPid)
		{
			*index = i;
			return GOSTSR_TRUE;
		}
	}
	
	return GOSTSR_FALSE;
}

static GOSTSR_BOOL TwoLevel_checkisPesPid(GOSTSR_U16 Pid,GOSTSR_U32 *index)
{
	GOSTSR_S32 i = 0;

	for(i = 0; i < PESPID_NUM_MAX; i++)
	{
		if(Pid == stPesInfo[i].u16PesPid)
		{
			*index = i;
			return GOSTSR_TRUE;
		}
	}
	
	return GOSTSR_FALSE;
}

static GOSTSR_F32 TwoLevel_getTsRateMs()
{
	GOSTSR_F32 f32AvTransportRate = 0.0;
	
	if(!gbTsRateisRecved)
		return 0.0;
	
	f32AvTransportRate = 1.0 * TsErrorCheck_TwoLevel_getAvTransportRate() / 1000;
	//printf("_______ f32AvTransportRate = %f\n",f32AvTransportRate );
	return f32AvTransportRate;
}
static GOSTSR_F32 TwoLevel_getTsRateUs()
{
	GOSTSR_F32 f32AvTransportRate = 0.0;
	
	if(!gbTsRateisRecved)
		return 0.0;
	
	f32AvTransportRate = 1.0 * TsErrorCheck_TwoLevel_getAvTransportRate() / 1000 / 1000;
	//printf("_______ f32AvTransportRate = %f\n",f32AvTransportRate );
	return f32AvTransportRate;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_setPid(SEARCH_INFO_S *stProgInfo)
{
	TsErrorCheck_TwoLevel_setPcrPid(stProgInfo);
	TsErrorCheck_TwoLevel_setPesPid(stProgInfo);
	return GOSTSR_SUCCESS;
}

//static GOSTSR_U32 TwoLevel_getTime(GOSTSR_U32 u32BytePos)
/*通过输入TS包的序号可以获取当前包的时间*/
GOSTSR_U32 TsErrorCheck_TwoLevel_getTimeUs_byBytePos(GOSTSR_U32 u32BytePos)
{
	GOSTSR_F32 f32TsRateUs= 0.0;
	GOSTSR_U32 u32TimeUsCur = 0; /*Unit:us*/

	if(!gbTsRateisRecved)
	{
		return u32TimeUsCur;
	}
	
	f32TsRateUs = TwoLevel_getTsRateUs();
	u32TimeUsCur = (1.0 * u32BytePos * 8) / f32TsRateUs;
	
	return u32TimeUsCur;
}
GOSTSR_S32 TsErrorCheck_TwoLevel_setTransportRate(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 index,GOSTSR_U64 u64Pcr_Base, GOSTSR_U16 u64Pcr_Ext)
{
	GOSTSR_U32 u32PrePos = 0;
	GOSTSR_U32 u32CurPos = 0;
	GOSTSR_U32 u32AvTransportRate = 0;
	GOSTSR_U64 u64PrePcrBaseValue = 0;
	GOSTSR_U64 u64CurPcrBaseValue = 0;
	GOSTSR_U16 u16PrePcrExtenValue = 0;
	GOSTSR_U16 u16CurPcrExtenValue = 0;

	if(stTsRateInfo[index].bFirstUsed)
	{
		stTsRateInfo[index].bFirstUsed = GOSTSR_FALSE;
		stTsRateInfo[index].u32BytePos= pstErrorInfo->bytePos;
		stTsRateInfo[index].u64PcrBase = u64Pcr_Base;
		stTsRateInfo[index].u16PcrExt = u64Pcr_Ext;
		return GOSTSR_FAILURE;
	}
	
	u32PrePos = stTsRateInfo[index].u32BytePos;
	u64PrePcrBaseValue = stTsRateInfo[index].u64PcrBase;
	u16PrePcrExtenValue = stTsRateInfo[index].u16PcrExt;
	
	u32CurPos = pstErrorInfo->bytePos;
	u64CurPcrBaseValue = u64Pcr_Base;
	u16CurPcrExtenValue = u64Pcr_Ext;
	
	if (u64CurPcrBaseValue < u64PrePcrBaseValue)
	{
		if(u32CurPos < u32PrePos)
		{	
			u32AvTransportRate = (TWOLEVEL_BYTEPOS_MAX - u32PrePos +  u32CurPos)*8*27000000.0/((0x1ffffffffULL - u64PrePcrBaseValue + u64CurPcrBaseValue)*300 + u16CurPcrExtenValue - u16PrePcrExtenValue);
		}
		else
		{	
			u32AvTransportRate = (u32CurPos - u32PrePos)*8*27000000.0/((0x1ffffffffULL - u64PrePcrBaseValue + u64CurPcrBaseValue)*300 + u16CurPcrExtenValue - u16PrePcrExtenValue);
		}
	}
	else
	{
		if(u32CurPos < u32PrePos)
		{		
			u32AvTransportRate = (GOSTSR_U32)((TWOLEVEL_BYTEPOS_MAX - u32PrePos +  u32CurPos)*8*27000000.0/((u64CurPcrBaseValue - u64PrePcrBaseValue)*300 + u16CurPcrExtenValue - u16PrePcrExtenValue));
		}
		else
		{
			u32AvTransportRate = (u32CurPos - u32PrePos)*8*27000000.0/((u64CurPcrBaseValue -u64PrePcrBaseValue)*300 + u16CurPcrExtenValue - u16PrePcrExtenValue);
		}
	}

	if ((stTsRateInfo[index].u32TransportRate != 0) && (abs(stTsRateInfo[index].u32TransportRate - u32AvTransportRate) > 1000000))
	{
		//printf("pre = %d----cur = %d\n",stTsRateInfo[index].u32TransportRate ,u32AvTransportRate);
		return GOSTSR_FAILURE;
	}
	
	stTsRateInfo[index].u32TransportRate = u32AvTransportRate;	/*保存这一路节目的码率*/
	stTsRateInfo[index].u32BytePos = pstErrorInfo->bytePos;
	stTsRateInfo[index].u64PcrBase = u64Pcr_Base;
	stTsRateInfo[index].u16PcrExt = u64Pcr_Ext;

	gbTsRateisRecved = GOSTSR_TRUE;
	
	return GOSTSR_SUCCESS;
}


GOSTSR_S32 TsErrorCheck_TwoLevel_checkTransportError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U8 u8Error_indicater)
{
	if(u8Error_indicater == 0)
	{
		return GOSTSR_FAILURE;
	}
	stTwoLevel.u32TsErrorCount++;
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_checkCrcError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U8 u8TableID,GOSTSR_U32 u32TableCrc,GOSTSR_U32 u32CalCrc)
{
	if(u32TableCrc == u32CalCrc)
	{
		return GOSTSR_FAILURE;
	}
	switch(u8TableID)
	{
		case PAT_TABLE_ID:
			stTwoLevel.stCrcErrorInfo.patCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case PMT_TABLE_ID:
			stTwoLevel.stCrcErrorInfo.pmCrctErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case CAT_TABLE_ID:
			stTwoLevel.stCrcErrorInfo.catCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case NIT_TABLE_ID_ACTUAL:
		case NIT_TABLE_ID_OTHER:
			stTwoLevel.stCrcErrorInfo.nitCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case BAT_TABLE_ID:
			stTwoLevel.stCrcErrorInfo.batCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case SDT_TABLE_ID_ACTUAL:
		case SDT_TABLE_ID_OTEHR:
			stTwoLevel.stCrcErrorInfo.sdtCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case EIT_TABLE_ID_ACTUAL:
		case EIT_TABLE_ID_ACTUAL_SHEDULE:
		case EIT_TABLE_ID_OTHER:
		case EIT_TABLE_ID_OTHER_SHEDULE:
			stTwoLevel.stCrcErrorInfo.eitCrcErrorCount++;
			stTwoLevel.stCrcErrorInfo.totalCrcErrorCount++;
			break;
		case TDT_TABLE_ID:
		case TOT_TABLE_ID:	
		case RST_TABLE_ID:
		case SI_TABLE_ID:
			break;
		default:
			break;
	}
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_checkPcrDiscontError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 index,GOSTSR_U64 u64Pcr_Base, GOSTSR_U16 u16Pcr_Ext,GOSTSR_BOOL *bErrorFlag)
{
	GOSTSR_U32 u32PcrValueNs_Diff = 0;
	GOSTSR_U32 u32PcrValueMs_Diff = 0;

	/*若是第一次使用，先备份数据*/
	if(stPcrInfo[index].bFirstUsed)
	{
		stPcrInfo[index].bFirstUsed = GOSTSR_FALSE;
		stPcrInfo[index].u64PcrBase = u64Pcr_Base;
		stPcrInfo[index].u16PcrExt = u16Pcr_Ext;
		stPcrInfo[index].u32PcrValueNs = TWOLEVEL_INVALID_U32;
		*bErrorFlag = GOSTSR_FALSE;
		return GOSTSR_FAILURE;
	}
	else
	{
		if(u64Pcr_Base >= stPcrInfo[index].u64PcrBase)
		{
			if(u16Pcr_Ext >= stPcrInfo[index].u16PcrExt)
			{
				u32PcrValueNs_Diff = ((u64Pcr_Base-stPcrInfo[index].u64PcrBase)*300+(u16Pcr_Ext-stPcrInfo[index].u16PcrExt))*1000/27;
			}
			else
			{
				u32PcrValueNs_Diff = ((u64Pcr_Base-1-stPcrInfo[index].u64PcrBase)*300+(0x1ff+u16Pcr_Ext-stPcrInfo[index].u16PcrExt))*1000/27;
			}
		}
		else
		{
			if(u16Pcr_Ext >= stPcrInfo[index].u16PcrExt)
			{
				u32PcrValueNs_Diff = ((0x1ffffffffULL - stPcrInfo[index].u64PcrBase+u64Pcr_Base)*300+(u16Pcr_Ext-stPcrInfo[index].u16PcrExt))*1000/27;
			}
			else
			{			
				u32PcrValueNs_Diff = ((0x1ffffffffULL - stPcrInfo[index].u64PcrBase+(u64Pcr_Base-1))*300+(0x1ff+u16Pcr_Ext-stPcrInfo[index].u16PcrExt))*1000/27;
			}
		}

		stPcrInfo[index].u32PcrValueNs = u32PcrValueNs_Diff;
		stPcrInfo[index].u64PcrBase = u64Pcr_Base;
		stPcrInfo[index].u16PcrExt = u16Pcr_Ext;
		
		/*ns 转换为 ms*/
		u32PcrValueMs_Diff = u32PcrValueNs_Diff / 1000 /1000;
		if(u32PcrValueMs_Diff  > 40)
		{
			//printf("Pcr_Pid = %#x___Byte = %d___u32PcrDiscont_Diff = %d\n",stPcrInfo[index].u16PcrPid,u32BytePos,u32PcrValueMs_Diff);
			
			stTwoLevel.u32PcrDisErrorCount++;
			*bErrorFlag = GOSTSR_TRUE;
		}
	}
	
	return GOSTSR_SUCCESS;
}

//PCR抖动算法有待优化
GOSTSR_S32 TsErrorCheck_TwoLevel_checkPcrJitterError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 index,GOSTSR_U64 u64Pcr_Base, GOSTSR_U16 u64Pcr_Ext)
{
	GOSTSR_U32 u32PcrValuePcr_Ns = 0;	/*两个PCR之间的差值*/
	GOSTSR_U32 u32PcrValueBit_Ns = 0;	/*两个Bit之间的差值*/	
	GOSTSR_U32 u32byteOffset = 0;
	GOSTSR_F32 f32TsRateUs = 0.0;

	
	if((stPcrInfo[index].u32BytePos == 0) || (stPcrInfo[index].u32PcrValueNs == TWOLEVEL_INVALID_U32)) /*第一个PCR的值，尚不能进行比较*/
	{
		stPcrInfo[index].u32BytePos = pstErrorInfo->bytePos;
		return GOSTSR_FAILURE;
	}
	else
	{
		/*获取连续两个PCR之间通过bit计算得到的PCR时间间隔,Unit:us*/
		u32PcrValuePcr_Ns =stPcrInfo[index].u32PcrValueNs;
		
		f32TsRateUs = TwoLevel_getTsRateUs(); /*通过平均码率计算*/
		if(f32TsRateUs == 0.0)
		{	
			return GOSTSR_FAILURE;
		}	

		if(pstErrorInfo->bCarryFlag)
		{
			u32byteOffset = TWOLEVEL_BYTEPOS_MAX+pstErrorInfo->bytePos - stPcrInfo[index].u32BytePos;
		}
		else
		{
			if(pstErrorInfo->bytePos >=  stPcrInfo[index].u32BytePos)
			{
				u32byteOffset = pstErrorInfo->bytePos - stPcrInfo[index].u32BytePos;
			}
			else
			{
				u32byteOffset =  stPcrInfo[index].u32BytePos - pstErrorInfo->bytePos;
			}
		}
		stPcrInfo[index].u32BytePos = pstErrorInfo->bytePos;
		
		u32PcrValueBit_Ns = (GOSTSR_U32)(1.0*u32byteOffset*8*1000/f32TsRateUs);	
		
		if(abs(u32PcrValueBit_Ns - u32PcrValuePcr_Ns) > 500)
		{
			stTwoLevel.u32PcrJitErrorCount++;
		}
	}
	
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_checkPcrError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 u16PcrPid,GOSTSR_U64 u64Pcr_Base, GOSTSR_U16 u64Pcr_Ext, GOSTSR_BOOL flag)
{
	GOSTSR_U32 index = 0;
	GOSTSR_BOOL bErrorFlag = GOSTSR_FALSE;
	
	if (!flag)
	{
		//printf("$$$$$$$$$$$$---------------->\n");
	}
	/*检测是不是PCR_PID*/
	if(!TwoLevel_checkisPcrPid(u16PcrPid, &index))
	{
		return GOSTSR_FAILURE;
	}

	/*不连续状态标志为1*/
	if (!flag)
	{
		stTsRateInfo[index].bFirstUsed = GOSTSR_TRUE;
		stTsRateInfo[index].u32TransportRate = 0;
		stPcrInfo[index].bFirstUsed = GOSTSR_TRUE;
		stPcrInfo[index].u32BytePos = 0;
		return GOSTSR_FAILURE;
	}

	/*PCR间隔检测*/
	TsErrorCheck_TwoLevel_checkPcrDiscontError(pstErrorInfo,index,u64Pcr_Base,u64Pcr_Ext,&bErrorFlag);
	
	/*PCR抖动检测*/
	TsErrorCheck_TwoLevel_checkPcrJitterError(pstErrorInfo,index,u64Pcr_Base,u64Pcr_Ext);

	if(!bErrorFlag)
	{
		/*码率的设置*/
		stTsRateInfo[index].u16PcrPid = u16PcrPid;
		TsErrorCheck_TwoLevel_setTransportRate(pstErrorInfo,index,u64Pcr_Base,u64Pcr_Ext);		
	}

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_checkPtsError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 u16PesPid)
{
	GOSTSR_U32 u32PtsTime = 0; /*Unit: ms*/
	GOSTSR_F32 f32PcrMsRate = 0;
	GOSTSR_U32 index = 0;
	GOSTSR_U32 u32byteOffset = 0;

	if(!gbTsRateisRecved)
	{
		return 0;
	}
	/*检测是不是Pes_PID,返回索引*/
	if(!TwoLevel_checkisPesPid(u16PesPid, &index))
	{
		return GOSTSR_FAILURE;
	}
	
	if((stPesInfo[index].bFirstUsed) || (stPesInfo[index].u32BytePos == 0))	/*当前包的字节数*/
	{
		stPesInfo[index].bFirstUsed = GOSTSR_FALSE;
		stPesInfo[index].u32BytePos = pstErrorInfo->bytePos;
		return GOSTSR_FAILURE;
	}
	
	f32PcrMsRate = TwoLevel_getTsRateMs();
	if(f32PcrMsRate == 0.0)
	{	
		return GOSTSR_FAILURE;
	}
	
	//printf("f32PcrMsRate = %f\n",f32PcrMsRate);
	if(pstErrorInfo->bCarryFlag)
	{
		u32byteOffset = TWOLEVEL_BYTEPOS_MAX+pstErrorInfo->bytePos - stPesInfo[index].u32BytePos;
	}
	else
	{
		if(pstErrorInfo->bytePos >=  stPesInfo[index].u32BytePos)
		{
			u32byteOffset = pstErrorInfo->bytePos - stPesInfo[index].u32BytePos;
		}
		else
		{
			u32byteOffset =  stPesInfo[index].u32BytePos - pstErrorInfo->bytePos;
		}
	}
	stPesInfo[index].u32BytePos  = pstErrorInfo->bytePos;

	u32PtsTime =(GOSTSR_U32)(1.0 * u32byteOffset * 8 / f32PcrMsRate);
	if(u32PtsTime > 700)
	{
		//printf("u32PtsTime_____________u32PtsTime = %d\n",u32PtsTime);
		stTwoLevel.u32PtsErrorCount++;
		return GOSTSR_SUCCESS;
	}
	return GOSTSR_FAILURE;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_checkCatError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 u16Pid, GOSTSR_U8 u8TableID, GOSTSR_U8 u8Scramble)
{
	if(u16Pid != CAT_PID)
	{
		return GOSTSR_FAILURE;
	}
	if(u8TableID != CAT_TABLE_ID)
	{
		stTwoLevel.stCatErrorInfo.tableIDErrorCount++;
		stTwoLevel.stCatErrorInfo.totalCatErrorCount++;
	}

	if(u8Scramble != 0)
	{
		stTwoLevel.stCatErrorInfo.scambleErrorCount++;
		stTwoLevel.stCatErrorInfo.totalCatErrorCount++;
	}
	
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_Init()
{
	GOSTSR_S32 i = 0;
	
	memset(&stTwoLevel,0 , sizeof(TSERROR_TWOLEVEL_S));
	memset(&stTsRateInfo,0 , sizeof(TwoLevel_TsRateDis_Info) * PCRPID_NUM_MAX);
	memset(&stPcrInfo,0 , sizeof(TwoLevel_PcrDis_Info) * PCRPID_NUM_MAX);
	memset(&stPesInfo,0 , sizeof(TwoLevel_PesDis_Info) * PESPID_NUM_MAX);

	for(i = 0 ; i < PCRPID_NUM_MAX; i++)
	{
		stPcrInfo[i].bFirstUsed = GOSTSR_TRUE;
		stPcrInfo[i].u16PcrPid = TWOLEVEL_INVALID_U16;

		stTsRateInfo[i].bFirstUsed = GOSTSR_TRUE;
		stTsRateInfo[i].u16PcrPid = TWOLEVEL_INVALID_U16;
	}
	for(i = 0 ; i < PESPID_NUM_MAX; i++)
	{
		stPesInfo[i].bFirstUsed = GOSTSR_TRUE;
		stPesInfo[i].u16PesPid = TWOLEVEL_INVALID_U16;
	}
	gbTsRateisRecved = GOSTSR_FALSE;
	
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_TwoLevel_DeInit()
{
	GOSTSR_S32 i = 0;
	
	memset(&stTwoLevel,0 , sizeof(TSERROR_TWOLEVEL_S));
	memset(&stTsRateInfo,0 , sizeof(TwoLevel_TsRateDis_Info) * PCRPID_NUM_MAX);
	memset(&stPcrInfo,0 , sizeof(TwoLevel_PcrDis_Info) * PCRPID_NUM_MAX);
	memset(&stPesInfo,0 , sizeof(TwoLevel_PesDis_Info) * PESPID_NUM_MAX);

	for(i = 0 ; i < PCRPID_NUM_MAX; i++)
	{
		stPcrInfo[i].bFirstUsed = GOSTSR_TRUE;
		stPcrInfo[i].u16PcrPid = TWOLEVEL_INVALID_U16;

		stTsRateInfo[i].bFirstUsed = GOSTSR_TRUE;
		stTsRateInfo[i].u16PcrPid = TWOLEVEL_INVALID_U16;
	}
	for(i = 0 ; i < PESPID_NUM_MAX; i++)
	{
		stPesInfo[i].bFirstUsed = GOSTSR_TRUE;
		stPesInfo[i].u16PesPid = TWOLEVEL_INVALID_U16;
	}
	gbTsRateisRecved = GOSTSR_FALSE;
	return GOSTSR_SUCCESS;
}

