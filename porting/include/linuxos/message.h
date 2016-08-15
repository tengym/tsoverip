#ifndef __OS_MSGQUEUE_MSG_H__
#define __OS_MSGQUEUE_MSG_H__

#include <pthread.h>
#include "user_define.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagMESSAGE_S
{
	GOS_U16  u16MsgLength;                                 /*data length*/
	GOS_U8*	pu8MsgData;                                   /*pointer to data*/

	struct tagMESSAGE_S * pstNext;
}stMESSAGE_S, *pstMESSAGE_S;

typedef struct tagstMSG_QUEUE_S
{
    GOS_U8*				pu8BaseAddr;           /*Init address,pointer to start position*/
    GOS_U32				u32MaxLen;             /*the max length of this Queue*/
    OS_Semaphore_t	*sem_QueueAccess;

    pstMESSAGE_S		pstMsgRear;            /* on receiving, pointer to write data */
    pstMESSAGE_S		pstMsgFront;           /* on parsing, pointer to read data */
}stMSG_QUEUE_S, *pstMSG_QUEUE_S;

GOS_S32 MsgQueue_Create(stMSG_QUEUE_S **pstMsgQueue, unsigned int ElementSize, unsigned int NoElements);
GOS_S32 MsgQueue_Destory(stMSG_QUEUE_S *pMsgQueue);
GOS_S32 MsgQueue_En(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg);
GOS_S32 MsgQueue_De(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg);
GOS_S32 MsgQueue_DeTimeOut(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg, GOS_U32 TimeOutMs);
GOS_S32 MsgQueue_Flush(stMSG_QUEUE_S *pstMsgQueue);



#ifdef __cplusplus
}
#endif

#endif


