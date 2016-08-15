#include "linuxos.h"
#include "socket_client.h"
#include "gos_socket.h"
#include "gos_sys.h"

static GOS_S32  gUDPSocketFd;
static GOS_S32  s32Tcp_Socket = -1;

static OS_Task_Handle_t  Handle_NetPort_Check;
static OS_Task_Handle_t  Handle_SMsg_Dispatch;
static OS_Task_Handle_t  Handle_ackState_Check;

static OS_Semaphore_t*	 ackState_Semaphore = GOS_NULL;
static OS_Semaphore_t*	 socket_Client_Semaphore = GOS_NULL;
 
static GOS_U8 			bNet_Status = SLAVE_NET_READY;
static GOS_U8    		bCallCnt = 0;
static GOS_BOOL 		bThread_Run = GOS_FALSE;
static GOS_BOOL   		bAckState = GOS_FALSE;

static GOS_S32 MAYBE_UNUSED socket_getipaddr(unsigned long *pnumaddr, char* pstraddr)
{
	char *pName = GOS_NULL;
	unsigned long ipaddr;

	if(PORT_SYS_Get_ChipID() % 2 == 0)/*偶板*/
	{
		pName = NETCARD_EVEN;
	}
	else
	{
		pName = NETCARD_ODD;
	}
	
	if (network_get_ipaddr(pName, &ipaddr) == GOS_SUCCESS)
	{
		if (pnumaddr)
		{
			*pnumaddr = ipaddr;
		}

		if (pstraddr)
		{
			ipaddr = SWAP32(ipaddr);
			inet_ntop(AF_INET, (HI_CHAR *)&ipaddr, pstraddr, 16);
		}

		return GOS_SUCCESS;
	}

	return GOS_FAILURE;
}

static void socket_client_board_check_task(void *arg)
{
	while(bThread_Run)
	{
		if (bAckState == GOS_TRUE)
		{
			OS_WaitSemaphore(ackState_Semaphore);

			bAckState = GOS_FALSE;
			bCallCnt = 0;

			OS_SignalSemaphore(ackState_Semaphore);
		}
		else
		{
			if(bCallCnt > 10)
			{
				OS_WaitSemaphore(ackState_Semaphore);

				bAckState = GOS_FALSE;
				bCallCnt = 0;
				
				OS_SignalSemaphore(ackState_Semaphore);

				printf("\n###Check Task TimeOut, Reboot Now!###\n");
				while(1)
				{
					reboot(RB_AUTOBOOT);
				}
			}
			else
			{
				OS_WaitSemaphore(ackState_Semaphore);
				bCallCnt++;
				OS_SignalSemaphore(ackState_Semaphore);
			}	
			sleep(3);
		}
	}
}

static GOS_S32 MAYBE_UNUSED socket_client_connect(HI_S32 s32Port, HI_S32* ps32SocketFd)
{
	HI_S32 	s32SocketFd = -1;
	struct sockaddr_in sevice_addr; /*服务器地址信息*/

   	s32SocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s32SocketFd < 0)
   	{
		DEBUG_ERROR("Socket error...\n");
		return HI_FAILURE;
	}

	bzero(&sevice_addr, sizeof(sevice_addr));
	sevice_addr.sin_family = AF_INET; /*地址家族*/
	sevice_addr.sin_port = htons(s32Port); /*注意转化为网络节序*/
	sevice_addr.sin_addr.s_addr = inet_addr(PORT_SYS_Get_HostAddr());
	
	DEBUG_INFO("start connect sevice...\n");

	if (connect(s32SocketFd, (struct sockaddr *)&sevice_addr, sizeof(sevice_addr)) < 0)
	{
		DEBUG_ERROR("connect failed! errno %d \n", errno);
		close(s32SocketFd); /*关闭套接字*/
		return HI_FAILURE;
	}

	if (ps32SocketFd)
	{
		*ps32SocketFd = s32SocketFd;
	}

	return HI_SUCCESS;
}

static GOS_S32 MAYBE_UNUSED socket_client_respond_server(GOS_S32 s32ChipID, char* ipdotdec)
{
	GOS_VOID*	send_buf = NULL;
	SOCKET_CLIENT_INFO_t stClientInfo;
	SOCKET_MSG_INFO_S stMsgHead;
	GOS_S32 s32Ret;

	stMsgHead.u16MsgID = GOS_MSG_OTHER_BOARDMATCH;
	stMsgHead.u16RecvID = HOST_ID;
	stMsgHead.u16SendID = s32ChipID;

	if(PORT_SYS_Get_ChipID() % 2 == 0)
	{
		stClientInfo.type = E_BOARD_EVEN;
	}
	else
	{
		stClientInfo.type = E_BOARD_ODD;
	}

	stClientInfo.chipIndex = s32ChipID;
	memset(stClientInfo.ipaddr, 0x00, sizeof(stClientInfo.ipaddr));
	strcpy(stClientInfo.ipaddr, ipdotdec);

	send_buf = PORT_SOCKET_Msg_Package(&stMsgHead, (GOS_VOID*)&stClientInfo, sizeof(stClientInfo));
	if (send_buf == NULL)
	{
		DEBUG_ERROR("PORT_SOCKET_Msg_Package Error!\n");
		return GOS_FAILURE;
	}

	s32Ret = Socket_Client_Send_TCP_Msg(send_buf, stMsgHead.u32DataLen);

	OS_FreeMemory(send_buf);

	return GOS_SUCCESS;
}

static void socket_client_check_netport_task(void)
{
	char *pName = "eth1";
	
	bNet_Status = SLAVE_NET_READY;
	while(1)
	{
		if(PORT_SOCKET_Detect_Netlink_Status(pName) == 1)/*连接*/
		{
			if(SLAVE_NET_DISCONNECT == bNet_Status)/*断开状态后，再重新连接上*/
			{	
				printf("\n=====Net Connect agian\n");
				bNet_Status = SLAVE_NET_CONNECT;
				
				if(PORT_SYS_Get_ChipID() % 2 != 0)/*奇板*/
				{
					usleep(500*1000);
				}
				do
				{
					PORT_SYS_HWReboot(); /*偶板控制奇板重启*/
					reboot(RB_AUTOBOOT);
				}while(1);
			}				
			bNet_Status = SLAVE_NET_CONNECT;
		}
		else
		{
			if(SLAVE_NET_CONNECT == bNet_Status)/*断开判断依据:上一个状态必须处于连接*/
			{				
				printf("\n########Net Break\n");
				bNet_Status = SLAVE_NET_DISCONNECT;	
			}			
		}
		usleep(500 * 1000);
	}
	
	return;
}

static void socket_client_dispatch_task(void *argv)
{
	int		temp_len = 0;
	HI_S32	s32Ret = GOS_FAILURE;
	HI_S32	s32RcvLength = -1;
	SOCKET_MSG_INFO_S stMsgHead;
	HI_CHAR ipdotdec[32] = {0};
    HI_CHAR u8RecvBuffer[1024*1024];
	HI_CHAR u8TempBuffer[1024*1024];
	HI_CHAR *pOffset;

	int i;

	while(bThread_Run)
	{
		if (socket_getipaddr(NULL, ipdotdec) == GOS_SUCCESS)
		{
			DEBUG_INFO("the ipaddr is : %s\n", ipdotdec);
			break;
		}
		else
		{
			sleep(1);
		}
	}

	printf("ipdotdec:%s--connect server...\n",ipdotdec);
	
	for(i=0; i<5; i++)
	{
		if (socket_client_connect(MYPORT, &s32Tcp_Socket) != GOS_SUCCESS)
		{
			printf("socket_client_connect failure!\n");
			usleep(200);
		}
		else
		{
			break;
		}
	}
	
	if(i >= 5)
	{
		close(s32Tcp_Socket);
		printf("chip:%d, connect time out, reboot new...\n",PORT_SYS_Get_ChipID());
		while(1)
		{
			sleep(1);
			system("reboot");
		}
	}

	socket_client_respond_server(PORT_SYS_Get_ChipID(), ipdotdec);
	
	/*子板创建"心跳包"自检线程*/
	if (!OS_CreateTask(socket_client_board_check_task, NULL, (OS_Task_Handle_t*)&Handle_ackState_Check))
	{
		DEBUG_ERROR("Create the socket_client_board_check_task Failed!\n");
	}	
	
	printf("The s32ChipIndex is %d--s32Tcp_Socket:%#x\n", PORT_SYS_Get_ChipID(), s32Tcp_Socket);

	while(bThread_Run)
	{
		memset(u8RecvBuffer, 0, sizeof(u8RecvBuffer));
		//printf("REecv .....\n");
		if (PORT_SOCKET_Recv_Msg_TCP(s32Tcp_Socket, u8RecvBuffer, &s32RcvLength) != GOS_SUCCESS)
		{
			continue;
		}

		if (s32RcvLength >  0)
		{
			pOffset = u8RecvBuffer;//获取得到的数据
			pOffset[s32RcvLength] = '\0';//为数据末尾添加结束符
	recv_msg :
			s32Ret = PORT_SOCKET_Msg_Check(&stMsgHead, pOffset, s32RcvLength);
			if (s32Ret == eMSG_GET_HEAD)
			{
				temp_len = 0;
				memset(u8TempBuffer, 0, sizeof(u8TempBuffer));
				memcpy(u8TempBuffer + temp_len, pOffset, s32RcvLength);
				temp_len = temp_len + s32RcvLength;
			}
			else if (s32Ret == eMSG_GET_PART)/*一条命令被分解成多次发送时*/
			{
				if(temp_len == 0)
				{
					break;
				}
				
				memcpy(u8TempBuffer + temp_len, pOffset, s32RcvLength);
				temp_len = temp_len + s32RcvLength;
				if (stMsgHead.u32DataLen == temp_len)
				{
					GOS_AVPLAYSLAVE_Dispatch_RecvMsg(0, u8TempBuffer, temp_len);
					temp_len = 0;
					memset(u8TempBuffer, 0, sizeof(u8TempBuffer));
				}
				else if(stMsgHead.u32DataLen < temp_len)
				{
					GOS_AVPLAYSLAVE_Dispatch_RecvMsg(0, u8TempBuffer, stMsgHead.u32DataLen);
					/*又重新处理后续的命令*/
					memset(u8TempBuffer, 0, sizeof(u8TempBuffer));
					temp_len = temp_len - stMsgHead.u32DataLen;
					if(s32RcvLength <= temp_len)
					{
						break;
					}
					
					pOffset = pOffset + s32RcvLength - temp_len;
					s32RcvLength = temp_len;
					temp_len = 0;
					goto recv_msg;
					
				}
			}	
			else if (s32Ret == eMSG_GET_ONE)
			{
				if (stMsgHead.u32DataLen <= s32RcvLength)
				{
					GOS_AVPLAYSLAVE_Dispatch_RecvMsg(0, pOffset, s32RcvLength);
				}
			}
			else if (s32Ret == eMSG_GET_MORE)/*同时接收到多条命令时*/
			{
				GOS_AVPLAYSLAVE_Dispatch_RecvMsg(0, pOffset, stMsgHead.u32DataLen);
				s32RcvLength = s32RcvLength - stMsgHead.u32DataLen;
				pOffset = pOffset + stMsgHead.u32DataLen;
				goto recv_msg;
			}
		}
    }
	return;
}

GOS_S32 Socket_Client_Open(void)
{
	bThread_Run = GOS_TRUE;
	bAckState = GOS_FALSE;
	
	if(socket_Client_Semaphore == GOS_NULL)
	{
		socket_Client_Semaphore = OS_CreateSemaphore(1);
	}

	if(ackState_Semaphore == GOS_NULL)
	{
		ackState_Semaphore = OS_CreateSemaphore(1);
	}

	gUDPSocketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (gUDPSocketFd < 0)
	{
		printf("create udp socket error [%d].\n", errno);
	}

	if (!OS_CreateTask(socket_client_dispatch_task, NULL, (OS_Task_Handle_t*)&Handle_SMsg_Dispatch))
	{
		DEBUG_ERROR("socket_client_dispatch_task Failure!\n");
		return GOS_FAILURE;
	}

	if(PORT_SYS_Get_ChipID()%2 == 0)/*偶板*/
	{
		if (!OS_CreateTask(socket_client_check_netport_task, NULL, (OS_Task_Handle_t*)&Handle_NetPort_Check))
		{
			DEBUG_ERROR("socket_client_check_netport_task Failure!\n");
			return GOS_FAILURE;
		}
	}
	DEBUG_INFO("Socket_Client_Open Success!\n");
	
	return GOS_SUCCESS;
}

GOS_S32 Socket_Client_Close(void)
{
	bThread_Run = GOS_FALSE;
	bAckState = GOS_FALSE;
	
	OS_WaitSemaphore(socket_Client_Semaphore);
	//OS_DeleteTask(Handle_SMsg_Dispatch);
	//OS_DeleteTask(Handle_NetPort_Check);

	if (s32Tcp_Socket > 0)
	{
		close(s32Tcp_Socket);
		s32Tcp_Socket = -1;
	}
	OS_SignalSemaphore(socket_Client_Semaphore);

	OS_DeleteSemaphore(socket_Client_Semaphore);	
	socket_Client_Semaphore = GOS_NULL;

	OS_DeleteSemaphore(ackState_Semaphore);	
	ackState_Semaphore = GOS_NULL;
	return GOS_SUCCESS;
}
GOS_S32 Socket_Client_TcpSocket_Open(void)
{
	if(bThread_Run)
		return GOS_FAILURE;
	
	bThread_Run = GOS_TRUE;

	if(!Handle_SMsg_Dispatch)
	{
		if (!OS_CreateTask(socket_client_dispatch_task, NULL, (OS_Task_Handle_t*)&Handle_SMsg_Dispatch))
		{
			DEBUG_ERROR("socket_client_dispatch_task Failure!\n");
			return GOS_FAILURE;
		}	
	}
	else
	{
		DEBUG_ERROR("Socket_Client_TcpSocket_Close Failure: pthread handle is not NULL!\n");
		return GOS_FAILURE;
	}
	DEBUG_INFO("=== Socket_Client_TcpSocket_Open Success !\n");

	return GOS_SUCCESS;
}
GOS_S32 Socket_Client_TcpSocket_Close(void)
{
	if(!bThread_Run)
		return GOS_FAILURE;
	
	bThread_Run = GOS_FALSE;
	usleep(500 *1000);
	
	OS_WaitSemaphore(socket_Client_Semaphore);

	if(0 != Handle_SMsg_Dispatch)
	{
		pthread_cancel(Handle_SMsg_Dispatch);
		Handle_SMsg_Dispatch = 0;	
	}

	if (s32Tcp_Socket > 0)
	{
		close(s32Tcp_Socket);
		s32Tcp_Socket = -1;
	}
	OS_SignalSemaphore(socket_Client_Semaphore);

	DEBUG_INFO("### Socket_Client_TcpSocket_Close OK !");

	return GOS_SUCCESS;
}

GOS_S32 Socket_Client_Send_TCP_Msg(const void* pData, int iDatalen)
{
	GOS_S32 s32Ret;
	if (!pData)
	{
		DEBUG_ERROR("The pData is NULL\n");
		return GOS_FAILURE;
	}
	
	if(s32Tcp_Socket < 0)
	{
		return GOS_FAILURE;
	}

	s32Ret = PORT_SOCKET_Send_Msg_TCP(s32Tcp_Socket, MYPORT, PORT_SYS_Get_HostAddr(), pData, iDatalen);

	return s32Ret;
}

GOS_S32 Socket_Client_Send_UDP_Msg(const void* pData, int iDatalen)
{
	GOS_U32  u32SendData = 0;
	struct sockaddr_in   udpServerAddr;

	if (!pData)
	{
		DEBUG_ERROR("The pData is NULL\n");
		return GOS_FAILURE;
	}

	if(iDatalen > (800*1024))
	{
		return GOS_FAILURE;
	}
	
	memset(&udpServerAddr, 0x00, sizeof(udpServerAddr)); //数据初始化--清零
	udpServerAddr.sin_addr.s_addr = inet_addr(PORT_SYS_Get_HostAddr());
	udpServerAddr.sin_port = htons(SEND_UDP_PORT);
	udpServerAddr.sin_family = AF_INET;
	u32SendData = sendto(gUDPSocketFd, pData, iDatalen, 
		0/*MSG_DONTWAIT*/, (struct sockaddr *)&udpServerAddr, sizeof(udpServerAddr));

	return GOS_SUCCESS;
}

GOS_S32 Socket_Client_Recv_Server(void)
{	
	OS_WaitSemaphore(ackState_Semaphore);

	bAckState = GOS_TRUE;

	OS_SignalSemaphore(ackState_Semaphore);
	
	return GOS_SUCCESS;
}

