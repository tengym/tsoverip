#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"

#include "search.h"

static GOSTSR_U32 gu32BytePos_PatBak = 0;
static CONTINUTY_COUNT_INFO stContinutyCount[PID_NUMBER_MAX];

static PMTPID_TIMEOUT_INFO pmtPid_timeout_info;
static PESPID_TIMEOUT_INFO pesPid_timeout_info;
static TSERROR_ONELEVEL_RECORD oneLevelError_record = {0};  

static GOSTSR_BOOL Getts_checkEnable()
{
	return TsErrorCheck_TwoLevel_getEnableCheckFlag();
}

static GOSTSR_U32 Getts_Speed_Ms()
{
	return TsErrorCheck_TwoLevel_getAvTransportRate();
}

static GOSTSR_U32 Getts_TimeMs_byByteDiff(GOSTSR_U32 u32ByteDiff)
{
	GOSTSR_U32 timeMs = 0;
	GOSTSR_F32 f32timeMs = 0;
	GOSTSR_U32 u32AvTransportRate =  0;
	
	u32AvTransportRate  = TsErrorCheck_TwoLevel_getAvTransportRate();
	
	f32timeMs = (1.0*u32ByteDiff * 8 / u32AvTransportRate) *1000;
	
	timeMs = f32timeMs;
	
	return timeMs;
}

static GOSTSR_S32 TsErrorCheck_OneLevel_setPmtPid(SEARCH_INFO_S *stProgInfo)
{
	GOSTSR_S32 i = 0;

	if(stProgInfo == GOSTSR_NULL)
	{
		return GOSTSR_FAILURE;
	}

	pmtPid_timeout_info.NbPmtInfo = stProgInfo->u16NbProg;
	for(i = 0 ; i < stProgInfo->u16NbProg ; i++)
	{
		pmtPid_timeout_info.pmtInfo[i].pid = stProgInfo->stProgInfo[i].PmtPid;
	}
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_S32 TsErrorCheck_OneLevel_setPesPid(SEARCH_INFO_S *stProgInfo)
{
	GOSTSR_S32 i = 0, j =0, k = 0;

	if(stProgInfo == GOSTSR_NULL)
	{
		return GOSTSR_FAILURE;
	}
	
	for(i = 0; i < stProgInfo->u16NbProg;i++)
	{
		for(j =0; j  < stProgInfo->stProgInfo[i].u8NbPes; j++)
		{	
			pesPid_timeout_info.pesInfo[k++].pid = stProgInfo->stProgInfo[i].PesPid[j];
		}
	}
	pesPid_timeout_info.NbPesInfo = k;
	
	return GOSTSR_SUCCESS;
}

static GOSTSR_BOOL TsErrorCheck_OneLevel_checkisPesPid(GOSTSR_U16 Pid,GOSTSR_U32 *index)
{
	GOSTSR_S32 i = 0;

	for(i = 0; i < pesPid_timeout_info.NbPesInfo; i++)
	{
		if(pesPid_timeout_info.pesInfo[i].pid == Pid)
		{
			*index = i;
			return GOSTSR_TRUE;
		}
	}
	return GOSTSR_FALSE;
}
static GOSTSR_BOOL TsErrorCheck_OneLevel_checkisPmtPid(GOSTSR_U16 Pid,GOSTSR_U32 *index)
{
	GOSTSR_S32 i = 0;

	for(i = 0; i < pmtPid_timeout_info.NbPmtInfo; i++)
	{
		if(pmtPid_timeout_info.pmtInfo[i].pid == Pid)
		{
			*index = i;
			return GOSTSR_TRUE;
		}
	}
	return GOSTSR_FALSE;
}


GOSTSR_S32 TsErrorCheck_OneLevel_setPid(SEARCH_INFO_S *stProgInfo)
{
	TsErrorCheck_OneLevel_setPmtPid(stProgInfo);
	TsErrorCheck_OneLevel_setPesPid(stProgInfo);
	return GOSTSR_SUCCESS;
}

/*同步错误分为同步丢失错误和同步字节错*/
//(1).同步丢失错;返回值为总共统计的同步丢失错的个数

GOSTSR_S32 TsErrorCheck_OneLevel_SyncLossError(GOSTSR_U32 tmpSyncOffset)
{	
	oneLevelError_record.tsSyncLossError++;//同步丢失
	//TS_ERROR_LOG(TYPE_ERROR_ONE,TR101290_ONE_ERROR_SYNCLOSS,0,GOSTSR_NULL,GOSTSR_NULL);
	return GOSTSR_SUCCESS;
}

//(2)同步字节错，返回的值是不断累积的结果，
GOSTSR_S32 TsErrorCheck_OneLevel_SyncByteError(GOSTSR_U32 tmpSyncOffset)
{
	oneLevelError_record.tsSyncByteError++;//同步字节
	//TS_ERROR_LOG(TYPE_ERROR_ONE,TR101290_ONE_ERROR_SYNCBYTE,0,GOSTSR_NULL,GOSTSR_NULL);
	return GOSTSR_SUCCESS;
}
//(3)连续计数器错。返回的值是不断累积的结果
GOSTSR_S32 TsErrorCheck_OneLevel_ContinuityCounterError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 pid,GOSTSR_U8 Continuity,GOSTSR_U8  adapter_control,GOSTSR_U8 disContinuityIndicator)
{
	GOSTSR_S32 i = 0, j = 0;
	//GOSTSR_U32 index = 0;
	GOSTSR_BOOL bRepeat = GOSTSR_FALSE;
	
	if((pid == 0x1fff) || (0xff == pid))
	{
		return GOSTSR_FAILURE;
	}

#if 0
	if(!TsErrorCheck_OneLevel_checkisPesPid(pid,&index))
	{
		return GOSTSR_FAILURE;
	}
#endif

	for(i = 0; i < PID_NUMBER_MAX; i++)
	{
		if((stContinutyCount[i].pid == pid) && (stContinutyCount[i].bUsed))
		{
			break;
		}
	}
	if(i == PID_NUMBER_MAX)
	{
		for(j = 0; j < PID_NUMBER_MAX; j++)
		{
			if(!stContinutyCount[j].bUsed)
			{
				stContinutyCount[j].bUsed = GOSTSR_TRUE;
				stContinutyCount[j].pid = pid;
				stContinutyCount[j].continutycount = Continuity;
				stContinutyCount[j].predisContinuityIndicator= disContinuityIndicator;
				break;
			}
		}
		return GOSTSR_FAILURE;
	}

	if((stContinutyCount[i].continutycount == Continuity) && (Continuity!=0))//待用
	{
		bRepeat = GOSTSR_TRUE;	
	}

	if(Continuity != 0)
	{
		if(adapter_control == 0)
		{
			if(stContinutyCount[i].continutycount != Continuity)
			{
				oneLevelError_record.tsContinuityCounterError++;	
			}
		}
		else if(adapter_control == 1)
		{
			if(stContinutyCount[i].continutycount != Continuity - 1)
			{
				oneLevelError_record.tsContinuityCounterError++;	
			}
		}
		else if(adapter_control == 2)
		{
			if(disContinuityIndicator == 0)
			{
				if(stContinutyCount[i].continutycount != Continuity)
				{
					oneLevelError_record.tsContinuityCounterError++;
				}
			}
		}
		else if(adapter_control == 3)
		{
			if(disContinuityIndicator == 0)
			{
				if(stContinutyCount[i].predisContinuityIndicator == 0)
				{
					if(stContinutyCount[i].continutycount != Continuity -1)
					{
						oneLevelError_record.tsContinuityCounterError++;
					}
				}
			}
		}
		
	}
	else
	{
		if(adapter_control == 0)
			{
				if(stContinutyCount[i].continutycount != Continuity)
				{	
					oneLevelError_record.tsContinuityCounterError++;
				}
			}
			else if(adapter_control == 1)
			{
				if(stContinutyCount[i].continutycount != 0xf)
				{	
					oneLevelError_record.tsContinuityCounterError++;
				}
			}
			else if(adapter_control == 2)
			{
				if(disContinuityIndicator == 0)
				{
					if(stContinutyCount[i].continutycount != Continuity)
					{
						oneLevelError_record.tsContinuityCounterError++;
					}
				}
			}
			else if(adapter_control == 3)
			{
				if(disContinuityIndicator == 0)
				{
					if(stContinutyCount[i].predisContinuityIndicator == 0)
					{
						if(stContinutyCount[i].continutycount != 0xf)
						{	
							oneLevelError_record.tsContinuityCounterError++;
						}
					}
				}
			}
	}
	stContinutyCount[i].continutycount = Continuity;
	stContinutyCount[i].predisContinuityIndicator= disContinuityIndicator;
	
	return GOSTSR_FAILURE;
}

//(4)PAT错,三种错误，超时错，tableID错,加扰错
GOSTSR_S32 TsErrorCheck_OneLevel_PatError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 pid,GOSTSR_U32 tableId,GOSTSR_U8  scramble_control)
{
	GOSTSR_U32 	u32bytePosDiff = 0; //保存两个pat之间的包的个数
		
	if((pid != PAT_PID))
	{
		return GOSTSR_FAILURE;
	}

	//比较时间检查是否超时
	if(gu32BytePos_PatBak != 0)
	{
		if(pstErrorInfo->bCarryFlag)
		{
			u32bytePosDiff = ONELEVEL_BYTEPOS_MAX+pstErrorInfo->bytePos -gu32BytePos_PatBak;
		}
		else
		{
			if(pstErrorInfo->bytePos >= gu32BytePos_PatBak)
			{
				u32bytePosDiff = pstErrorInfo->bytePos -gu32BytePos_PatBak;
			}
			else
			{
				u32bytePosDiff = gu32BytePos_PatBak - pstErrorInfo->bytePos;
			}
		}
			
		gu32BytePos_PatBak = pstErrorInfo->bytePos;
		/*超时检测*/	
		if(Getts_Speed_Ms() > 0)
		{
			if(Getts_TimeMs_byByteDiff(u32bytePosDiff) > 500)
			{		
				oneLevelError_record.tsPatError.timeout_error++;
				oneLevelError_record.tsPatError.total_error++;
				return GOSTSR_SUCCESS;
			}
		}
			
	}
	else
	{
		gu32BytePos_PatBak = pstErrorInfo->bytePos;
	}
	
	if(pid == PAT_PID)
	{	
		if(tableId != PAT_TABLE_ID) //tabel id 错
		{
			oneLevelError_record.tsPatError.tableid_error++;	
			oneLevelError_record.tsPatError.total_error++;
		}
		else if(scramble_control != 0) //加扰错
		{
			oneLevelError_record.tsPatError.scramble_error++;
			oneLevelError_record.tsPatError.total_error++;
		}	
	}
	
	return GOSTSR_SUCCESS;		
}

//(5)PMT错两种情况，超时和加扰
GOSTSR_S32 TsErrorCheck_OneLevel_PmtError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 pid,GOSTSR_U8  scramble_control)
{
	GOSTSR_S32 i = 0;
	GOSTSR_U32 index = 0;
	GOSTSR_U32 u32BytePosDiff= 0;
		
	if(!Getts_checkEnable())
	{
		return GOSTSR_FAILURE;
	}

	/*检测PID是否是PMT_PID*/
	if(!TsErrorCheck_OneLevel_checkisPmtPid(pid,&index))
	{
		return GOSTSR_FAILURE;
	}
	/*加扰检测*/
	if(scramble_control != 0)
	{
		oneLevelError_record.tsPmtError.scramble_error++;
		oneLevelError_record.tsPmtError.total_error++;
	}
	
	if(pmtPid_timeout_info.pmtInfo[index].bytePos != 0)
	{
		if(pstErrorInfo->bCarryFlag)
		{
			u32BytePosDiff = ONELEVEL_BYTEPOS_MAX+pstErrorInfo->bytePos -pmtPid_timeout_info.pmtInfo[i].bytePos;
		}
		else
		{
			if(pstErrorInfo->bytePos >= pmtPid_timeout_info.pmtInfo[i].bytePos)
			{
				u32BytePosDiff = pstErrorInfo->bytePos -pmtPid_timeout_info.pmtInfo[i].bytePos;
			}
			else
			{
				u32BytePosDiff = pmtPid_timeout_info.pmtInfo[i].bytePos - pstErrorInfo->bytePos;
			}
		}
		
		pmtPid_timeout_info.pmtInfo[i].bytePos = pstErrorInfo->bytePos;
		/*超时检测*/
		//if(Getts_TimeMs_byByteDiff(u32BytePosDiff) > 100)
		//	printf("PMT_PID----Pos:%d---Pid = %#x----u32BytePosDiff = %d___time = %d\n",u32bytePos,pid,u32BytePosDiff,Getts_TimeMs_byByteDiff(u32BytePosDiff));
		
		if(Getts_Speed_Ms() > 0)
		{
			if(Getts_TimeMs_byByteDiff(u32BytePosDiff) > 500)
			{
			
				oneLevelError_record.tsPmtError.timeout_error ++;
				oneLevelError_record.tsPmtError.total_error ++;
			}
		}
		
	}
	else
	{
		pmtPid_timeout_info.pmtInfo[index].bytePos = pstErrorInfo->bytePos;
	}

	return GOSTSR_FAILURE;
}

//(6)PID错音视频PID 的发送间隔超过5S
GOSTSR_S32 TsErrorCheck_OneLevel_pidMissError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 pid)
{
	GOSTSR_U32 u32BytePosDiff = 0;
	GOSTSR_U32 index = 0;
	
	if(!Getts_checkEnable())
	{
		return GOSTSR_FAILURE;
	}
	
	if(!TsErrorCheck_OneLevel_checkisPesPid(pid,&index))
	{
		return GOSTSR_FAILURE;
	}
	
	if(pesPid_timeout_info.pesInfo[index].bytePos != 0) 
	{
		if(pstErrorInfo->bCarryFlag)
		{
			u32BytePosDiff = ONELEVEL_BYTEPOS_MAX+pstErrorInfo->bytePos -pesPid_timeout_info.pesInfo[index].bytePos;
		}
		else
		{
			if(pstErrorInfo->bytePos >= pesPid_timeout_info.pesInfo[index].bytePos)
			{
				u32BytePosDiff = pstErrorInfo->bytePos -pesPid_timeout_info.pesInfo[index].bytePos;
			}
			else
			{
				u32BytePosDiff = pesPid_timeout_info.pesInfo[index].bytePos - pstErrorInfo->bytePos;
			}
		}	
		
		pesPid_timeout_info.pesInfo[index].bytePos = pstErrorInfo->bytePos;
		/*超时检测*/
		if(Getts_Speed_Ms() > 0)
		{
			if(Getts_TimeMs_byByteDiff(u32BytePosDiff) > 5000)
			{					
				oneLevelError_record.tsPIDMissError++;
				return GOSTSR_SUCCESS;
			}
		}
	}
	else
	{
		pesPid_timeout_info.pesInfo[index].recvFlag = GOSTSR_TRUE;
		pesPid_timeout_info.pesInfo[index].bytePos = pstErrorInfo->bytePos;
		return GOSTSR_FAILURE;
	}

	return GOSTSR_FAILURE;
}
/*检测没有到达的PID*/
GOSTSR_S32 TsErrorCheck_OneLevel_pidMissNotReach()
{
	GOSTSR_S32 i = 0;
	for(i = 0 ; i < pesPid_timeout_info.NbPesInfo;i++)
	{
		if(!pesPid_timeout_info.pesInfo[i].recvFlag)
		{		
			oneLevelError_record.tsPIDMissError++;
		}
	}
	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_OneLevel_GetErrorInfo(TSERROR_ONELEVEL_RECORD *oneLevelErrorInfo)
{
	if(oneLevelErrorInfo == NULL)
		return GOSTSR_FAILURE;
	
	memcpy(oneLevelErrorInfo,&oneLevelError_record,sizeof(TSERROR_ONELEVEL_RECORD));
	
	return GOSTSR_SUCCESS;
} 

GOSTSR_S32 TsErrorCheck_OneLevel_Init()
{
	GOSTSR_S32 i = 0;
	
	memset(&oneLevelError_record, 0x00, sizeof(TSERROR_ONELEVEL_RECORD));
	memset(&pmtPid_timeout_info, 0x00, sizeof(PMTPID_TIMEOUT_INFO));
	memset(&pesPid_timeout_info, 0x00, sizeof(PESPID_TIMEOUT_INFO));

	for(i = 0; i < MAX_PMT_NUMS; i++)
	{
		pmtPid_timeout_info.pmtInfo[i].pid = 0xffff;
		pmtPid_timeout_info.pmtInfo[i].recvFlag = GOSTSR_FALSE;
	}
	
	for(i = 0; i < MAX_PES_NUMS; i++)
	{
		pesPid_timeout_info.pesInfo[i].pid = 0xffff;
		pesPid_timeout_info.pesInfo[i].recvFlag = GOSTSR_FALSE;
	}
	
	for(i = 0; i < PID_NUMBER_MAX; i++)
	{
		stContinutyCount[i].bUsed = GOSTSR_FALSE;
		stContinutyCount[i].pid = 0xffff;
	}

	gu32BytePos_PatBak = 0;

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsErrorCheck_OneLevel_DeInit()
{
	GOSTSR_S32 i = 0;
	
	memset(&oneLevelError_record, 0x00, sizeof(TSERROR_ONELEVEL_RECORD));
	memset(&pmtPid_timeout_info, 0x00, sizeof(PMTPID_TIMEOUT_INFO));
	memset(&pesPid_timeout_info, 0x00, sizeof(PESPID_TIMEOUT_INFO));

	for(i = 0; i < MAX_PMT_NUMS; i++)
	{
		pmtPid_timeout_info.pmtInfo[i].pid = 0xffff;
		pmtPid_timeout_info.pmtInfo[i].recvFlag = GOSTSR_FALSE;
	}
	
	for(i = 0; i < MAX_PES_NUMS; i++)
	{
		pesPid_timeout_info.pesInfo[i].pid = 0xffff;
		pesPid_timeout_info.pesInfo[i].recvFlag = GOSTSR_FALSE;
	}
	
	for(i = 0; i < PID_NUMBER_MAX; i++)
	{
		stContinutyCount[i].bUsed = GOSTSR_FALSE;
		stContinutyCount[i].pid = 0xffff;
	}

	gu32BytePos_PatBak = 0;
	return GOSTSR_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
