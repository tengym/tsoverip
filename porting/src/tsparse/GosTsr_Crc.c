#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "GosTsr_Crc.h"

static GOSTSR_U32 CRC_crc32_table[CRC32_MAX_COEFFICIENTS];

GOSTSR_S32 GosTsr_AnalysisData_CRCInit(void)
{
    GOSTSR_U32 loopCntCoef;
    GOSTSR_U32 loopCntBit;
    GOSTSR_U32 coef32;

    for (loopCntCoef = 0; loopCntCoef < CRC32_MAX_COEFFICIENTS; loopCntCoef++)
    {
        coef32 = loopCntCoef << 24;
        for (loopCntBit=0; loopCntBit<8; loopCntBit++)
        {
            if (coef32 & 0x80000000)
                coef32 = ((coef32 << 1) ^ CRC32_POLYNOMIAL);
            else
                coef32 <<= 1;
        }
        CRC_crc32_table[loopCntCoef] = coef32;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_U32 GosTsr_AnalysisData_CRCCheck(const GOSTSR_U8 *data, GOSTSR_U32 dataLen)
{
    GOSTSR_U32 crc32 = 0xFFFFFFFF;
    GOSTSR_U32 cntByte;

    for (cntByte = 0; cntByte < dataLen; cntByte++)
    {
        crc32 = (crc32 << 8 ) ^ CRC_crc32_table[((crc32 >> 24) ^ *data++) & 0xFF];
    }

    return  crc32;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
