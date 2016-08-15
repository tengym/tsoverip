#ifndef _PORTING_SOCKET_H_
#define _PORTING_SOCKET_H_

#include "user_define.h"
#include "linuxos.h"
#include "gosca.h"
#include "database.h"
#include "upgrd_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MYPORT 3490 /*用户接入端口*/
#define BACKLOG 32 /* 多少等待连接控制*/

#define IPCKEY				0x344370
#define HOST_ID				100
#define MSG_CH_LEN			3
#define MAX_SEND_BUF_LEN	1500

/*主板UDP传输地址*/
#define  RECV_ES_PORT		6000
#define  RECV_UDP_PORT	4567

/*子板UDP传输地址*/
//#define  SEND_ADDR			DEFAULT_SERVICE_IP
#define  SEND_ES_PORT 		6000

//#define  UDP_SERVER_ADDR	DEFAULT_SERVICE_IP
#define  SEND_UDP_PORT 		4567

typedef struct socket_data_head
{
	GOS_U16 send_id;
	GOS_U16 recv_id;
	GOS_U16 msg_id;
	GOS_U32 data_len;
}Socket_Msg_Data_Head;

typedef struct socket_msg_info
{
	HI_U16 u16SendID;	/*for slave:Window ID,for host:Flag ID*/
	HI_U16 u16RecvID;	/*for slave:Flag ID,for host:Window ID*/
	HI_U16 u16MsgID;	/*Message ID*/
	HI_U32 u32DataLen;
}SOCKET_MSG_INFO_S;

typedef struct
{
	HI_U8   u8DataType;
	HI_U32 u32DataLen;
	HI_U8* u8Data;
}SOCKET_MSG_RECV_S;

typedef struct
{
	GOS_CHAR  ipaddr[32];
	GOS_U8    type;
	GOS_U8    chipIndex;
}SOCKET_CLIENT_INFO_t;

typedef enum
{
	GOS_MSG_SVC_CALL = 0,
	
	GOS_MSG_MODEL_TOTAL
}GOS_MSG_Type;

typedef enum
{
	eMSG_GET_ONE = 0,
	eMSG_GET_MORE,
	eMSG_GET_HEAD,
	eMSG_GET_PART,
	eMSG_GET_FAIL,
	
}SOCKET_RECV_MSG_TYPE_E;

typedef enum
{
	SLAVE_NET_CONNECT = 0,
	SLAVE_NET_DISCONNECT,
	SLAVE_NET_READY
	
}SOCKET_NETPORT_STATUS_E;

GOS_S32 PORT_SOCKET_GetIP_ByChipID(char *desIP, char *srcIP, int chipID);
GOS_S32 PORT_SOCKET_Get_HostIP(char *HostIP, const char *eth_name);
GOS_S32 PORT_SOCKET_Send_Msg_TCP(int sockedfd, int portnum, const char* ipaddr, const char* pdata, int datalen);
GOS_S32 PORT_SOCKET_Recv_Msg_TCP(int sockedfd, void* pdata, int* pdatalen);
GOS_S32 PORT_SOCKET_Msg_Check(SOCKET_MSG_INFO_S *stMsgInfo, const char* pMsgBuf, int u32DataLen);
GOS_VOID* PORT_SOCKET_Msg_Package(SOCKET_MSG_INFO_S *stMsgInfo, const char* pDataBuf, HI_S32 s32DataLen);
GOS_VOID* PORT_SOCKET_Msg_UnPackage(SOCKET_MSG_INFO_S *stMsgInfo, const char* pDataBuf, HI_S32 *ps32DataLen);
GOS_S32 PORT_SOCKET_Detect_NetStatus_byPing(const char *if_name, const char *pingAddr);

#ifdef __cplusplus
}
#endif

#endif


