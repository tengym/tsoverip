#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "GosTsr_AnalysisData.h"

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"
#include "TsErrorCheck_ThreeLevel.h"

#define DEFAULT_CODE 0x02

const static GOSTSR_U16 iso8859_2[] = {
    0x00a0, 0x0104, 0x02d8, 0x0a41, 0x00a4, 0x013d, 0x015a, 0x00a7, \
    0x00a8, 0x0160, 0x015e, 0x0164, 0x0179, 0x00ad, 0x017d, 0x0a7b, \
    0x00b0, 0x0105, 0x02db, 0x0142, 0x00b4, 0x013e, 0x015b, 0x02c7, \
    0x00b8, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c, \
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7, \
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e, \
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7, \
    0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df, \
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7, \
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f, \
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7, \
    0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9
};

const static GOSTSR_U16 iso8859_3[] = {
    0x00a0, 0x0126, 0x02d8, 0x00a3, 0x0002, 0x00a5, 0x0124, 0x00a7, \
    0x00a8, 0x0130, 0x015e, 0x011e, 0x0134, 0x00ad, 0x0002, 0x017b, \
    0x00b0, 0x0127, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x0125, 0x00b7, \
    0x00b8, 0x0131, 0x015f, 0x011f, 0x0135, 0x00bd, 0x017c, 0x00c0, \
    0x00c1, 0x00c2, 0x00c4, 0x010a, 0x0108, 0x00c7, 0x00c8, 0x00c9, \
    0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, 0x00d1, 0x00d2, \
    0x00d3, 0x00d4, 0x0120, 0x00d6, 0x00d7, 0x011c, 0x00d9, 0x00da, \
    0x00db, 0x00dc, 0x016c, 0x015c, 0x00df, 0x00e0, 0x00e1, 0x00e2, \
    0x00e4, 0x010b, 0x0109, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, \
    0x00ec, 0x00ed, 0x00ee, 0x00ef, 0x00f1, 0x00f2, 0x00f3, 0x00f4, \
    0x0121, 0x00f6, 0x00f7, 0x011d, 0x00f9, 0x00fa, 0x00fb, 0x00fc, \
    0x016d, 0x015d, 0x02d9, 0x0002, 0x0002, 0x0002, 0x0025, 0x0002
};

const static GOSTSR_U16 iso8859_4[] = {
    0x00a0, 0x0104, 0x0138, 0x0156, 0x00a4, 0x0128, 0x013b, 0x00a7, \
    0x00a8, 0x0160, 0x0112, 0x0122, 0x0166, 0x00ad, 0x017d, 0x00af, \
    0x00b0, 0x0105, 0x02db, 0x0157, 0x00b4, 0x0129, 0x013c, 0x02c7, \
    0x00b8, 0x0161, 0x0113, 0x0123, 0x0167, 0x014a, 0x017e, 0x014b, \
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e, \
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x012a, \
    0x0110, 0x0145, 0x014c, 0x0136, 0x00d4, 0x00d5, 0x00d6, 0x00d7, \
    0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x0168, 0x016a, 0x00df, \
    0x0101, 0x00e1, 0x00e2, 0x0033, 0x00e4, 0x00e5, 0x00e6, 0x012f, \
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x012b, \
    0x0111, 0x0146, 0x014d, 0x0137, 0x00f4, 0x00f5, 0x00f6, 0x00f7, \
    0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x0169, 0x016b, 0x02d9
};

const static GOSTSR_U16 iso8859_7[] = {
    0x00a0, 0x2018, 0x2019, 0x00a3, 0x20ac, 0x20af, 0x00a6, 0x00a7, \
    0x00a8, 0x00a9, 0x037a, 0x00ab, 0x00ac, 0x00ad, 0x0002, 0x2015, \
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x0384, 0x0385, 0x0386, 0x00b7, \
    0x0388, 0x0389, 0x038a, 0x00bb, 0x038c, 0x00bd, 0x038e, 0x038f, \
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, \
    0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f, \
    0x03a0, 0x03a1, 0x03a2, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7, \
    0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03ae, 0x03af, \
    0x03b0, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7, \
    0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf, \
    0x03c0, 0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7, \
    0x03c8, 0x03c9, 0x03ca, 0x03cb, 0x03cc, 0x03cd, 0x03ce, 0x0002
};

const static GOSTSR_U16 iso8859_8[] = {
    0x00a0, 0x0002, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, \
    0x00a8, 0x00a9, 0x00d7, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af, \
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, \
    0x00b8, 0x00b9, 0x00f7, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x0002, \
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, \
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, \
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, \
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x2017, \
    0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7, \
    0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df, \
    0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7, \
    0x05e8, 0x05e9, 0x05ea, 0x0002, 0x0002, 0x200e, 0x200f, 0x0002
};

const static GOSTSR_U16 iso8859_10[] = {
    0x00a0, 0x0104, 0x0112, 0x0122, 0x012a, 0x0128, 0x0136, 0x00a7, \
    0x013b, 0x0110, 0x0160, 0x0166, 0x017d, 0x00ad, 0x016a, 0x014a, \
    0x00b0, 0x0105, 0x0113, 0x0123, 0x012b, 0x0129, 0x0137, 0x00b7, \
    0x013c, 0x0111, 0x0161, 0x0167, 0x017e, 0x2015, 0x016b, 0x014b, \
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e, \
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x00cf, \
    0x00d0, 0x0145, 0x014c, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x0168, \
    0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df, \
    0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f, \
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x00ef, \
    0x00f0, 0x0146, 0x014d, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x0169, \
    0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x0138
};

const static GOSTSR_U16 iso8859_13[] = {
    0x00a0, 0x201d, 0x00a2, 0x00a3, 0x00a4, 0x201e, 0x00a6, 0x00a7, \
    0x00d8, 0x00a9, 0x0156, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00c6, \
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x201c, 0x00b5, 0x00b6, 0x00b7, \
    0x00f8, 0x00b9, 0x0157, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00e6, \
    0x0104, 0x012e, 0x0100, 0x0106, 0x00c4, 0x00c5, 0x0118, 0x0112, \
    0x010c, 0x00c9, 0x0179, 0x0116, 0x0122, 0x0136, 0x012a, 0x013b, \
    0x0160, 0x0143, 0x0145, 0x00d3, 0x014c, 0x00d5, 0x00d6, 0x00d7, \
    0x0172, 0x0141, 0x015a, 0x016a, 0x00dc, 0x017b, 0x017d, 0x00df, \
    0x0105, 0x012f, 0x0101, 0x0107, 0x00e4, 0x00e5, 0x0119, 0x0113, \
    0x010d, 0x00e9, 0x017a, 0x0117, 0x0123, 0x0137, 0x012b, 0x013c, \
    0x0161, 0x0144, 0x0146, 0x00f3, 0x014d, 0x00f5, 0x00f6, 0x00f7, \
    0x0173, 0x0142, 0x015b, 0x016b, 0x00fc, 0x017c, 0x017e, 0x2019
};

const static GOSTSR_U16 iso8859_14[] = {
    0x00a0, 0x1e02, 0x1e03, 0x00a3, 0x010a, 0x010b, 0x1e0a, 0x00a7, \
    0x1e80, 0x00a9, 0x1e82, 0x1e0b, 0x1ef2, 0x00ad, 0x00ae, 0x0178, \
    0x1e1e, 0x1e1f, 0x0120, 0x0121, 0x1e40, 0x1e41, 0x00b6, 0x1e56, \
    0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61, \
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7, \
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, \
    0x0174, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x1e6a, \
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x0176, 0x00df, \
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, \
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, \
    0x0175, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x1e6b, \
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x0177, 0x00ff
};

const static GOSTSR_U16 iso8859_16[] = {
    0x00a0, 0x0104, 0x0105, 0x0141, 0x20ac, 0x201e, 0x0160, 0x00a7, \
    0x0161, 0x00a9, 0x0218, 0x00ab, 0x0179, 0x00ad, 0x017a, 0x017b, \
    0x00b0, 0x00b1, 0x010c, 0x0142, 0x017d, 0x201d, 0x00b6, 0x00b7, \
    0x017e, 0x010d, 0x0219, 0x00bb, 0x0152, 0x0153, 0x0178, 0x017c, \
    0x00c0, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0106, 0x00c6, 0x00c7, \
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf, \
    0x0110, 0x0143, 0x00d2, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x015a, \
    0x0170, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0118, 0x021a, 0x00df, \
    0x00e0, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x0107, 0x00e6, 0x00e7, \
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef, \
    0x0111, 0x0144, 0x00f2, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x015b, \
    0x0171, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0119, 0x021b, 0x00ff
};

/*static ts stream hander data*/
static TS_PACKAGE_INFO *PackageHead[TABLE_NUMBER] = {GOSTSR_NULL};
static GOSTSR_U16 pmtsPID[GOS_MAX_SERVICE_NUM];
static GOSTSR_U16 pmtsPIDNumber = 0;
static AnalysisData_FUN callBackFunc[ANALYSIS_CALLBACKFUNCTION_NUM];
static GOSTSR_PSISI_PAT_S gstPatInfo;

GOSTSR_S32 GosTsr_AnalysisData_PacketType(GOSTSR_U32 *pu32PacketLen, char *filePath)
{
    FILE *sFp = GOSTSR_NULL;
    GOSTSR_U64 fileLen = 0;
    GOSTSR_U8 chr = '\0';
    GOSTSR_BOOL checkFlag = GOSTSR_FALSE;
    GOSTSR_U64 offset = 0;
    GOSTSR_U64 tempOffset = 0;
    GOSTSR_U32 retLen = 0;
    GOSTSR_U32 packageLen = 0;
    GOSTSR_U8 *tsData = GOSTSR_NULL;

    if (NULL == filePath)
    {
        printf("filePath is NULL");
        return GOSTSR_FAILURE;
    }

    sFp = fopen(filePath, "rb");
    if (NULL == sFp)
    {
        printf("fopen failed,path:%s\n", filePath);
        return GOSTSR_FAILURE;
    }

    fseek(sFp, 0, SEEK_END);
    fileLen = ftell(sFp);
    fseek(sFp, 0, SEEK_SET);
    do
    {
        chr = fgetc(sFp);
        offset++;

        if ((chr == EOF) && (offset >= fileLen))
        {
            break;
        }

        if (chr == SYNC_BYTE)
        {
            if (tempOffset == 0)
            {
                tempOffset = offset;
            }
            else
            {
                if (((offset - tempOffset) == TS_LENGTH_188) || ((offset - tempOffset) == TS_LENGTH_204))
                {
                    offset = tempOffset;
                    checkFlag = GOSTSR_TRUE;
                }
                else
                {
                    tempOffset = offset;
                }
            }
        }

    }while(!checkFlag);

    if (chr == EOF)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        printf("file is EOF\n");
        return GOSTSR_FAILURE;
    }

    offset = offset - 1;
    fseek(sFp, offset, SEEK_SET);

    /*choose package size 188 0r 204*/
    tsData = (GOSTSR_U8 *)malloc(TS_LENGTH_204 + 1);
    if (GOSTSR_NULL == tsData)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        printf("malloc failed\n");
        return GOSTSR_FAILURE;
    }
    memset(tsData, 0x00, TS_LENGTH_204 + 1);
    retLen = fread(tsData, 1, TS_LENGTH_204 + 1, sFp);
    if (retLen != TS_LENGTH_204 + 1)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        if (GOSTSR_NULL != tsData)
        {
            free(tsData);
            tsData = GOSTSR_NULL;
        }
        printf("fread failed,%d\n", __LINE__);
        return GOSTSR_FAILURE;
    }

    if((tsData[0] == SYNC_BYTE) && (tsData[188] == SYNC_BYTE))
    {
        /*package size 188*/
        packageLen = TS_LENGTH_188;
    }
    else if((tsData[0] == SYNC_BYTE) && (tsData[188] != SYNC_BYTE) && \
            (tsData[204] == SYNC_BYTE))
    {
        /*package size 204*/
        packageLen = TS_LENGTH_204;
    }
    else
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        if (GOSTSR_NULL != tsData)
        {
            free(tsData);
            tsData = GOSTSR_NULL;
        }
        printf("NO SYNC_BYTE\n,%d", __LINE__);
        return GOSTSR_FAILURE;
    }

    if (GOSTSR_NULL != tsData)
    {
        free(tsData);
        tsData = GOSTSR_NULL;
    }

    *pu32PacketLen = packageLen;

    return GOSTSR_SUCCESS;
}
GOSTSR_S32 GosTsr_AnalysisData_ResetPMTID()
{
    memset(pmtsPID, 0xffff, sizeof(pmtsPID));
    pmtsPIDNumber = 0;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_ResetPatInfo()
{
	memset(&gstPatInfo, 0, sizeof(GOSTSR_PSISI_PAT_S));
    gstPatInfo.u8Version = 0xff;

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_getPatInfo(GOSTSR_PSISI_PAT_S *patInfo)
{
	if(gstPatInfo.u8Version != patInfo->u8Version)
	memcpy(&gstPatInfo,patInfo,sizeof(GOSTSR_PSISI_PAT_S));

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TsEnvInit()
{
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;
    TS_PACKAGE_INFO *prePackage = GOSTSR_NULL;
    GOSTSR_U8 i = 0;

    for (i = 0; i < TABLE_NUMBER; i++)
    {
        /*free all package list member*/
        pCurPackage = PackageHead[i];
        while (pCurPackage != GOSTSR_NULL)
        {
            prePackage = pCurPackage;
            pCurPackage = pCurPackage->next;
            free(prePackage);
            prePackage = GOSTSR_NULL;
        }
        PackageHead[i] = GOSTSR_NULL;
    }

    for (i = 0; i < ANALYSIS_CALLBACKFUNCTION_NUM; i++)
    {
        callBackFunc[i] = GOSTSR_NULL;
    }

    GosTsr_AnalysisData_CRCInit();
    GosTsr_AnalysisData_ResetPMTID();
	GosTsr_AnalysisData_ResetPatInfo();

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TsEnvDeInit()
{
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;
    TS_PACKAGE_INFO *prePackage = GOSTSR_NULL;
    GOSTSR_U8 i = 0;

    for (i = 0; i < TABLE_NUMBER; i++)
    {
        /*free all package list member*/
        pCurPackage = PackageHead[i];
        while (pCurPackage != GOSTSR_NULL)
        {
            prePackage = pCurPackage;
            pCurPackage = pCurPackage->next;
            free(prePackage);
            prePackage = GOSTSR_NULL;
        }
        PackageHead[i] = GOSTSR_NULL;
    }

    for (i = 0; i < ANALYSIS_CALLBACKFUNCTION_NUM; i++)
    {
        callBackFunc[i] = GOSTSR_NULL;
    }

    GosTsr_AnalysisData_ResetPMTID();

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_CallBack(TS_SECTION_INFO *tsSectionInfo)
{
    GOSTSR_U8 i = 0;

    for (i = 0; i < ANALYSIS_CALLBACKFUNCTION_NUM; i++)
    {
        if (GOSTSR_NULL != callBackFunc[i])
        {
            callBackFunc[i](tsSectionInfo);
        }
    }

	return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_RegisterCallBack(AnalysisData_FUN function, GOSTSR_U8 *index)
{
    GOSTSR_U8 i = 0;

    if ((GOSTSR_NULL == function) || (GOSTSR_NULL == index))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < ANALYSIS_CALLBACKFUNCTION_NUM; i++)
    {
        if (GOSTSR_NULL == callBackFunc[i])
        {
            break;
        }
    }

    if (i != ANALYSIS_CALLBACKFUNCTION_NUM)
    {
        callBackFunc[i] = function;
        *index = i;

        return GOSTSR_SUCCESS;
    }

    return GOSTSR_FAILURE;
}

GOSTSR_S32 GosTsr_AnalysisData_UnRegisterCallBack(GOSTSR_U8 index)
{
    if (GOSTSR_NULL != callBackFunc[index])
    {
        callBackFunc[index] = GOSTSR_NULL;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_GetPMTID(GOSTSR_PSISI_PAT_S *patInfo)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    /*get PMTs PID value*/
    for (i = 0; i < patInfo->u16NbElements; i++)
    {
        if (0 == patInfo->astElement[i].u16ProgramNumber)
        {
            continue;
        }

        pmtsPID[index++] = patInfo->astElement[i].u16Pid;
    }

    /*get PMTs PID number*/
    pmtsPIDNumber = index;


    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_LinkPackage(GOSTSR_U8 tableID, TS_PACKAGE_INFO *PackageHead_Temp, GOSTSR_U8 lastDataLen, GOSTSR_U8 *lastData)
{
    GOSTSR_U8 *pSectionData = GOSTSR_NULL;
    GOSTSR_U16 curSectionLength = 0;
    GOSTSR_U16 allSectionLength = 0;
    TS_SECTION_INFO *curSectionInfo = GOSTSR_NULL;
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;

    if((GOSTSR_NULL == PackageHead_Temp) || (GOSTSR_NULL == lastData))
    {
        return  GOSTSR_FAILURE;
    }

    pCurPackage = PackageHead_Temp;
    while (GOSTSR_NULL != pCurPackage)
    {
        allSectionLength += pCurPackage->packageLen;
        pCurPackage = pCurPackage->next;
    }

    curSectionLength = allSectionLength;
    curSectionLength += lastDataLen;

    curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
    if(GOSTSR_NULL == curSectionInfo)
    {
        return  GOSTSR_FAILURE;
    }
    curSectionInfo->sectionLength = curSectionLength;
    curSectionInfo->PID = PackageHead_Temp->PID;
    curSectionInfo->tableID = PackageHead_Temp->tableID;
	memcpy(&curSectionInfo->stErrorInfo,&PackageHead_Temp->stErrorInfo,sizeof(TR101290_ERROR_S));
    curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
    if(GOSTSR_NULL == curSectionInfo->sectionData)
    {
        return  GOSTSR_FAILURE;
    }

    memset(curSectionInfo->sectionData, 0x00, curSectionLength);
    pSectionData = curSectionInfo->sectionData;
    pCurPackage = PackageHead_Temp;

    while (GOSTSR_NULL != pCurPackage)
    {
        memcpy(pSectionData, pCurPackage->packageData, pCurPackage->packageLen);
        pSectionData += pCurPackage->packageLen;
        pCurPackage = pCurPackage->next;
    }
    memcpy(pSectionData, lastData, lastDataLen);

    GOSTSR_U16 curPos = curSectionInfo->sectionLength - 4;
    GOSTSR_U8  *pData = &(curSectionInfo->sectionData[curPos]);
    GOSTSR_U32 crc32 = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    GOSTSR_U32 calCrc32 = 0;

    calCrc32 = GosTsr_AnalysisData_CRCCheck(curSectionInfo->sectionData, curSectionInfo->sectionLength - 4);
	/*Tow Level Error Check,若CRC错误则不进行其他错误检测*/
	if(TsErrorCheck_TwoLevel_checkCrcError(&curSectionInfo->stErrorInfo,curSectionInfo->tableID, crc32, calCrc32) ==  GOSTSR_SUCCESS)
	{
		return GOSTSR_FAILURE;
	}	
	
	TsErrorCheck_ThreeLevel_SectionsTimeError(*curSectionInfo);
    if ((crc32 == calCrc32) || (TDT_TABLE_ID == tableID))
    {
        GosTsr_AnalysisData_CallBack(curSectionInfo);
    }

    if (GOSTSR_NULL != curSectionInfo)
    {
        if (GOSTSR_NULL != curSectionInfo->sectionData)
        {
            free(curSectionInfo->sectionData);
            curSectionInfo->sectionData = GOSTSR_NULL;
        }
        free(curSectionInfo);
        curSectionInfo = GOSTSR_NULL;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TableID(GOSTSR_U8 tableID,const void *data, TS_HEAD_INFO tsHeadInfo)
{
    GOSTSR_U8 sectionFlag = GOSTSR_FALSE;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U8 *pcurData = GOSTSR_NULL;
    GOSTSR_U8 adaptLength = 0;
    GOSTSR_U8 pointFieldLength = 0;
    GOSTSR_U8 frontSectionData[256] = {0,};
    GOSTSR_U8 curTableID = BULT_TABLE_ID;
    GOSTSR_U16 curTempData = 0;
    GOSTSR_U16 curSectionLength = 0;
    GOSTSR_U16 remainLen = 0;
    TS_SECTION_INFO *curSectionInfo = GOSTSR_NULL;
    TS_PACKAGE_INFO *pCurPackage = GOSTSR_NULL;
    TS_PACKAGE_INFO *prePackage = GOSTSR_NULL;
    GOSTSR_U16 packageAllLen = 0;
    GOSTSR_U8 *pSectionData = GOSTSR_NULL;
    GOSTSR_U16 allSectionLength = 0;
	
    if (GOSTSR_NULL == data)
    {
        return  GOSTSR_FAILURE;
    }

    pcurData  = (GOSTSR_U8 *)data;
    pcurData += TS_HEAD_LENGTH;

    /*check whether or no data contains adaptation data*/
    if (tsHeadInfo.adapter_control & BIT2)
    {
        /*Get adaptation length*/
        adaptLength = pcurData[0];
        pcurData += 1;

        /*Jump to load data*/
        pcurData += adaptLength;
    }

    /*check payload_unit_start_indicator*/
    if (tsHeadInfo.load_indicater == BIT1)
    {
        /*Get pointer_field value*/
        pointFieldLength = pcurData[0];
        pcurData += 1;

        /*Jump to table ID*/
        memset(frontSectionData, 0x00, sizeof(frontSectionData));
        memcpy(frontSectionData, pcurData, pointFieldLength);
        pcurData += pointFieldLength;

        /*Get current table ID*/
        curTableID = pcurData[0];
        pcurData += 1;

		TsErrorCheck_OneLevel_PatError(&tsHeadInfo.stErrorInfo,tsHeadInfo.ts_pid,curTableID,tsHeadInfo.scramble_control);	
		TsErrorCheck_OneLevel_PmtError(&tsHeadInfo.stErrorInfo,tsHeadInfo.ts_pid,tsHeadInfo.scramble_control);
		TsErrorCheck_TwoLevel_checkCatError(&tsHeadInfo.stErrorInfo,tsHeadInfo.ts_pid, curTableID, tsHeadInfo.scramble_control);
        if (curTableID != tableID)
        {
            return GOSTSR_FAILURE;
        }
        /*when other section arrive and free all previous section packages*/
        for (i = 0; i < TABLE_NUMBER; i++)
        {
            if (PackageHead[i] == GOSTSR_NULL)
            {
                continue;
            }

            if (PackageHead[i]->PID == tsHeadInfo.ts_pid)
            {
                allSectionLength = 0;

                pCurPackage = PackageHead[i];
                while (GOSTSR_NULL != pCurPackage)
                {
                    allSectionLength += pCurPackage->packageLen;
                    pCurPackage = pCurPackage->next;
                }

                if((allSectionLength + pointFieldLength) >= PackageHead[i]->sectionLen)
                {
                    GosTsr_AnalysisData_LinkPackage(tableID,PackageHead[i],pointFieldLength,frontSectionData);
                }

                pCurPackage = PackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    prePackage = pCurPackage;
                    pCurPackage = pCurPackage->next;
                    free(prePackage);
                    prePackage = GOSTSR_NULL;
                }

                PackageHead[i] = GOSTSR_NULL;
            }
        }

        /*Get current section length*/
        curTempData = pcurData[0]*256 + pcurData[1];
        curSectionLength = curTempData & (~((BIT8 | BIT7 | BIT6 | BIT5) * 256));
        pcurData += 2;

        /*jump to section head*/
        pcurData -= 3;
        curSectionLength += 3;

        /*get ts package remain char*/
        if (tsHeadInfo.adapter_control & BIT2)
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                remainLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - adaptLength - 1 - pointFieldLength);
            }
            else
            {
                remainLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - adaptLength - 1 - pointFieldLength);
            }
        }
        else
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                remainLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - pointFieldLength);
            }
            else
            {
                remainLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - pointFieldLength);
            }
        }

        /*when one section length <= one package valid loader data length*/
        if (curSectionLength <= remainLen)
        {
            curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
            curSectionInfo->sectionLength = curSectionLength;
            curSectionInfo->tableID = tableID;
            curSectionInfo->PID = tsHeadInfo.ts_pid;
			
		memcpy(&curSectionInfo->stErrorInfo,&tsHeadInfo.stErrorInfo,sizeof(TR101290_ERROR_S));

            curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
            memset(curSectionInfo->sectionData, 0x00, curSectionLength);
            memcpy(curSectionInfo->sectionData, pcurData, curSectionLength);

            /*one section receive over*/
            sectionFlag = GOSTSR_TRUE;
        }
        else /*create a new section package list head*/
        {
            /*find the empty package list head*/
            for (i = 0; i < TABLE_NUMBER; i++)
            {
                if (PackageHead[i] == GOSTSR_NULL)
                {
                    break;
                }
            }

            if (TABLE_NUMBER != i)
            {
                PackageHead[i] = (TS_PACKAGE_INFO*)malloc(sizeof(TS_PACKAGE_INFO));
                PackageHead[i]->next = GOSTSR_NULL;
                PackageHead[i]->packageLen = remainLen;
                memset(PackageHead[i]->packageData, 0x00, sizeof(PackageHead[i]->packageData));
                memcpy(PackageHead[i]->packageData, pcurData, remainLen);
                PackageHead[i]->PID = tsHeadInfo.ts_pid;
                PackageHead[i]->number = tsHeadInfo.counter;
                PackageHead[i]->sectionLen = curSectionLength;
                PackageHead[i]->tableID = tableID;
			memcpy(&PackageHead[i]->stErrorInfo,&tsHeadInfo.stErrorInfo,sizeof(TR101290_ERROR_S));
            }
        }
    }
    else
    {
        /*find the list head of current package*/
        for (i = 0; i < TABLE_NUMBER; i++)
        {
            if (PackageHead[i] == GOSTSR_NULL)
            {
                continue;
            }

            if (PackageHead[i]->PID == tsHeadInfo.ts_pid)
            {
                break;
            }
        }

        /*no find the same PID package list head*/
        if (i == TABLE_NUMBER)
        {
            return GOSTSR_FAILURE;
        }

        /*jump to the package list end*/
        packageAllLen = 0;
        pCurPackage = PackageHead[i];
        prePackage = PackageHead[i];
        while (pCurPackage != GOSTSR_NULL)
        {
            packageAllLen += pCurPackage->packageLen;
            prePackage = pCurPackage;
            pCurPackage = pCurPackage->next;
        }

        /*apply the memory of current package and inset current package at the end of package list end*/
        pCurPackage = (TS_PACKAGE_INFO*)malloc(sizeof(TS_PACKAGE_INFO));
        if (GOSTSR_NULL == pCurPackage)
        {
            return GOSTSR_FAILURE;
        }

        prePackage->next = pCurPackage;
        pCurPackage->next = GOSTSR_NULL;
        if (tsHeadInfo.adapter_control & BIT2)
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                pCurPackage->packageLen = (TS_LENGTH_188 - TS_HEAD_LENGTH - 1 - adaptLength);
            }
            else
            {
                pCurPackage->packageLen = (TS_LENGTH_204 - TS_HEAD_LENGTH - 1 - adaptLength);
            }
        }
        else
        {
            if (tsHeadInfo.packageType == PACKAGE_188)
            {
                pCurPackage->packageLen = (TS_LENGTH_188 - TS_HEAD_LENGTH);
            }
            else
            {
                pCurPackage->packageLen = (TS_LENGTH_204 - TS_HEAD_LENGTH);
            }
        }
        memset(pCurPackage->packageData, 0x00, sizeof(pCurPackage->packageData));
        memcpy(pCurPackage->packageData, pcurData, pCurPackage->packageLen);
        pCurPackage->PID = tsHeadInfo.ts_pid;
        pCurPackage->number = tsHeadInfo.counter;
        pCurPackage->sectionLen = PackageHead[i]->sectionLen;
        pCurPackage->tableID = PackageHead[i]->tableID;
		memcpy(&pCurPackage->stErrorInfo,&tsHeadInfo.stErrorInfo,sizeof(TR101290_ERROR_S));
        /*update total received packages length*/
        packageAllLen += pCurPackage->packageLen;

        if (packageAllLen >= PackageHead[i]->sectionLen)
        {
            packageAllLen = 0;
            j = PackageHead[i]->number;

            /*check: whether all packages of one section have arrived or no?*/
            while(packageAllLen < PackageHead[i]->sectionLen)
            {
                pCurPackage = PackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    if(pCurPackage->number == j)
                    {
                        packageAllLen += pCurPackage->packageLen;
                        j++;
                        /*when j reach the max value 15 ,j return zero*/
                        if (j == 16)
                        {
                            j = 0;
                        }
                        break;
                    }
                    pCurPackage = pCurPackage->next;
                }

                if (GOSTSR_NULL == pCurPackage)
                {
                    break;
                }
            }

            /*when found any package is loss*/
            if (GOSTSR_NULL == pCurPackage)
            {
                /*wait*/
            }
            else /*while all packages of one section have arrived*/
            {
                tableID = PackageHead[i]->tableID;
                curSectionLength = PackageHead[i]->sectionLen;
                curSectionInfo = (TS_SECTION_INFO *)malloc(sizeof(TS_SECTION_INFO));
                curSectionInfo->sectionLength = curSectionLength;
                curSectionInfo->PID = tsHeadInfo.ts_pid;
                curSectionInfo->tableID = tableID;
			memcpy(&curSectionInfo->stErrorInfo,&tsHeadInfo.stErrorInfo,sizeof(TR101290_ERROR_S));
                curSectionInfo->sectionData = (GOSTSR_U8 *)malloc(curSectionLength);
                memset(curSectionInfo->sectionData, 0x00, curSectionLength);
                pSectionData = curSectionInfo->sectionData;

                packageAllLen = 0;
                while (packageAllLen < curSectionLength)
                {
                    /*get package pointer ,It's number is j*/
                    pCurPackage = PackageHead[i];
                    prePackage = PackageHead[i];
                    j = PackageHead[i]->number;

                    while (pCurPackage != GOSTSR_NULL)
                    {
                        if(pCurPackage->number == j)
                        {
                            j++;
                            /*when j reach the max value ,j return zero*/
                            if (j == 16)
                            {
                                j = 0;
                            }
                            break;
                        }
                        prePackage = pCurPackage;
                        pCurPackage = pCurPackage->next;
                    }

                    if (GOSTSR_NULL != pCurPackage)
                    {
                        /*get section data from  current package*/
                        if ((packageAllLen + pCurPackage->packageLen) > curSectionLength)
                        {
                            remainLen =  curSectionLength - packageAllLen;
                        }
                        else
                        {
                            remainLen = pCurPackage->packageLen;
                        }

                        memcpy(pSectionData, pCurPackage->packageData, remainLen);

                        pSectionData += remainLen;
                        packageAllLen += pCurPackage->packageLen;

                        /*free memory of current package*/
                        if (prePackage == PackageHead[i]) /*if current package is head package*/
                        {
                            PackageHead[i] = prePackage->next;
                            free(prePackage);
                            prePackage = GOSTSR_NULL;
                        }
                        else
                        {
                            prePackage->next = pCurPackage->next;
                            free(pCurPackage);
                            pCurPackage = GOSTSR_NULL;
                        }

                    }
                }

                /*free all package list member*/
                pCurPackage = PackageHead[i];
                while (pCurPackage != GOSTSR_NULL)
                {
                    prePackage = pCurPackage;
                    pCurPackage = pCurPackage->next;
                    free(prePackage);
                    prePackage = GOSTSR_NULL;
                }
                PackageHead[i] = GOSTSR_NULL;

                /*one section receive over*/
                sectionFlag = GOSTSR_TRUE;
            }
        }
    }

    if (GOSTSR_TRUE == sectionFlag)
    {
        switch(tableID)
        {
        case PAT_TABLE_ID:
        case PMT_TABLE_ID:
        case CAT_TABLE_ID:
        case NIT_TABLE_ID_ACTUAL:
        case NIT_TABLE_ID_OTHER:
        case BAT_TABLE_ID:
        case SDT_TABLE_ID_ACTUAL:
        case SDT_TABLE_ID_OTEHR:
        case EIT_TABLE_ID_ACTUAL:
        case EIT_TABLE_ID_ACTUAL_SHEDULE:
        case EIT_TABLE_ID_OTHER:
        case EIT_TABLE_ID_OTHER_SHEDULE:
        case TDT_TABLE_ID:
        case TOT_TABLE_ID:
            {
                GOSTSR_U16 curPos = curSectionInfo->sectionLength - 4;
                GOSTSR_U8  *pData = &(curSectionInfo->sectionData[curPos]);
                GOSTSR_U32  crc32 = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
                GOSTSR_U32  calCrc32 = 0;
                calCrc32 = GosTsr_AnalysisData_CRCCheck(curSectionInfo->sectionData, curSectionInfo->sectionLength - 4);

		/*Tow Level Error Check,若CRC错误则不进行其他错误检测*/
		if(TsErrorCheck_TwoLevel_checkCrcError(&curSectionInfo->stErrorInfo,curSectionInfo->tableID, crc32, calCrc32) ==  GOSTSR_SUCCESS)
		{
			return GOSTSR_FAILURE;
		}	
		TsErrorCheck_ThreeLevel_SectionsTimeError(*curSectionInfo);
			
		if ((crc32 == calCrc32) || (TDT_TABLE_ID == tableID))
		{
			GosTsr_AnalysisData_CallBack(curSectionInfo);
		}
	}
	break;

        case RST_TABLE_ID:
        case SI_TABLE_ID:
        default:
            break;
        }
    }

    if (GOSTSR_NULL != curSectionInfo)
    {
        if (GOSTSR_NULL != curSectionInfo->sectionData)
        {
            free(curSectionInfo->sectionData);
            curSectionInfo->sectionData = GOSTSR_NULL;
        }
        free(curSectionInfo);
        curSectionInfo = GOSTSR_NULL;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TSHeadInfo( const void *srcData, TS_HEAD_INFO *desData)
{
    GOSTSR_U8 *curData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 s32Ret = 0;

    if ((GOSTSR_NULL == srcData) || (GOSTSR_NULL == desData))
    {
        return GOSTSR_FAILURE;
    }

    curData = (GOSTSR_U8 *)srcData;

    /*sync_byte*/
    if (SYNC_BYTE != curData[0])
    {
        return GOSTSR_FAILURE;
    }

    curData++;
    /*transport_error_indicator*/
    desData->error_indicater = (curData[0] & BIT8) / BIT8;
	/*Tow Level Error Check*/
	s32Ret = TsErrorCheck_TwoLevel_checkTransportError(&desData->stErrorInfo,desData->error_indicater);
	if(s32Ret == GOSTSR_SUCCESS)
	{
		return GOSTSR_FAILURE;
	}

    /*payload_unit_start_indicator*/
    desData->load_indicater = (curData[0] & BIT7) / BIT7;

    /*transport_priority*/

    /*PID*/
    tempData = curData[0]*256 + curData[1];
    desData->ts_pid = tempData  & (~((BIT8 | BIT7 | BIT6) * 256));
	s32Ret = TsErrorCheck_OneLevel_pidMissError(&desData->stErrorInfo,desData->ts_pid);
	if(s32Ret == GOSTSR_SUCCESS)
	{
		//return GOSTSR_FAILURE;
	}
	
    curData += 2;
    /*transport_scrambling_control*/
    desData->scramble_control = (curData[0] & (BIT8 | BIT7)) / BIT7;

    /*adaptation_field_control*/
    desData->adapter_control = (curData[0] & (BIT6 | BIT5)) / BIT5;

    /*continuity_counter*/
    desData->counter = curData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
	
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_CheckTsHeadInfo(TS_HEAD_INFO tsHeadInfo)
{
    GOSTSR_S32 ret = GOSTSR_SUCCESS;

    /*check transport_error_indicator*/
    if (tsHeadInfo.error_indicater == BIT1)
    {
        ret |=  GOSTSR_FAILURE;
    }

    /*check transport_scrambling_control*/
    if (tsHeadInfo.scramble_control != BIT0)
    {
        ret |= GOSTSR_FAILURE;
    }

    /*check adaptation_field_control*/
    if ((tsHeadInfo.adapter_control & BIT1) != BIT1)
    {
        ret |= GOSTSR_FAILURE;
    }

    return ret;
}

GOSTSR_S32 GosTsr_AnalysisData_AdaptationInfo(TS_HEAD_INFO *desData, const void *data, TS_ADAPT_INFO *adaptInfo)
{
    GOSTSR_U8 *curData = GOSTSR_NULL;
	

    if ((GOSTSR_NULL == data) || (GOSTSR_NULL == adaptInfo))
    {
        return GOSTSR_FAILURE;
    }
	
	if ((0 == desData->adapter_control) || (1 == desData->adapter_control))
	{	
		TsErrorCheck_OneLevel_ContinuityCounterError(&desData->stErrorInfo,desData->ts_pid,desData->counter,desData->adapter_control,adaptInfo->disContinuityIndicator);
		return GOSTSR_SUCCESS;
	}

    curData = (GOSTSR_U8 *)data;
    memset(adaptInfo, 0x00, sizeof(TS_ADAPT_INFO));

    /*adaptation field length*/
    adaptInfo->adaptLen = curData[0];
    if (adaptInfo->adaptLen > 0)
    {
        curData++;

        /*discontinuity indicator*/
        adaptInfo->disContinuityIndicator = (BIT8 & curData[0]) / BIT8;
		/*One Level Error Check*/
		TsErrorCheck_OneLevel_ContinuityCounterError(&desData->stErrorInfo,desData->ts_pid,desData->counter,desData->adapter_control,adaptInfo->disContinuityIndicator);
		//printf("-------------------------disContinuityIndicator = %d\n", adaptInfo->disContinuityIndicator);

        /*random access indicator*/
        adaptInfo->randomAccessIndicator = (BIT7 & curData[0]) / BIT7;

        /*elementary stream priority indicator*/
        adaptInfo->esPriorityIndicator = (BIT6 & curData[0]) / BIT6;

        /*PCR flag*/
        adaptInfo->pcrFlag = (BIT5 & curData[0]) / BIT5;

        /*OPCR flag*/
        adaptInfo->opcrFlag = (BIT4 & curData[0]) / BIT4;

        /*splicing point flag*/
        adaptInfo->splicingPointFlag = (BIT3 & curData[0]) / BIT3;

        /*transport private data flag*/
        adaptInfo->tsPrivateFlag = (BIT2 & curData[0]) / BIT2;

        /*adaptation field extension flag*/
        adaptInfo->adaptFieldExtenFlag = BIT1 & curData[0];

        curData++;

        /*pcr information*/
        if (adaptInfo->pcrFlag == BIT1)
        {
            /*program clock reference base*/
            adaptInfo->pcrInfo.pgClockRfBase = ((((curData[0]*256 + curData[1])*256 + curData[2])*256 + curData[3])*256 + (curData[4] & BIT8)) / BIT8;
            curData += 4;

            /*reserved*/

            /*program clock reference extension*/
            adaptInfo->pcrInfo.pgClockRfExten = (curData[0] & BIT1)*256 + curData[1];
            curData += 2;
			
			/*Two Level Error Check*/
			if (adaptInfo->disContinuityIndicator == BIT0)
			{
				TsErrorCheck_TwoLevel_checkPcrError(&desData->stErrorInfo,desData->ts_pid,adaptInfo->pcrInfo.pgClockRfBase, adaptInfo->pcrInfo.pgClockRfExten, GOSTSR_TRUE);
			}
			else
			{
				TsErrorCheck_TwoLevel_checkPcrError(&desData->stErrorInfo,desData->ts_pid,adaptInfo->pcrInfo.pgClockRfBase, adaptInfo->pcrInfo.pgClockRfExten, GOSTSR_FALSE);
			}
        }

        /*opcr information*/
        if (adaptInfo->opcrFlag == BIT1)
        {
            /*original program clock reference base*/
            adaptInfo->pcrInfo.pgClockRfBase = ((((curData[0]*256 + curData[1])*256 + curData[2])*256 + curData[3])*256 + (curData[4] & BIT8)) / BIT8;
            curData += 4;

            /*original reserved*/

            /*original program clock reference extension*/
            adaptInfo->pcrInfo.pgClockRfExten = (curData[0] & BIT1)*256 + curData[1];
            curData += 2;		
        }

        /*splicing point*/
        if (adaptInfo->splicingPointFlag == BIT1)
        {
            adaptInfo->spliceCountDown = curData[0];
            curData++;
        }

        /*transport private data*/
        if (adaptInfo->tsPrivateFlag == BIT1)
        {
            /*transport private data length*/
            adaptInfo->privateInfo.priDataLen = curData[0];
            curData++;

            /*private data byte*/
            memcpy(adaptInfo->privateInfo.priData, curData, adaptInfo->privateInfo.priDataLen);
            curData += adaptInfo->privateInfo.priDataLen;
        }

        /*adaptation field extension*/
        if (adaptInfo->adaptFieldExtenFlag == BIT1)
        {
            /*adaptation field extension length*/
            adaptInfo->extensionInfo.adaptExtenLen = curData[0];
            curData++;

            /*ltw flag*/
            adaptInfo->extensionInfo.ltwFlag = (curData[0] & BIT8) / BIT8;

            /*piecewise rate flag*/
            adaptInfo->extensionInfo.piecewiseRateFlag = (curData[0] & BIT7) / BIT7;

            /*seamless splice flag*/
            adaptInfo->extensionInfo.seamlessSpliceFlag = (curData[0] & BIT6) / BIT6;

            /*reserved*/
            curData++;

            /*ltw info*/
            if (adaptInfo->extensionInfo.ltwFlag == BIT1)
            {
               /*ltw valid flag*/
               adaptInfo->extensionInfo.ltwValidFlag = (curData[0] & BIT8) / BIT8;

               /*ltw offset*/
               adaptInfo->extensionInfo.ltwOffset = (curData[0] & (~BIT8))*256 + curData[1];
               curData += 2;
            }

            /*piecewise rate info*/
            if (adaptInfo->extensionInfo.piecewiseRateFlag == BIT1)
            {
                /*reserved*/

                /*piecewise rate*/
                adaptInfo->extensionInfo.piecewiseRate = (curData[0] & (~(BIT8 + BIT7)))*256*256 + curData[1]*256 + curData[2];
                curData += 3;
            }

            /*seamless splice info*/
            if (adaptInfo->extensionInfo.seamlessSpliceFlag == BIT1)
            {
                /*splice type*/
                adaptInfo->extensionInfo.spliceType = curData[0] & (~(BIT4 + BIT3 + BIT2 + BIT1));

                /*DTS next au*/
                adaptInfo->extensionInfo.DTSNextAU = ((curData[0] & (BIT4 + BIT3 + BIT2))<<29) + \
                        ((curData[1]*256 + (curData[2] & (~BIT1)))<<14) + (curData[3]*256 + (curData[4] & (~BIT1))) / BIT2;
                curData += 5;
            }

            /*reserved*/
        }

        /*stuffing byte*/
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_PESInfo(TS_HEAD_INFO *desData,const void *data, TS_PES_INFO *pesInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == data) || (GOSTSR_NULL == pesInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = (GOSTSR_U8 *)data;
    memset(pesInfo, 0x00, sizeof(TS_PES_INFO));

    /*packet start code prefix*/
    /*0000 0000 0000 0000 0000 0001*/
    pesInfo->pscp = (pData[0]*256 + pData[1])*256 + pData[2];
    pData += 3;

    /*stream id*/
    pesInfo->streamId = pData[0];
    pData += 1;

    /*PES packet length*/
    pesInfo->pesPacketLen = pData[0]*256 + pData[1];
    pData += 2;

    if ((pesInfo->streamId != PROGRAM_STREAM_MAP) \
        && (pesInfo->streamId != PADDING_STREAM) \
        && (pesInfo->streamId != PRIVATE_STREAM_TWO) \
        && (pesInfo->streamId != ECM_STREAM) \
        && (pesInfo->streamId != EMM_STREAM) \
        && (pesInfo->streamId != PROGRAM_STREAM_DIR))
    {
        /*'10'*/

        /*PES scrambling control*/
        pesInfo->pesScrambleCtr = (pData[0] & (BIT6 + BIT5)) / BIT5;

        /*PES priority*/
        pesInfo->pesPriorty = (pData[0] & BIT4) / BIT4;

        /*data alignment indicator*/
        pesInfo->dataAlignmentIndicator = (pData[0] & BIT3) / BIT3;

        /*copyright*/
        pesInfo->copyright = (pData[0] & BIT2) / BIT2;

        /*original or copy*/
        pesInfo->originalOrCopy = pData[0] & BIT1;
        pData += 1;

        /*PTS DTS flag*/
        pesInfo->PTSDTSFlag = (pData[0] & (BIT8 + BIT7)) / BIT7;

        /*ESCR flag*/
        pesInfo->ESCRFlag = (pData[0] & BIT6) / BIT6;

        /*ES rate flag*/
        pesInfo->EsRateFlag = (pData[0] & BIT5) / BIT5;

        /*DSM trick mode flag*/
        pesInfo->DSMTrickModeFlag = (pData[0] & BIT4) / BIT4;

        /*additional copy info flag*/
        pesInfo->additionalCopyInfoFlag = (pData[0] & BIT3) / BIT3;

        /*PES CRC flag*/
        pesInfo->PESCRCFlag = (pData[0] & BIT2) / BIT2;

        /*PES extension flag*/
        pesInfo->PESExtensionFlag = pData[0] & BIT1;
        pData += 1;

        /*PES header data length*/
        pesInfo->PESHeaderDataLen = pData[0];
        pData += 1;

        /*PTS DTS information*/
        if (BIT2 == pesInfo->PTSDTSFlag)
        {
            /*0010*/
            /*PTS*/
            pesInfo->PTS = ((pData[0] & (BIT4 + BIT3 + BIT2))<<29) + \
                    ((pData[1]*256 + (pData[2] & (~BIT1)))<<14) + ((pData[3]*256 + (pData[4] & (~BIT1))) / BIT2);
            pData += 5;
	/*Two Level Error Check*/
	TsErrorCheck_TwoLevel_checkPtsError(&desData->stErrorInfo,desData->ts_pid);
        }
        else if ((BIT1 + BIT2) == pesInfo->PTSDTSFlag)
        {
            /*0010*/
            /*PTS*/
            pesInfo->PTS = ((pData[0] & (BIT4 + BIT3 + BIT2))<<29) + \
                    ((pData[1]*256 + (pData[2] & (~BIT1)))<<14) + ((pData[3]*256 + (pData[4] & (~BIT1))) / BIT2);
            pData += 5;

			/*Two Level Error Check*/
			TsErrorCheck_TwoLevel_checkPtsError(&desData->stErrorInfo,desData->ts_pid);
			/*0010*/

            /*DTS*/
            pesInfo->DTS = ((pData[0] & (BIT4 + BIT3 + BIT2))<<29) + \
                    ((pData[1]*256 + (pData[2] & (~BIT1)))<<14) + ((pData[3]*256 + (pData[4] & (~BIT1))) / BIT2);
            pData += 5;
        }

        /*ESCR information*/
        if (pesInfo->ESCRFlag == BIT1)
        {
            /*reserve*/

            /*ESCR base*/
            pesInfo->ESCRBase = ((pData[0] & (BIT6 + BIT5 + BIT4))<<27) + \
                    ((((pData[0] & (BIT2 + BIT1))*256 + pData[1])*256 + (pData[2] & (~(BIT3 + BIT2 + BIT1))))<<12) + \
                    (((pData[2] & (BIT2 + BIT1))*256 + pData[3])*256 + (pData[4]& (~(BIT3 + BIT2 + BIT1)))) / BIT4;


            /*ESCR extension*/
            pesInfo->ESCRExten = ((pData[4] & (BIT1 + BIT2))*256 + (pData[5] & (~BIT1))) / BIT2;
            pData += 5;
        }

        /*ES rate information*/
        if (pesInfo->EsRateFlag == BIT1)
        {
            /*ES rate*/
            pesInfo->ESRate = (((pData[0] & (~BIT8))*256 + pData[1])*256 + (pData[2] & (~BIT1))) / BIT2;
            pData += 3;
        }

        /*DSM trick mode information*/
        if (pesInfo->DSMTrickModeFlag == BIT1)
        {
            /*trick mode control*/
            pesInfo->trickModeCtr = (pData[0] & (BIT8 + BIT7 + BIT6)) / BIT6;

            if ((BIT0 == pesInfo->trickModeCtr) || ((BIT2 + BIT1) == pesInfo->trickModeCtr))
            {
                /*field id*/
                pesInfo->fieldId = (pData[0] & (BIT5 + BIT4)) / BIT4;

                /*intra slice refresh*/
                pesInfo->intraSliceRefresh = (pData[0] & BIT3) / BIT3;

                /*frequency truncation*/
                pesInfo->frequencyTruncation = pData[0] & (BIT2 + BIT1);
            }
            else if ((BIT1 == pesInfo->trickModeCtr) || (BIT3 == pesInfo->trickModeCtr))
            {
                /*field rep cntrl*/
                pesInfo->fieldRepCntrl = pData[0] & (~(BIT8 + BIT7 + BIT6));
            }
            else if (BIT2 ==  pesInfo->trickModeCtr)
            {
                /*field id*/
                 pesInfo->fieldId = (pData[0] & (BIT5 + BIT4)) / BIT4;

                 /*reserve*/
            }
            else
            {
                /*reserve*/
            }

            pData += 1;
        }

        /*additional copy info*/
        if (pesInfo->additionalCopyInfoFlag == BIT1)
        {
            /*additional copy info*/
            pesInfo->additionalCopyInfo = pData[0] & (~BIT8);
            pData += 1;
        }

        /*PES CRC*/
        if (pesInfo->PESCRCFlag == BIT1)
        {
            /*previous PES packet CRC*/
            pesInfo->previousPESPacketCRC = pData[0]*256 + pData[1];
            pData += 2;
        }

        /*PES extension information*/
        if (pesInfo->PESExtensionFlag == BIT1)
        {
            /*PES private data flag*/
            pesInfo->pesExtensionInfo.PESPrivateDataFlag = (pData[0] & BIT8) / BIT8;

            /*pack header field flag*/
            pesInfo->pesExtensionInfo.packHeaderFieldFlag = (pData[0] & BIT7) / BIT7;

            /*program packet sequence counter flag*/
            pesInfo->pesExtensionInfo.programPacketSequenceCounterFlag = (pData[0] & BIT6) / BIT6;

            /*PSTD buffer flag*/
            pesInfo->pesExtensionInfo.PSTDBufferFlag = (pData[0] & BIT5) / BIT5;

            /*reserve*/

            /*PES extension flag 2*/
            pesInfo->pesExtensionInfo.PESExtensionFlag2 = pData[0] & BIT1;
            pData += 1;

            /*PES private data*/
            if (pesInfo->pesExtensionInfo.PESPrivateDataFlag == BIT1)
            {
                /*PES private data*/

                pData += 16; /* 16*8 = 128 */
            }

            /*pack header field*/
            if (pesInfo->pesExtensionInfo.packHeaderFieldFlag == BIT1)
            {
                /*pack field length*/
                pesInfo->pesExtensionInfo.packFieldLen = pData[0];
                pData += 1;

                /*pack header*/
                pData += pesInfo->pesExtensionInfo.packFieldLen;
            }

            /*program packet sequence counter information*/
            if (pesInfo->pesExtensionInfo.programPacketSequenceCounterFlag == BIT1)
            {
                /*program packet sequence counter*/
                pesInfo->pesExtensionInfo.programPacketSequenceCounter = pData[0] & (~BIT8);
                pData += 1;

                /*MPEG1 MPEG2 identifier*/
                pesInfo->pesExtensionInfo.MPEG1MPEG2Identifier = (pData[0] & BIT7) / BIT7;

                /*original stuff length*/
                pesInfo->pesExtensionInfo.originalStuffLen = pData[0] & (~(BIT8 + BIT7));
                pData += 1;
            }

            /*PSTD buffer information*/
            if (pesInfo->pesExtensionInfo.PSTDBufferFlag == BIT1)
            {
                /*01*/

                /*P-STD buffer scale*/
                pesInfo->pesExtensionInfo.PSTDBufferScale = (pData[0] & BIT6) / BIT6;

                /*P-STD buffer size*/
                pesInfo->pesExtensionInfo.PSTDBufferSize = (pData[0] & (~(BIT8 + BIT7 + BIT6)))*256 + pData[1];
                pData += 2;
            }

            /*PES extension 2*/
            if (pesInfo->pesExtensionInfo.PESExtensionFlag2 == BIT1)
            {
                /*PES extension field length*/
                pesInfo->pesExtensionInfo.PESExtensionFieldLen = pData[0] & (~BIT8);
                pData += 1;

                /*reserve*/
                pData += pesInfo->pesExtensionInfo.PESExtensionFieldLen;
            }
        }

        /*stuffing byte*/

        /*PES packet data byte*/
    }
    else if((pesInfo->streamId == PROGRAM_STREAM_MAP) \
        || (pesInfo->streamId == PRIVATE_STREAM_TWO) \
        || (pesInfo->streamId == ECM_STREAM) \
        || (pesInfo->streamId == EMM_STREAM) \
        || (pesInfo->streamId == PROGRAM_STREAM_DIR))
    {
        /*PES packet data byte*/
    }
    else if (pesInfo->streamId == PADDING_STREAM)
    {
        /*padding byte*/
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_PSI(const void *data, TS_HEAD_INFO tsHeadInfo)
{
	GOSTSR_S32 i = 0;
    GOSTSR_S32 ret = GOSTSR_FAILURE;

    if (GOSTSR_NULL == data)
    {
        return  GOSTSR_FAILURE;
    }

	TsErrorCheck_ThreeLevel_UnreferencedPidError(tsHeadInfo);
	//printf("########################## %x \n",tsHeadInfo.ts_pid);
	
    switch(tsHeadInfo.ts_pid)
    {
    case PAT_PID:
		//	printf("PAT_PID########################## %x #### %d \n",tsHeadInfo.ts_pid,tsHeadInfo.scramble_control);
		ret = GosTsr_AnalysisData_TableID(PAT_TABLE_ID, data, tsHeadInfo);
        break;

    case CAT_PID:
		//	printf("CAT_PID########################## %x \n",tsHeadInfo.ts_pid);
        ret = GosTsr_AnalysisData_TableID(CAT_TABLE_ID, data, tsHeadInfo);
        break;

    case NIT_PID:
		//	printf("NIT_PID########################## %x \n",tsHeadInfo.ts_pid);
        ret = GosTsr_AnalysisData_TableID(NIT_TABLE_ID_ACTUAL, data, tsHeadInfo);
        if (ret == GOSTSR_FAILURE)
        {
            ret = GosTsr_AnalysisData_TableID(NIT_TABLE_ID_OTHER, data, tsHeadInfo);
        } 
        break;

    default:

	
	for(i = 0;  i < gstPatInfo.u16NbElements;i++)
	{
		if(gstPatInfo.astElement[i].u16Pid == tsHeadInfo.ts_pid)
		{
		//printf("PMT_PID########################## %x #### %d \n",tsHeadInfo.ts_pid,tsHeadInfo.scramble_control);
			ret = GosTsr_AnalysisData_TableID(PMT_TABLE_ID, data, tsHeadInfo);
			break;
		}
	}
	
	 break;

    }

    return ret;
}

GOSTSR_S32 GosTsr_AnalysisData_SI(const void *data, TS_HEAD_INFO tsHeadInfo)
{
    GOSTSR_S32 ret = GOSTSR_FAILURE;

    if (GOSTSR_NULL == data)
    {
        return  GOSTSR_FAILURE;
    }

    switch(tsHeadInfo.ts_pid)
    {
    case BAT_SDT_PID:
        ret = GosTsr_AnalysisData_TableID(BAT_TABLE_ID, data, tsHeadInfo); /*BAT_PID*/
        if (GOSTSR_FAILURE == ret) /*SDT_PID*/
        {
            ret = GosTsr_AnalysisData_TableID(SDT_TABLE_ID_ACTUAL, data, tsHeadInfo);
        }
        if (ret == GOSTSR_FAILURE) /*SDT_PID*/
        {
            ret = GosTsr_AnalysisData_TableID(SDT_TABLE_ID_OTEHR, data, tsHeadInfo);
        }
        break;

    case EIT_PID:
        ret = GosTsr_AnalysisData_TableID(EIT_TABLE_ID_ACTUAL, data, tsHeadInfo);
        if (ret == GOSTSR_FAILURE)
        {
            ret = GosTsr_AnalysisData_TableID(EIT_TABLE_ID_OTHER, data, tsHeadInfo);
        }
        if (ret == GOSTSR_FAILURE) /*EIT_PID*/
        {
            ret = GosTsr_AnalysisData_TableID(EIT_TABLE_ID_OTHER_SHEDULE, data, tsHeadInfo);
        }
        if (ret == GOSTSR_FAILURE) /*EIT_PID*/
        {
            ret = GosTsr_AnalysisData_TableID(EIT_TABLE_ID_ACTUAL_SHEDULE, data, tsHeadInfo);
        }
        break;

    case RST_PID:
        ret = GosTsr_AnalysisData_TableID(RST_TABLE_ID, data, tsHeadInfo);
        break;

    case TDT_TOT_PID:
        ret = GosTsr_AnalysisData_TableID(TDT_TABLE_ID, data, tsHeadInfo); /*TDT_PID*/
        if (GOSTSR_FAILURE == ret) /*TOT_PID*/
        {
            ret = GosTsr_AnalysisData_TableID(TOT_TABLE_ID, data, tsHeadInfo);
        }
        break;

    default:
        if ((SI_PID_MIN <= tsHeadInfo.ts_pid) && (tsHeadInfo.ts_pid <= SI_PID_MAX))
        {
            ret = GosTsr_AnalysisData_TableID(SI_TABLE_ID, data, tsHeadInfo);
        }
    }

    return ret;
}

GOSTSR_S32 GosTsr_AnalysisData_PAT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_PAT_S *patInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_U16 i = 0;
    GOSTSR_U32 crcValue = 0;
    GOSTSR_U16 u16OverNbElements = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == patInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(patInfo, 0x00, sizeof(GOSTSR_PSISI_PAT_S));
    pData += 3;

    /*table ID*/
    patInfo->u8TableId = sectionInfo->tableID;

    /*tansport stream id*/
    tempData = pData[0]*256 + pData[1];
    patInfo->u16TransportStreamId = tempData;
    pData += 2;

    /*reserved*/

    /*version number*/
    patInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    patInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    patInfo->u8LastSectionNum = pData[0];
    pData += 1;

    patInfo->u16NbElements = (sectionInfo->sectionLength - 8 - 4) / 4;

    if (patInfo->u16NbElements > GOS_MAX_SERVICE_NUM)
    {
        u16OverNbElements = patInfo->u16NbElements - GOS_MAX_SERVICE_NUM;
        patInfo->u16NbElements = GOS_MAX_SERVICE_NUM;
    }

    for (i = 0; i < patInfo->u16NbElements; i++)
    {
        /*program number*/
        patInfo->astElement[i].u16ProgramNumber = pData[0]*256 + pData[1];

        /*PID*/
        patInfo->astElement[i].u16Pid = (pData[2] & (~(BIT8 | BIT7 | BIT6)))*256 + pData[3] ;

        pData += 4;
    }

    if (0 != u16OverNbElements)
    {
        pData += 4 * u16OverNbElements;
    }

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    patInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;
	
    GosTsr_AnalysisData_ResetPMTID();
    GosTsr_AnalysisData_GetPMTID(patInfo);
	GosTsr_AnalysisData_getPatInfo(patInfo);
	
    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_CAT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_CAT_S *catInfo)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 desTotalLength = 0;
    GOSTSR_U16 desSingleLength = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == catInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(catInfo, 0x00, sizeof(GOSTSR_PSISI_CAT_S));
    pData += 3;

    /*table ID*/
    catInfo->u8TableId = sectionInfo->tableID;

    /*reserved*/
    pData += 2;

    /*version number*/
    catInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    catInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    catInfo->u8LastSectionNum = pData[0];
    pData += 1;

    desTotalLength = sectionInfo->sectionLength - 8 - 4;
    i = 0;

    while(desTotalLength > 0)
    {
        /*descriptor tag*/
        catInfo->astDescriptor[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*descriptor length*/
        desSingleLength = pData[0];
        catInfo->astDescriptor[i].u16Size = desSingleLength;
        pData += 1;

        if ((desTotalLength - (desSingleLength + 2)) < 0)
        {
            pData += (desTotalLength - 2);
            break;
        }

        /*descriptor data*/
        catInfo->astDescriptor[i].Data = (GOSTSR_U8 *) malloc(desSingleLength);
        memset(catInfo->astDescriptor[i].Data, 0x00, desSingleLength);
        memcpy(catInfo->astDescriptor[i].Data, pData, desSingleLength);

        pData += desSingleLength;
        desTotalLength -= (desSingleLength + 2);
        i++;

        if (i >= GOS_MAX_CAT_DESCRIPTOR)
        {
            pData += desTotalLength;
            break;
        }
    }
    catInfo->u16NbDescriptors = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    catInfo->u32CrcValue = crcValue;


    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_NIT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_NIT_S *nitInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 netDesTotalLen = 0;
    GOSTSR_U16 netDesSingleLen = 0;
    GOSTSR_S32 netElementTotalLen = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == nitInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(nitInfo, 0x00, sizeof(GOSTSR_PSISI_NIT_S));
    pData += 3;

    /*table ID*/
    nitInfo->u8TableId = sectionInfo->tableID;

    /*network ID*/
    nitInfo->u16NetWorkId = pData[0]*256 +pData[1];
    pData += 2;

    /*reserved*/

    /*version number*/
    nitInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    nitInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    nitInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*reserved future use*/

    /*network description length*/
    netDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
    pData += 2;

    /*descriptor*/
    i = 0;
    while(netDesTotalLen > 0)
    {
        /*descriptor tags*/
        nitInfo->astDescriptor[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*descriptor length*/
        netDesSingleLen = pData[0];
        nitInfo->astDescriptor[i].u16Size = netDesSingleLen;
        pData += 1;

        if ((netDesTotalLen - (netDesSingleLen + 2)) < 0)
        {
            pData += (netDesTotalLen - 2);
            break;
        }

        /*descriptor data*/
        nitInfo->astDescriptor[i].Data = (GOSTSR_U8 *)malloc(netDesSingleLen);
        memset(nitInfo->astDescriptor[i].Data, 0x00, netDesSingleLen);
        memcpy(nitInfo->astDescriptor[i].Data, pData, netDesSingleLen);
        pData += netDesSingleLen;

        netDesTotalLen -= (netDesSingleLen + 2);
        i++;

        if (i >= GOS_MAX_NIT_DESCIP_LOOP)
        {
            pData += netDesTotalLen;
            break;
        }
    }
    nitInfo->u16NbDescriptors = i;

    /*reserved future use*/

    /*transport stream loop length*/
    netElementTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
    pData += 2;

    /*element*/
    j = 0; i = 0;
    while(netElementTotalLen > 0)
    {
        /*transport stream id*/
        nitInfo->astElement[i].u16TransportStreamId = pData[0]*256 + pData[1];
        pData += 2;

        /*original network id*/
        nitInfo->astElement[i].u16OriginalNetWorkId = pData[0]*256 + pData[1];
        pData += 2;

        /*reseved future use*/

        /*transport descriptor length*/
        netDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
        pData += 2;

        if ((netElementTotalLen - (6 + netDesTotalLen)) < 0)
        {
            pData += (netElementTotalLen - 6);
            break;
        }

        netElementTotalLen -= (6 + netDesTotalLen);

        j = 0;
        while(netDesTotalLen > 0)
        {
            /*transport descriptor tag*/
            nitInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*transport descriptor Length*/
            netDesSingleLen = pData[0];
            nitInfo->astElement[i].astDescriptor[j].u16Size = netDesSingleLen;
            pData += 1;

            if ((netDesTotalLen - (netDesSingleLen + 2)) < 0)
            {
                pData += (netDesTotalLen - 2);
                break;
            }

            /*transport descriptor data*/
            nitInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(netDesSingleLen);
            memset(nitInfo->astElement[i].astDescriptor[j].Data, 0x00, netDesSingleLen);
            memcpy(nitInfo->astElement[i].astDescriptor[j].Data, pData, netDesSingleLen);

            pData += netDesSingleLen;
            netDesTotalLen -= (netDesSingleLen + 2);
            j++;

            if (j >= GOS_MAX_NIT_ELEMENT_DESCRIPTOR)
            {
                pData += netDesTotalLen;
                break;
            }
        }
        nitInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >= GOS_MAX_NIT_ELEMENT)
        {
            pData += netElementTotalLen;
            break;
        }
    }
    nitInfo->u16NbElements = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    nitInfo->u32CrcValue = crcValue;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_PMT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_PMT_S *pmtInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_S32 pmtDesTotalLen = 0;
    GOSTSR_U16 pmtDesSingleLen = 0;
    GOSTSR_S32 pmtElmTotalLen = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == pmtInfo) || (0 == pmtsPIDNumber))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(pmtInfo, 0x00, sizeof(GOSTSR_PSISI_PMT_S));
    pData += 3;

    /*table ID*/
    pmtInfo->u8TableId = sectionInfo->tableID;

    /*program number*/
    pmtInfo->u16ProgramNumber = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*version number*/
    pmtInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    pmtInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    pmtInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*reserved*/

    /*PCR PID*/
    pmtInfo->u16PcrPid = (pData[0] & (~(BIT8 | BIT7 | BIT6)))*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*descriptor length*/
    pmtDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
    pData += 2;

    pmtElmTotalLen = sectionInfo->sectionLength - 4 - pmtDesTotalLen - 12;

    i = 0;
    /*descriptor*/
    while(pmtDesTotalLen > 0)
    {
        /*pmt descriptor tag*/
        pmtInfo->astDescriptor[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*pmt descriptor length*/
        pmtDesSingleLen = pData[0];
        pmtInfo->astDescriptor[i].u16Size = pmtDesSingleLen;
        pData += 1;

        if ((pmtDesTotalLen - (pmtDesSingleLen + 2)) < 0)
        {
            pData += (pmtDesTotalLen - 2);
            break;
        }

        /*pmt descriptor data*/
        pmtInfo->astDescriptor[i].Data = (GOSTSR_U8 *)malloc(pmtDesSingleLen);
        memset(pmtInfo->astDescriptor[i].Data, 0x00, pmtDesSingleLen);
        memcpy(pmtInfo->astDescriptor[i].Data, pData, pmtDesSingleLen);

        pData += pmtDesSingleLen;
        pmtDesTotalLen -= (pmtDesSingleLen + 2);
        i++;

        if (i >= GOS_MAX_PMT_DESCIP_LOOP1)
        {
            pData += pmtDesTotalLen;
            break;
        }
    }
    pmtInfo->u16NbDescriptors = i;

    i = 0;j = 0;
    while(pmtElmTotalLen > 0)
    {
        /*stream type*/
        pmtInfo->astElement[i].eType = (GOS_PSISI_STREAMTYPE_E)pData[0];
        pData += 1;

        /*reserved*/

        /*elementary PID*/
        pmtInfo->astElement[i].u16Pid  = (pData[0] & (~(BIT8 | BIT7 | BIT6)))*256 + pData[1];
        pData += 2;

        /*reserved*/

        /*ES info length*/
        pmtDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
        pData += 2;

        if ((pmtElmTotalLen - (pmtDesTotalLen + 5)) < 0)
        {
            pData += (pmtElmTotalLen - 5);
            break;
        }

        pmtElmTotalLen -= (pmtDesTotalLen + 5);

        j = 0;
        /*pmt descriptor*/
        while(pmtDesTotalLen > 0)
        {
            /*pmt descriptor tag*/
            pmtInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*pmt descriptor length*/
            pmtDesSingleLen = pData[0];
            pmtInfo->astElement[i].astDescriptor[j].u16Size = pmtDesSingleLen;
            pData += 1;

            if ((pmtDesTotalLen - (pmtDesSingleLen + 2)) < 0)
            {
                pData += (pmtDesTotalLen - 2);
                break;
            }

            /*pmt descriptor data*/
            pmtInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(pmtDesSingleLen);
            memset(pmtInfo->astElement[i].astDescriptor[j].Data, 0x00, pmtDesSingleLen);
            memcpy(pmtInfo->astElement[i].astDescriptor[j].Data, pData, pmtDesSingleLen);

            pData += pmtDesSingleLen;
            pmtDesTotalLen -= (pmtDesSingleLen + 2);
            j++;

            if (j >= GOS_MAX_PMT_ELEMENT_DESCRIPTOR)
            {
                pData += pmtDesTotalLen;
                break;
            }
        }
        pmtInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >= GOS_MAX_PMT_ELEMENT)
        {
            pData += pmtElmTotalLen;
            break;
        }
    }
    pmtInfo->u16NbElements = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    pmtInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_SDT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_SDT_S *sdtInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 sdtDesTotalLen = 0;
    GOSTSR_U16 sdtDesSingleLen = 0;
    GOSTSR_S32 sdtEleTotalLen = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == sdtInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(sdtInfo, 0x00, sizeof(GOSTSR_PSISI_SDT_S));
    pData += 3;

    /*table ID*/
    sdtInfo->u8TableId = sectionInfo->tableID;

    /*transport stream ID*/
    sdtInfo->u16TransportStreamId = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*version number*/
    sdtInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    sdtInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    sdtInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*original network id*/
    sdtInfo->u16OriginalNetWorkdId = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved future use*/
    pData += 1;

    /*element*/
    sdtEleTotalLen = sectionInfo->sectionLength - 11 - 4;

    i = 0;
    while(sdtEleTotalLen > 0)
    {
        /*service id*/
        sdtInfo->astElement[i].u16ServiceId = pData[0]*256 + pData[1];
        pData += 2;

        /*reserved future use*/

        /*EIT schedule flag*/
        sdtInfo->astElement[i].bEITScheduleFlag = (pData[0] & BIT2) / BIT2;

        /*EIT present following flag*/
        sdtInfo->astElement[i].bEITPresentFollowingFlag = pData[0] & BIT1;
        pData += 1;

        /*running status*/
        sdtInfo->astElement[i].eStatus =  (GOS_PSISI_TABLE_STATUS_E)((pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6);

        /*free CA mode*/
        sdtInfo->astElement[i].bScrambled = (pData[0] & BIT5) / BIT5;

        /*descriptor loop length*/
        sdtDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
        pData  += 2;

        if ((sdtEleTotalLen - (5 + sdtDesTotalLen)) < 0)
        {
            pData += (sdtEleTotalLen - 5);
            break;
        }

        sdtEleTotalLen -= (5 + sdtDesTotalLen);

        j = 0;
        while(sdtDesTotalLen > 0)
        {
            /*sdt descriptor tag*/
            sdtInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*sdt descriptor length*/
            sdtDesSingleLen = pData[0];
            sdtInfo->astElement[i].astDescriptor[j].u16Size = sdtDesSingleLen;
            pData += 1;

            if ((sdtDesTotalLen - (sdtDesSingleLen + 2)) < 0)
            {
                pData += (sdtDesTotalLen - 2);
                break;
            }

            /*sdt descriptor data*/
            sdtInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(sdtDesSingleLen);
            memset(sdtInfo->astElement[i].astDescriptor[j].Data, 0x00, sdtDesSingleLen);
            memcpy(sdtInfo->astElement[i].astDescriptor[j].Data, pData, sdtDesSingleLen);

            pData += sdtDesSingleLen;
            sdtDesTotalLen -= (sdtDesSingleLen + 2);
            j++;

            if (j >= GOS_MAX_SDT_ELEMENT_DESCRIPTOR)
            {
                pData += sdtDesTotalLen;
                break;
            }
        }
        sdtInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >= GOS_MAX_SDT_ELEMENT)
        {
            pData += sdtEleTotalLen;
            break;
        }
    }
    sdtInfo->u16NbElements = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    sdtInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_BAT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_BAT_S *batInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 batDesTotalLen = 0;
    GOSTSR_U16 batDesSingleLen = 0;
    GOSTSR_S32 batElmTotalLen = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == batInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(batInfo, 0x00, sizeof(GOSTSR_PSISI_BAT_S));
    pData += 3;

    /*table ID*/
    batInfo->u8TableId = sectionInfo->tableID;

    /*boquet ID*/
    batInfo->u16Bouquet_id = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*version number*/
    batInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    batInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    batInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*reserved future use*/

    /*descriptor length*/
    batDesTotalLen = (pData[0] & (BIT4 | BIT3 | BIT2 | BIT1))*256 + pData[1];
    pData += 2;

    i = 0;
    /*descriptor*/
    while(batDesTotalLen > 0)
    {
        /*bat descriptor tag*/
        batInfo->astDescriptor[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*bat descriptor length*/
        batDesSingleLen = pData[0];
        batInfo->astDescriptor[i].u16Size = batDesSingleLen;
        pData += 1;

        if ((batDesTotalLen - (batDesSingleLen + 2)) < 0)
        {
            pData += (batDesTotalLen - 2);
            break;
        }

        /*bat descriptor data*/
        batInfo->astDescriptor[i].Data = (GOSTSR_U8 *)malloc(batDesSingleLen);
        memset(batInfo->astDescriptor[i].Data, 0x00, batDesSingleLen);
        memcpy(batInfo->astDescriptor[i].Data, pData, batDesSingleLen);

        pData += batDesSingleLen;
        batDesTotalLen -= (batDesSingleLen + 2);
        i++;

        if (i >= GOS_MAX_BAT_DESCIP_LOOP1)
        {
            pData += batDesTotalLen;
            break;
        }
    }
    batInfo->u16NbDescriptors = i;

    /*reserved future use*/

    /*transport stream loop length*/
    batElmTotalLen = (pData[0] & (BIT4 | BIT3 | BIT2 | BIT1))*256 + pData[1];
    pData += 2;

    i = 0;j = 0;
    /*element*/
    while(batElmTotalLen > 0)
    {
        /*transport stream ID*/
        batInfo->astElement[i].u16TransportStreamId = pData[0]*256 + pData[1];
        pData += 2;

        /*original network ID*/
        batInfo->astElement[i].u16OriginalNetworkId = pData[0]*256 + pData[1];
        pData += 2;

        /*reserved future use*/

        /*transport descripor length*/
        batDesTotalLen = (pData[0] & (BIT4 | BIT3 | BIT2 | BIT1))*256 + pData[1];
        pData += 2;

        if ((batElmTotalLen - (6 + batDesTotalLen)) < 0)
        {
            pData += (batElmTotalLen - 6);
            break;
        }

        batElmTotalLen -= (6 + batDesTotalLen);

        j = 0;
        /*batt descriptor*/
        while(batDesTotalLen > 0)
        {
            /*bat descriptor tag*/
            batInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*bat descriptor length*/
            batDesSingleLen = pData[0];
            batInfo->astElement[i].astDescriptor[j].u16Size = batDesSingleLen;
            pData += 1;

            if ((batDesTotalLen - (batDesSingleLen + 2)) < 0)
            {
                pData += (batDesTotalLen - 2);
                break;
            }

            /*bat descriptor data*/
            batInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(batDesSingleLen);
            memset(batInfo->astElement[i].astDescriptor[j].Data, 0x00, batDesSingleLen);
            memcpy(batInfo->astElement[i].astDescriptor[j].Data, pData, batDesSingleLen);

            pData += batDesSingleLen;
            batDesTotalLen -= (batDesSingleLen + 2);
            j++;

            if (j >= GOS_MAX_BAT_ELEMENT_DESCRIPTOR)
            {
              pData += batDesTotalLen;
              break;
            }
        }
        batInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >= GOS_MAX_BAT_ELEMENT)
        {
            pData += batElmTotalLen;
            break;
        }
    }
    batInfo->u16NbElements = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    batInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_EIT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_EIT_S *eitInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 eitDesTotalLen = 0;
    GOSTSR_U16 eitDesSingleLen = 0;
    GOSTSR_S32 eitEleTotalLen = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == eitInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(eitInfo, 0x00, sizeof(GOSTSR_PSISI_SDT_S));
    pData += 3;

    /*table ID*/
    eitInfo->u8TableId = sectionInfo->tableID;

    /*service ID*/
    eitInfo->u16ServiceId = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*version number*/
    eitInfo->u8Version = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    eitInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    eitInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*transport stream ID*/
    eitInfo->u16TransportStreamId = pData[0]*256 + pData[1];
    pData += 2;

    /*original network ID*/
    eitInfo->u16OriginalNetWorkdId = pData[0]*256 + pData[1];
    pData += 2;

    /*segment last section number*/
    eitInfo->u8SegLastSectionNum = pData[0];
    pData += 1;

    /*last table ID*/
    eitInfo->u8LastTabeId = pData[0];
    pData += 1;

    eitEleTotalLen = sectionInfo->sectionLength - 14 - 4;

    i = 0; j = 0;
    while(eitEleTotalLen > 0)
    {
        /*event ID*/
        eitInfo->astElement[i].u16EventId = pData[0]*256 + pData[1];
        pData += 2;

        /*start Time*/
        eitInfo->astElement[i].u32StartTimeWord[0] = pData[0];
        eitInfo->astElement[i].u32StartTimeWord[1] = (pData[1]<<24) + (pData[2]<<16) +	(pData[3]<<8) + pData[4];
        pData += 5;

        /*duration*/
        eitInfo->astElement[i].u32DurationTimeWord = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;

        /*running status*/
        eitInfo->astElement[i].eStatus = (GOS_PSISI_TABLE_STATUS_E)((pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6);

        /*free CA mode*/
        eitInfo->astElement[i].bScrambled = (pData[0] & BIT5) / BIT5;

        /*descriptor loop length*/
        eitDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
        pData  += 2;

        if ((eitEleTotalLen - (12 + eitDesTotalLen)) < 0)
        {
            pData += (eitEleTotalLen - 12);
            break;
        }

        eitEleTotalLen -= (12 + eitDesTotalLen);

        j = 0;
        while (eitDesTotalLen > 0)
        {
            /*eit descriptor tag*/
            eitInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*eit descriptor length*/
            eitDesSingleLen = pData[0];
            eitInfo->astElement[i].astDescriptor[j].u16Size = eitDesSingleLen;
            pData += 1;

            if ((eitDesTotalLen - (eitDesSingleLen + 2)) < 0)
            {
                pData += (eitDesTotalLen - 2);
                break;
            }

            /*eit descriptor data*/
            eitInfo->astElement[i].astDescriptor[j].Data = GOSTSR_NULL;
            eitInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(eitDesSingleLen);
            if (GOSTSR_NULL == eitInfo->astElement[i].astDescriptor[j].Data)
            {
                return  GOSTSR_FAILURE;
            }
            memset(eitInfo->astElement[i].astDescriptor[j].Data, 0x00, eitDesSingleLen);
            memcpy(eitInfo->astElement[i].astDescriptor[j].Data, pData, eitDesSingleLen);

            pData += eitDesSingleLen;

            eitDesTotalLen -= (eitDesSingleLen + 2);

            j++;

            if (j >= GOS_MAX_EIT_ELEMENT_DESCRIPTOR)
            {
                pData += eitDesTotalLen;
                break;
            }
        }
        eitInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >= GOS_MAX_EIT_ELEMENT)
        {
           pData += eitEleTotalLen;
           break;
        }
    }
    eitInfo->u16NbElements = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    eitInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TDT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_TDT_S *tdtInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == tdtInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(tdtInfo, 0x00, sizeof(GOSTSR_PSISI_TDT_S));

    pData += 3;

    /*UTC time*/
    tdtInfo->u16MJDtime = pData[0]*256 + pData[1];
    pData += 2;

    tdtInfo->u32UTCtimeDWord = (pData[0]<<16) + (pData[1]<<8) + pData[2];

    /*hour*/
    tdtInfo->utcTime.u8Hour = (pData[0] & 0xf0)*10 + (pData[0] &0x0f);

    /*minute*/
    tdtInfo->utcTime.u8Minute = (pData[1] & 0xf0)*10 + (pData[1] &0x0f);

    /*second*/
    tdtInfo->utcTime.u8Second= (pData[2] & 0xf0)*10 + (pData[2] &0x0f);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_TOT(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_TOT_S *totInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 totDesTotalLen = 0;
    GOSTSR_U16 totDesSingleLen = 0;
    GOSTSR_U16 i = 0;
    GOSTSR_U32 crcValue = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == totInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(totInfo, 0x00, sizeof(GOSTSR_PSISI_TOT_S));

    pData += 3;

    /*UTC time*/
    totInfo->u16MJDtime = pData[0]*256 + pData[1];
    pData += 2;

    totInfo->u32UTCtimeDWord = (pData[0]<<16) + (pData[1]<<8) + pData[2];
    pData += 3;

    /*reserved*/

    /*descriptor length*/
    totDesTotalLen = (pData[0] & (~(BIT8 | BIT7 | BIT6 | BIT5)))*256 + pData[1];
    pData += 2;

    i = 0;
    while (totDesTotalLen > 0)
    {
        /*tot descriptor tag*/
        totInfo->stLocalTimeOffDesc[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*tot descriptor length*/
        totDesSingleLen = pData[0];
        totInfo->stLocalTimeOffDesc[i].u16Size = totDesSingleLen;
        pData += 1;

        if ((totDesTotalLen - (totDesSingleLen + 2)) < 0)
        {
            pData += (totDesTotalLen - 2);
            break;
        }

        /*tot descriptor data*/
        totInfo->stLocalTimeOffDesc[i].Data = (GOSTSR_U8*)malloc(totDesSingleLen);
        memset(totInfo->stLocalTimeOffDesc[i].Data, 0x00, totDesSingleLen);
        memcpy(totInfo->stLocalTimeOffDesc[i].Data, pData, totDesSingleLen);

        pData += totDesSingleLen;
        totDesTotalLen -= (totDesSingleLen + 2);
        i++;

        if (i >= GOS_MAX_TOT_DESCRIPTOR)
        {
            pData += totDesTotalLen;
            break;
        }
    }
    totInfo->u16NbDescriptors = i;

    /*CRC*/
    crcValue = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    totInfo->u32CrcValue = crcValue;
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_RST(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_RST_S *rstInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sectionInfo) || (GOSTSR_NULL == sectionInfo->sectionData) || (GOSTSR_NULL == rstInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionInfo->sectionData;
    memset(rstInfo, 0x00, sizeof(GOSTSR_PSISI_RST_S));

    pData += 3;

    /*element number*/
    rstInfo->u16NbElements = sectionInfo->sectionLength / 9;
    if (rstInfo->u16NbElements > GOS_MAX_RTT_ELEMENT)
    {
        rstInfo->u16NbElements = GOS_MAX_RTT_ELEMENT;
    }

    for (i = 0; i < rstInfo->u16NbElements; i++)
    {
        /*transport stream id*/
        rstInfo->astElement[i].u16TransportStreamId = pData[0]*256 + pData[1];
        pData += 2;

        /*original network id*/
        rstInfo->astElement[i].u16OriginalNetWorkdId = pData[0]*256 + pData[1];
        pData += 2;

        /*service id*/
        rstInfo->astElement[i].u16ServiceId = pData[0]*256 + pData[1];
        pData += 2;

        /*event id*/
        rstInfo->astElement[i].u16EventId = pData[0]*256 + pData[1];
        pData += 2;

        /*reserved future use*/

        /*running status*/
        rstInfo->astElement[i].u8RunStatus = pData[0] & (BIT3 | BIT2 | BIT1);
        pData += 1;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_ST(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_ST_S *stInfo)
{
   GOSTSR_U8 *pData = GOSTSR_NULL;
   GOSTSR_U16 tempSectionLen = 0;
   GOSTSR_U16 tempData = 0;

    if ((GOSTSR_NULL == sectionData) || (GOSTSR_NULL == sectionData->sectionData) || (GOSTSR_NULL == stInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionData->sectionData;
    memset(stInfo, 0x00, sizeof(GOSTSR_PSISI_DIT_S));

    /*Table ID */
    stInfo->u8TableId = sectionData->tableID;
    pData += 1;

    /*section syntax indicator*/
    tempData = (pData[0] & BIT8) / BIT8;

    /*reserved future user*/
    tempData = (pData[0] & BIT7) / BIT7;

    /*reserved*/
    tempData = (pData[0] & (BIT6 |BIT5)) / BIT5;

    /*section length*/
    tempSectionLen =  ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 8) + pData[1];
    pData += 2;

    /*have not meaning*/
    while(tempSectionLen > 0)
    {
        /*date byte*/
        tempData = pData[0];
    }

    if (tempData)
    {

    }
    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_DIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_DIT_S *ditInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;

    if ((GOSTSR_NULL == sectionData) || (GOSTSR_NULL == sectionData->sectionData) || (GOSTSR_NULL == ditInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionData->sectionData;
    memset(ditInfo, 0x00, sizeof(GOSTSR_PSISI_DIT_S));

    /*Table ID */
    ditInfo->u8TableId = sectionData->tableID;
    pData += 1;

    /*section syntax indicator*/
    tempData = (pData[0] & BIT8) / BIT8;
    if (0x00 != tempData)
    {
        return GOSTSR_FAILURE;
    }

    /*reserved future user*/
    tempData = (pData[0] & BIT7) / BIT7;

    /*reserved*/
    tempData = (pData[0] & BIT6) / BIT6;

    /*section length*/
    tempData =  ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 8) + pData[1];
    pData += 2;

    /*transition flag*/
    ditInfo->u8TransitionFlag =  (pData[0] & BIT8) / BIT8;

    /*reserved future user*/
    tempData = pData[0] & (~(BIT8));

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_AnalysisData_SIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_SIT_S *sitInfo)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 tempData = 0;
    GOSTSR_S32 sitDesTempLen = 0;
    GOSTSR_S32 sitDesTotalLen = 0;
    GOSTSR_U16 sitDesSingleLen = 0;
    GOSTSR_S32 sitElmTotalLen = 0;
    GOSTSR_U16 i = 0;
    GOSTSR_U16 j = 0;

    if ((GOSTSR_NULL == sectionData) || (GOSTSR_NULL == sectionData->sectionData) || (GOSTSR_NULL == sitInfo))
    {
        return GOSTSR_FAILURE;
    }

    pData = sectionData->sectionData;
    memset(sitInfo, 0x00, sizeof(GOSTSR_PSISI_SIT_S));

    /*table ID*/
    sitInfo->u8TableId = sectionData->tableID;
    pData += 1;

    /*section syntax indicator*/
    tempData = (pData[0] & BIT8) / BIT8;
    if (0x01 != tempData)
    {
        return GOSTSR_FAILURE;
    }

    /*DVB reserved future use*/
    tempData = (pData[0] & BIT7) / BIT7;

    /*ISO reserved*/
    tempData = (pData[0] & (BIT6 | BIT5)) / BIT5;

    /*section length*/
    tempData =  ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 8 )+ pData[1];
    pData += 2;

    /*DVB reserved future use*/
    tempData = (pData[0] << 8) + pData[1];
    pData += 2;

    /*ISO reserved*/
    tempData = (pData[0] & (BIT8 | BIT7)) / BIT7;

    /*version number*/
    sitInfo->u8VersionNum = (pData[0] & (~(BIT8 | BIT7 | BIT1))) / BIT2;

    /*current next indicator*/
    tempData = pData[0] & BIT1;
    if (BIT1 != tempData)
    {
        return GOSTSR_FAILURE;
    }
    pData += 1;

    /*section number*/
    sitInfo->u8SectionNum = pData[0];
    pData += 1;

    /*last section number*/
    sitInfo->u8LastSectionNum = pData[0];
    pData += 1;

    /*DVB reserved future use*/
    tempData = (pData[0] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;

    /*descriptor length*/
    sitDesTempLen = (pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) * 256 + pData[1];
    pData += 2;

    i = 0;
    sitDesTotalLen = sitDesTempLen;

    /*descriptor*/
    while(sitDesTotalLen > 0)
    {
        /*sit descriptor tag*/
        sitInfo->astDescriptor[i].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
        pData += 1;

        /*sit descriptor length*/
        sitDesSingleLen = pData[0];
        sitInfo->astDescriptor[i].u16Size = sitDesSingleLen;
        pData += 1;

        if ((sitDesTotalLen - (sitDesSingleLen + 2)) < 0)
        {
            pData += (sitDesTotalLen - 2);
            break;
        }

        /*sit descriptor data*/
        sitInfo->astDescriptor[i].Data = (GOSTSR_U8 *)malloc(sitDesSingleLen);
        memset(sitInfo->astDescriptor[i].Data, 0x00, sitDesSingleLen);
        memcpy(sitInfo->astDescriptor[i].Data, pData, sitDesSingleLen);

        pData += sitDesSingleLen;
        sitDesTotalLen -= (sitDesSingleLen + 2);
        i++;

        if (i >= GOS_MAX_SIT_DESCIP_LOOP1)
        {
            pData += sitDesTotalLen;
            break;
        }
    }
    sitInfo->u16NbDescriptors = i;

    /*transmission info loop length*/
    sitElmTotalLen = sitDesTempLen;
    i = 0;j = 0;

    /*element*/
    while(sitElmTotalLen > 0)
    {
        /*ServiceID*/
        sitInfo->astElement[i].u16ServiceId = pData[0]*256 + pData[1];
        pData += 2;

        /*DVB reserved future use*/
        tempData = (pData[0] & (BIT8)) / BIT8;

        /*Running Status*/
        sitInfo->astElement[i].u8RunStatus=  (pData[0] & (BIT7 | BIT6 | BIT5 )) / BIT5;

        /*Service loop length*/
        sitDesTotalLen = (pData[0] & (BIT4 | BIT3 | BIT2 | BIT1))*256 + pData[1];
        pData += 2;

        if ((sitElmTotalLen - (4 + sitDesTotalLen)) < 0)
        {
            pData += (sitElmTotalLen - 4);
            break;
        }

        sitElmTotalLen -= (4 + sitDesTotalLen);

        j = 0;
        /*sit descriptor*/
        while(sitDesTotalLen > 0)
        {
            /*sit descriptor tag*/
            sitInfo->astElement[i].astDescriptor[j].Tag = (GOS_PSISI_DESCRIPTORTYPE_E)pData[0];
            pData += 1;

            /*sit descriptor length*/
            sitDesSingleLen = pData[0];
            sitInfo->astElement[i].astDescriptor[j].u16Size = sitDesSingleLen;
            pData += 1;

            if ((sitDesTotalLen - (sitDesSingleLen + 2)) < 0)
            {
                pData += (sitDesTotalLen - 2);
                break;
            }

            /*sit descriptor data*/
            sitInfo->astElement[i].astDescriptor[j].Data = (GOSTSR_U8 *)malloc(sitDesSingleLen);
            memset(sitInfo->astElement[i].astDescriptor[j].Data, 0x00, sitDesSingleLen);
            memcpy(sitInfo->astElement[i].astDescriptor[j].Data, pData, sitDesSingleLen);

            pData += sitDesSingleLen;
            sitDesTotalLen -= (sitDesSingleLen + 2);
            j++;

            if (j >= GOS_MAX_SIT_ELEMENT_DESCRIPTOR)
            {
                pData += sitDesTotalLen;
                break;
            }
        }
        sitInfo->astElement[i].u16NbDescriptors = j;

        i++;

        if (i >=  GOS_MAX_SIT_ELEMENT)
        {
            pData += sitElmTotalLen;
            break;
        }
    }
    sitInfo->u16NbElements = i;

    /*CRC*/
    tempData = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_DATE_S GosTsr_TIME_MJDtoYMD(GOSTSR_U16 mjd)
{
    GOSTSR_S32 year_x, month_x;
    GOSTSR_S32 m_day, m_month, m_year;
    GOSTSR_U8 k;
    GOSTSR_DATE_S date;

    GOSTSR_U32  cal_mjd;

    cal_mjd = mjd;

    year_x 	 = (int)( (cal_mjd*100 - 1507820) / 36525 );
    month_x  = (int)( (cal_mjd*10000 - 149561000 - (int)((year_x * 36525)/100)*10000) / 306001 );
    m_day = cal_mjd - 14956 - (int) ((year_x * 36525)/100) - (int) ((month_x * 306001)/10000);

    if (month_x == 14 || month_x == 15)
        k = 1;
    else
        k = 0;

    m_year = (int) (year_x + k);
    m_month  = (int) (month_x - 1 - (k * 12));

    if( m_month > 12 )
    {
        m_year++;
        m_month -= 12;
    }
    /* Validate results before accessing array */

    m_year += 1900;

    date.u16year = m_year;
    date.u8month = m_month;
    date.u8day = m_day;

    return date;
}

GOSTSR_TIME_S GosTsr_TIME_UTCtoHMS(GOSTSR_U32 utc)
{
    GOSTSR_TIME_S time;

    time.u8hour     = (GOSTSR_U8) ( ( ((utc >> 20) & 0x0F) * 10 ) + ( (utc >> 16) & 0x0F ) );
    time.u8minute   = (GOSTSR_U8) ( ( ((utc >> 12) & 0x0F) * 10 ) + ( (utc >> 8) & 0x0F ) );
    time.u8second   = (GOSTSR_U8) ( ( ((utc >> 4) & 0x0F) * 10 ) + ( utc & 0x0F ) );

    if (60 <= time.u8second)
    {
        time.u8minute += time.u8second / 60;
        time.u8second = time.u8second % 60;
    }

    if (60 <= time.u8minute)
    {
        time.u8hour += time.u8minute / 60;
        time.u8minute = time.u8minute % 60;
    }

    if (24 <= time.u8hour)
    {
        time.u8hour = time.u8hour % 24;
    }

    return time;
}

GOS_STREAMTYPE GosTsr_AnalysisData_StreamType(GOS_PSISI_STREAMTYPE_E StreamData)
{
    GOS_STREAMTYPE StreamType = BULT_STREAM;

    if (  (StreamData) == GOS_PSISI_STREAM_VIDEO_MPEG1 \
          ||(StreamData) == GOS_PSISI_STREAM_VIDEO_MPEG2 \
          ||(StreamData) == GOS_PSISI_STREAM_VIDEO_MPEG4 \
          ||(StreamData) == GOS_PSISI_STREAM_VIDEO_H264 \
          ||(StreamData) == GOS_PSISI_STREAM_VIDEO_AVS \
          ||(StreamData) == GOS_PSISI_STREAM_VIDEO_WM9 \
          )
    {
        StreamType = VIDEO_STREAM;
    }
    else if ( (StreamData) == GOS_PSISI_STREAM_AUDIO_MPEG1 \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_MPEG2 \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_AAC_ADTS \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_AAC_LATM \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_AAC_RAW \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_AVS \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_AC3 \
              ||(StreamData) == GOS_PSISI_STREAM_AUDIO_DTS \
              )
    {
        StreamType = AUDIO_STREAM;
    }
    else if( (StreamData) == GOS_PSISI_STREAM_SCTE_SUBT)
    {
        StreamType = SCTE_SUBTITLE_STREAM;
    }
    else if( (StreamData) == GOS_PSISI_STREAM_PRIVATE_PES)
    {
        StreamType = PRIVATE_PES_STREAM;
    }
    else
    {
        StreamType = BULT_STREAM;
    }

    return StreamType;
}

GOSTSR_S32 GosTsr_AnalysisData_OTAPrivate(GOSTSR_U8 *privateData, GOSTSR_PSISI_OTAINFO *otaInfo)
{
    GOSTSR_S32 i = 0;
    GOSTSR_U8 *pData = privateData;

    if (GOSTSR_NULL == pData)
    {
        return GOSTSR_FAILURE;
    }

    /*Len*/
    otaInfo->u8DscLen = pData[0];
    pData += 1;

    /*OUI*/
    otaInfo->u32OUI = (pData[0] << 16) + (pData[1] << 8) + pData[2];
    pData += 3;

    /*section  Length*/
    otaInfo->u8SelectorLength = pData[0];
    pData += 1;

    /*Selector*/
    for(i  = 0;i < 12;i++)
    {
        otaInfo->u8Selector[i] = pData[i];
    }

    pData += 12;

    /*Soft Ver*/
    otaInfo->u8SoftVer = pData[0];
    pData += 1;

    /*Update mode*/
    otaInfo->u8UpdateMode = pData[0];
    pData += 1;

    /*Soft mode*/
    otaInfo->u8SoftMode = pData[0];
    pData += 1;

    /*OAD Pid*/
    otaInfo->u16OADPid = (pData[0] << 8) + pData[1];
    pData += 2;

    /*Frequency*/
    otaInfo->u32Frequency = (pData[0] << 16) + (pData[1] << 8) + pData[2];

    pData += 3;

    /*Bandwidth*/
    otaInfo->u8BandWidth = pData[0];
    pData += 1;

    /*Symbolrate*/
    otaInfo->u16Symbolrate = (pData[0] << 8) + pData[1];
    pData += 2;

    /*Qam*/
    otaInfo->u8Qam = pData[0];
    pData += 1;

    /*Polarity*/
    otaInfo->u8Polarity = pData[0];
    pData += 1;

    return GOSTSR_SUCCESS;
}

static GOSTSR_S32 GosTsr_Unicode_to_UTF8(GOSTSR_U16 unicode, GOSTSR_U8 *pOutdata, GOSTSR_U8 *pOutlen)
{
    GOSTSR_U8 count = 0;

    if ((GOSTSR_NULL == pOutdata) || (GOSTSR_NULL == pOutlen))
    {
        return GOSTSR_FAILURE;
    }

    if (unicode <= 0x0000007F)
    {
        *pOutdata = (unicode & 0x7F);
        count = 1;
    }
    else if ( unicode >= 0x00000080 && unicode <= 0x000007FF)
    {
        *(pOutdata+1) = (unicode & 0x3F) | 0x80;
        *pOutdata = ((unicode >> 6) & 0x1F) | 0xC0;
        count = 2;
    }
    else if ( unicode >= 0x00000800 && unicode <= 0x0000FFFF)
    {
        *(pOutdata+2) = (unicode & 0x3F) | 0x80;
        *(pOutdata+1) = ((unicode >>  6) & 0x3F) | 0x80;
        *pOutdata = ((unicode >> 12) & 0x0F) | 0xE0;
        count = 3;
    }
    else if (unicode >= 0x00010000 && unicode <= 0x001FFFFF)
    {
        *(pOutdata+3) = (unicode & 0x3F) | 0x80;
        *(pOutdata+2) = ((unicode >>  6) & 0x3F) | 0x80;
        *(pOutdata+1) = ((unicode >> 12) & 0x3F) | 0x80;
        *pOutdata = ((unicode >> 18) & 0x07) | 0xF0;
        count = 4;
    }
    else if (unicode >= 0x00200000 && unicode <= 0x03FFFFFF)
    {
        *(pOutdata+4) = (unicode & 0x3F) | 0x80;
        *(pOutdata+3) = ((unicode >>  6) & 0x3F) | 0x80;
        *(pOutdata+2) = ((unicode >> 12) & 0x3F) | 0x80;
        *(pOutdata+1) = ((unicode >> 18) & 0x3F) | 0x80;
        *pOutdata = ((unicode >> 24) & 0x03) | 0xF8;
        count = 5;
    }
    else if (unicode >= 0x04000000 && unicode <= 0x7FFFFFFF)
    {
        *(pOutdata+5) = (unicode & 0x3F) | 0x80;
        *(pOutdata+4) = ((unicode >>  6) & 0x3F) | 0x80;
        *(pOutdata+3) = ((unicode >> 12) & 0x3F) | 0x80;
        *(pOutdata+2) = ((unicode >> 18) & 0x3F) | 0x80;
        *(pOutdata+1) = ((unicode >> 24) & 0x3F) | 0x80;
        *pOutdata = ((unicode >> 30) & 0x01) | 0xFC;
        count = 6;
    }
    *pOutlen = count;
    if (0 == count)
        return GOSTSR_FAILURE;
    else
        return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_UnicodeString_to_UTF8String(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U8 tempbuff[6];
    GOSTSR_U8 tempLen = 0;
    GOSTSR_U32 totallen = 0;
    GOSTSR_U16 unicode = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i = i+2)
    {
        if ((sour_len % 2 != 0) && ((i+1) == sour_len))
        {
            unicode = 0x0000;
        }
        else
        {
            unicode = sour_string[i]*256 + sour_string[i+1];
        }
        if (GOSTSR_SUCCESS == GosTsr_Unicode_to_UTF8(unicode, tempbuff, &tempLen))
        {
            memcpy(&des_string[totallen], tempbuff, tempLen);
            totallen += tempLen;
            tempLen = 0;
        }
    }
    des_string[totallen] = '\0';
    totallen++;

    if (GOSTSR_NULL != des_len)
    {
        *des_len = totallen ;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_1String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        des_string[i*2] = 0x00;
        des_string[i*2 + 1] = sour_string[i];
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_2String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_2[index] / 256;
        des_string[i*2 + 1] = iso8859_2[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_3String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_3[index] / 256;
        des_string[i*2 + 1] = iso8859_3[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_4String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_4[index] / 256;
        des_string[i*2 + 1] = iso8859_4[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_5String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        switch (sour_string[i])
        {
        case 0xa0:
        case 0xad:
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = sour_string[i];
            break;

        case 0xf0:
            des_string[i*2] = 0x21;
            des_string[i*2 + 1] = 0x16;
            break;

        case 0xfd:
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = 0xa7;
            break;

        default:
            des_string[i*2] = 0x04;
            des_string[i*2 + 1] = sour_string[i] - 0xa0;
            break;
        }
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_6String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xac)
        {
            if (((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e)) || \
                    (0xa0 == sour_string[i]) || (sour_string[i] == 0xa4))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        if (sour_string[i] == 0xad)
        {
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = sour_string[i];
            continue;
        }

        des_string[i*2] = 0x06;
        des_string[i*2 + 1] = sour_string[i] - 0xa0;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_7String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_7[index] / 256;
        des_string[i*2 + 1] = iso8859_7[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_8String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_8[index] / 256;
        des_string[i*2 + 1] = iso8859_8[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_9String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as default code*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        switch (sour_string[i])
        {
        case 0xd0:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x1e;
            break;

        case 0xdd:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x30;
            break;

        case 0xde:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x5e;
            break;

        case 0xf0:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x1f;
            break;

        case 0xfd:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x31;
            break;

        case 0xfe:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x5f;
            break;

        default:
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = sour_string[i];
            break;
        }
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_10String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_10[index] / 256;
        des_string[i*2 + 1] = iso8859_10[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_11String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        if (0xa0 == sour_string[i])
        {
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = sour_string[i];
            continue;
        }

        des_string[i*2] = 0x0e;
        des_string[i*2 + 1] = sour_string[i] - 0xa0;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_13String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_13[index] / 256;
        des_string[i*2 + 1] = iso8859_13[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_14String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_14[index] / 256;
        des_string[i*2 + 1] = iso8859_14[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_15String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        switch (sour_string[i])
        {
        case 0xa4:
            des_string[i*2] = 0x20;
            des_string[i*2 + 1] = 0xac;
            break;

        case 0xa6:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x60;
            break;

        case 0xa8:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x61;
            break;

        case 0xb4:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x7d;
            break;

        case 0xb8:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x7e;
            break;

        case 0xbc:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x52;
            break;

        case 0xbd:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x53;
            break;

        case 0xbe:
            des_string[i*2] = 0x01;
            des_string[i*2 + 1] = 0x78;
            break;

        default:
            des_string[i*2] = 0x00;
            des_string[i*2 + 1] = sour_string[i];
            break;
        }
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_ISO8859_16String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U16 i = 0;
    GOSTSR_U16 index = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string))
    {
        return GOSTSR_FAILURE;
    }

    for (i = 0; i < sour_len; i++)
    {
        if (sour_string[i] < 0xa0)
        {
            if ((0x20 <= sour_string[i]) && (sour_string[i] <= 0x7e))
            {
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = sour_string[i];
                continue;
            }
            else
            {
                /*when no unicode match current encode, regards as 0x0002*/
                des_string[i*2] = 0x00;
                des_string[i*2 + 1] = DEFAULT_CODE;
                continue;
            }
        }

        index = sour_string[i] - 0xa0;

        des_string[i*2] = iso8859_16[index] / 256;
        des_string[i*2 + 1] = iso8859_16[index] % 256;
    }

    des_string[i*2] = 0x00;
    des_string[i*2 + 1] = 0x00;

    *des_len = 2 * sour_len + 2;

    return GOSTSR_SUCCESS;
}

ENCODE_TYPE GosTsr_CheckLanguageHeadCode(GOSTSR_U8 headCode, GOSTSR_U8 secondCode, GOSTSR_U8 thridCode)
{
    ENCODE_TYPE encodeType = ENCODE_ASCII;

    switch (headCode)
    {
    case FRIST_ISO10646:
        encodeType = ENCODE_UNICODE;
        break;

    case FRIST_ISO8859_5:
        encodeType = ENCODE_ISO8859_5;
        break;

    case FRIST_ISO8859_6:
        encodeType = ENCODE_ISO8859_6;
        break;

    case FRIST_ISO8859_7:
        encodeType = ENCODE_ISO8859_7;
        break;

    case FRIST_ISO8859_8:
        encodeType = ENCODE_ISO8859_8;
        break;

    case FRIST_ISO8859_9:
        encodeType = ENCODE_ISO8859_9;
        break;

    case FRIST_ISO8859_10:
        encodeType = ENCODE_ISO8859_10;
        break;

    case FRIST_ISO8859_11:
        encodeType = ENCODE_ISO8859_11;
        break;

    case FRIST_ISO8859_13:
        encodeType = ENCODE_ISO8859_13;
        break;

    case FRIST_ISO8859_14:
        encodeType = ENCODE_ISO8859_14;
        break;

    case FRIST_ISO8859_15:
        encodeType = ENCODE_ISO8859_15;
        break;

    case FRIST_ISO8859:
    {
        if (SECOND_ISO8859 == secondCode)
        {
            switch (thridCode)
            {
            case THIRD_ISO8859_1:
                encodeType = ENCODE_ISO8859_1;
                break;

            case THIRD_ISO8859_2:
                encodeType = ENCODE_ISO8859_2;
                break;

            case THIRD_ISO8859_3:
                encodeType = ENCODE_ISO8859_3;
                break;

            case THIRD_ISO8859_4:
                encodeType = ENCODE_ISO8859_4;
                break;

            case THIRD_ISO8859_5:
                encodeType = ENCODE_ISO8859_5;
                break;

            case THIRD_ISO8859_6:
                encodeType = ENCODE_ISO8859_6;
                break;

            case THIRD_ISO8859_7:
                encodeType = ENCODE_ISO8859_7;
                break;

            case THIRD_ISO8859_8:
                encodeType = ENCODE_ISO8859_8;
                break;

            case THIRD_ISO8859_9:
                encodeType = ENCODE_ISO8859_9;
                break;

            case THIRD_ISO8859_10:
                encodeType = ENCODE_ISO8859_10;
                break;

            case THIRD_ISO8859_11:
                encodeType = ENCODE_ISO8859_11;
                break;

            case THIRD_ISO8859_13:
                encodeType = ENCODE_ISO8859_13;
                break;

            case THIRD_ISO8859_14:
                encodeType = ENCODE_ISO8859_14;
                break;

            case THIRD_ISO8859_15:
                encodeType = ENCODE_ISO8859_15;
                break;

            default:
                break;
            }
        }
    }
        break;

    case FRIST_KSX1001:
        break;

    case FRIST_GB2312:
        //encodeType = ENCODE_GB2312;
        break;

    case FRIST_ISO10646_TC:
        encodeType = ENCODE_UNICODE;
        break;

    case FRIST_ISO10646_UTF8:
        encodeType = ENCODE_UTF8;
        break;

    case FRIST_ENCODE_TYPEID:
        break;

    default:
        break;
    }

    return encodeType;
}

GOSTSR_S32 GosTsr_SourceString_to_UTF8String(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len)
{
    GOSTSR_U8 index = 1;
    ENCODE_TYPE encodeType = ENCODE_BULT;
    GOSTSR_U8 *tempString = GOSTSR_NULL;
    GOSTSR_U16 tempStringLen = 0;

    if ((GOSTSR_NULL == sour_string) || (GOSTSR_NULL == des_string) || (GOSTSR_NULL == des_len) || (0 == sour_len))
    {
        if (GOSTSR_NULL != des_string)
        {
            des_string[0] = '\0';
        }
        return GOSTSR_FAILURE;
    }

    encodeType = GosTsr_CheckLanguageHeadCode(sour_string[0], sour_string[1], sour_string[2]);
    if (FRIST_ISO8859 == sour_string[0])
    {
        index = 3;
    }

    tempString = (GOSTSR_U8 *)malloc(sour_len*2 + 2);
    if (GOSTSR_NULL == tempString)
    {
        return GOSTSR_FAILURE;
    }
    memset(tempString, 0x00, 2*sour_len + 2);

    switch(encodeType)
    {
    case ENCODE_ASCII:
    case ENCODE_UTF8:
        memcpy(des_string, sour_string, sour_len);
        *des_len = sour_len;
        break;

    case ENCODE_UNICODE:
        GosTsr_UnicodeString_to_UTF8String(&sour_string[index], sour_len - index, des_string, des_len);
        break;

    case ENCODE_ISO8859_1:
        GosTsr_ISO8859_1String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_2:
        GosTsr_ISO8859_2String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_3:
        GosTsr_ISO8859_3String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_4:
        GosTsr_ISO8859_4String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_5:
        GosTsr_ISO8859_5String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_6:
        GosTsr_ISO8859_6String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_7:
        GosTsr_ISO8859_7String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_8:
        GosTsr_ISO8859_8String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_9:
        GosTsr_ISO8859_9String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_10:
        GosTsr_ISO8859_10String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_11:
        GosTsr_ISO8859_11String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_13:
        GosTsr_ISO8859_13String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_14:
        GosTsr_ISO8859_14String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_15:
        GosTsr_ISO8859_15String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    case ENCODE_ISO8859_16:
        GosTsr_ISO8859_16String_to_UnicodeString(&sour_string[index], sour_len - index, tempString, &tempStringLen);
        GosTsr_UnicodeString_to_UTF8String(tempString, tempStringLen, des_string, des_len);
        break;

    default:
        break;
    }

    if (GOSTSR_NULL != tempString)
    {
        free(tempString);
        tempString = GOSTSR_NULL;
    }

    return GOSTSR_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
