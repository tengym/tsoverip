#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef _TSERRORCHECK_ONELEVEL_H
#define _TSERRORCHECK_ONELEVEL_H

#include "TsErrorCheck_Common.h"
#include "GosTsr_AnalysisData.h"
#include "tsparse_search.h"

#define PACKAGE_LEN		188
#define MAX_PMT_NUMS	256
#define MAX_AVPID_NUMS	256
#define CONTINUITYCOUNTER_MAX	15
#define PID_NUMBER_MAX	0x1fff
#define ONELEVEL_BYTEPOS_MAX 0xffffffff


#define MAX_PES_NUMS		(MAX_PMT_NUMS*20)
typedef struct
{
	GOSTSR_U32 timeout_error; //����pat��֮���ʱ�䳬��0.5��
	GOSTSR_U32 tableid_error; //tabelId is not 0x000
	GOSTSR_U32 scramble_error; //pat ����
	GOSTSR_U32 total_error;
}ONELEVEL_PATERROR;

typedef struct
{
	GOSTSR_U32 timeout_error; //����pmt��֮���ʱ�䳬��0.5��
	GOSTSR_U32 scramble_error; //pmt ����
	GOSTSR_U32 total_error;
}ONELEVEL_PMTERROR;

/*
typedef struct
{
	GOSTSR_U32 timeout_error; 		//��������Ƶ��֮���ʱ�䳬��0.5��
	GOSTSR_U32 pidNotExist_error; 	//����ƵPID ������
	GOSTSR_U32 total_error;
}ONELEVEL_PIDERROR;
*/

typedef struct
{
	GOSTSR_U32 tsSyncLossError;	//ͬ����ʧ��
	GOSTSR_U32 tsSyncByteError;	//ͬ���ֽڴ�
	GOSTSR_U32 tsContinuityCounterError;//����������

	ONELEVEL_PATERROR tsPatError;		// PAT��
	ONELEVEL_PMTERROR tsPmtError;		//PMT ��
	GOSTSR_U32 tsPIDMissError;			//PID��ʧ��
}TSERROR_ONELEVEL_RECORD;//һ������ļ�¼

typedef struct
{
	GOSTSR_BOOL bUsed;
	GOSTSR_U16 pid;
	GOSTSR_U8 continutycount;
	GOSTSR_BOOL predisContinuityIndicator;
}CONTINUTY_COUNT_INFO;



typedef struct
{
	GOSTSR_U32	bytePos;
	GOSTSR_U16	pid;
	GOSTSR_BOOL	recvFlag;
}PID_TIMEOUT_INFO;

typedef struct
{
	GOSTSR_U16			NbPmtInfo;
	PID_TIMEOUT_INFO	pmtInfo[MAX_PMT_NUMS];
}PMTPID_TIMEOUT_INFO;

typedef struct
{
	GOSTSR_U16			NbPesInfo;
	PID_TIMEOUT_INFO	pesInfo[MAX_PES_NUMS];
}PESPID_TIMEOUT_INFO;

extern GOSTSR_S32 TsErrorCheck_OneLevel_Init();
extern GOSTSR_S32 TsErrorCheck_OneLevel_DeInit();
extern GOSTSR_S32 TsErrorCheck_OneLevel_GetErrorInfo(TSERROR_ONELEVEL_RECORD *oneLevelErrorInfo);
extern GOSTSR_S32 TsErrorCheck_OneLevel_PatError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 pid,GOSTSR_U32 tableId,GOSTSR_U8  scramble_control);
extern GOSTSR_S32 TsErrorCheck_OneLevel_PmtError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 pid,GOSTSR_U8  scramble_control);
extern GOSTSR_S32 TsErrorCheck_OneLevel_pidMissError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U16 pid);
extern GOSTSR_S32 TsErrorCheck_OneLevel_ContinuityCounterError(TR101290_ERROR_S *pstErrorInfo,GOSTSR_U32 pid,GOSTSR_U8 Continuity,GOSTSR_U8  adapter_control,GOSTSR_U8 disContinuityIndicator);

extern GOSTSR_S32 TsErrorCheck_OneLevel_SyncLossError(GOSTSR_U32 tmpSyncOffset);
extern GOSTSR_S32 TsErrorCheck_OneLevel_SyncByteError(GOSTSR_U32 tmpSyncOffset);

extern GOSTSR_S32 TsErrorCheck_OneLevel_setPid(SEARCH_INFO_S *stProgInfo);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
