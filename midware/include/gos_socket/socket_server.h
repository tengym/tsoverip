#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "porting_socket.h"

GOS_S32 Socket_Server_Open(void);
GOS_S32 Socket_Server_Close(void);
GOS_S32 Socket_Server_Recv_Client(GOS_U32 askChipIndex);
GOS_S32 Socket_Server_Match_Client(SOCKET_CLIENT_INFO_t* pstClientInfo);
GOS_S32 Socket_Server_Send_TCP_Msg(int iChipIndex, const void* pData, int iDatalen);
GOS_S32 Socket_Check_Task_Close(void);
GOS_S32 Socket_Server_Send_TCP_Msg_All(const void* pData, int iDatalen);
GOS_S32 Socket_Server_CloseSlave(int chipID);
GOS_S32 Socket_Server_Broadcast_toReboot(int chipID);
GOS_BOOL Socket_Server_GetSlaveStatus(int chipID);
GOS_BOOL Socket_Server_GetSlaveStatus_Ex(int chipID, char *pAddr);
GOS_S32 Socket_Server_GetSlaveChipID(char *pAddr);

#ifdef __cplusplus
}
#endif

#endif

