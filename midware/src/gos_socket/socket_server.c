#include "linuxos.h"
#include "gos_sys.h"
#include "socket_server.h"
#include "socke_common.h"
//#include "webui.h"
#include "check_net.h"
typedef struct
{
    GOS_S32   activeFlag;
    GOS_S32   s32TcpFd;
    GOS_S32   s32UdpFd;
    GOS_CHAR  ipaddr[32];
    GOS_U8    chipIndex;
    GOS_U8    type;
    HI_BOOL   ackState;
    GOS_U8    callCnt;
    OS_Semaphore_t 	*lockSem;
    GOS_CHAR u8RecvBuffer[1024*1024];
    GOS_S32    dataLen;
}GOS_Client_Info_t;

typedef struct
{
    GOS_S32   activeClientCnt;
    GOS_S32   rebootGpioIndex;
    GOS_S32   slotTimeCnt;
    GOS_S32   playFlag;
    GOS_Client_Info_t    *pClientInfo[2];
    SOCKET_MSG_PROGRAM_t  stServiceInfo[4];
}GOS_Slot_Info_t;

static OS_Task_Handle_t    Handle_Board_Check;
static OS_Task_Handle_t    Handle_HMsg_Dispatch;
static GOS_Slot_Info_t     gSlotInfo[MAX_SLOT_NUM];
static GOS_Client_Info_t   gClientInfo[MAX_MAIN_AVPLAY_NUM];
static GOS_BOOL            gCheck_client_run = GOS_TRUE;
static GOS_BOOL            gGetMsg_client_run = GOS_TRUE;

static OS_Task_Handle_t	 Handle_Serv_Netport_Process;
static GOS_U8 			bNet_Serv_Status = SLAVE_NET_READY;

GOS_S32 socket_server_bulid(HI_S32 s32Port, HI_S32* ps32SocketFd)
{
    HI_S32 	s32SocketFd = -1;
    struct 	sockaddr_in	sevice_addr;

    s32SocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (s32SocketFd < 0)
    {
        DEBUG_ERROR("Socket error...\n");
        return HI_FAILURE;
    }

    DEBUG_INFO("s32SocketFd = %d\n", s32SocketFd);

    bzero(&sevice_addr, sizeof(sevice_addr));
    sevice_addr.sin_family = AF_INET;
    sevice_addr.sin_addr.s_addr = INADDR_ANY;/* auto-fill with my IP */
    sevice_addr.sin_port = htons(s32Port);
    bzero(&(sevice_addr.sin_zero), sizeof(sevice_addr.sin_zero));
    if (bind(s32SocketFd, (struct sockaddr *)&sevice_addr, sizeof(struct sockaddr)) < 0)
    {
        close(s32SocketFd);
        DEBUG_ERROR("Bind error...\n");
        return HI_FAILURE;
    }

    listen(s32SocketFd, BACKLOG);
    if (s32SocketFd < 0)
    {
        close(s32SocketFd);
        DEBUG_ERROR("Listen error...\n");
        return HI_FAILURE;
    }

    if (ps32SocketFd)
    {
        *ps32SocketFd = s32SocketFd;
    }

    return HI_SUCCESS;
}

#if 1
static GOS_S32 socket_server_call_client(GOS_U16 u16ChipID)
{
    int i;
    GOS_S32   Event = 1;
    GOS_VOID* send_buf = NULL;
    SOCKET_MSG_INFO_S stMsgHead;
    int ret = GOS_SUCCESS;

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if((gClientInfo[i].chipIndex != 0xFF) &&
                (gClientInfo[i].chipIndex != 0x00) &&
                (gClientInfo[i].chipIndex == u16ChipID))
        {
            break;
        }
    }

    if (i >= MAX_MAIN_AVPLAY_NUM)
        return GOS_FAILURE;

    if (gClientInfo[i].activeFlag != HI_TRUE)
        return GOS_FAILURE;

    stMsgHead.u16MsgID = GOS_MSG_SVC_CALL;
    stMsgHead.u16RecvID= u16ChipID;
    stMsgHead.u16SendID = HOST_ID;
    send_buf = PORT_SOCKET_Msg_Package(&stMsgHead, (GOS_VOID*)&Event, sizeof(GOS_S32));
    if (send_buf == NULL)
    {
        DEBUG_ERROR("PORT_SOCKET_Msg_Package Error!\n");
        return GOS_FAILURE;
    }

    ret = PORT_SOCKET_Send_Msg_TCP(gClientInfo[i].s32TcpFd, MYPORT, gClientInfo[i].ipaddr, send_buf, stMsgHead.u32DataLen);
    if(ret == GOS_FAILURE)
    {
        if (gClientInfo[i].s32TcpFd > 0)
        {
            OS_WaitSemaphore(gClientInfo[i].lockSem);
            gClientInfo[i].callCnt = 4;
            gClientInfo[i].activeFlag = HI_FALSE;
            gClientInfo[i].ackState = HI_FALSE;

            if(gClientInfo[i].s32TcpFd > 0)
                close(gClientInfo[i].s32TcpFd);
            gClientInfo[i].s32TcpFd = -1;
            OS_SignalSemaphore(gClientInfo[i].lockSem);
        }
    }

    OS_FreeMemory(send_buf);

    return GOS_SUCCESS;
}
#endif

static void socket_udpmsg_dispatch_task(void *arg)
{
    int index;
    int sin_len;
    struct sockaddr_in 	sin;
    GOS_S32   SocketFd;

    HI_S32 	s32RcvLength = -1;
    int		temp_len = 0;
    int 	socket_index = 0;

    HI_S32  s32Ret = -1;

    SOCKET_MSG_INFO_S stMsgHead;
    HI_CHAR u8RecvBuffer[1024*1024];
    HI_CHAR u8TempBuffer[1024*1024];
    HI_CHAR *pOffset;

    index = (int)arg;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(RECV_UDP_PORT);
    sin_len = sizeof(sin);

    SocketFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);  //UDP
    bind(SocketFd, (struct sockaddr *)&sin, sizeof(sin));

    DEBUG_INFO("Creat socket_msg_dispatch_task!\n");

    while(1)
    {
        memset(u8RecvBuffer, 0, sizeof(u8RecvBuffer));
        s32RcvLength = recvfrom(SocketFd, u8RecvBuffer, 1024*1024, 0, (struct sockaddr *)&sin, (socklen_t*)&sin_len);;

        if (s32RcvLength > 0)
        {
            pOffset = u8RecvBuffer;//获取得到的数据
            pOffset[s32RcvLength] = '\0';//为数据末尾添加结束符
msg_check_type:
            s32Ret = PORT_SOCKET_Msg_Check(&stMsgHead, pOffset, s32RcvLength);

            if (s32Ret == eMSG_GET_HEAD)/*一条命令被分解成多次发送时*/
            {
                //printf("2.eMSG_GET_HEAD:%d\n",temp_len);
                temp_len = 0;
                memset(u8TempBuffer, 0x00, sizeof(u8TempBuffer));
                memcpy(u8TempBuffer, pOffset, s32RcvLength);
                temp_len = s32RcvLength;
                //DEBUG_ERROR("---------temp_len :%d \n",temp_len);
            }
            else if (s32Ret == eMSG_GET_PART)/*一条命令被分解成多次发送时*/
            {
                //printf("2.eMSG_GET_PART:%d\n",temp_len);
                if(temp_len == 0)
                {
                    break;
                }

                memcpy(u8TempBuffer + temp_len, pOffset, s32RcvLength);
                temp_len = temp_len + s32RcvLength;
                //DEBUG_ERROR("11#########temp_len :%d \n",temp_len);
                if (stMsgHead.u32DataLen == temp_len)
                {
                    GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, u8TempBuffer, temp_len);
                    temp_len = 0;
                    memset(u8TempBuffer, 0x00, sizeof(u8TempBuffer));
                }
                else if(stMsgHead.u32DataLen < temp_len)
                {
                    GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, u8TempBuffer, stMsgHead.u32DataLen);
                    /*又重新处理后续的命令*/
                    memset(u8TempBuffer, 0, sizeof(u8TempBuffer));
                    temp_len = temp_len - stMsgHead.u32DataLen;
                    if(s32RcvLength <= temp_len)
                    {
                        break;
                    }

                    pOffset = pOffset + (s32RcvLength - temp_len);
                    s32RcvLength = temp_len;
                    //OS_Printf_Buffer(pOffset,s32RcvLength);

                    temp_len = 0;
                    //printf("$$$$$ s32RcvLength :%d \n",s32RcvLength);
                    goto msg_check_type;
                }
            }	
            else if (s32Ret == eMSG_GET_ONE)
            {
                //printf("2.eMSG_GET_ONE:%d\n",s32RcvLength);
                if (stMsgHead.u32DataLen <= s32RcvLength)
                {
                    //	DEBUG_ERROR("stMsgHead.u32DataLen :%d s32RcvLength :%d \n",stMsgHead.u32DataLen ,s32RcvLength);
                    GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, pOffset, s32RcvLength);
                }
            }
            else if (s32Ret == eMSG_GET_MORE)/*同时接收到多条命令时*/
            {
                //printf("2.eMSG_GET_MORE:%d\n",stMsgHead.u32DataLen);
                GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, pOffset, stMsgHead.u32DataLen);
                s32RcvLength = s32RcvLength - stMsgHead.u32DataLen;
                pOffset = pOffset + stMsgHead.u32DataLen;
                goto msg_check_type;
            }
        }
    }
}

static void socket_hmsg_dispatch_task(void *arg)
{
    int		i = 0;
    int 	socket_index = 0;
    int     maxfdp;
    HI_S32  s32Ret = -1;
    HI_S32 	s32Size = 0;
    HI_S32 	s32SockFd = -1;
    HI_S32 	s32SocketFd = -1;
    HI_S32 	s32RcvLength = -1;
    fd_set fd_set_r, fd_set_w;
    struct sockaddr_in client_addr;
    SOCKET_MSG_INFO_S stMsgHead;
    HI_CHAR u8RecvBuffer[1024*1024];
    HI_CHAR *pOffset;

    DEBUG_INFO("Creat socket_msg_dispatch_task!\n");

    /*清空集合*/
    FD_ZERO(&fd_set_r);
    FD_ZERO(&fd_set_w);

    if (HI_SUCCESS != socket_server_bulid(MYPORT, &s32SocketFd))
    {
        DEBUG_ERROR("socket_server_bulid failure!\n");
    }

    DEBUG_INFO("Server-->s32SocketFd : %d\n", s32SocketFd);

    FD_SET(s32SocketFd, &fd_set_r);
    maxfdp = s32SocketFd;

    while(gGetMsg_client_run)
    {
        FD_ZERO(&fd_set_w);
        FD_ZERO(&fd_set_r);
        FD_SET(s32SocketFd, &fd_set_r);
        maxfdp = s32SocketFd;

        for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
        {
            if (gClientInfo[i].activeFlag == HI_TRUE)
            {
                FD_SET(gClientInfo[i].s32TcpFd, &fd_set_r);
                maxfdp = gClientInfo[i].s32TcpFd > maxfdp ? gClientInfo[i].s32TcpFd : maxfdp;
            }
        }

        if (select(maxfdp + 1, &fd_set_r, &fd_set_w, NULL, NULL) <= 0)
        {
            DEBUG_ERROR("select failure!\n");
            continue;
        }

        if (FD_ISSET(s32SocketFd, &fd_set_r))
        {
            //printf("wait client connect...\n");
            s32Size = sizeof(client_addr);
            bzero(&client_addr, s32Size);
            if ((s32SockFd = accept(s32SocketFd, (struct sockaddr *)&client_addr, (socklen_t *)&s32Size)) < 0)
            {
                if(errno== EINTR)
                    continue;
            }
            else
            {
                DEBUG_INFO("\nClient Online::=========s32SockFd:%d,IP:%s\n", s32SockFd, inet_ntoa(client_addr.sin_addr));

                for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
                {
                    if (gClientInfo[i].activeFlag == HI_FALSE)
                    {
                        OS_WaitSemaphore(gClientInfo[i].lockSem);

                        gClientInfo[i].s32TcpFd = s32SockFd;
                        strcpy(gClientInfo[i].ipaddr, inet_ntoa(client_addr.sin_addr));
                        gClientInfo[i].activeFlag = HI_TRUE;
                        gClientInfo[i].callCnt = 0;
                        gClientInfo[i].ackState = HI_FALSE;
                        gClientInfo[i].chipIndex = 0x00;

                        OS_SignalSemaphore(gClientInfo[i].lockSem);

                        break;
                    }
                }

                if (i >= MAX_MAIN_AVPLAY_NUM)
                {
                    continue;
                }		
            }
        }

        for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
        {
            /*检查是否在这个集合里面*/
            if ((FD_ISSET(gClientInfo[i].s32TcpFd, &fd_set_r)) && (gClientInfo[i].activeFlag == HI_TRUE))
            {
                memset(gClientInfo[i].u8RecvBuffer, 0x00, sizeof(gClientInfo[i].u8RecvBuffer));
                if (PORT_SOCKET_Recv_Msg_TCP(gClientInfo[i].s32TcpFd, u8RecvBuffer, &s32RcvLength) != GOS_SUCCESS)
                {

                    DEBUG_INFO("FD_ISSET::=====i:%d==activeFlag:%d==s32SockFd:%d,IP:%s\n",i, gClientInfo[i].activeFlag,gClientInfo[i].s32TcpFd, gClientInfo[i].ipaddr);
                    OS_WaitSemaphore(gClientInfo[i].lockSem);
                    gClientInfo[i].callCnt = 4;
                    gClientInfo[i].activeFlag = HI_FALSE;
                    gClientInfo[i].ackState = HI_FALSE;

                    if(gClientInfo[i].s32TcpFd > 0)
                        close(gClientInfo[i].s32TcpFd);
                    gClientInfo[i].s32TcpFd = -1;
                    OS_SignalSemaphore(gClientInfo[i].lockSem);
                    continue;
                }

                //DEBUG_INFO("s32RcvLength : %d %d\n", s32RcvLength, gClientInfo[i].activeFlag);
                if (s32RcvLength == 0)
                {
                    OS_WaitSemaphore(gClientInfo[i].lockSem);
                    gClientInfo[i].callCnt = 4;
                    gClientInfo[i].activeFlag = HI_FALSE;
                    gClientInfo[i].ackState = HI_FALSE;

                    if(gClientInfo[i].s32TcpFd > 0)
                        close(gClientInfo[i].s32TcpFd);
                    gClientInfo[i].s32TcpFd = -1;
                    OS_SignalSemaphore(gClientInfo[i].lockSem);
                    continue;
                }

                if (gClientInfo[i].activeFlag != HI_TRUE)
                    continue;

                if (s32RcvLength > 0)
                {
                    pOffset = u8RecvBuffer;//获取得到的数据
                    pOffset[s32RcvLength] = '\0';//为数据末尾添加结束符
msg_check_type:
                    s32Ret = PORT_SOCKET_Msg_Check(&stMsgHead, pOffset, s32RcvLength);
                    if (s32Ret == eMSG_GET_HEAD)/*一条命令被分解成多次发送时*/
                    {
                        gClientInfo[i].dataLen = 0;
                        memset(gClientInfo[i].u8RecvBuffer, 0x00, sizeof(gClientInfo[i].u8RecvBuffer));
                        memcpy(gClientInfo[i].u8RecvBuffer, pOffset, s32RcvLength);

                        gClientInfo[i].dataLen = s32RcvLength;
                    }
                    else if (s32Ret == eMSG_GET_PART)/*一条命令被分解成多次发送时*/
                    {
                        if(gClientInfo[i].dataLen == 0)
                        {
                            break;
                        }

                        memcpy(gClientInfo[i].u8RecvBuffer + gClientInfo[i].dataLen, pOffset, s32RcvLength);
                        gClientInfo[i].dataLen = gClientInfo[i].dataLen + s32RcvLength;
                        if (stMsgHead.u32DataLen == gClientInfo[i].dataLen)
                        {
                            GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, gClientInfo[i].u8RecvBuffer, gClientInfo[i].dataLen);
                            gClientInfo[i].dataLen = 0;
                            memset(gClientInfo[i].u8RecvBuffer, 0x00, sizeof(gClientInfo[i].u8RecvBuffer));
                        }
                        else if(stMsgHead.u32DataLen < gClientInfo[i].dataLen)
                        {
                            GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index,  gClientInfo[i].u8RecvBuffer, stMsgHead.u32DataLen);
                            memset(gClientInfo[i].u8RecvBuffer, 0x00, sizeof(gClientInfo[i].u8RecvBuffer));
                            gClientInfo[i].dataLen = gClientInfo[i].dataLen - stMsgHead.u32DataLen;
                            if(s32RcvLength <= gClientInfo[i].dataLen)
                            {
                                break;
                            }

                            pOffset = pOffset + (s32RcvLength - gClientInfo[i].dataLen);
                            s32RcvLength = gClientInfo[i].dataLen;
                            gClientInfo[i].dataLen = 0;
                            goto msg_check_type;

                        }
                    }	
                    else if (s32Ret == eMSG_GET_ONE)
                    {
                        if (stMsgHead.u32DataLen <= s32RcvLength)
                        {
                            GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, pOffset, s32RcvLength);
                        }
                    }
                    else if (s32Ret == eMSG_GET_MORE)/*同时接收到多条命令时*/
                    {
                        GOS_AVPLAYHOST_Dispatch_RecvMsg(socket_index, pOffset, stMsgHead.u32DataLen);
                        s32RcvLength = s32RcvLength - stMsgHead.u32DataLen;
                        pOffset = pOffset + stMsgHead.u32DataLen;
                        goto msg_check_type;
                    }
                }
            }
        }
    }
}

static void socket_board_check_task(void *arg)
{
    int i, j;

    while(gCheck_client_run)
    {
#if 1
        for (i = 0; i < MAX_SLOT_NUM; i++)
        {
            for (j = 0; j < 2; j++)
            {
                if ((0 == i)&&(0 == j))
                {
                    continue;
                }

                if (gSlotInfo[i].pClientInfo[j] != NULL)
                {
#if 0
                    printf("socket_board_check_task %d : ip : %s chip : %d states : %d %d %d\n" , i,
                            gSlotInfo[i].pClientInfo[j]->ipaddr,
                            gSlotInfo[i].pClientInfo[j]->chipIndex,
                            gSlotInfo[i].pClientInfo[j]->activeFlag,
                            gSlotInfo[i].pClientInfo[j]->ackState,
                            gSlotInfo[i].pClientInfo[j]->callCnt);
#endif
                    if (/*(gSlotInfo[i].pClientInfo[j]->activeFlag == HI_TRUE) &&*/
                            (gSlotInfo[i].pClientInfo[j]->chipIndex != 0xFF) &&
                            (gSlotInfo[i].pClientInfo[j]->chipIndex != 0x00))
                    {
                        if (gSlotInfo[i].pClientInfo[j]->ackState == HI_TRUE)
                        {
                            OS_WaitSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                            gSlotInfo[i].pClientInfo[j]->ackState = HI_FALSE;
                            gSlotInfo[i].pClientInfo[j]->callCnt = 0;

                            OS_SignalSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                            socket_server_call_client(gSlotInfo[i].pClientInfo[j]->chipIndex);
                        }
                        else
                        {
                            if (gSlotInfo[i].pClientInfo[j]->callCnt > 3)
                            {	
                                printf("===========Lost Link===socket:%d--chip:%d=====================\n",gSlotInfo[i].pClientInfo[j]->s32TcpFd,gSlotInfo[i].pClientInfo[j]->chipIndex);
                                if (gSlotInfo[i].pClientInfo[j] != NULL)
                                {
                                    /*通知重启*/
                                    Socket_Server_Broadcast_toReboot(gSlotInfo[i].pClientInfo[j]->chipIndex);

                                    OS_WaitSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                                    gSlotInfo[i].pClientInfo[j]->callCnt = 0;
                                    gSlotInfo[i].pClientInfo[j]->activeFlag = HI_FALSE;
                                    gSlotInfo[i].pClientInfo[j]->ackState = HI_FALSE;
                                    gSlotInfo[i].pClientInfo[j]->chipIndex = 0x00;

                                    OS_SignalSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                                    if(gSlotInfo[i].pClientInfo[j]->s32TcpFd > 0)
                                        close(gSlotInfo[i].pClientInfo[j]->s32TcpFd);
                                    gSlotInfo[i].pClientInfo[j]->s32TcpFd = -1;
                                    gSlotInfo[i].pClientInfo[j] = NULL;
                                }
                            }
                            else
                            {
                                OS_WaitSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                                gSlotInfo[i].pClientInfo[j]->ackState = HI_FALSE;
                                gSlotInfo[i].pClientInfo[j]->callCnt++;

                                OS_SignalSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                                socket_server_call_client(gSlotInfo[i].pClientInfo[j]->chipIndex);
                            }
                        }
                    }
                }
            }
        }
#endif
        sleep(3);
    }
}

GOS_S32 Socket_Server_Open(void)
{
    int i = 0;

    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        gSlotInfo[i].activeClientCnt = 0;
        gSlotInfo[i].pClientInfo[0] = NULL;
        gSlotInfo[i].pClientInfo[1] = NULL;
        gSlotInfo[i].rebootGpioIndex = i;
        gSlotInfo[i].slotTimeCnt = 0;
    }

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        gClientInfo[i].activeFlag = HI_FALSE;
        gClientInfo[i].s32TcpFd = -1;
        gClientInfo[i].s32UdpFd = -1;
        memset((void *)gClientInfo[i].ipaddr, 0x00, sizeof(gClientInfo[i].ipaddr));
        gClientInfo[i].chipIndex = 0xFF;
        gClientInfo[i].callCnt = 0;
        gClientInfo[i].ackState = HI_FALSE;
        gClientInfo[i].lockSem = OS_CreateSemaphore(1);
    }

    OS_Task_Handle_t  msg_dispatch_handle;	 	
    if (OS_CreateTask(socket_udpmsg_dispatch_task, (void*)i, (OS_Task_Handle_t*)&msg_dispatch_handle) != GOS_TRUE)
    {
        LOG_OUTPUTF(E_LOG_LEVELERROR, "Failed to create the install_task \n" );
        return GOS_FAILURE;
    }

    gGetMsg_client_run = GOS_TRUE;
    gCheck_client_run = GOS_TRUE;
    if (!OS_CreateTask(socket_hmsg_dispatch_task, NULL, (OS_Task_Handle_t*)&Handle_HMsg_Dispatch))
    {
        DEBUG_ERROR("Create the socket_msg_dispatch_task Failed!\n");
        return GOS_FAILURE;
    }

    if (!OS_CreateTask(socket_board_check_task, NULL, (OS_Task_Handle_t*)&Handle_Board_Check))
    {
        DEBUG_ERROR("Create the media_moniterSocket_task Failed!\n");
        return GOS_FAILURE;
    }

    DEBUG_INFO("Socket_Server_Open Success!\n");
    return GOS_SUCCESS;
}

GOS_S32 Socket_Check_Task_Close(void)
{
    gCheck_client_run = GOS_FALSE;
    return GOS_SUCCESS;
}

/*获取子板的状态*/
GOS_BOOL Socket_Server_GetSlaveStatus(int chipID)
{
    int i = 0;
    int j = 0;
    GOS_BOOL bStatus = GOS_FALSE;

    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (gSlotInfo[i].pClientInfo[j] != NULL)
            {
                if(gSlotInfo[i].pClientInfo[j]->chipIndex == chipID)
                {
                    DEBUG_INFO("### Online chip:%d\n",chipID);
                    bStatus = GOS_TRUE;	
                    break;
                }
            }
        }
    }
    return bStatus;
}

/*根据IP地址获取子板的chipID*/
GOS_S32 Socket_Server_GetSlaveChipID(char *pAddr)
{
    int i = 0;
    int j = 0;
    int chipid = -1;

    if(GOS_NULL == pAddr)
    {
        return chipid;
    }

    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (gSlotInfo[i].pClientInfo[j] != NULL)
            {
                if (0 == memcmp((const char *)gSlotInfo[i].pClientInfo[j]->ipaddr, (const char *)pAddr,32))			
                {
                    DEBUG_INFO("###ip seach chipID :%d\n",chipid);
                    chipid = i * 2 + j;	
                    break;
                }
            }
        }
    }

    if(chipid == 0x00)	/*host chipID*/
        chipid = -1;

    return chipid;
}

/*根据chipID和IP地址获取状态*/
GOS_BOOL Socket_Server_GetSlaveStatus_Ex(int chipID, char *pAddr)
{
    int i = 0;
    int j = 0;
    GOS_BOOL bStatus = GOS_FALSE;

    if(GOS_NULL == pAddr)
    {
        return bStatus;
    }

    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (gSlotInfo[i].pClientInfo[j] != NULL)
            {
                if((gSlotInfo[i].pClientInfo[j]->chipIndex == chipID) && (!memcmp((const char *)gSlotInfo[i].pClientInfo[j]->ipaddr, (const char *)pAddr,32)))
                {
                    bStatus = GOS_TRUE;	
                    break;
                }
            }
        }
    }
    return bStatus;
}

GOS_S32 Socket_Server_CloseSlave(int chipID)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (gSlotInfo[i].pClientInfo[j] != NULL)
            {
                if(gSlotInfo[i].pClientInfo[j]->chipIndex == chipID)
                {
                    printf("##############-------Manual Close chip:%d\n",chipID);	
                    OS_WaitSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                    gSlotInfo[i].pClientInfo[j]->callCnt = 0;
                    gSlotInfo[i].pClientInfo[j]->activeFlag = HI_FALSE;
                    gSlotInfo[i].pClientInfo[j]->ackState = HI_FALSE;
                    gSlotInfo[i].pClientInfo[j]->chipIndex = 0x00;

                    OS_SignalSemaphore(gSlotInfo[i].pClientInfo[j]->lockSem);

                    if(gSlotInfo[i].pClientInfo[j]->s32TcpFd > 0)
                        close(gSlotInfo[i].pClientInfo[j]->s32TcpFd);
                    gSlotInfo[i].pClientInfo[j]->s32TcpFd = -1;
                    gSlotInfo[i].pClientInfo[j] = NULL;
                }
            }
        }
    }
    return GOS_SUCCESS;
}

GOS_S32 Socket_Server_Close(void)
{
    int	i = 0;

    gGetMsg_client_run = GOS_FALSE;

    //OS_DeleteTask(Handle_HMsg_Dispatch);
    //OS_DeleteTask(Handle_Board_Check);
    //OS_DeleteTask(Handle_Serv_Netport_Process);

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if (gClientInfo[i].s32TcpFd > 0)
        {
            close(gClientInfo[i].s32TcpFd);
            gClientInfo[i].activeFlag = HI_FALSE;
            gClientInfo[i].s32TcpFd = -1;
            memset((void *)gClientInfo[i].ipaddr, 0x00, sizeof(gClientInfo[i].ipaddr));
            gClientInfo[i].chipIndex = 0xFF;
            gClientInfo[i].callCnt = 0;
            gClientInfo[i].ackState = HI_FALSE;
            OS_DeleteSemaphore(gClientInfo[i].lockSem);
            gClientInfo[i].lockSem = NULL;
        }
    }

    for (i = 0; i < MAX_SLOT_NUM; i++)
    {
        gSlotInfo[i].activeClientCnt = 0;
        gSlotInfo[i].pClientInfo[0] = NULL;
        gSlotInfo[i].pClientInfo[1] = NULL;
        gSlotInfo[i].rebootGpioIndex = i;
        gSlotInfo[i].slotTimeCnt = 0;
    }

    return GOS_SUCCESS;
}

GOS_S32 Socket_Server_Recv_Client(GOS_U32 askChipIndex)
{
    int i = 0;

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if ((gClientInfo[i].chipIndex != 0xFF) &&
                (gClientInfo[i].chipIndex != 0x00) &&
                (gClientInfo[i].chipIndex == askChipIndex) )
        {
            //DEBUG_INFO("received chip<%d> ask!\n", askChipIndex);
            OS_WaitSemaphore(gClientInfo[i].lockSem);

            gClientInfo[i].ackState = HI_TRUE;

            OS_SignalSemaphore(gClientInfo[i].lockSem);
        }
    }

    return GOS_SUCCESS;
}

GOS_S32 Socket_Server_Match_Client(SOCKET_CLIENT_INFO_t* pstClientInfo)
{
    int i = 0;
    int iSlotIndex = 0;

    if (!pstClientInfo)
    {
        DEBUG_ERROR("The pstClientInfo is NULL\n");
        return GOS_FAILURE;
    }

    //DEBUG_INFO("Socket_Server_Match_Client->type:%d, chipIndex:%d, ipaddr:%s\n", pstClientInfo->type, pstClientInfo->chipIndex, pstClientInfo->ipaddr);

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if (gClientInfo[i].activeFlag == HI_TRUE)
        {
            if (0 == memcmp((const char *)gClientInfo[i].ipaddr, (const char *)pstClientInfo->ipaddr,sizeof(pstClientInfo->ipaddr)))
            {
                OS_WaitSemaphore(gClientInfo[i].lockSem);

                gClientInfo[i].type = pstClientInfo->type;
                gClientInfo[i].chipIndex = pstClientInfo->chipIndex;

                DEBUG_INFO("Socket_Server_Match_Client->type:%d, chipIndex:%d, ipaddr:%s\n", pstClientInfo->type, pstClientInfo->chipIndex, pstClientInfo->ipaddr);

                OS_SignalSemaphore(gClientInfo[i].lockSem);

                iSlotIndex = (int)(gClientInfo[i].chipIndex /2);
                if(gClientInfo[i].chipIndex % 2 ==0)
                {
                    gSlotInfo[iSlotIndex].pClientInfo[0] = &gClientInfo[i];
                }
                else
                {
                    gSlotInfo[iSlotIndex].pClientInfo[1] = &gClientInfo[i];
                }

                gSlotInfo[iSlotIndex].rebootGpioIndex = iSlotIndex;
                break;
            }
        }
    }

    if (i == MAX_MAIN_AVPLAY_NUM)
    {
        DEBUG_ERROR("Not match any ipaddress !\n");
        return GOS_FAILURE;
    }

    return GOS_SUCCESS;
}

GOS_S32 Socket_Server_Send_TCP_Msg(int iChipIndex, const void* pData, int iDatalen)
{
    int  i = 0;
    int ret = GOS_SUCCESS;

    if (!pData)
    {
        DEBUG_ERROR("The pData is NULL\n");
        return GOS_FAILURE;
    }

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if (gClientInfo[i].chipIndex == (iChipIndex ))
            break;
    }

    if (i >= MAX_MAIN_AVPLAY_NUM)
    {
        //DEBUG_ERROR("no find chipIndex,iChipIndex:%d\n",iChipIndex);
        return GOS_FAILURE;
    }

    if (gClientInfo[i].activeFlag == HI_FALSE)
    {
        //DEBUG_ERROR("Maybe ChipIndex(%d) not connect\n", iChipIndex);
        return GOS_FAILURE;
    }

    OS_WaitSemaphore(gClientInfo[i].lockSem);
    ret = PORT_SOCKET_Send_Msg_TCP(gClientInfo[i].s32TcpFd, MYPORT, gClientInfo[i].ipaddr, pData, iDatalen);
    if(ret == GOS_FAILURE)
    {
        if (gClientInfo[i].s32TcpFd > 0)
        {
            //OS_WaitSemaphore(gClientInfo[i].lockSem);
            gClientInfo[i].callCnt = 4;
            gClientInfo[i].activeFlag = HI_FALSE;
            gClientInfo[i].ackState = HI_FALSE;

            if(gClientInfo[i].s32TcpFd > 0)
                close(gClientInfo[i].s32TcpFd);
            gClientInfo[i].s32TcpFd = -1;
            //OS_SignalSemaphore(gClientInfo[i].lockSem);
        }
    }
    OS_SignalSemaphore(gClientInfo[i].lockSem);

    return ret;
}

GOS_S32 Socket_Server_Send_TCP_Msg_All(const void* pData, int iDatalen)
{
    int  i = 0;
    int ret = GOS_SUCCESS;

    for (i = 0; i < MAX_MAIN_AVPLAY_NUM; i++)
    {
        if (gClientInfo[i].activeFlag == HI_FALSE)
            continue;

        OS_WaitSemaphore(gClientInfo[i].lockSem);
        ret = PORT_SOCKET_Send_Msg_TCP(gClientInfo[i].s32TcpFd, MYPORT, gClientInfo[i].ipaddr, pData, iDatalen);
        if(ret == GOS_FAILURE)
        {
            if (gClientInfo[i].s32TcpFd > 0)
            {
                gClientInfo[i].callCnt = 4;
                gClientInfo[i].activeFlag = HI_FALSE;
                gClientInfo[i].ackState = HI_FALSE;

                if(gClientInfo[i].s32TcpFd > 0)
                    close(gClientInfo[i].s32TcpFd);
                gClientInfo[i].s32TcpFd = -1;
            }
        }
        else
        {
            DEBUG_INFO("----------Send Slave Chip:%d Reboot OK!!!-------\n", i+1);
        }
        OS_SignalSemaphore(gClientInfo[i].lockSem);
    }

    return ret;
}

/*使用广播方式通知从芯片重启(tvwall_dog接收)，0:所有芯片重启，1-15分别是chip1-chip15*/
#define  BROADCAST_PORT 		7676
static int getBroadcastIP(struct sockaddr_in *sin)
{
    int sock;
    char *ETH_NAME = "eth1";

    struct ifreq ifr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return -1;
    }
    memset(&ifr,0,sizeof(ifr));
    memcpy(ifr.ifr_name,ETH_NAME,sizeof(ETH_NAME));

    if (ioctl(sock, SIOCGIFBRDADDR, &ifr) < 0)
    {
        perror("ioctl");
        close(sock);
        return -1;
    }
    close(sock);

    memcpy(sin,&(ifr.ifr_broadaddr),sizeof(struct sockaddr_in));

    return 0;
}

GOS_S32 Socket_Server_Broadcast_toReboot(int chipID)
{
    int sockfd;
    struct sockaddr_in 	ser_addr;

    if((15 < chipID) || (0 > chipID))
    {
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    getBroadcastIP(&ser_addr);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port   = htons(BROADCAST_PORT);

    int opt=1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    printf("Creat Success! broadCastIP is %s \n",inet_ntoa(ser_addr.sin_addr));

    int sendmsg = chipID;
    int i = 0;
    int n = sendto(sockfd, &sendmsg, sizeof(sendmsg), 0, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));//发送数据
    if( n < 0)
    {
        for(i = 0; i < 5; i++)
        {
            sendto(sockfd, &sendmsg, sizeof(sendmsg), 0, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr_in));
        }
        //perror("sendto()");
        close(sockfd);
        return 0;
    } 

    printf("\nSend to chip:%d Reboot OK!!!\n", chipID);

    close(sockfd);

    return 0;
}

