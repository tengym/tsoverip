/// @file main.c
/// @brief 
/// @author tengym <tengym@gospell.com>
/// 0.01_0
/// @date 2016-08-05

#include "user_define.h"
#include "usbdesk.h"
#include "web.h"
#include "svc_control.h"
#include "tsparse_api.h"
#include "crc.h"
#include "gos_sys.h"

int test_main(void)
{
#if 0
    char str[4*1024] = {0};
    TsParse_Api_Get_TsName(str);
#endif

    int count = 0;
    while(1)
    {
        if((USB_GetDiskPartNum() > 0) || (count >= 100))
        {
            break;
        }
        usleep(50 * 1000);
        count++;
    }

    int i = 0;
    GOS_SVC_TsParse_t tsinfo;
    memset(&tsinfo, 0, sizeof(tsinfo));

#if 0
    char filePath[MAX_THREAD_NUM][32] = {"sd_1.ts","sd_2.ts","sd_3.ts","hd_1.ts","hd_2.ts"}; 
#endif
    //char filePath[MAX_THREAD_NUM][32] = {"hd_1.ts","hd_2.ts","hd_1.ts"}; 
    char filePath[5][32] = {"hd_1.ts", "hd_2.ts","hd_1.ts","hd_2.ts", "hd_2.ts"}; 

    for(i = 0; i < 5; i++)
    {
        tsinfo.tsparse_info[i].protocol = 0;
        tsinfo.tsparse_info[i].port = 5000;
        sprintf(tsinfo.tsparse_info[i].paddr, "224.100.100.%d", i+1);
        sprintf(tsinfo.tsparse_info[i].pfilename, "%s", filePath[i]);
    }

    printf("begin to parse ts ....\n");

    TsParse_Api_TsParseProcess((TSPARSE_API_INFO_t *)&tsinfo);
    
    return 0;
}

int main(int argc, char *argv[])
{
    if(GOS_SUCCESS != GOS_SYS_Init())
    {
        DEBUG_ERROR("GOS_SYS_Init Failed\n");
        return GOS_FAILURE;
    }

    if(GOS_SUCCESS != USB_Init(NULL))
    {
        DEBUG_ERROR("USB_Init Failed\n");
        return GOS_FAILURE;
    }

    if(GOS_SUCCESS != GOS_CRC_Init())
    {
        DEBUG_ERROR("CRC_Init Failed\n");
        return GOS_FAILURE;
    }

    if(GOS_SUCCESS != GOS_SVC_CONTROL_Init())
    {
        DEBUG_ERROR("GOS_SVC_CONTROL_Init Failed\n");
        return GOS_FAILURE;
    }

    if(GOS_SUCCESS != TsParse_Api_Init())
    {
        DEBUG_ERROR("TsParse_Api_Init Failed\n");
        return GOS_FAILURE;
    }

    TsParse_Api_Restart();

#if 0
    test_main();
#endif

    while(1)
    {
        sleep(10);
    }

    return 0;
}
