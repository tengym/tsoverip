#ifndef __TSPARSE_UPDATE_H__
#define __TSPARSE_UPDATE_H__

#include "TsErrorCheck_Common.h"
#include "tsparse_search.h"

typedef struct
{
	GOSTSR_U8 u8PatVersion;
	GOSTSR_BOOL	bUpdateFlag;
	
	SEARCH_INFO_S stProgInfo;
} UPDATE_INFO_S;

GOSTSR_S32 TsParse_Update_ProgSearch_Init();
GOSTSR_S32 TsParse_Update_ProgSearch_DeInit();

#endif
