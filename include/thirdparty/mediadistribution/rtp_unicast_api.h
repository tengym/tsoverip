#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef __RTP_UNICAST_API_H__
#define __RTP_UNICAST_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RTP_UNICAST_FAILURE		(-1) /*operate failure*/
#define RTP_UNICAST_SUCCESS   	(0)  /*operate success*/
#define RTP_UNICAST_NULL      	(-2) /*null poiter*/

#define RTP_UNICAST_TRUE	    (1)
#define RTP_UNICAST_FALSE	    (0)

#define MAX_RTP_UNICAST_ONLINE_NUM 120

typedef void (*RTP_UNICAST_RECVMSG_FUNC)(unsigned char *data, int datalen);

extern signed int rtp_unicast_init(void);
extern signed int rtp_unicast_deinit(void);

extern signed int rtp_unicast_create(int port, int payloadtype, const char *ipaddr, int *rtphandle);
extern signed int rtp_unicast_destory(int rtphandle);

extern signed int rtp_unicast_sendmsg(int rtphandle, const char *data, int datalen, int payloadtype);

extern signed int rtp_unicast_unregister_recvmsg_func(int rtphandle);
extern signed int rtp_unicast_register_recvmsg_func(int rtphandle, RTP_UNICAST_RECVMSG_FUNC recvfunc);
extern signed int rtp_unicast_open_recvmsg(int rtphandle);
extern signed int rtp_unicast_close_recvmsg(int rtphandle);

#ifdef __cplusplus
}
#endif

#endif
