#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#include "user_define.h"

GOS_S32 GOS_CRC_Init(void);
void GOS_CRC_16bCalculate(GOS_U8  *buffer, GOS_U32 size, GOS_U16 *CRC16);
void GOS_CRC_32bCalculate(GOS_U8 *buffer, GOS_U32 size, GOS_U32 *CRC32);
GOS_U32 GOS_CRC_Crc32 (GOS_U32 crc, const GOS_U8* buf, GOS_U32 len);

#ifdef __cplusplus
 #if __cplusplus
}

 #endif
#endif

#endif

