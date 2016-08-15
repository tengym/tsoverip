#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef GOSTSR_CRC_H
#define GOSTSR_CRC_H

#include "GosTsr_Common.h"

#define CRC32_MAX_COEFFICIENTS	256
#define CRC32_POLYNOMIAL		0x04C11DB7

/*CRC check hander functions*/
extern GOSTSR_S32 GosTsr_AnalysisData_CRCInit(void);
extern GOSTSR_U32 GosTsr_AnalysisData_CRCCheck(const GOSTSR_U8 *buf, GOSTSR_U32 len);

#endif // GOSTSR_CRC_H

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
