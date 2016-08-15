#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef __UDP_UNICAST_API_H__
#define __UDP_UNICAST_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define UDP_UNICAST_FAILURE		(-1) /*operate failure*/
#define UDP_UNICAST_SUCCESS   	(0)  /*operate success*/
#define UDP_UNICAST_NULL      	(-2) /*null poiter*/

#define UDP_UNICAST_TRUE	    (1)
#define UDP_UNICAST_FALSE	    (0)

#define MAX_UDP_UNICAST_ONLINE_NUM 120

#define UDP_UNICAST_PACKET_LEN  (188*8)

typedef enum
{
	UDP_UNICAST_SEND = 0x01,
	UDP_UNICAST_RECEIVE,
	UDP_UNICAST_BULT
}UDP_UNICAST_TYPE;

typedef void (*UDP_UNICAST_RECVMSG_FUNC)(unsigned char *data, int datalen);

extern signed int udp_unicast_init(void);
extern signed int udp_unicast_deinit(void);

extern signed int udp_unicast_create(UDP_UNICAST_TYPE type, int port, const char *ipaddr, int *udphandle);
extern signed int udp_unicast_destory(int udphandle);

extern signed int udp_unicast_sendmsg(int udphandle, const char *data, int datalen);

extern signed int udp_unicast_unregister_recvmsg_func(int udphandle);
extern signed int udp_unicast_register_recvmsg_func(int udphandle, UDP_UNICAST_RECVMSG_FUNC recvfunc);

extern signed int udp_unicast_open_recvmsg(int udphandle);
extern signed int udp_unicast_close_recvmsg(int udphandle);

#ifdef __cplusplus
}
#endif

#endif
