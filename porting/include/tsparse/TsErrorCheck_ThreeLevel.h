#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef __TSERRORCHECK_THREELEVEL_H__
#define __TSERRORCHECK_THREELEVEL_H__

#include "TsErrorCheck_Common.h"
#include "GosTsr_AnalysisData.h"
#include "tsparse_search.h"

#define MAX_PMT_NUMS	256
#define MAX_CAT_NUMS	10

/*nit error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 tableIDError;
	GOSTSR_U16 actualOverTimeError;
	GOSTSR_U16 actualLimitTimeError;
	GOSTSR_U16 otherOverTimeError;
	GOSTSR_U16 otherLimitTimeError;
}NitError_Info;

/*si repetition rate error information*/
typedef struct
{
	GOSTSR_U32 startTime;
	GOSTSR_U8  sectionFlag[256];
	GOSTSR_U8  lastSectionNum;
	GOSTSR_U8  number;
}SiRepetRateSections_Info;

typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 siSectionLimitTimeError;
	GOSTSR_U16 nitOverTimeError;
	GOSTSR_U16 batOverTimeError;
	GOSTSR_U16 sdtActualOverTimeError;
	GOSTSR_U16 sdtOtherOverTimeError;
	GOSTSR_U16 eitActualOverTimeError;
	GOSTSR_U16 eitOtherOverTimeError;
	GOSTSR_U16 eitActualScheduleOverTimeError;
	GOSTSR_U16 eitOtherScheduleOverTimeError;
	GOSTSR_U16 tdtOverTimeError;
	GOSTSR_U16 totOverTimeError;
}SiRepetitionRateError_Info;

/*buffer error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 TBBufferError;
	GOSTSR_U16 TBsysBufferError;
	GOSTSR_U16 MBBufferError;
	GOSTSR_U16 EBBufferError;
	GOSTSR_U16 BBufferError;
	GOSTSR_U16 BsysBufferError;
}BufferError_Info;

/*unreferenced pid error information*/
#define MAX_UNREFERPID_NUM	1024

typedef struct
{
	GOSTSR_U32 startTime;
	GOSTSR_U16 pid;
	GOSTSR_U8  flag;
}UnreferencedPid_Info;

typedef struct
{
	GOSTSR_U16 totalError;
}UnreferencedPidError_Info;

/*sdt error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 tableIDError;
	GOSTSR_U16 actualOverTimeError;
	GOSTSR_U16 actualLimitTimeError;
	GOSTSR_U16 otherOverTimeError;
}SdtError_Info;

/*eit error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 tableIDError;
	GOSTSR_U16 actualOverTimeError;
	GOSTSR_U16 actualLimtTimeError;
	GOSTSR_U16 actualSec0OverTimeError;
	GOSTSR_U16 actualSec1OverTimeError;
	GOSTSR_U16 actualPFPairError;
	GOSTSR_U16 otherSec0OverTimeError;
	GOSTSR_U16 otherSec1OverTimeError;
	GOSTSR_U16 otherPFPairError;
}EitError_info;

/*rst error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 tableIDError;
	GOSTSR_U16 limtTimeError;
}RstError_Info;

/*tdt error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 overTimeError;
	GOSTSR_U16 tableIDError;
	GOSTSR_U16 limitTimeError;
}TdtError_Info;

/*empty buffer error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 TBBufferEmptyError;
	GOSTSR_U16 TBsysBufferEmptyError;
	GOSTSR_U16 MBBufferEmptyError;
}EmptyBufferError_Info;

/*data delay error information*/
typedef struct
{
	GOSTSR_U16 totalError;
	GOSTSR_U16 stillPicOverTimeError;
	GOSTSR_U16 generalDataOverTimeError;
}DataDelayError_Info;

/*ts three level error check information*/
typedef struct
{
	NitError_Info nitErrorInfo;
	SiRepetitionRateError_Info siRepetitionRateErrorInfo;
	BufferError_Info bufferErrorInfo;
	UnreferencedPidError_Info unreferencedPidErrorInfo;
	SdtError_Info sdtErrorInfo;
	EitError_info eitErrorInfo;
	RstError_Info rstErrorInfo;
	TdtError_Info tdtErrorInfo;
	EmptyBufferError_Info emptyBufferErrorInfo;
	DataDelayError_Info dataDelayErrorInfo;
}TsThreeLevelErrorCheck_Info;

typedef struct
{
	GOSTSR_U16	pmtPidInfo[MAX_PMT_NUMS];
}ThreeLevel_PmtDis_Info;

typedef struct
{
	GOSTSR_U16	CatPidInfo[MAX_CAT_NUMS];
}ThreeLevel_CatDis_Info;

extern GOSTSR_S32 TsErrorCheck_ThreeLevel_Init();
extern GOSTSR_S32 TsErrorCheck_ThreeLevel_SectionsTimeError(TS_SECTION_INFO sectionInfo);
extern GOSTSR_S32 TsErrorCheck_ThreeLevel_BufferError(GOSTSR_U8 *data, GOSTSR_U16 dataLen, GOSTSR_U32 time);
extern GOSTSR_S32 TsErrorCheck_ThreeLevel_GetErrorInfo(TsThreeLevelErrorCheck_Info *info);
extern GOSTSR_S32 TsErrorCheck_ThreeLevel_UnreferencedPidError(TS_HEAD_INFO tsHeadInfo);
extern GOSTSR_S32 TsErrorCheck_ThreeLevel_DeInit();
GOSTSR_S32 TsErrorCheck_ThreeLevel_setPid(SEARCH_INFO_S *pstProgInfo);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
