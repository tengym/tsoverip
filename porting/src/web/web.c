/*
接收数据，校验数据，将数据转换成svc可以用的数据格式
发送数据
*/
#include "linuxos.h"
#include "crc.h"
#include "web.h"
#include "svc_control.h"
#include "gos_sys.h"

#define SLAVE_PORT			9000
#define HOST_PORT			8001
#define MAX_SOCKET_BUF_LEN	6500
#define UNIX_DOMAIN			"/tmp/UNIX.domain"

union semun {
         int val;               // SETVAL使用的值
         struct semid_ds *buf;  // IPC_STAT、IPC_SET 使用缓存区
         unsigned short *array; // GETALL,、SETALL 使用的数组
         struct seminfo *__buf; // IPC_INFO(Linux特有) 使用缓存区
};

static int	webui_task_flag = 1;
static int 	socket_fd_conect = 0;
static int 	socket_fd_server = 0;

static OS_Semaphore_t*		sem_webuiAccess  = GOS_NULL;
static OS_Task_Handle_t		Webui_TaskHandle =(pthread_t)GOS_NULL;
static GOS_WEBUI_Callback	ui_callback = GOS_NULL;

static GOS_S32 socket_select_data(int socket, GOS_U8* readset,  GOS_U8* writeset,  GOS_U8* exceptset,int timeout_ms)
{
	int s32Ret = -1;
	int select_flag = 0;
	struct timeval tm;
	fd_set fds_r ,fds_w ,fds_e;
	fd_set* pfds_r = GOS_NULL ;
	fd_set* pfds_w = GOS_NULL;
	fd_set* pfds_e =GOS_NULL;

	if ((socket <= 0) || (timeout_ms < -1) || (!readset && !writeset && !exceptset))
	{
	 	printf("-11param in err!!\n");
	   	goto ERROR_EXIT;
	}

    if (timeout_ms != -1) /*set timeout*/
	{
		tm.tv_sec = timeout_ms/1000;
		tm.tv_usec = 1000*(timeout_ms%1000);
	}
    else
	{
		tm.tv_sec = 50000;
		tm.tv_usec = 0;
	}

    FD_ZERO(&fds_r);
	FD_ZERO(&fds_w);
    FD_ZERO(&fds_e);

	if (readset)
	{
		FD_SET(socket, &fds_r);
		select_flag = 1;
		pfds_r= &fds_r;
	}

	if (writeset)
	{
		FD_SET(socket, &fds_w);
		select_flag = 1;
		pfds_w = &fds_w;
	}

	if (exceptset)
	{
		FD_SET(socket, &fds_e);
		select_flag = 1;
		pfds_e = &fds_e;
	}

	if (select_flag == 0)
	{
    	goto ERROR_EXIT;
	}

	s32Ret = select( socket+1, pfds_r, pfds_w, pfds_e, &tm );
	if (s32Ret < 0)
	{
    	goto ERROR_EXIT;
	}

	if (readset) *readset = 0;
	if (writeset) *writeset = 0;
	if (exceptset) *exceptset = 0;

	if (readset)
	{
		if (FD_ISSET(socket, &fds_r))
		{
			*readset = 1;
		}
	}

	if (writeset)
	{
		if (FD_ISSET(socket, &fds_w))
		{
			*writeset = 1;
		}
	}

	if (exceptset)
	{
		if (FD_ISSET(socket, &fds_e))
		{
			*exceptset = 1;
		}
	}
	return GOS_SUCCESS;

ERROR_EXIT:
	printf("%s-select err!!\n",__FUNCTION__);
	
   	return GOS_FAILURE;
}

#define MAX_SOCKET_UNIT_BUF_LEN	1440
static int 	socket_slave_rev_data(int socket, GOS_U8* buf, GOS_U32 length, GOS_U16 flags)
{
	int s32Ret = -1;
    GOS_U8 recv_buff[MAX_SOCKET_UNIT_BUF_LEN];

    GOS_U32 total_len = 0;
    length = 0;

    do 
    {
        memset(recv_buff, 0, sizeof(recv_buff));
        s32Ret = recv(socket, recv_buff, MAX_SOCKET_UNIT_BUF_LEN, flags);
        if (s32Ret < 0)
        {
            if(errno== EAGAIN)
            {
                continue;
            }
            printf("%s-rev err = %d,errno=%d!!\n",__FUNCTION__,s32Ret,errno);
            return -1;
        }

        if(length == 0)/*接收第一个包*/
        {
            Web_Socket_Msg_Data_Head *msg_head = (Web_Socket_Msg_Data_Head*)recv_buff; 
            total_len = msg_head->data_len+sizeof(Web_Socket_Msg_Data_Head);
        }
        memcpy(buf+length, recv_buff, s32Ret);
        length += s32Ret;

    }while(total_len > length);

	return total_len;
}

static GOS_S32 socket_slave_send_data(int socket, GOS_U8* buf, GOS_U32 length, GOS_U16 flags)
{
	int s32Ret = -1;

    Web_Socket_Msg_Data_Head *msg_head = (Web_Socket_Msg_Data_Head*)buf; 
    GOS_U32 total_len = msg_head->data_len+sizeof(Web_Socket_Msg_Data_Head);
    GOS_U8 send_buff[MAX_SOCKET_UNIT_BUF_LEN];
    length = 0;

    do
    {
        int send_len = (total_len - length > MAX_SOCKET_UNIT_BUF_LEN) ? MAX_SOCKET_UNIT_BUF_LEN : (total_len - length);
        memset(send_buff, 0, sizeof(send_buff));
        memcpy(send_buff, buf+length, send_len);
        s32Ret = send(socket, send_buff, send_len, flags);
        if (s32Ret < 0)
        {
            if(errno== EAGAIN)
            {
                continue;
            }
            printf("%s-len = %d--send err = %d,errno=%d!!\n",__FUNCTION__,length,s32Ret,errno);
            return GOS_FAILURE;
	    }

        length += s32Ret;

    }while(total_len > length);

	return GOS_SUCCESS;
}

static int 	socket_rev_data(int socket, GOS_U8* buf, GOS_U32 length, GOS_U16 flags)
{
	int s32Ret = -1;

    s32Ret = recv(socket, buf, length, flags);
    if (s32Ret < 0)
    {
        printf("%s-rev err = %d,errno=%d!!\n",__FUNCTION__,s32Ret,errno);
        return -1;
    }

	return s32Ret;
}

static GOS_S32 socket_send_data(int socket, GOS_U8* buf, GOS_U32 length, GOS_U16 flags)
{
	int s32Ret = -1;

	s32Ret = send(socket, buf, length, flags);
	if (s32Ret < 0)
	{
		printf("%s-len = %d--send err = %d,errno=%d!!\n",__FUNCTION__,length,s32Ret,errno);
    	return GOS_FAILURE;
	}

	return GOS_SUCCESS;
}

static void web_send_data(unsigned int svc_model, unsigned int svc_cmd, int s32Ret, void* data, unsigned int data_len, GOS_BOOL flag)
{
	Web_Socket_Msg_Data_Head msg_head;
	GOS_U8 send_buf[MAX_SOCKET_BUF_LEN];
	GOS_U32 send_data_len = 0;

	if ((data == GOS_NULL) && (data_len > 0))
	{
 		printf("-00param in err!!\n");
   		return;
	}

	memset(&msg_head, 0, sizeof(Web_Socket_Msg_Data_Head));
	memset(send_buf, 0, MAX_SOCKET_BUF_LEN);
	msg_head.module_id = svc_model;
	msg_head.command_id = svc_cmd;
	msg_head.data_len = data_len+sizeof(GOS_U32);
	memcpy(send_buf, &msg_head, sizeof(Web_Socket_Msg_Data_Head));
	*((GOS_U32*)(send_buf+sizeof(Web_Socket_Msg_Data_Head))) = (s32Ret==GOS_SUCCESS)?1:0;
	if (data_len > 0)
	{
		memcpy(send_buf+sizeof(Web_Socket_Msg_Data_Head)+sizeof(GOS_U32), data, data_len);
	}
	GOS_CRC_32bCalculate(send_buf, (data_len+sizeof(GOS_U32)+sizeof(Web_Socket_Msg_Data_Head)), &msg_head.data_crc);
	memcpy(send_buf, &msg_head, sizeof(Web_Socket_Msg_Data_Head));
	send_data_len = sizeof(Web_Socket_Msg_Data_Head)+sizeof(GOS_U32)+data_len;

    if(GOS_SYS_Get_ChipID() == 0x00)
    {
        socket_send_data(socket_fd_server, send_buf, send_data_len, 0);
    }
    else
    {
        socket_slave_send_data(socket_fd_server, send_buf, send_data_len, 0);
    }
	if (flag)
	{
		//这里设计的很失败，还需要用到一个信号量来控制。
		OS_SignalSemaphore(sem_webuiAccess);
	}
	return;
}

static GOS_S32 parse_tsparsemodel_cmd(unsigned int tsparse_cmd, void* data, unsigned int data_len)
{
	GOS_BOOL is_send = GOS_FALSE;
	GOS_SVC_Model_TsParse* tsparsemodel = GOS_NULL;
	GOS_U8* tempdata ;

	tsparsemodel = (GOS_SVC_Model_TsParse*)OS_AllocMemory(sizeof(GOS_SVC_Model_TsParse));
	if (tsparsemodel == GOS_NULL)
		return GOS_FAILURE;

	tsparsemodel->tsparse_cmd = tsparse_cmd;
	tsparsemodel->callback = web_send_data;
	switch(tsparse_cmd)
	{
		case GOS_SVC_CMD_TSPARSE_GET_INFO:
			{
                tsparsemodel->tsparse_data = GOS_NULL;
                is_send = GOS_TRUE;
			}
			break;

		case GOS_SVC_CMD_TSPARSE_SET_INFO:
			{
				tempdata = (GOS_U8*)OS_AllocMemory(data_len);
				if(GOS_NULL != tempdata)
				{
					memset(tempdata,0,data_len);
					memcpy(tempdata,data,data_len);
					tsparsemodel->tsparse_data = tempdata;
					is_send = GOS_TRUE;
				}	
			}
			break;

		case GOS_SVC_CMD_TSPARSE_STOP:
			{
                tsparsemodel->tsparse_data = GOS_NULL;
                is_send = GOS_TRUE;
			}
			break;

		case GOS_SVC_CMD_TSPARSE_GET_BITRATE:
			{
                tsparsemodel->tsparse_data = GOS_NULL;
                is_send = GOS_TRUE;
			}
			break;

		default:
			break;
	}

	if ((is_send) && (ui_callback != GOS_NULL))
	{
		ui_callback(GOS_SVC_MODEL_TSPARSE, tsparsemodel);
	}
	else
	{
		web_send_data(GOS_SVC_MODEL_TSPARSE, tsparse_cmd, GOS_FAILURE, GOS_NULL, 0, 0);
		OS_FreeMemory(tsparsemodel);
	}
	return (is_send ? GOS_SUCCESS : GOS_FAILURE);
}

static GOS_S32 parse_othermodel_cmd(unsigned int other_cmd, void* data, unsigned int data_len)
{
	GOS_BOOL is_send = GOS_FALSE;
	GOS_U8* tempdata ;
	GOS_SVC_Model_Other* othermodel = GOS_NULL;

	othermodel = (GOS_SVC_Model_Other*)OS_AllocMemory(sizeof(GOS_SVC_Model_Other));
	if (othermodel == GOS_NULL)
		return GOS_FAILURE;

	othermodel->other_cmd = other_cmd;
	othermodel->callback = web_send_data;
	switch(other_cmd)
	{
		case GOS_SVC_CMD_OTHER_SOFTWARE_UPGRADE:
			{
				tempdata = (GOS_U8*)OS_AllocMemory(data_len);
				if(GOS_NULL != tempdata)
				{
					memset(tempdata,0,data_len);
					memcpy(tempdata,data,data_len);
					othermodel->other_data = tempdata;
					is_send = GOS_TRUE;
				}	
			}
			break;
		case GOS_SVC_CMD_OTHER_SYSTEM_SETTING:
			{
				tempdata = (GOS_U8*)OS_AllocMemory(data_len);
				if(GOS_NULL != tempdata)
				{
					memset(tempdata,0,data_len);
					memcpy(tempdata,data,data_len);
					othermodel->other_data = tempdata;
					is_send = GOS_TRUE;
				}	
			}
			break;
		default:
			break;
	}

	if ((is_send) && (ui_callback != GOS_NULL))
	{
		ui_callback(GOS_SVC_MODEL_OTHER, othermodel);
	}
	else
	{
		web_send_data(GOS_SVC_MODEL_OTHER, other_cmd, GOS_FAILURE, GOS_NULL, 0, 0);
		OS_FreeMemory(othermodel);
	}
	return (is_send ? GOS_SUCCESS : GOS_FAILURE);
}

static GOS_S32 recv_msg_parse(GOS_U8 *data_buf, int data_len)
{
	GOS_S32 s32Ret = GOS_SUCCESS;
	GOS_U8* data_ps = GOS_NULL;
	GOS_U32 data_crc = 0;
	GOS_U32 data_crc_temp = 0;
	Web_Socket_Msg_Data_Head* msg_head = (Web_Socket_Msg_Data_Head*)data_buf;
	
	if ((data_buf == GOS_NULL) || (data_len < (sizeof(Web_Socket_Msg_Data_Head)+msg_head->data_len)))
	{
	 	LOG_ERROR("-recv_msg_parse data_len = %d!!\n",data_len);
		return GOS_FAILURE;
	}
	data_crc = msg_head->data_crc;
	msg_head->data_crc = 0;
	GOS_CRC_32bCalculate(data_buf, (sizeof(Web_Socket_Msg_Data_Head)+msg_head->data_len), &data_crc_temp);
	if (data_crc == data_crc_temp)
	{
		data_ps = data_buf+sizeof(Web_Socket_Msg_Data_Head);

		switch(msg_head->module_id)
		{
			case GOS_SVC_MODEL_TSPARSE:
			{
				s32Ret = parse_tsparsemodel_cmd(msg_head->command_id, data_ps, msg_head->data_len);
			}
			break;
			case GOS_SVC_MODEL_OTHER:
			{
				s32Ret = parse_othermodel_cmd(msg_head->command_id, data_ps, msg_head->data_len);		
			}
			break;
			default:
			break;
		}
	}
	else
	{
	 	LOG_ERROR("-data_crc_temp = %x--data crc=%x!\n", data_crc_temp, data_crc);
		s32Ret = GOS_FAILURE;
	}

	return s32Ret;
}

static void webui_task(void* param)
{
	GOS_U8 readset = 0;
	GOS_U8 rev_buf[MAX_SOCKET_BUF_LEN];
	GOS_S32 s32Ret = -1;
	GOS_S32 receive_len = 0;
	socklen_t 	sin_size;
	struct sockaddr_un 	client_addr;

 	sin_size = sizeof(client_addr);

	while (webui_task_flag)
	{
		socket_fd_server = accept(socket_fd_conect, (struct sockaddr *)&client_addr, &sin_size);
		if (socket_fd_server > 0)
		{
			s32Ret = socket_select_data(socket_fd_server, &readset, GOS_NULL, GOS_NULL, -1);
			if ((readset == 1) && (s32Ret == GOS_SUCCESS))
			{
				memset(rev_buf, 0, MAX_SOCKET_BUF_LEN);

                if(GOS_SYS_Get_ChipID() == 0x00)
                {
                    receive_len = socket_rev_data(socket_fd_server, rev_buf, MAX_SOCKET_BUF_LEN, MSG_DONTWAIT);
                }
                else
                {
                    receive_len = socket_slave_rev_data(socket_fd_server, rev_buf, MAX_SOCKET_BUF_LEN, MSG_DONTWAIT);
                }

                //printf("recv_len:%d\n", receive_len);

				s32Ret = recv_msg_parse(rev_buf, receive_len);
				//这里设计的很失败，还需要用到一个信号量来控制。
				if (s32Ret == GOS_SUCCESS)
					OS_WaitSemaphore(sem_webuiAccess);
			}
			close(socket_fd_server);
		}
		else
		{
		 	LOG_ERROR("socket_fd_server !! socket_fd_server =%d\n",socket_fd_server);
		}
	}

	return;
}

GOS_S32 GOS_WEB_Slave_Init(GOS_WEBUI_Callback callback)
{
    int port = 0;
	GOS_S32	s32Ret = 0;
    struct sockaddr_in server_addr;

	socket_fd_conect = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_conect <= 0)
	{
	 	LOG_ERROR("temp_socket_fd !! temp_socket_fd =%d\n",socket_fd_conect);
		return GOS_FAILURE;
	}

    port = SLAVE_PORT + GOS_SYS_Get_ChipID();
    printf("\nweb socket, chip:%d--port:%d\n", GOS_SYS_Get_ChipID(), port);

	bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;   
    server_addr.sin_port   = htons(port);   
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	int opt = 1;
	setsockopt(socket_fd_conect, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
	

	s32Ret = bind(socket_fd_conect, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (s32Ret  < 0)
	{
	 	LOG_ERROR("bind !! s32Ret =%d  errno:%d \n", s32Ret, errno);
	 	return GOS_FAILURE;
	}

	s32Ret = listen(socket_fd_conect, 1);
	if (s32Ret  < 0)
	{
	 	LOG_ERROR("listen !! s32Ret = %d\n", s32Ret);
	 	return GOS_FAILURE;
	}

	webui_task_flag = GOS_TRUE;
	ui_callback = callback;
	//这里设计的很失败，还需要用到一个信号量来控制。
	sem_webuiAccess  = OS_CreateSemaphore (0);
	if (!OS_CreateTask((void*)webui_task, GOS_NULL, &Webui_TaskHandle))
	{
		LOG_ERROR("creat webui_task failure !!\n");
		return GOS_FAILURE;
	}

	LOG_INFO("GOS_WEB_Init Success!\n");
	
	return GOS_SUCCESS;
}

GOS_S32 GOS_WEB_Init(GOS_WEBUI_Callback callback)
{
	GOS_S32	s32Ret = 0;
	struct sockaddr_un 	server_addr;

	socket_fd_conect = socket(PF_UNIX, SOCK_STREAM, 0);
	if (socket_fd_conect <= 0)
	{
	 	LOG_ERROR("temp_socket_fd !! temp_socket_fd =%d\n",socket_fd_conect);
		return GOS_FAILURE;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, UNIX_DOMAIN, sizeof(server_addr.sun_path) - 1);

	int opt = 1;
	setsockopt(socket_fd_conect, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
	unlink(UNIX_DOMAIN);	/*删除socket文件，否则会出现bind Error*/
	
	s32Ret = bind(socket_fd_conect, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (s32Ret  < 0)
	{
	 	LOG_ERROR("bind !! s32Ret =%d  errno:%d \n", s32Ret, errno);
	 	return GOS_FAILURE;
	}

	s32Ret = listen(socket_fd_conect, 1);
	if (s32Ret  < 0)
	{
	 	LOG_ERROR("listen !! s32Ret = %d\n", s32Ret);
	 	return GOS_FAILURE;
	}

	webui_task_flag = GOS_TRUE;
	ui_callback = callback;
	//这里设计的很失败，还需要用到一个信号量来控制。
	sem_webuiAccess  = OS_CreateSemaphore (0);
	if (!OS_CreateTask((void*)webui_task, GOS_NULL, &Webui_TaskHandle))
	{
		LOG_ERROR("creat webui_task failure !!\n");
		return GOS_FAILURE;
	}

	LOG_INFO("GOS_WEB_Init Success!\n");
	
	return GOS_SUCCESS;
}

GOS_S32 GOS_WEB_DeInit(void)
{
	webui_task_flag = 0;

	if (socket_fd_server > 0)
		close(socket_fd_server);

	if (socket_fd_conect > 0)
		close(socket_fd_conect);

	ui_callback = GOS_NULL;
	return 0;
}
