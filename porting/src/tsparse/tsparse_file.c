#include "GosTsr_AnalysisData.h"
#include "GosTsr_Descriptor.h"

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"
#include "TsErrorCheck_ThreeLevel.h"

#include "tsparse_api.h"
#include "tsparse_file.h"

#define TSPARSE_PARAMETER_PATH "/home/data/tsparse_para.db"

GOSTSR_S32 TsParse_File_Write_TsInfoFile(void *pParam)
{
	FILE *fp = NULL;
    TSPARSE_API_INFO_t *tsParam = (TSPARSE_API_INFO_t *)pParam;

	if (GOSTSR_NULL == tsParam)
	{
		return GOSTSR_FAILURE;
	}
	
	fp = fopen(TSPARSE_PARAMETER_PATH, "wb+");
	if (GOSTSR_NULL ==  fp)
	{
		printf("open %s failure!\n", TSPARSE_PARAMETER_PATH);
		return GOSTSR_FAILURE;
	}

	fseek(fp, 0, SEEK_SET);
	
	if (1 != fwrite((void *)tsParam, sizeof(TSPARSE_API_INFO_t), 1, fp))
	{
		printf("write failed!\n");
		fclose(fp);
		fp = GOSTSR_NULL;
		return GOSTSR_FAILURE;
	}

	fclose(fp);
	fp = GOSTSR_NULL;

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_File_Read_TsInfoFile(void *pParam)
{
	FILE *fp = GOSTSR_NULL;
    GOSTSR_S32 index = 0;
    TSPARSE_API_INFO_t *tsParam = (TSPARSE_API_INFO_t *)pParam;

	if (GOSTSR_NULL == tsParam)
	{
		return GOSTSR_FAILURE;
	}
	
	fp = fopen(TSPARSE_PARAMETER_PATH, "rb+");
	if (GOSTSR_NULL ==  fp)
	{
        memset(tsParam->unit_info, 0x00, sizeof(tsParam->unit_info));
        tsParam->packet_num = 7;
        tsParam->chip_state = 0; 
        for(index = 0; index < 5; index++) 
        {
            tsParam->unit_info[index].state = 1;    /*0:off , 1:on*/
            tsParam->unit_info[index].protocol = 0; /*0:udp , 1:rtp*/
            tsParam->unit_info[index].port = 5000;
            sprintf(tsParam->unit_info[index].paddr, "224.10.10.%d", index+1);
        }

        TsParse_File_Write_TsInfoFile((void *)tsParam);

        return 0;
	}

    memset(tsParam->unit_info, 0x00, sizeof(tsParam->unit_info));
	if (1 != fread((void *)tsParam, sizeof(TSPARSE_API_INFO_t), 1, fp))
	{
		printf("read failed\n!");
		fclose(fp);
		fp = GOSTSR_NULL;
		return GOSTSR_FAILURE;
	}

	fclose(fp);
	fp = GOSTSR_NULL;
	
	return GOSTSR_SUCCESS;
}

