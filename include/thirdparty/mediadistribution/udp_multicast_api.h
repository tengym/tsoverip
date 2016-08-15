#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef __UDP_MULTICAST_API_H__
#define __UDP_MULTICAST_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define UDP_MULTICAST_FAILURE		(-1) /*operate failure*/
#define UDP_MULTICAST_SUCCESS   	(0)  /*operate success*/
#define UDP_MULTICAST_NULL      	(-2) /*null poiter*/

#define UDP_MULTICAST_TRUE	    (1)
#define UDP_MULTICAST_FALSE	    (0)

#define MAX_UDP_MULTICAST_ONLINE_NUM 120

#define UDP_MULTICAST_PACKET_LEN  (188*8)

typedef enum
{
	UDP_MULTICAST_SEND = 0x01,
	UDP_MULTICAST_RECEIVE,
	UDP_MULTICAST_BULT
}UDP_MULTICAST_TYPE;

typedef void (*UDP_MULTICAST_RECVMSG_FUNC)(unsigned char *data, int datalen);

extern signed int udp_multicast_init(void);
extern signed int udp_multicast_deinit(void);

extern signed int udp_multicast_create(UDP_MULTICAST_TYPE type, int port, const char *ipaddr, int *udphandle);
extern signed int udp_multicast_destory(int udphandle);
extern signed int udp_multicast_addroute(const char *ipaddr, const char *netdevice);

extern signed int udp_multicast_sendmsg(int udphandle, const char *data, int datalen);

extern signed int udp_multicast_unregister_recvmsg_func(int udphandle);
extern signed int udp_multicast_register_recvmsg_func(int udphandle, UDP_MULTICAST_RECVMSG_FUNC recvfunc);
extern signed int udp_multicast_open_recvmsg(int udphandle);
extern signed int udp_multicast_close_recvmsg(int udphandle);

#ifdef __cplusplus
}
#endif

#endif
