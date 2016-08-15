#ifndef _GOS_SYS_H_
#define _GOS_SYS_H_

#include "user_define.h"
#include "linuxos.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUF_SIZE 				1024
#define INVALID_CHIP_ID			0xffffffff

GOS_S32 GOS_SYS_Init(void);
GOS_S32 GOS_SYS_DeInit(void);
GOS_U32 GOS_SYS_Get_ChipID();

#ifdef __cplusplus
}
#endif

#endif

