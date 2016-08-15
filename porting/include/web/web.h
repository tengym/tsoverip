#ifndef __WEB_H__
#define __WEB_H__

#include "user_define.h"

#define MAX_CHIP_NUM        16          //最大芯片个数
#define MAX_TASK_PERCHIP    5           //每个芯片最大处理任务
#define MAX_FILENAME_LEN    4*1024      //文件名字符串最大值

typedef enum
{
	GOS_SVC_MODEL_TSPARSE= 0,    //TS处理模块
	GOS_SVC_MODEL_OTHER,		 //其他模块
	
	GOS_SVC_MODEL_TOTAL
}GOS_SVC_Model_Type;

typedef enum
{
	GOS_SVC_CMD_TSPARSE_GET_INFO = 0,		//获取信息
	GOS_SVC_CMD_TSPARSE_SET_INFO,		    //设置信息
	GOS_SVC_CMD_TSPARSE_GET_BITRATE,		//获取码率
	GOS_SVC_CMD_TSPARSE_STOP,					
	GOS_SVC_CMD_TSPARSE_PAUSE,
	GOS_SVC_CMD_TSPARSE_GET_STATUS,
	GOS_SVC_CMD_TSPARSE_TOTAL
}GOS_SVC_CMD_TsParse;

typedef enum
{
	GOS_SVC_CMD_OTHER_SOFTWARE_UPGRADE,     //软件升级
	GOS_SVC_CMD_OTHER_SYSTEM_SETTING,       //系统设置
	GOS_SVC_CMD_OTHER_LANGUAGE_CHOOSE,
	
	GOS_SVC_CMD_OTHER_TOTAL,
}GOS_SVC_CMD_Other;

enum
{
	E_LANGTYPE_EN = 0,		/*英文*/
	E_LANGTYPE_RU = 1,		/*俄文*/
	E_LANGTYPE_CH = 2,		/*中文*/
	
	E_LANGTYPE_BUTT
};

typedef  void(*GOS_SVC_Callback) (unsigned int svc_model, unsigned int svc_cmd, int ret, void* data, unsigned int data_len, GOS_BOOL flag);
typedef  void(*GOS_WEBUI_Callback) (GOS_SVC_Model_Type model, void* data);

/***************
*
tsparse  mode
*
****************/

typedef	struct
{
    GOS_CHAR    pfilename[64];                  /*文件名字符串*/
    GOS_CHAR    paddr[32];                      /*IP地址*/
    GOS_U32     port;                           /*端口号*/

    GOS_U8      protocol;                       /*协议类型；0:UDP, 1:RTP*/
    GOS_U8      state;                          /*操作状态；0:暂停，1:开始*/
}GOS_SVC_TsParse_Info_t;

typedef struct
{
    GOS_U8      chipID;                                              /*芯片ID, 0 ~ 15*/
    GOS_U8      chip_state;                                              /*芯片状态:0:停止1:开始*/
    GOS_U32     packet_num;                     /*每次发包个数，单位为：188Byte(一个Ts包), 默认值为7*/
    GOS_CHAR    pfilename[MAX_FILENAME_LEN];    /*文件名字符串*/
    GOS_SVC_TsParse_Info_t tsparse_info[MAX_TASK_PERCHIP];       /*每个芯片最大发送IP*/
}GOS_SVC_TsParse_t;

typedef struct
{
    GOS_U8       chipID;
    GOS_U32     bitrate[MAX_TASK_PERCHIP];      /*单码率,单位:Mbps*/
    GOS_U32     total_bitrate;                  /*总码率,单位:Mbps*/
}GOS_SVC_TsParse_BitRate_t;

typedef struct
{
	GOS_SVC_CMD_TsParse tsparse_cmd;
	GOS_SVC_Callback callback;
	void* tsparse_data;
}GOS_SVC_Model_TsParse;

/***************
*
other mode
*
****************/

typedef struct
{
	GOS_SVC_CMD_Other other_cmd;
	GOS_SVC_Callback callback;
	void* other_data;
}GOS_SVC_Model_Other;

/***************
*
socket mode
*
****************/
typedef struct web_socket_data_head
{
	GOS_U16 module_id;
	GOS_U16 command_id;
	GOS_U32 data_len;
	GOS_U32 data_crc;
}Web_Socket_Msg_Data_Head;

GOS_S32 GOS_WEB_Slave_Init(GOS_WEBUI_Callback callback);
GOS_S32 GOS_WEB_Init(GOS_WEBUI_Callback callback);
GOS_S32 GOS_WEB_DeInit(void);

#endif
