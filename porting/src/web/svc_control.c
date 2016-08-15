#include "linuxos.h"
#include "crc.h"
#include "web.h"
#include "gos_sys.h"

#include "svc_control.h"
#include "tsparse_api.h"

typedef struct
{
	GOS_SVC_Model_Type  model;
	void* data;
}GOS_SVC_Msg_t;

#define SVC_MAX_MSG_NUM 250

static GOS_BOOL SvcTaskFlag = GOS_TRUE;
static OS_Task_Handle_t   SvcTaskHandle = (OS_Task_Handle_t)GOS_NULL;
static OS_MessageQueue_t* SVC_MsgQid = GOS_NULL;

static void svc_tsparse_control(void* data)
{
    GOS_S32 ret = GOS_FAILURE;
	GOS_SVC_Model_TsParse* tsparse_model_param = (GOS_SVC_Model_TsParse*)data;
	
	if (tsparse_model_param == GOS_NULL)
	{
		LOG_ERROR("data is NULL\n");
	}

	if (tsparse_model_param != GOS_NULL)
	{
		switch(tsparse_model_param->tsparse_cmd)
		{
			case GOS_SVC_CMD_TSPARSE_GET_INFO:
				{
                    DEBUG("cmd: GOS_SVC_CMD_TSPARSE_GET_INFO\n");

                    GOS_SVC_TsParse_t tsparse_info;
                    memset(&tsparse_info, 0, sizeof(tsparse_info));

                    ret = TsParse_Api_Get_TsInfo((TSPARSE_API_INFO_t *)&tsparse_info);

                    DEBUG_INFO("packet:%d, FileName:%s\n", tsparse_info.packet_num, tsparse_info.pfilename);

					if (tsparse_model_param->callback != GOS_NULL)
					{
						tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_SUCCESS, &tsparse_info, sizeof(tsparse_info), 1);
					}
				}
				break;
				
			case GOS_SVC_CMD_TSPARSE_SET_INFO:
				{
                    DEBUG("cmd: GOS_SVC_CMD_TSPARSE_SET_INFO\n");

                    if(GOS_NULL != tsparse_model_param->tsparse_data)
                    {
                        GOS_SVC_TsParse_t *tsparse_info = (GOS_SVC_TsParse_t *)tsparse_model_param->tsparse_data;

                        ret = TsParse_Api_TsParseProcess((TSPARSE_API_INFO_t*)tsparse_info);
                    }

					if (tsparse_model_param->callback != GOS_NULL)
					{
						tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
					}
				}
				break;
			case GOS_SVC_CMD_TSPARSE_STOP:
				{
                    DEBUG("cmd: GOS_SVC_CMD_TSPARSE_STOP\n");

                    ret = TsParse_Api_StopSend();

					if (tsparse_model_param->callback != GOS_NULL)
					{
						tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
					}
				}
				break;

			case GOS_SVC_CMD_TSPARSE_GET_BITRATE:
				{
                    //DEBUG("cmd: GOS_SVC_CMD_TSPARSE_GET_BITRATE\n");

                    GOS_SVC_TsParse_BitRate_t stbit_rate;

                    memset(&stbit_rate, 0, sizeof(stbit_rate));

                    ret = TsParse_Api_Get_Bitrate(&stbit_rate);

					if (tsparse_model_param->callback != GOS_NULL)
					{
						tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_SUCCESS, &stbit_rate,sizeof(stbit_rate), 1);
					}
				}
				break;

			default:
				if (tsparse_model_param->callback != GOS_NULL)
				{
                    tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
				}
				break;
		}
	}
	else
	{
		if (tsparse_model_param->callback != GOS_NULL)
		{
            tsparse_model_param->callback(GOS_SVC_MODEL_TSPARSE, tsparse_model_param->tsparse_cmd, GOS_FAILURE, GOS_NULL,0, 1);
		}
	}

	if (tsparse_model_param->tsparse_data != GOS_NULL)
	{
		OS_FreeMemory(tsparse_model_param->tsparse_data);
	}

	return;
}

static void svc_other_control(void* data)
{
	GOS_SVC_Model_Other* other_model_param = (GOS_SVC_Model_Other*)data;
	
	if (other_model_param == GOS_NULL)
	{
		LOG_ERROR("data is NULL\n");
	}

	if (other_model_param != GOS_NULL)
	{
		switch(other_model_param->other_cmd)
		{
			case GOS_SVC_CMD_OTHER_SOFTWARE_UPGRADE:
				{
					if (other_model_param->callback != GOS_NULL)
					{
						other_model_param->callback(GOS_SVC_MODEL_OTHER, other_model_param->other_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
					}
				}
				break;
				
			case GOS_SVC_CMD_OTHER_SYSTEM_SETTING:
				{
					if (other_model_param->callback != GOS_NULL)
					{
						other_model_param->callback(GOS_SVC_MODEL_OTHER, other_model_param->other_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
					}
				}
				break;
				
				
			default:
				if (other_model_param->callback != GOS_NULL)
				{
					other_model_param->callback(GOS_SVC_MODEL_OTHER, other_model_param->other_cmd, GOS_SUCCESS, GOS_NULL,0, 1);
				}
				break;
		}
	}
	else
	{
		if (other_model_param->callback != GOS_NULL)
		{
			other_model_param->callback(GOS_SVC_MODEL_OTHER, other_model_param->other_cmd, GOS_FAILURE, GOS_NULL,0, 1);
		}
	}

	if (other_model_param->other_data != GOS_NULL)
	{
		OS_FreeMemory(other_model_param->other_data);
	}

	return;
}

static void svc_control_task(void* param)
{
	GOS_SVC_Msg_t* msgReceived = GOS_NULL;

	while(SvcTaskFlag)
	{
		if ((msgReceived = (GOS_SVC_Msg_t *)OS_ReceiveMessage(SVC_MsgQid)) == GOS_NULL)
		{
			usleep(100*1000);
			continue;
		}

		switch(msgReceived->model)
		{
			case GOS_SVC_MODEL_TSPARSE:
				{
					svc_tsparse_control(msgReceived->data);
				}
				break;
			
			case GOS_SVC_MODEL_OTHER:
				{
					svc_other_control(msgReceived->data);
				}
				break;
			
			default:
				break;
		}

		if (msgReceived->data != GOS_NULL)
		{
			OS_FreeMemory(msgReceived->data);
		}

		OS_ReleaseMessageBuffer(SVC_MsgQid,(void *)msgReceived);
	}

	return;
}

static void svc_webui_callback(GOS_SVC_Model_Type model, void* data)
{
	GOS_SVC_Msg_t 	msgSent;

	msgSent.model = model;
	msgSent.data = data;

	if (SVC_MsgQid != GOS_NULL)
	{
		OS_SendMessage(SVC_MsgQid, (void*)(&msgSent), sizeof(GOS_SVC_Msg_t), 0);
	}

	return;
}

GOS_S32 GOS_SVC_CONTROL_Init(void)
{
	GOS_S32 s32Ret = GOS_FAILURE;

    if(0x00 == GOS_SYS_Get_ChipID())
    {
        s32Ret = GOS_WEB_Init(svc_webui_callback);
    }
    else
    {
        s32Ret = GOS_WEB_Slave_Init(svc_webui_callback);
    }

	if (s32Ret != GOS_SUCCESS)
	{
		printf("GOS_WEB_Init err!!\n");
		return GOS_FAILURE;
	}

	/* 创建消息队列 */
	if ((SVC_MsgQid = OS_CreateMessageQueue("/SVC_MsgQid", sizeof(GOS_SVC_Msg_t), SVC_MAX_MSG_NUM)) == GOS_NULL)
	{
		LOG_ERROR("create SVC_MsgQid error\n");
		return GOS_FAILURE;
	}
	
	/*创建任务用于接收web发送过来的消息*/
	SvcTaskFlag = GOS_TRUE;
	if (!OS_CreateTask((void*)svc_control_task, GOS_NULL, &SvcTaskHandle))
	{
		LOG_ERROR("create svc_control_task error\n");
		return GOS_FAILURE;
	}

	printf("%s success!\n", __FUNCTION__);
	
	return GOS_SUCCESS;
}

GOS_S32 GOS_SVC_CONTROL_DeInit(void)
{
	SvcTaskFlag = GOS_FALSE;

	OS_DeleteMessageQueue(SVC_MsgQid);

	GOS_WEB_DeInit();
	
	return GOS_SUCCESS;
}
