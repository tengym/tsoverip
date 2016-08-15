#ifndef __TSPARSE_SEARCH_H__
#define __TSPARSE_SEARCH_H__

#include "TsErrorCheck_Common.h"
#include "GosTsr_AnalysisData.h"

#define SEARCH_PROG_NUM_MAX		255
#define SEARCH_CA_NUM_MAX		10
#define SEARCH_PROG_PES_MAX		20

typedef struct
{
	GOSTSR_BOOL	bUsed;

	GOSTSR_U16 PmtPid;
	GOSTSR_U16 PcrPid;
	GOSTSR_U8 u8NbPes;
	GOSTSR_U16 PesPid[SEARCH_PROG_PES_MAX];
} SEARCH_PROG_INFO_S;

typedef struct
{
	GOSTSR_U16 CaPid;
} SEARCH_CA_INFO_S;

typedef struct
{
	GOSTSR_U16	u16NbProg;
	SEARCH_PROG_INFO_S stProgInfo[SEARCH_PROG_NUM_MAX];

	GOSTSR_U8	u16NbCa;
	SEARCH_CA_INFO_S stCaInfo[SEARCH_CA_NUM_MAX];	/*有可能有多个CA系统，最大不差过10*/
} SEARCH_INFO_S;

GOSTSR_S32 TsParse_Search_TsErrorCheck_setPid(SEARCH_INFO_S *pstProgInfo);
GOSTSR_S32 TsParse_Search_getProgInfo(SEARCH_INFO_S *pstProgInfo);
GOSTSR_S32 TsParse_Search_StoreProg_byPat(GOSTSR_PSISI_PAT_S *patInfo);
GOSTSR_S32 TsParse_Search_StoreProg_byPmt(GOSTSR_U16 Pmt_pid, GOSTSR_PSISI_PMT_S *pmtInfo);
GOSTSR_S32 TsParse_Search_StoreProg_byCat(GOSTSR_PSISI_CAT_S *catInfo);
GOSTSR_S32 TsParse_Search_ParseSection(GOSTSR_U8 tableID,TS_SECTION_INFO *curSectionInfo);
GOSTSR_S32 TsParse_Search_LinkPackage(GOSTSR_U8 tableID, TS_PACKAGE_INFO *PackageHead_Temp, GOSTSR_U8 lastDataLen, GOSTSR_U8 *lastData);
GOSTSR_S32 TsParse_Search_BuildSection(GOSTSR_U8 tableID,const void *data, TS_HEAD_INFO tsHeadInfo);
GOSTSR_S32 TsParse_Search_TSHeadInfo(const void *srcData, TS_HEAD_INFO *desData);
GOSTSR_S32 TsParse_Search_AnalysisPacket(GOSTSR_U8 *pPacketData, GOSTSR_U32 u32DataLen);
GOSTSR_S32 TsParse_Search_ReadStaticTs(char *filePath);
GOSTSR_S32 TsParse_Search_Init(char *filePath);
GOSTSR_S32 TsParse_Search_DeInit();

#endif
