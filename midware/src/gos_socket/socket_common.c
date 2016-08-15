#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "user_define.h"
#include "linuxos.h"
#include "hi_type.h"
#include "hi_common.h"
#include "gos_socket.h"
#include "gos_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOCKET_IS_MSG(a) ((a[0] == 'M') && (a[1] == 'S') && (a[2]== 'G'))

GOS_S32 PORT_SOCKET_Detect_NetStatus_byPing(const char *if_name, const char *pingAddr)
{
	int ret ;
	char SYS_CMD[256] = {0};

	if((NULL == if_name) || (NULL == pingAddr))
		return -1;

	sprintf(SYS_CMD,"ping %s -c 1 -W 1 > /dev/null 2>/dev/null",pingAddr);

	ret = system(SYS_CMD);		//成功返回0
	return ret;
}


GOS_S32 PORT_SOCKET_Get_HostIP(char *HostIP, const char *eth_name)
{
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		perror("socket");
		return -1;
	}
	strncpy(ifr.ifr_name, eth_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl");
		close(sock);
		return -1;
	}
	close(sock);
	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	sprintf(HostIP,"%s", inet_ntoa(sin.sin_addr));
	return 0;
}

/* desIP = srcIP + ChipID, desIP一般是从板IP,srcIP为主板IP */
GOS_S32 PORT_SOCKET_GetIP_ByChipID(char *desIP, char *srcIP, int chipID)
{
	unsigned long uNativeIP;
	char pName[32] = {0};
	
	if((NULL == desIP) || (NULL == srcIP) )
	{
		return -1;
	}	

	uNativeIP = ntohl(inet_addr(srcIP));
	uNativeIP += chipID;
	uNativeIP = SWAP32(uNativeIP);

	ip2stringex(uNativeIP, pName);

	strcpy(desIP, pName);

	return 0;
}

GOS_S32 PORT_SOCKET_Send_Msg_TCP(int sockedfd, int portnum, const char* ipaddr, const char* pdata, int datalen)
{
	struct iovec io;
	struct msghdr msg;
	struct sockaddr_in sevice_addr;

	if (!ipaddr || !pdata)
	{
		LOG_ERROR("The input param invalied !\n");
		return GOS_FAILURE;
	}
	if(sockedfd <= 0)
	{
		LOG_ERROR("The input param invalied !\n");
		return GOS_FAILURE;
	}

	bzero(&sevice_addr, sizeof(sevice_addr));
	sevice_addr.sin_family = AF_INET;
	sevice_addr.sin_port = htons(portnum);
	inet_pton(AF_INET, ipaddr, &sevice_addr.sin_addr);

	//LOG_INFO("the ip is %s the portnum is %d\n", ipaddr, portnum);

	io.iov_base = (void*)pdata;
	io.iov_len = datalen;
	msg.msg_name = &sevice_addr;
	msg.msg_namelen = sizeof(struct sockaddr_in);
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	msg.msg_control = 0;
    msg.msg_controllen = 0;
   // msg.msg_flags = 0;
    msg.msg_flags = MSG_NOSIGNAL;
#if 0
	OS_Printf_Buffer(io.iov_base, io.iov_len);
#endif
	if (sendmsg(sockedfd, &msg, 0) < 0)
	{
		LOG_ERROR("> sendmsg error... :%d\n", errno);
		
		if(errno == SIGPIPE)
		{
			printf("socket closed 1...\n");
		}
		else if(errno == ECONNRESET)
		{
			printf("socket closed 2...\n");
		}
		else if(errno == EAGAIN)
		{
			printf("发送缓冲区剩余空间小于发送包大小\n");
		}
		else if(errno== EINTR)
		{
        	printf("errno EINTR,异常，等待再次触发\n");
        	return GOS_SUCCESS;
    	}
		else if(errno== EDESTADDRREQ)
		{
	    	printf("errno EDESTADDRREQ,套接字不处于连接模式，没有指定对端地址\n");
	    	return GOS_SUCCESS;
		}
		else if(errno== EFAULT)
		{
	    	printf("errno EFAULT,内存空间访问出错\n");
	    	return GOS_SUCCESS;
		}

		return GOS_FAILURE;
	}

	return GOS_SUCCESS;
}

GOS_S32 PORT_SOCKET_Recv_Msg_TCP(int sockedfd, void* pdata, int* pdatalen)
{
	int recvlen = 0;
	struct msghdr msg;
	struct iovec  io;

	if (!pdata)
	{
		LOG_ERROR("The input param invalied !\n");
		return GOS_FAILURE;
	}
	if(sockedfd <= 0)
	{
		LOG_ERROR("The input param invalied !\n");
		return GOS_FAILURE;
	}

	io.iov_base = pdata;//只用了一个缓冲区
	io.iov_len	= 1024 * 1024;//定义返回数据长度
	msg.msg_name   = NULL;//在tcp中，可以设置为NULL
	msg.msg_iov    = &io;
	msg.msg_iovlen = 1;//只用了一个缓冲区，所以长度为1
	msg.msg_flags = 0;//MSG_WAITALL;

	if ((recvlen = recvmsg(sockedfd, &msg, 0)) < 0)
	{
		printf("> recvmsg error... :%d\n", errno);
		if(errno== EAGAIN)
		{
        	printf("RE-Len:%d errno EAGAIN,异常，等待再次触发读取\n", recvlen);
        	return GOS_SUCCESS;
    	}
		else if(errno== EINTR)// 被信号所中断
		{
        	printf("RE-Len:%d errno EINTR,异常，等待再次触发读取\n", recvlen);
    	}
		else if(errno== ENOBUFS) // 系统的缓冲内存不足
		{
        	printf("RE-Len:%d errno ENOBUFS, 系统的缓冲内存不足\n", recvlen);
    	}
		else if(errno==ENOMEM) // 核心内存不足
		{
        	printf("RE-Len:%d errno ENOMEM,  核心内存不足\n", recvlen);
    	}
		
    	if(0 != PORT_SYS_Get_ChipID())
		{
            while(1)
            {
			    system("reboot");
            }
		}
		return GOS_FAILURE;
	}
	else if(recvlen == 0)
	{
		//printf("PORT_SOCKET_Recv_Msg_TCP Get FIN\n");
		printf("> recvmsg error... :%d\n", errno);
		printf("PORT_SOCKET_Recv_Msg_TCP recvlen =0,msg_flags=%d\n",msg.msg_flags);
		switch(msg.msg_flags)
		{
		case MSG_EOR:
			printf("指示记录边界,收到的数据完成了一个记录\n");
			break;
		case MSG_TRUNC:
			printf("表明数据报的尾部被丢弃了，因为数据报比提供的缓冲大\n");
			break;
		case MSG_CTRUNC:
			printf("表明一些控制数据被丢弃了，因为没有更多的空间存储辅助数据\n");
			break;
		case MSG_OOB:
			printf("返回表明收到了紧急或带外数据\n");
			break;
		case MSG_ERRQUEUE:
			printf(">MSG_ERRQUEUE recvmsg error... :%d\n", errno);
			break;

		//case MSG_WAITALL:
			//printf(">MSG_WAITALL 等待所有数据0x100\n");
			//break;
		case MSG_FIN:
			printf(">MSG_FIN\n");
			break;
		}
		
		if(0 != PORT_SYS_Get_ChipID())
		{
            while(1)
            {
			    system("reboot");
            }
		}
		return GOS_FAILURE;
	}
#if 0
	OS_Printf_Buffer(msg.msg_iov[0].iov_base, recvlen);
#endif
	if (pdatalen)
	{
		*pdatalen = recvlen;
	}

	if (recvlen > 0)
	{
		memcpy(pdata, msg.msg_iov[0].iov_base, recvlen);
	}

	return GOS_SUCCESS;
}

GOS_S32 PORT_SOCKET_Msg_Check(SOCKET_MSG_INFO_S *stMsgInfo, const char* pMsgBuf, int u32DataLen)
{
	if (pMsgBuf == HI_NULL || stMsgInfo == HI_NULL)
	{
 		LOG_ERROR("param in error!!\n");
   		return eMSG_GET_FAIL;
	}

	if (!SOCKET_IS_MSG(pMsgBuf))
	{
		return eMSG_GET_PART;
	}
	
	memcpy(stMsgInfo, pMsgBuf + MSG_CH_LEN, sizeof(SOCKET_MSG_INFO_S));

	if (stMsgInfo->u32DataLen > u32DataLen)
	{
		return eMSG_GET_HEAD;
	}
	else if (stMsgInfo->u32DataLen < u32DataLen)
	{
		return eMSG_GET_MORE;
	}

	return eMSG_GET_ONE;
}

GOS_VOID* PORT_SOCKET_Msg_Package(SOCKET_MSG_INFO_S *stMsgInfo, const char* pDataBuf, HI_S32 s32DataLen)
{
	if ((s32DataLen <= 0))
	{
 		LOG_ERROR("param is error!!\n");
   		return HI_NULL;
	}

	stMsgInfo->u32DataLen = MSG_CH_LEN + sizeof(SOCKET_MSG_INFO_S) + s32DataLen;
	HI_VOID *pSendBuf = (HI_VOID*)OS_AllocMemory(stMsgInfo->u32DataLen);
	if ((pSendBuf == HI_NULL))
	{
 		LOG_ERROR("malloc error!!\n");
   		return HI_NULL;
	}

	memcpy(pSendBuf, "MSG", MSG_CH_LEN);
	memcpy(pSendBuf + MSG_CH_LEN, stMsgInfo, sizeof(SOCKET_MSG_INFO_S));
	memcpy(pSendBuf + MSG_CH_LEN + sizeof(SOCKET_MSG_INFO_S), pDataBuf, s32DataLen);

	return pSendBuf;
}

GOS_VOID* PORT_SOCKET_Msg_UnPackage(SOCKET_MSG_INFO_S *stMsgInfo, const char* pDataBuf, HI_S32 *ps32DataLen)
{
	if ((pDataBuf == HI_NULL) || (*ps32DataLen <= MSG_CH_LEN + sizeof(SOCKET_MSG_INFO_S)))
	{
 		LOG_ERROR("param in error!\n");
   		return HI_NULL;
	}
	if (!SOCKET_IS_MSG(pDataBuf))
	{
		//LOG_ERROR("No Msg....\n");
		return HI_NULL;
	}

	memcpy(stMsgInfo, pDataBuf + MSG_CH_LEN, sizeof(SOCKET_MSG_INFO_S));
	GOS_VOID* pu8DataBuf = (GOS_VOID*)(pDataBuf + (MSG_CH_LEN + sizeof(SOCKET_MSG_INFO_S)));

	if (stMsgInfo->u32DataLen != *ps32DataLen)
	{
		LOG_ERROR("stMsgInfo->u32DataLen :%d *pu32DataLen :%d\n", stMsgInfo->u32DataLen, *ps32DataLen);
		return HI_NULL;
	}

	*ps32DataLen = *ps32DataLen - MSG_CH_LEN - sizeof(SOCKET_MSG_INFO_S);
	return pu8DataBuf;
}

#ifdef __cplusplus
}
#endif



