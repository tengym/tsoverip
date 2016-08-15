#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include<time.h>
#include<stdio.h>
#include<signal.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<assert.h>

typedef int (*EPOLL_EV_CB)(void *arg);

int threadpool_api_create(int thread_num);
int threadpool_api_destory(void);
int threadpool_api_addtask(EPOLL_EV_CB callback, void *arg);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

