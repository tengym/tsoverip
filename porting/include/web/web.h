#ifndef __WEB_H__
#define __WEB_H__

#include "user_define.h"

#define MAX_CHIP_NUM        16          //���оƬ����
#define MAX_TASK_PERCHIP    5           //ÿ��оƬ���������
#define MAX_FILENAME_LEN    4*1024      //�ļ����ַ������ֵ

typedef enum
{
	GOS_SVC_MODEL_TSPARSE= 0,    //TS����ģ��
	GOS_SVC_MODEL_OTHER,		 //����ģ��
	
	GOS_SVC_MODEL_TOTAL
}GOS_SVC_Model_Type;

typedef enum
{
	GOS_SVC_CMD_TSPARSE_GET_INFO = 0,		//��ȡ��Ϣ
	GOS_SVC_CMD_TSPARSE_SET_INFO,		    //������Ϣ
	GOS_SVC_CMD_TSPARSE_GET_BITRATE,		//��ȡ����
	GOS_SVC_CMD_TSPARSE_STOP,					
	GOS_SVC_CMD_TSPARSE_PAUSE,
	GOS_SVC_CMD_TSPARSE_GET_STATUS,
	GOS_SVC_CMD_TSPARSE_TOTAL
}GOS_SVC_CMD_TsParse;

typedef enum
{
	GOS_SVC_CMD_OTHER_SOFTWARE_UPGRADE,     //�������
	GOS_SVC_CMD_OTHER_SYSTEM_SETTING,       //ϵͳ����
	GOS_SVC_CMD_OTHER_LANGUAGE_CHOOSE,
	
	GOS_SVC_CMD_OTHER_TOTAL,
}GOS_SVC_CMD_Other;

enum
{
	E_LANGTYPE_EN = 0,		/*Ӣ��*/
	E_LANGTYPE_RU = 1,		/*����*/
	E_LANGTYPE_CH = 2,		/*����*/
	
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
    GOS_CHAR    pfilename[64];                  /*�ļ����ַ���*/
    GOS_CHAR    paddr[32];                      /*IP��ַ*/
    GOS_U32     port;                           /*�˿ں�*/

    GOS_U8      protocol;                       /*Э�����ͣ�0:UDP, 1:RTP*/
    GOS_U8      state;                          /*����״̬��0:��ͣ��1:��ʼ*/
}GOS_SVC_TsParse_Info_t;

typedef struct
{
    GOS_U8      chipID;                                              /*оƬID, 0 ~ 15*/
    GOS_U8      chip_state;                                              /*оƬ״̬:0:ֹͣ1:��ʼ*/
    GOS_U32     packet_num;                     /*ÿ�η�����������λΪ��188Byte(һ��Ts��), Ĭ��ֵΪ7*/
    GOS_CHAR    pfilename[MAX_FILENAME_LEN];    /*�ļ����ַ���*/
    GOS_SVC_TsParse_Info_t tsparse_info[MAX_TASK_PERCHIP];       /*ÿ��оƬ�����IP*/
}GOS_SVC_TsParse_t;

typedef struct
{
    GOS_U8       chipID;
    GOS_U32     bitrate[MAX_TASK_PERCHIP];      /*������,��λ:Mbps*/
    GOS_U32     total_bitrate;                  /*������,��λ:Mbps*/
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
