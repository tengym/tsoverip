/******************************************************************************
*
*             Copyright 2009 - 2050, Gospell Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
*
 ******************************************************************************
* File Name     : linuxos.c
* Description   :
*
* History       :
* Version     Date        Author      DefectNum    Modification:
* 1.0         2013/10/19   XX      NULL         Created file
*
 ******************************************************************************/
/* Include ------------------------------------------------------------------*/

#include <time.h>
#include "linuxos.h"
#include "linuxlist.h"
#include "message.h"

/* Definition ---------------------------------------------------------------*/

typedef struct MsgList
{
    struct list_head        stList;         /*链表头*/
    char                    QueName[32];    /*消息队列名称*/
    OS_MessageQueue_t      *MsgHandle;      /*消息队列句柄*/
} stMsgList, *pstMsgList;

static struct list_head     gListHead = LIST_HEAD_INIT(gListHead);

/* Function Prototype -------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
// 信号量
/*---------------------------------------------------------------------------*/
OS_Semaphore_t *OS_CreateSemaphore(const int InitialValue)
{
    OS_Semaphore_t *Semaphore_p = NULL;

    Semaphore_p = (OS_Semaphore_t *)OS_AllocMemory(sizeof(OS_Semaphore_t));

    if (Semaphore_p != NULL)
    {
        if (sem_init(Semaphore_p, 0, InitialValue))
        {
            OS_FreeMemory(Semaphore_p);
            return NULL;
        }
    }

    return Semaphore_p;
}

GOS_BOOL OS_DeleteSemaphore(OS_Semaphore_t *Semaphore_p)
{
    if (Semaphore_p != NULL)
    {
        sem_destroy(Semaphore_p);
        OS_FreeMemory(Semaphore_p);
    }
    else
    {
        return GOS_FALSE;
    }

    return GOS_TRUE;
}

GOS_BOOL OS_SignalSemaphore(OS_Semaphore_t *Semaphore_p)
{
    if (Semaphore_p != NULL)
    {
        sem_post(Semaphore_p);
        return GOS_TRUE;
    }

    return GOS_FALSE;
}

GOS_BOOL OS_WaitSemaphore(OS_Semaphore_t *Semaphore_p)
{
    if (Semaphore_p != NULL)
    {
        return (sem_wait(Semaphore_p) == 0) ? GOS_TRUE : GOS_FALSE;
    }

    return GOS_FALSE;
}

GOS_BOOL OS_WaitSemaphoreTimeOut(OS_Semaphore_t *Semaphore_p, GOS_U32 TimeOutMs)
{
    struct timespec  time_value;

    if (Semaphore_p != NULL)
    {
        clock_gettime(CLOCK_REALTIME, &time_value);
        time_value.tv_sec += TimeOutMs / 1000;

        if ((time_value.tv_nsec / 1000000 + TimeOutMs % 1000) >= 1000)
        {
            time_value.tv_sec += 1;
            time_value.tv_nsec -= (1000 - (TimeOutMs % 1000)) * 1000000;
        }
        else
        {
            time_value.tv_nsec += (TimeOutMs % 1000) * 1000000;
        }

        if (sem_timedwait(Semaphore_p, &time_value) == 0)
        {
            return GOS_TRUE;
        }
    }

    return GOS_FALSE;
}

GOS_BOOL OS_GetSemaphoreValue(OS_Semaphore_t *Semaphore_p, int *sval)
{
    if (Semaphore_p != NULL)
    {
        sem_getvalue(Semaphore_p, sval);
        return GOS_TRUE;
    }

    return GOS_FALSE;
}

/*---------------------------------------------------------------------------*/
// 消息队列
/*---------------------------------------------------------------------------*/
OS_MessageQueue_t *OS_CreateMessageQueue(const char *Qname, unsigned int ElementSize, unsigned int NoElements)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;
    stMSG_QUEUE_S      *pstMsgQueue = NULL;

    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && !memcmp(pstMsgList->QueName, Qname, strlen(Qname)))
        {
            return pstMsgList->MsgHandle;
        }
    }

    if (GOS_FAILURE != MsgQueue_Create(&pstMsgQueue, ElementSize, NoElements))
    {
        pstMsgList = (stMsgList *)OS_AllocMemory(sizeof(stMsgList));

        if (pstMsgList == NULL)
        {
            return NULL;
        }

        strncpy(pstMsgList->QueName, Qname, strlen(Qname));
        pstMsgList->MsgHandle = (OS_MessageQueue_t *)pstMsgQueue;

        list_add_tail(&(pstMsgList->stList), &gListHead);
    }

    return (OS_MessageQueue_t *)(pstMsgQueue);
}

GOS_S32 OS_DeleteMessageQueue(OS_MessageQueue_t *MsgQueue_p)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;

    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && pstMsgList->MsgHandle == MsgQueue_p)
        {
            pstMsgList->MsgHandle = NULL;
            memset(pstMsgList->QueName, 0, sizeof(pstMsgList->QueName));
            list_del(&(pstMsgList->stList));

            OS_FreeMemory(pstMsgList);
            MsgQueue_Destory((pstMSG_QUEUE_S)MsgQueue_p);
            break;
        }
    }

    return GOS_SUCCESS;
}

GOS_S32 OS_ReleaseMessageBuffer(OS_MessageQueue_t *MsgQueue_p, GOS_VOID *Msg_p)
{
    return GOS_SUCCESS;
}

GOS_VOID *OS_ReceiveMessage(OS_MessageQueue_t *MsgQueue_p)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;
    stMESSAGE_S         stMsg;

    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && pstMsgList->MsgHandle == MsgQueue_p)
        {
            if (GOS_FAILURE == MsgQueue_De((pstMSG_QUEUE_S)MsgQueue_p, &stMsg))
            {
                return NULL;
            }
        }
    }

    return ((GOS_VOID *)stMsg.pu8MsgData);
}

GOS_S32 OS_FlushMessage(OS_MessageQueue_t *MsgQueue_p)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;
	
    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && pstMsgList->MsgHandle == MsgQueue_p)
        {
            if (GOS_FAILURE == MsgQueue_Flush((pstMSG_QUEUE_S)MsgQueue_p))
            {
                return GOS_FAILURE;
            }
        }
    }

    return GOS_SUCCESS;
}

GOS_VOID *OS_ReceiveMessageTimeOut(OS_MessageQueue_t *MsgQueue_p, unsigned int  TimeOutMs)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;
    stMESSAGE_S         stMsg;

    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && pstMsgList->MsgHandle == MsgQueue_p)
        {
            if (GOS_FAILURE == MsgQueue_DeTimeOut((pstMSG_QUEUE_S)MsgQueue_p, &stMsg, TimeOutMs))
            {
                return NULL;
            }
        }
    }

    return ((GOS_VOID *)stMsg.pu8MsgData);
}

GOS_S32 OS_SendMessage(OS_MessageQueue_t *MsgQueue_p, GOS_VOID *Msg_p, unsigned int Msg_Size, int Priority)
{
    struct list_head   *pstPos = NULL;
    stMsgList          *pstMsgList = NULL;
    stMESSAGE_S         stMsg;

    stMsg.pu8MsgData = (unsigned char *)OS_AllocMemory(Msg_Size);

    if (!stMsg.pu8MsgData)
    {
        LOG_OUTPUTF(E_LOG_LEVELINFO,"OS_AllocMemory Failed!\n");
        return -1;
    }

    stMsg.u16MsgLength = Msg_Size;
    memcpy((char *)stMsg.pu8MsgData, (const char *)Msg_p, Msg_Size);

    list_for_each(pstPos, &gListHead)
    {
        pstMsgList = list_entry(pstPos, stMsgList, stList);

        if  (pstMsgList && pstMsgList->MsgHandle == MsgQueue_p)
        {
            if (GOS_FAILURE == MsgQueue_En((pstMSG_QUEUE_S)MsgQueue_p, &stMsg))
            {
				OS_FreeMemory(stMsg.pu8MsgData);
                return GOS_FAILURE;
            }
        }
    }

    OS_FreeMemory(stMsg.pu8MsgData);

    return GOS_SUCCESS;
}

OS_Mutex_t *OS_CreateMutexFifo(GOS_VOID)
{
    OS_Mutex_t *Mutex_p;

    Mutex_p = (OS_Mutex_t *)OS_AllocMemory(sizeof(OS_Mutex_t));

    if (Mutex_p != NULL)
    {
        pthread_mutex_init(Mutex_p, NULL);
    }

    return Mutex_p;
}

GOS_BOOL OS_DeleteMutex(OS_Mutex_t *Mutex_p)
{
    if (Mutex_p != NULL)
    {
        pthread_mutex_destroy(Mutex_p);
        OS_FreeMemory(Mutex_p);
        return GOS_TRUE;
    }

    return GOS_FALSE;
}

GOS_BOOL OS_LockMutex(OS_Mutex_t *Mutex_p)
{
    if (Mutex_p != NULL)
    {
        pthread_mutex_lock(Mutex_p);
        return GOS_TRUE;
    }

    return GOS_FALSE;
}

GOS_BOOL OS_UnlockMutex(OS_Mutex_t *Mutex_p)
{
    if (Mutex_p != NULL)
    {
        pthread_mutex_unlock(Mutex_p);
        return GOS_TRUE;
    }

    return GOS_FALSE;
}

/*---------------------------------------------------------------------------*/
// 任务
/*---------------------------------------------------------------------------*/
GOS_BOOL OS_CreateTask(GOS_VOID *Function, GOS_VOID *Param, OS_Task_Handle_t *Task)
{
    int result;

    result = pthread_create(Task, NULL, (GOS_VOID *)Function, Param);

    if (result != 0)
    {
        return GOS_FALSE;
    }

    return GOS_TRUE;
}

GOS_BOOL OS_DeleteTask (OS_Task_Handle_t Task)
{
    int result = 0;

    if (Task)
	{
		if (!pthread_join(Task, NULL))
		{
			result = pthread_cancel(Task);
		}
	}

    return result;
}


/*---------------------------------------------------------------------------*/
//
/*---------------------------------------------------------------------------*/
GOS_VOID OS_DelayTaskMs(int millisecon)
{
    usleep(1000 * millisecon);
}


GOS_U32 OS_TimeNow(GOS_VOID)
{
    GOS_U32     retval;
    struct  timeval time_value;

    gettimeofday(&time_value, NULL);

    retval = time_value.tv_sec * 1000 + time_value.tv_usec / 1000;

    return (retval);
}


GOS_VOID OS_Printf_Buffer(char *buffer, int length)
{
    printf("******************************\n");
    int ii = 0, jj = 0;

    for (ii = 0; ii < length / 16 + 1; ii++)
    {
        for (jj = 0; jj < 16; jj++)
        {
            if ((ii * 16 + jj) >= length)
            {
                break;
            }

            printf("0x%02x ", buffer[ii * 16 + jj]);
        }

        printf("\n");
    }

    printf("******************************\n");
}


