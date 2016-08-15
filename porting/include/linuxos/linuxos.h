#ifndef __LINUXOS_H__
#define __LINUXOS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*Linux OS*/
#include <pthread.h>
#include <sched.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <semaphore.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>

#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <net/if.h>
#include <net/route.h>				/* network_parseRoutes*/
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/fb.h>
#include <linux/mii.h>				/*miire_read*/
#include <linux/sockios.h>			/*network_get_interface_name SIOCGIFCONF*/
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>		/*for rtnetlink*/

#include "linuxlist.h"
#include "log.h"

#ifdef __GNUC__
	#ifndef MAYBE_UNUSED
	#define MAYBE_UNUSED	__attribute__((unused))
	#endif
#else
	#ifndef MAYBE_UNUSED
	#define MAYBE_UNUSED
	#endif
#endif

#ifndef CHECK_NOTEQU_RET
#define CHECK_NOTEQU_RET(ret,sret,a) \
	do{\
        if (ret != sret){\
            LOG_OUTPUTF(E_LOG_LEVELERROR,"Line %d Call %s failed!,ret == 0x%x\n",__LINE__,__FUNCTION__,ret);\
            a;\
        }\
    }while(0);
#endif

#ifndef CHECK_EQU_RET
#define CHECK_EQU_RET(ret,sret,a) \
	do{\
        if (ret == sret){\
            LOG_OUTPUTF(E_LOG_LEVELERROR,"Line %d Call %s failed!,ret == 0x%x\n",__LINE__,__FUNCTION__,ret);\
            a;\
        }\
    }while(0);
#endif

#ifndef HIAPI_RUN
#define HIAPI_RUN(api, ret) \
    do {\
        HI_S32 errCode; \
        errCode = api; \
        if (errCode != HI_SUCCESS)\
        {\
            LOG_OUTPUTF(E_LOG_LEVELERROR, "\033[0;31m" "[Function: %s line: %d] %s failed ret = 0x%x \n" "\033[0m", __FUNCTION__, __LINE__ , #api, errCode); \
        }\
        ret = errCode;\
    } while (0)
#endif

#ifndef HIAPI_RUN_RETURN
#define HIAPI_RUN_RETURN(api) \
    do {\
        HI_S32 errCode; \
        errCode = api; \
        if (errCode != 0)\
        {\
            LOG_OUTPUTF(E_LOG_LEVELERROR, "\033[0;31m" "[Function: %s line: %d] %s failed ret = 0x%x \n" "\033[0m",  __FUNCTION__, __LINE__ , #api, errCode); \
            return HI_FAILURE; \
        }\
    } while (0)
#endif

#define WriteDword(pBuf, val) {					\
    *((pBuf) + 0) = (GOS_U8)(((val) >> 24) & 0xff);\
    *((pBuf) + 1) = (GOS_U8)(((val) >> 16) & 0xff);\
    *((pBuf) + 2) = (GOS_U8)(((val) >> 8)  & 0xff);\
    *((pBuf) + 3) = (GOS_U8)((val) & 0xff);		\
}

#define ReadDword(pBuf, val) {					\
	val = ((pBuf[0] << 24) |					\
	(pBuf[1] << 16) |							\
	(pBuf[2] << 8)| pBuf[3]);					\
}

#define WriteWord(pBuf, val) {					\
    *((pBuf) + 0) = ((val) >> 8)  & 0xff;		\
    *((pBuf) + 1) = (val) & 0xff;				\
}

#define BIT_SET(v32, id)		(v32 |= (1 << id))
#define BIT_CHK(v32, id)		((v32 >> id) & 0x1)

#define IS_VIDEO_STREAM(eStreamType) ((eStreamType) == ePSISI_STREAM_VIDEO_AVS	\
									 ||(eStreamType) == ePSISI_STREAM_VIDEO_MPEG1\
									 ||(eStreamType) == ePSISI_STREAM_VIDEO_MPEG2\
									 ||(eStreamType) == ePSISI_STREAM_VIDEO_MPEG4\
									 ||(eStreamType) == ePSISI_STREAM_VIDEO_H264\
									 ||(eStreamType) == ePSISI_STREAM_VIDEO_WM9)

#define IS_AUDIO_STREAM(eStreamType) ((eStreamType) == ePSISI_STREAM_AUDIO_MPEG1\
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_MPEG2 \
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AAC_ADTS\
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AAC_LATM\
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AAC_RAW\
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AC3	 \
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AAC_ADTS\
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_AVS	 \
									 ||(eStreamType) == ePSISI_STREAM_AUDIO_DTS)
/*
#define IS_DVBC_SINGNAL(value)      (HI_UNF_TUNER_SIG_TYPE_CAB == value)
#define IS_DVBT_SINGNAL(value)      ((HI_UNF_TUNER_SIG_TYPE_DVB_T <= value) && (HI_UNF_TUNER_SIG_TYPE_DTMB >= value))
#define IS_DVBS_SINGNAL(value)      (HI_UNF_TUNER_SIG_TYPE_SAT == value)
*/
#define Ter_Freq_Max_Offset			500
#define mTerFreqInRange(x,y) 		(((x == y) || ((x >= y - Ter_Freq_Max_Offset) && (x <= y + Ter_Freq_Max_Offset))) ? 1 : 0)

#define	SETIPADDRESS(addr1,addr2,addr3,addr4,ip)	do{ip = htonl((addr1 << 24) | (addr2 << 16) | (addr3 << 8) | addr4);}while(0)

/* Exported Macros --------------------------------------------------------- */

/* Exported Functions ------------------------------------------------------ */

#define OS_AllocMemory(size)                    calloc(1, size)     //calloc自动为分配的空间清0
#define OS_FreeMemory(ptr)                      do{if (ptr){free(ptr); ptr = NULL;}}while(0)

#define OS_time_minus(t1, t2)                   abs(t1-t2)
#define OS_time_plus(t1, t2)                    t1+t2
#define OS_time_after(t1, t2)                   ((t1 > t2) ? 1 : 0)
#define OS_ConvertMstoTick(t1)                  t1/1
#define OS_ConvertTicktoMs (t1)                 t1

#define Comp_Min(x, y)							(((int)x < (int)y) ? (x) : (y))
#define Comp_Max(x, y)							(((int)x > (int)y) ? (x) : (y))

#define GET_HGH_BYTE(a)                         (((a) & 0xFF00) >> 8)
#define GET_LOW_BYTE(a)                         ((a) & 0x00FF)

#define ALIGN_4(_x_)                            (((_x_) + 3) & ~3)
#define ALIGN_8(_x_)                            (((_x_) + 7) & ~7)
#define ALIGN_16(_x_)                           (((_x_) + 15) & ~15)
#define ALIGN_32(_x_)                           (((_x_) + 31) & ~31)

#ifndef ASSERT
#define ASSERT(x)                               assert(x)
#endif

#define SCREEN_WIDTH                            1280
#define SCREEN_HIGHT                            720

/* Typedef ------------------------------------------------------------------*/
typedef sem_t                                   OS_Semaphore_t;
typedef GOS_U32                                 OS_MessageQueue_t;
typedef pthread_mutex_t                         OS_Mutex_t;
typedef pthread_t                               OS_Task_Handle_t;
typedef GOS_U32                                 OS_TaskDesc_t;
typedef GOS_U32                                 OS_TaskFlag_t;

/* Function Declaration -----------------------------------------------------*/

/* 信号量 */
OS_Semaphore_t *OS_CreateSemaphore (const int InitialValue);
GOS_BOOL OS_DeleteSemaphore      (OS_Semaphore_t *Semaphore_p);
GOS_BOOL OS_SignalSemaphore      (OS_Semaphore_t *Semaphore_p);
GOS_BOOL OS_WaitSemaphore        (OS_Semaphore_t *Semaphore_p);
GOS_BOOL OS_WaitSemaphoreTimeOut (OS_Semaphore_t *Semaphore_p, GOS_U32 TimeOutMs);
GOS_BOOL OS_GetSemaphoreValue    (OS_Semaphore_t *Semaphore_p, int *sval);


/* 消息队列 */
OS_MessageQueue_t *OS_CreateMessageQueue	(const char *Qname, unsigned int ElementSize, unsigned int NoElements);
GOS_S32  OS_DeleteMessageQueue			(OS_MessageQueue_t *MsgQueue_p);
GOS_S32  OS_SendMessage						(OS_MessageQueue_t *MsgQueue_p, void *Msg_p, GOS_U32 Msg_Size, int Priority);
GOS_VOID*OS_ReceiveMessage					(OS_MessageQueue_t *MsgQueue_p);
GOS_VOID*OS_ReceiveMessageTimeOut		(OS_MessageQueue_t *MsgQueue_p, GOS_U32 TimeOutMs);
GOS_S32  OS_ReleaseMessageBuffer			(OS_MessageQueue_t *MsgQueue_p, void *Msg_p);
GOS_S32  OS_FlushMessageBuffer			(OS_MessageQueue_t *MsgQueue_p);
GOS_S32 OS_FlushMessage						(OS_MessageQueue_t *MsgQueue_p);


OS_Mutex_t *OS_CreateMutexFifo	(void);
GOS_BOOL OS_DeleteMutex			(OS_Mutex_t *Mutex_p);
GOS_BOOL OS_LockMutex				(OS_Mutex_t *Mutex_p);
GOS_BOOL OS_UnlockMutex			(OS_Mutex_t *Mutex_p);

/* 任务 */
GOS_BOOL  OS_CreateTask     (void *Function, void *Param, OS_Task_Handle_t *Task);
GOS_BOOL  OS_DeleteTask     (OS_Task_Handle_t Task);

GOS_U32   OS_TimeNow        (void);
GOS_VOID  OS_DelayTaskMs    (int millisecon);
GOS_VOID  OS_Printf_Buffer  (char *buffer, int length);

#ifdef __cplusplus
}
#endif

#endif

