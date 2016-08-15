#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

GOS_S32 Socket_Client_Open(void);
GOS_S32 Socket_Client_Close(void);
GOS_S32 Socket_Get_ChipIndex(void);
GOS_S32 Socket_Client_Send_TCP_Msg(const void* pData, int iDatalen);
GOS_S32 Socket_Client_Send_UDP_Msg(const void* pData, int iDatalen);
GOS_S32 SOCKET_Creat_RecvIP_Data();
GOS_S32 Socket_Client_TcpSocket_Open(void);
GOS_S32 Socket_Client_TcpSocket_Close(void);
GOS_S32 Socket_Client_Recv_Server(void);

#ifdef __cplusplus
}
#endif

#endif

