#ifndef __TSPARSE_API_H__
#define __TSPARSE_API_H__

#include "TsErrorCheck_Common.h"
#include "user_define.h"
#include "web.h"

#define MAX_TSPARSE_FILE_LEN      4*1024 
#define MAX_THREAD_NUM      5 
#define TS_OVER_IP_SIZE     7*188

typedef	struct
{
    GOS_CHAR    pfilename[64];    /*文件名字符串*/
    GOS_CHAR    paddr[32];                      /*IP地址*/
    GOS_U32     port;                           /*端口号*/

    GOS_U8      protocol;                       /*协议类型；0:UDP, 1:RTP*/
    GOS_U8      state;                          /*操作状态；0:暂停，1:开始*/
}TSPARSE_UNIT_Info_t;

typedef struct
{
    GOS_U8  chipID;                                              /*芯片ID, 0 ~ 15*/
    GOS_U8  chip_state;                                              /*芯片状态:0:停止1:开始*/
    GOS_U32 packet_num;                     /*每次发包个数，单位为：188Byte(一个Ts包), 默认值为7*/
    GOS_CHAR pfilename[MAX_TSPARSE_FILE_LEN];    /*文件名字符串*/
    TSPARSE_UNIT_Info_t unit_info[5];       /*每个芯片最大发送IP数*/
}TSPARSE_API_INFO_t;

GOSTSR_S32 TsParse_Api_Get_TsInfo(TSPARSE_API_INFO_t *tsparse_info);
GOSTSR_S32 TsParse_Api_ReadStaticTs(char *pfilepath, int *bitrate);
GOSTSR_S32 TsParse_Api_Threadpool_AddTask(int handle, char *pfilepath, int bit_rate, int protocol, int port, char *paddr);
GOSTSR_S32 TsParse_Api_TsParseProcess(TSPARSE_API_INFO_t *tsparse_info);
GOSTSR_S32 TsParse_Api_Get_Bitrate(GOS_SVC_TsParse_BitRate_t *stbitrate);
GOSTSR_S32 TsParse_Api_Restart(void);
GOSTSR_S32 TsParse_Api_StopSend(void);

GOSTSR_S32 TsParse_Api_Init();
GOSTSR_S32 TsParse_Api_DeInit();
GOSTSR_S32 TsParse_Api_ResInit();

#endif
