#include "linuxos.h"
#include "message.h"

#define MIN_GAP (64) /* 64 Bytes*/

static GOS_S32 IsEmptyQueue(stMSG_QUEUE_S *pstMsgQueue)
{
    if (pstMsgQueue->pstMsgRear == pstMsgQueue->pstMsgFront)
    {
        return GOS_SUCCESS;
    }

    return GOS_FAILURE;
}

static GOS_S32 IsFullQueue(stMSG_QUEUE_S *pstMsgQueue)
{
    GOS_U32 u32Step = sizeof(stMESSAGE_S) + pstMsgQueue->pstMsgRear->u16MsgLength;

    GOS_U32 u32RemainSpace = 0;

    if (pstMsgQueue->pstMsgRear >= pstMsgQueue->pstMsgFront)
    {
        u32RemainSpace = ((GOS_U32)pstMsgQueue->pu8BaseAddr + pstMsgQueue->u32MaxLen) - (GOS_U32)pstMsgQueue->pstMsgRear;

        if ((GOS_U32)pstMsgQueue->pstMsgFront > (GOS_U32)pstMsgQueue->pu8BaseAddr)
        {
            u32RemainSpace += ((GOS_U32)pstMsgQueue->pstMsgFront - MIN_GAP - (GOS_U32)pstMsgQueue->pu8BaseAddr);
        }
        else
        {
            u32RemainSpace -= MIN_GAP;
        }
    }
    else
    {
        u32RemainSpace = (GOS_U32)pstMsgQueue->pstMsgFront - MIN_GAP - (GOS_U32)pstMsgQueue->pstMsgRear;
    }

    if (u32Step > u32RemainSpace)
    {
        /* Queue  full*/
        return GOS_SUCCESS;
    }

    return GOS_FAILURE;
}

static GOS_S32 MsgQueueEn(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pstMsg)
{
    GOS_U32 u32ItemSize = 0;
    GOS_U32 u32DataTypeSize = sizeof(stMESSAGE_S);
    stMESSAGE_S    *pstNextMsgAddr = GOS_NULL;
    stMSG_QUEUE_S  *pMsgQueue = pstMsgQueue;

    if ((pstMsg == GOS_NULL) || (pMsgQueue->pu8BaseAddr == GOS_NULL))
    {
        return GOS_FAILURE;
    }

    pMsgQueue->pstMsgRear->u16MsgLength = pstMsg->u16MsgLength;

    if (IsFullQueue(pMsgQueue) == GOS_SUCCESS)
    {
        return GOS_FAILURE;
    }

    u32ItemSize = u32DataTypeSize + pstMsg->u16MsgLength;

    if (((GOS_U32)pMsgQueue->pstMsgRear + u32ItemSize) > ((GOS_U32)pMsgQueue->pu8BaseAddr + pMsgQueue->u32MaxLen))
    {
        pMsgQueue->pstMsgRear->pu8MsgData = (GOS_U8 *)pMsgQueue->pu8BaseAddr + u32DataTypeSize;
    }
    else
    {
        pMsgQueue->pstMsgRear->pu8MsgData = (GOS_U8 *)((GOS_U32)pMsgQueue->pstMsgRear + u32DataTypeSize);
    }
    pMsgQueue->pstMsgRear->u16MsgLength = pstMsg->u16MsgLength;
    memcpy(pMsgQueue->pstMsgRear->pu8MsgData, pstMsg->pu8MsgData, pstMsg->u16MsgLength);
	pstNextMsgAddr = (stMESSAGE_S *)((GOS_U32)pMsgQueue->pstMsgRear + u32ItemSize);
	if (((GOS_U32)pstNextMsgAddr) > ((GOS_U32)pMsgQueue->pu8BaseAddr + pMsgQueue->u32MaxLen))
 	{
		pstNextMsgAddr = (stMESSAGE_S *)((GOS_U8*)pMsgQueue->pu8BaseAddr + u32ItemSize);
 	}
	else if ( (GOS_U32)pstNextMsgAddr == ((GOS_U32)pMsgQueue->pu8BaseAddr + pMsgQueue->u32MaxLen))
	{
		pstNextMsgAddr = (stMESSAGE_S *)pMsgQueue->pu8BaseAddr;
    }


    pstNextMsgAddr->pu8MsgData = GOS_NULL;
    pstNextMsgAddr->pstNext = GOS_NULL;
    pMsgQueue->pstMsgRear->pstNext = pstNextMsgAddr;
    pMsgQueue->pstMsgRear = pstNextMsgAddr;
    pMsgQueue->pstMsgRear->u16MsgLength = 0;

    if (IsFullQueue(pMsgQueue) == GOS_SUCCESS)
    {
        return GOS_FAILURE;
    }

    return GOS_SUCCESS;
}

static GOS_S32 MsgQueueDe(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pstMsg)
{
    stMSG_QUEUE_S  *pMsgQueue = pstMsgQueue;

    if (pstMsg == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    if (IsEmptyQueue(pMsgQueue) == GOS_SUCCESS)
    {
        return GOS_FAILURE;
    }

    pstMsg->u16MsgLength = pMsgQueue->pstMsgFront->u16MsgLength;
    pstMsg->pu8MsgData = pMsgQueue->pstMsgFront->pu8MsgData;
    pstMsg->pstNext = GOS_NULL;

    pMsgQueue->pstMsgFront = pMsgQueue->pstMsgFront->pstNext;

    return GOS_SUCCESS;
}

GOS_S32 MsgQueue_Create(stMSG_QUEUE_S **pstMsgQueue, unsigned int ElementSize, unsigned int NoElements)
{
    GOS_U8 *pszBufAddr = GOS_NULL;
    GOS_U32 u32BufLen = 0;

    u32BufLen = (ElementSize + sizeof(stMESSAGE_S)) * NoElements;
    pszBufAddr = (GOS_U8 *) OS_AllocMemory(u32BufLen);

    if (pszBufAddr == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    *pstMsgQueue = (pstMSG_QUEUE_S)OS_AllocMemory(sizeof(stMSG_QUEUE_S));

    if (*pstMsgQueue == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    (*pstMsgQueue)->sem_QueueAccess = OS_CreateSemaphore(1);

    (*pstMsgQueue)->pu8BaseAddr = pszBufAddr;
    (*pstMsgQueue)->u32MaxLen = u32BufLen;

    (*pstMsgQueue)->pstMsgFront = (stMESSAGE_S *)pszBufAddr;
    (*pstMsgQueue)->pstMsgFront->pu8MsgData = GOS_NULL;
    (*pstMsgQueue)->pstMsgFront->pstNext = GOS_NULL;

    (*pstMsgQueue)->pstMsgRear = (stMESSAGE_S *)pszBufAddr;
    (*pstMsgQueue)->pstMsgRear->pu8MsgData = GOS_NULL;
    (*pstMsgQueue)->pstMsgRear->pstNext = GOS_NULL;

    return GOS_SUCCESS;
}


GOS_S32 MsgQueue_Flush(stMSG_QUEUE_S *pstMsgQueue)
{

    if (pstMsgQueue == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    pstMsgQueue->pstMsgFront = (stMESSAGE_S *) pstMsgQueue->pu8BaseAddr;
    pstMsgQueue->pstMsgFront->pu8MsgData = GOS_NULL;
    pstMsgQueue->pstMsgFront->pstNext = GOS_NULL;

    pstMsgQueue->pstMsgRear = (stMESSAGE_S *) pstMsgQueue->pu8BaseAddr;
    pstMsgQueue->pstMsgRear->pu8MsgData = GOS_NULL;
    pstMsgQueue->pstMsgRear->pstNext = GOS_NULL;

    return GOS_SUCCESS;
}


GOS_S32 MsgQueue_Destory(stMSG_QUEUE_S *pMsgQueue)
{
    if (pMsgQueue == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    OS_DeleteSemaphore(pMsgQueue->sem_QueueAccess);

    if (pMsgQueue->pu8BaseAddr != GOS_NULL)
    {
        OS_FreeMemory(pMsgQueue->pu8BaseAddr);
    }

    pMsgQueue->pu8BaseAddr = GOS_NULL;
    pMsgQueue->u32MaxLen   = 0;
    pMsgQueue->pstMsgFront = GOS_NULL;
    pMsgQueue->pstMsgRear  = GOS_NULL;

    OS_FreeMemory(pMsgQueue);

    return GOS_SUCCESS;
}

GOS_S32 MsgQueue_En(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg)
{
    stMSG_QUEUE_S  *pMsgQueue = pstMsgQueue;

    if (pMsg == GOS_NULL || pstMsgQueue == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    OS_WaitSemaphore(pMsgQueue->sem_QueueAccess);

    if (GOS_SUCCESS != MsgQueueEn(pMsgQueue, pMsg))
    {
        OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);
        return GOS_FAILURE;
    }

    OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);

    return GOS_SUCCESS;
}

GOS_S32 MsgQueue_De(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg)
{
    stMSG_QUEUE_S  *pMsgQueue = pstMsgQueue;

    if (pMsg == GOS_NULL || pstMsgQueue == GOS_NULL)
    {
        return GOS_FAILURE;
    }

    OS_WaitSemaphore(pMsgQueue->sem_QueueAccess);

    if (GOS_SUCCESS != MsgQueueDe(pMsgQueue, pMsg))
    {
        OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);

        return GOS_FAILURE;
    }

    OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);

    return GOS_SUCCESS;
}

GOS_S32 MsgQueue_DeTimeOut(stMSG_QUEUE_S *pstMsgQueue, stMESSAGE_S *pMsg, GOS_U32 TimeOutMs)
{
	stMSG_QUEUE_S  *pMsgQueue = pstMsgQueue;

	if (pMsg == GOS_NULL || pstMsgQueue == GOS_NULL)
	{
		return GOS_FAILURE;
	}

	OS_WaitSemaphoreTimeOut(pMsgQueue->sem_QueueAccess,TimeOutMs);

	if (GOS_SUCCESS != MsgQueueDe(pMsgQueue, pMsg))
	{
		OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);

		return GOS_FAILURE;
	}

	OS_SignalSemaphore(pMsgQueue->sem_QueueAccess);

	return GOS_SUCCESS;
}


