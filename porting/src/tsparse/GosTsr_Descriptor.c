#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "GosTsr_Descriptor.h"

GOSTSR_S32 GosTsr_Descriptor_AudioStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AUDIOSTRAM_S *asDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == asDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_AUDIO_STREAM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(asDes, 0x00, sizeof(GOS_DESC_AUDIOSTRAM_S));
    pData = srcDes->Data;

    /*free format flag*/
    asDes->u8FreeFormatFlag = (pData[0] & BIT8) / BIT8;

    /*ID*/
    asDes->u8ID = (pData[0] & BIT7) / BIT7;

    /*layer*/
    asDes->u8Layer = (pData[0] & (BIT6 | BIT5)) / BIT5;

    /*reserved*/

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_AVCVideo(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AVCVIDEO_S *avDes)
{
    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == avDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_AVC_VIDEO != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    /*avc video length*/
    avDes->u16AVCVideoLen = srcDes->u16Size;

    if (avDes->u16AVCVideoLen > GOS_MAX_AVCVIDEO_NUM)
    {
        avDes->u16AVCVideoLen = GOS_MAX_AVCVIDEO_NUM;
    }

    /*avc video data*/
    memset(avDes->u8AVCVideoData, 0x00, GOS_MAX_AVCVIDEO_NUM);
    memcpy(avDes->u8AVCVideoData, srcDes->Data, avDes->u16AVCVideoLen);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Bouquetname(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_BOUQUETNAME_S *bndes)
{
    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == bndes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_BOUQUET_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(bndes, 0x00, sizeof(GOS_DESC_BOUQUETNAME_S));
    bndes->u8NameLen = (srcDes->u16Size < GOS_MAX_BOUQUET_NAME_LEN)? srcDes->u16Size : (GOS_MAX_BOUQUET_NAME_LEN - 1);
    memcpy(bndes->au8BouqName, srcDes->Data, bndes->u8NameLen);
    bndes->au8BouqName[bndes->u8NameLen] = '\0';

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CableSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CABDELIVSYS_S *csDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == csDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CABLE_DELIVERY_SYSTEM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(csDes, 0x00, sizeof(GOS_DESC_CABDELIVSYS_S));
    pData = srcDes->Data;

    /*frequency*/
    csDes->u32Freq =((pData[0] & 0xf0) / BIT5)*10000000 + (pData[0] & 0x0f)*1000000 + \
            ((pData[1] & 0xf0) / BIT5)*100000 + (pData[1] & 0x0f)*10000 + \
            ((pData[2] & 0xf0) / BIT5)*1000 + (pData[2] & 0x0f)*100 + \
            ((pData[3] & 0xf0) / BIT5)*10 + (pData[3] & 0x0f);
    pData += 4;

    /*reserved future use*/
    pData += 1;

    /*FEC_outer*/
    csDes->u8FEC_Outer = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*modulation*/
    csDes->u8Modulation = pData[0];
    pData += 1;

    /*symbol rate*/
    csDes->u32Symbol_Rate = ((pData[0] & 0xf0) / BIT5)*1000000 + (pData[0] & 0x0f)*100000 + \
            ((pData[1] & 0xf0) / BIT5)*10000 + (pData[1] & 0x0f)*1000 + \
            ((pData[2] & 0xf0) / BIT5)*100 + (pData[2] & 0x0f)*10 + \
            ((pData[3] & 0xf0) / BIT5);
    pData += 3;

    /*FEC inner*/
    csDes->u8FEC_Inner = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CaIdentify(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CAIDF_S *ciDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ciDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CA_IDENTIFIER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ciDes, 0x00, sizeof(GOS_DESC_CAIDF_S));
    pData = srcDes->Data;

    ciDes->u8CANum = srcDes->u16Size / 2;

    for(i = 0; i < ciDes->u8CANum; i++)
    {
        ciDes->au16CA_Sys_ID[i] = pData[0]*256 + pData[1];
        pData += 2;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CA(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CA_S *caDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == caDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CA != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(caDes, 0x00, sizeof(GOS_DESC_CA_S));
    pData = srcDes->Data;

    /*CA system ID*/
    caDes->u16CaSystemId = pData[0]*256 + pData[1];
    pData += 2;

    /*reserved*/

    /*CA_PID*/
    caDes->u16CaPId = (pData[0] & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1))*256 + pData[1];
    pData += 2;

    /*CA private data len*/
    caDes->u8PivateDataLen = srcDes->u16Size - 4;

    /*CA pricate data*/
    caDes->pu8Data = (GOSTSR_U8 *)malloc(caDes->u8PivateDataLen);
    memset(caDes->pu8Data, 0x00, caDes->u8PivateDataLen);
    memcpy(caDes->pu8Data, pData, caDes->u8PivateDataLen);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CA_System(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CA_SYSTEM_S *caDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == caDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CA_SYSTEM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(caDes, 0x00, sizeof(GOS_DESC_CA_SYSTEM_S));
    pData = srcDes->Data;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CopyRight(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COPYRIGHT_S *cDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == cDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_COPYRIGHT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(cDes, 0x00, sizeof(GOS_DESC_COPYRIGHT_S));
    pData = srcDes->Data;

    /*copyriht indentifier*/
    cDes->u32CopyrightIdentifier = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    pData += 4;

    /*additional copyright info length*/
    cDes->u32AddCopyrightInfoLen = srcDes->u16Size - 4;
    if (cDes->u32AddCopyrightInfoLen > (GOS_MAX_ADDCOPYRIGHTINFO_NUM - 1))
    {
        cDes->u32AddCopyrightInfoLen = (GOS_MAX_ADDCOPYRIGHTINFO_NUM - 1);
    }

    /*additional copyright info*/
    memcpy(cDes->u8AddCopyrightInfo, pData, cDes->u32AddCopyrightInfoLen);
    cDes->u8AddCopyrightInfo[cDes->u32AddCopyrightInfoLen] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Component(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COMPONENT_S *cDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 remainLen = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == cDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_COMPONENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(cDes, 0x00, sizeof(GOS_DESC_COMPONENT_S));
    pData = srcDes->Data;

    /*reserved future use*/

    /*stream content*/
    cDes->u8StreamContent = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*component type*/
    cDes->u8ComponentType = pData[0];
    pData += 1;

    /*component tag*/
    cDes->u8ComponentTag = pData[0];
    pData += 1;

    /*ISO 639 language code*/
    cDes->au8ISO639_Language_Code[0] = pData[0];
    cDes->au8ISO639_Language_Code[1] = pData[1];
    cDes->au8ISO639_Language_Code[2] = pData[2];
    pData += 3;

    /*text char*/
    remainLen = srcDes->u16Size - 6;
    remainLen = remainLen < sizeof(cDes->acTextChar)? remainLen: sizeof(cDes->acTextChar);
    memset(cDes->acTextChar, 0x00, sizeof(cDes->acTextChar));
    memcpy(cDes->acTextChar, pData, remainLen);
    cDes->acTextChar[sizeof(cDes->acTextChar) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Component_Service_Name(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COMPONENT_SERVICE_NAME_S *cDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == cDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_COMPONENT_SERVICE_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(cDes, 0x00, sizeof(GOS_DESC_COMPONENT_SERVICE_NAME_S));
    pData = srcDes->Data;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Content(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CONTENT_S *cDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == cDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CONTENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(cDes, 0x00, sizeof(GOS_DESC_CONTENT_S));
    pData = srcDes->Data;

    cDes->u16NbContent = srcDes->u16Size / 2;
    for (i = 0; i < cDes->u16NbContent; i++)
    {
        cDes->astDetailList[i].u8NibbleLevel1 = (pData[0] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;
        cDes->astDetailList[i].u8NibbleLevel2 = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
        cDes->astDetailList[i].u8UserNibble1= (pData[1] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;
        cDes->astDetailList[i].u8UserNibble2 = pData[1] & (BIT4 | BIT3 | BIT2 | BIT1);
        pData += 2;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CountryAvailability(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COUNTRY_AVAILABILITY_S *caDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == caDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_COUNTRY_AVAIBILITY != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(caDes, 0x00, sizeof(GOS_DESC_COUNTRY_AVAILABILITY_S));
    pData = srcDes->Data;

    /*country availability flag*/
    caDes->u8Country_Availability_Flag = (pData[0] & BIT8) / BIT8;

    /*reserved future use*/
    pData += 1;

    caDes->u32CountryNum = (srcDes->u16Size - 1) / 3;
    for (i = 0; i < caDes->u32CountryNum; i++)
    {
        caDes->au32CountryCode[i] = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_DataStreamAlignment(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATASTRAMALIGNMENT_S *dsaDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == dsaDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_DATA_STREAM_ALIGNMENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(dsaDes, 0x00, sizeof(GOS_DESC_DATASTRAMALIGNMENT_S));
    pData = srcDes->Data;

    /*alignment type*/
    dsaDes->u8AlignmentType = pData[0];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_DataBroadcast(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATA_BROADCAST_S *dbDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == dbDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_DATA_BROADCAST != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(dbDes, 0x00, sizeof(GOS_DESC_DATA_BROADCAST_S));
    pData = srcDes->Data;

    /*data boradcast ID*/
    dbDes->u16DataBroadcastId = pData[0]*256 + pData[1];
    pData += 2;

    /*component tag*/
    dbDes->u8ComponentTag = pData[0];
    pData += 1;

    /*selector length*/
    dbDes->u8SelectorLength = pData[0];
    pData += 1;

    /*selector type*/
    memcpy(dbDes->au8SelectorByte, pData, dbDes->u8SelectorLength);
    pData += dbDes->u8SelectorLength;

    /*ISO 639 language code*/
    dbDes->au8ISO639_Language_Code[0] = pData[0];
    dbDes->au8ISO639_Language_Code[1] = pData[1];
    dbDes->au8ISO639_Language_Code[2] = pData[2];
    pData += 3;

    /*text length*/
    dbDes->u8TextLength = pData[0];
    pData += 1;

    /*text char*/
    memcpy(dbDes->au8TextChar, pData, dbDes->u8TextLength);
    dbDes->au8TextChar[sizeof(dbDes->au8TextChar) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_DataBroadcastID(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATA_BROADCAST_ID_S *dIDDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == dIDDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_DATA_BROADCAST_ID != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(dIDDes, 0x00, sizeof(GOS_DESC_DATA_BROADCAST_ID_S));
    pData = srcDes->Data;

    /*data broadcast ID*/
    dIDDes->u16DataBroadcastId = pData[0]*256 + pData[0];
    pData += 2;

    /*selector byte*/
    memcpy(dIDDes->au8IdSelectorByte, pData, srcDes->u16Size - 2);
    dIDDes->au8IdSelectorByte[sizeof(dIDDes->au8IdSelectorByte) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_ExtendEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_EXTEVT_S *eeDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S16 itemLen = 0;
    GOSTSR_U8 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == eeDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_EXTENDED_EVENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(eeDes, 0x00, sizeof(GOS_DESC_EXTEVT_S));
    pData = srcDes->Data;

    /*descriptor number*/
    eeDes->u8Desc_Num = (pData[0] & 0xf0) / BIT5;

    /*last descriptor number*/
    eeDes->u8Last_Desc_Num = pData[0] & 0x0f;
    pData += 1;

    /*ISO 639 language code*/
    eeDes->u32Lag_Code = (pData[0]<<16) + (pData[1]<<8) + pData[2];
    pData += 3;

    /*length of items*/
    itemLen = pData[0];
    eeDes->u8ItemNum = pData[0];
    pData += 1;

    i = 0;
    while (itemLen > 0)
    {
        /*item description length*/
        eeDes->astItemEle[i].u8ItemDescLen = pData[0];
        pData += 1;

        /*item description char*/
        memcpy(eeDes->astItemEle[i].ItemDesc, pData, eeDes->astItemEle[i].u8ItemDescLen);
        eeDes->astItemEle[i].ItemDesc[eeDes->astItemEle[i].u8ItemDescLen] = '\0';
        pData += eeDes->astItemEle[i].u8ItemDescLen;

        /*item length*/
        eeDes->astItemEle[i].u8ItemCharLen = pData[0];
        pData += 1;

        /*item char*/
        memcpy(eeDes->astItemEle[i].ItemChar, pData, eeDes->astItemEle[i].u8ItemCharLen);
        eeDes->astItemEle[i].ItemChar[eeDes->astItemEle[i].u8ItemCharLen] = '\0';
        pData += eeDes->astItemEle[i].u8ItemCharLen;

        itemLen -= (2 + eeDes->astItemEle[i].u8ItemCharLen + eeDes->astItemEle[i].u8ItemDescLen);
        i++;
    }
    eeDes->u8ItemNum = i;

    /*text length*/
    eeDes->u32Txt_Length = pData[0];
    pData += 1;

    /*text char*/
    memcpy(eeDes->acTxt_Char, pData, eeDes->u32Txt_Length);
    eeDes->acTxt_Char[sizeof(eeDes->acTxt_Char) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_FrequencyList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_FREQLST_S *flDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == flDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_FREQUENCY_LIST != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(flDes, 0x00, sizeof(GOS_DESC_FREQLST_S));
    pData = srcDes->Data;

    /*reserved future use*/

    /*coding type*/
    flDes->u8Coding_Type = pData[0] & (BIT2 | BIT1);
    pData += 1;

    flDes->u32FreNum = ((srcDes->u16Size - 1) / 4) < GOS_MAX_FREQUENCY_NUM? ((srcDes->u16Size - 1) / 4) : GOS_MAX_FREQUENCY_NUM;

    /*centre frequency*/
    for (i = 0; i < flDes->u32FreNum; i++)
    {
        flDes->au32FrequencyValue[i] = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
        pData += 4;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Hierarchy(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_HIERARCHY_S *hDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == hDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_HIERARCHY != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(hDes, 0x00, sizeof(GOS_DESC_HIERARCHY_S));
    pData = srcDes->Data;

    /*reserced*/

    /*hierarchy type*/
    hDes->u8HierarchyType = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*reserved*/

    /*hierarchy layer index*/
    hDes->u8HierarchyLayerIndex = pData[0] & (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*reserved*/

    /*hierarchy embedded layer*/
    hDes->u8HierarchyEmbeddedLayer = pData[0] & (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*reserved*/

    /*hierarchy priority*/
    hDes->u8HierarchyPriority = pData[0] & (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_ISO639LanguageCode(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ISOLANGUAGECODE_S *lcDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == lcDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_LANGUAGE_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(lcDes, 0x00, sizeof(GOS_DESC_ISOLANGUAGECODE_S));
    pData = srcDes->Data;

    /*ISO 639 language code length*/
    lcDes->u16LanguageCodeLen = (srcDes->u16Size - 1) / 3;
    if (lcDes->u16LanguageCodeLen > GOS_MAX_LANGUAGECODE_NUM)
    {
        lcDes->u16LanguageCodeLen = GOS_MAX_LANGUAGECODE_NUM;
    }

    for (i = 0; i < lcDes->u16LanguageCodeLen; i++)
    {
        lcDes->u32LanguageCode[i] = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;
    }

    /*audio type*/
    lcDes->u8AudioType = pData[0];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Linkage(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LINKAGE_S *lDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 u8TempOffset = 0;
    GOSTSR_U16 u16TempLen = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == lDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_LINKAGE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(lDes, 0x00, sizeof(GOS_DESC_LINKAGE_S));
    pData = srcDes->Data;

    u16TempLen = srcDes->u16Size;

    /*transport stream ID*/
    lDes->u16TsId = pData[0]*256 + pData[1];
    pData += 2;

    /*original network ID*/
    lDes->u16Orid = pData[0]*256 + pData[1];
    pData += 2;

    /*service id*/
    lDes->u16ServiceId = pData[0]*256 + pData[1];
    pData += 2;

    /*linkage type*/
    lDes->u8LinkType = pData[0];
    pData += 1;

    if(lDes->u8LinkType == 0x08)
    {
        /*hand over type*/
        lDes->u8HandOverType = (pData[0] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;

        /*reserved future use*/

        /*origin type*/
        lDes->u8OriginType = pData[0] & BIT1;
        pData[0] += 1;

        if((lDes->u8HandOverType == 0x01) ||( lDes->u8HandOverType == 0x02) || (lDes->u8HandOverType == 0x03))
        {
            /*network id*/
            lDes->u16NetworkID = (pData[0] << 8) + pData[1];
            u8TempOffset += 2;
            pData += 2;
        }
        if(lDes->u8OriginType == 0x00)
        {
            /*initial service id*/
            lDes->u16InitSerID = (pData[0] << 8) + pData[1];
            u8TempOffset += 2;
            pData += 2;
        }

        /*private data Length*/
        lDes->u16PrivDataLen = (GOSTSR_U8)u16TempLen - 8 - u8TempOffset;

        /*private data byte*/
        memcpy(lDes->u8PrivData,pData,lDes->u16PrivDataLen);

    }
    else if (0x0D == lDes->u8LinkType)
    {
        /*target event id*/
        lDes->u16TargetEventID = pData[0]*256 + pData[1];
        pData += 2;

        /*target listed*/
        lDes->u8TargetListed = (pData[0] & BIT8) / BIT8;

        /*event simulcast*/
        lDes->u8EventSimulcast = (pData[0] & BIT7) / BIT7;

        /*reserved*/
        pData += 1;

        u8TempOffset += 3;
    }
    else
    {
        /*private data Length*/
        lDes->u16PrivDataLen = (GOSTSR_U8)u16TempLen - 7 - u8TempOffset;

        /*private data byte*/
        memcpy(lDes->u8PrivData,pData,lDes->u16PrivDataLen);
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_LocalTimeOffset(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LOCALTMOFFSET_S *ltoDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ltoDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_LOCAL_TIME_OFFSET != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ltoDes, 0x00, sizeof(GOS_DESC_LOCALTMOFFSET_S));
    pData = srcDes->Data;

    ltoDes->u32TimeOffsetEleNum = (srcDes->u16Size / 13) < GOS_MAX_LOCALTIME_ELENUM? (srcDes->u16Size / 13) : GOS_MAX_LOCALTIME_ELENUM;

    for (i = 0; i < ltoDes->u32TimeOffsetEleNum; i++)
    {
        /*country code*/
        ltoDes->astTimeOffsetInfo[i].u32Country_Code = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;

        /*country region ID*/
        ltoDes->astTimeOffsetInfo[i].u8Country_Region_Id = (pData[0] & (~(BIT2 | BIT1))) / BIT3;

        /*reserved*/

        /*local time offset polarity*/
        ltoDes->astTimeOffsetInfo[i].u8Local_Time_Offset_Polarity = pData[0] & BIT1;
        pData += 1;

        /*local time offset*/
        ltoDes->astTimeOffsetInfo[i].u16Local_Time_Offset = ((pData[0] & 0xf0) / BIT5)*1000 + (pData[0] & 0x0f)*100 + \
                ((pData[1] & 0xf0) / BIT5)*10 + (pData[1] & 0x0f);
        pData += 2;

        /*time of change*/
        ltoDes->astTimeOffsetInfo[i].u16time_Of_Change_MJD = pData[0]*256 + pData[1];
        pData += 2;

        ltoDes->astTimeOffsetInfo[i].u32Time_Of_Change_UTC = ((pData[0] & 0xf0) / BIT5)*100000 + (pData[0] & 0x0f)*10000 + \
                ((pData[1] & 0xf0) / BIT5)*1000 + (pData[1] & 0x0f)*100 + \
                ((pData[2] & 0xf0) / BIT5)*10 + (pData[2] & 0x0f);
        pData += 3;

        /*next time offset*/
        ltoDes->astTimeOffsetInfo[i].u16Next_Time_Offset = ((pData[0] & 0xf0) / BIT5)*1000 + (pData[0] & 0x0f)*100 + \
                ((pData[1] & 0xf0) / BIT5)*10 + (pData[1] & 0x0f);
        pData += 2;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_LCN_83(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LCN83_S *lcn83Des)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == lcn83Des))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_LCN_0X83 != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(lcn83Des, 0x00, sizeof(GOS_DESC_LCN83_S));
    pData = srcDes->Data;

    for (i  = 0; i < (srcDes->u16Size / 4); i++)
    {
        /*service id*/
        lcn83Des->LCNServiceInfo[i].u16ServiceId = pData[0]*256 + pData[1];
        pData += 2;

        /*visible service flag*/
        lcn83Des->LCNServiceInfo[i].u8VisFlag = (pData[0] & BIT8) / BIT8;

        /*reserved*/

        /*Logical channel number*/
        lcn83Des->LCNServiceInfo[i].u16LCN = (pData[0] & (~(BIT8 + BIT7)))*256 + pData[1];
        pData += 2;
    }
    lcn83Des->u16LCNServiceNum = i;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_LCN_87(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LCN87_S *lcn87Des)
{
    GOSTSR_S32 desLen;
    GOSTSR_U16 lcnInfoLen = 0;
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0, j =0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == lcn87Des))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_LCN_0X87 != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(lcn87Des, 0x00, sizeof(GOS_DESC_LCN87_S));
    pData = srcDes->Data;
    desLen = srcDes->u16Size;

    while(desLen > 0)
    {
        /*channel list id*/
        lcn87Des->channelListInfo[i].u8ChannelListId = pData[0];
        pData++;

        /*channel list name length*/
        lcn87Des->channelListInfo[i].u16ChannelListNameLen = pData[0];
        pData++;

        /*channel list name*/
        memset(lcn87Des->channelListInfo[i].u16ChannelListName, '\0', sizeof(lcn87Des->channelListInfo[i].u16ChannelListName));
        memcpy(lcn87Des->channelListInfo[i].u16ChannelListName, pData, lcn87Des->channelListInfo[i].u16ChannelListNameLen);
        pData += lcn87Des->channelListInfo[i].u16ChannelListNameLen;

        /*country code*/
        lcn87Des->channelListInfo[i].u32Country_Code = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
        pData += 4;

        lcnInfoLen = pData[0];
        pData++;

        for (j = 0; j < (lcnInfoLen / 4); j++)
        {
            /*service id*/
            lcn87Des->channelListInfo[i].LCNServiceInfo[j].u16ServiceId = pData[0]*256 + pData[1];
            pData += 2;

            /*visible service flag*/
            lcn87Des->channelListInfo[i].LCNServiceInfo[j].u8VisFlag = (pData[0] & BIT8) / BIT8;

            /*reserved*/

            /*Logical channel number*/
            lcn87Des->channelListInfo[i].LCNServiceInfo[j].u16LCN = (pData[0] & (~(BIT8 + BIT7)))*256 + pData[1];
            pData += 2;
        }
        lcn87Des->channelListInfo[i].u16LCNServiceNum = j;

        desLen -= lcnInfoLen + 5 + lcn87Des->channelListInfo[i].u16ChannelListNameLen + 2;
        i++;
    }
    lcn87Des->u16ChannelListNum = i;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Mosaic(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MOSAIC_S *mDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 remainLen = 0;
    GOSTSR_U16 i = 0, j = 0;
    GOSTSR_U8 linkAge = 0;
    GOSTSR_U8 linkAgeLen = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MOSAIC != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mDes, 0x00, sizeof(GOS_DESC_MOSAIC_S));
    pData = srcDes->Data;

    /*mosaic entry point*/
    mDes->u8Mosaic_entry_point = (pData[0] & BIT8) / BIT8;

    /*number of horizontal elementary cells*/
    mDes->u8NumberHElementaryCells = (pData[0] & (BIT7 | BIT6 | BIT5)) / BIT5;

    /*reserved future use*/

    /*number of vertical elementary cells*/
    mDes->u8NumberVElementaryCells = pData[0] & (BIT3 | BIT2 | BIT1);
    pData += 1;

    remainLen = srcDes->u16Size - 1;

    i = 0;
    while(remainLen > 0)
    {
        /*logical cell ID*/
        mDes->astLogicCellInfo[i].u8LogicalCellId = (pData[0] & (~(BIT2 | BIT1))) / BIT3;

        /*reserved future use*/
        pData += 1;

        /*logical cell presentation info*/
        mDes->astLogicCellInfo[i].u8LogicalCellInfo = pData[0] & (BIT3 | BIT2 | BIT1);
        pData += 1;

        /*elementary cell field length*/
        mDes->astLogicCellInfo[i].u8EleCellNum = pData[0];
        if (mDes->astLogicCellInfo[i].u8EleCellNum > GOS_MAX_MOSAIC_ElE_CELL_NUM)
        {
            mDes->astLogicCellInfo[i].u8EleCellNum = GOS_MAX_MOSAIC_ElE_CELL_NUM;
        }
        pData += 1;

        for(j = 0; j < mDes->astLogicCellInfo[i].u8EleCellNum; j++)
        {
            /*reserved future use*/

            /*elementary cell ID*/
            mDes->astLogicCellInfo[i].astEleCellId[j] = pData[0] & (BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
            pData += 1;
        }

        /*cell linkage info*/
        linkAge = pData[0];
        mDes->astLogicCellInfo[i].u8CellLinkageInfo = linkAge;
        pData += 1;

        linkAgeLen = 0;

        if (linkAge == 0x01)
        {
            /*bouquet ID*/
            mDes->astLogicCellInfo[i].u16BouquetId = pData[0]*256 + pData[1];
            pData += 2;

            linkAgeLen = 2;
        }
        else if ((linkAge == 0x02) || (linkAge == 0x03))
        {
            /*original network ID*/
            mDes->astLogicCellInfo[i].u16OrId = pData[0]*256 + pData[1];
            pData += 2;

            /*transport stream ID*/
            mDes->astLogicCellInfo[i].u16TsId = pData[0]*256 + pData[1];
            pData += 2;

            /*sevice ID*/
            mDes->astLogicCellInfo[i].u16ServiceId = pData[0]*256 + pData[1];
            pData += 2;

            linkAgeLen = 6;
        }
        else if (linkAge == 0x04)
        {
            /*original network ID*/
            mDes->astLogicCellInfo[i].u16OrId = pData[0]*256 + pData[1];
            pData += 2;

            /*transport stream ID*/
            mDes->astLogicCellInfo[i].u16TsId = pData[0]*256 + pData[1];
            pData += 2;

            /*sevice ID*/
            mDes->astLogicCellInfo[i].u16ServiceId = pData[0]*256 + pData[1];
            pData += 2;

            /*Event ID*/
            mDes->astLogicCellInfo[i].u16EventId = pData[0]*256 + pData[1];
            pData += 2;

            linkAgeLen = 8;
        }

        remainLen -= linkAgeLen + mDes->astLogicCellInfo[i].u8EleCellNum + 4;
        i++;
    }
    mDes->u8NumberLogicCells = (GOSTSR_U8)i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_MaximumBitrate(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MAXBITRATE_S *mbDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mbDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MAX_BITRATE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mbDes, 0x00, sizeof(GOS_DESC_MAXBITRATE_S));
    pData = srcDes->Data;

    /*reserved*/

    /*maximum bitrate*/
    mbDes->u32MaxBitrate = ((pData[0] & (~(BIT8 | BIT7)))<<16) + (pData[1]<<8) + pData[2];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}	

GOSTSR_S32 GosTsr_Descriptor_MultilingualBouquetName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_BOUQUET_NAME_S *mbnDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 remainLen = 0;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mbnDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mbnDes, 0x00, sizeof(GOS_DESC_MULT_BOUQUET_NAME_S));
    pData = srcDes->Data;

    remainLen = srcDes->u16Size;

    i = 0;
    while (remainLen > 0)
    {
        /*ISO 639 language code*/
        mbnDes->astBouquetName[i].au8ISO639_Language_Code[0] = pData[0];
        mbnDes->astBouquetName[i].au8ISO639_Language_Code[1] = pData[1];
        mbnDes->astBouquetName[i].au8ISO639_Language_Code[2] = pData[2];
        pData += 3;

        /*bouquet name length*/
        mbnDes->astBouquetName[i].u8NameLength = pData[0];
        pData += 1;

        /*bouquer name*/
        memcpy(mbnDes->astBouquetName[i].au8Char, pData, mbnDes->astBouquetName[i].u8NameLength);
        mbnDes->astBouquetName[i].au8Char[255] = '\0';
        pData += mbnDes->astBouquetName[i].u8NameLength;

        remainLen -= 4 + mbnDes->astBouquetName[i].u8NameLength;
        i++;
    }
    mbnDes->u8BouquetNameNum = (GOSTSR_U8)i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_MultilingualComponent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_COMPONENT_S *mcDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 remainLen = 0;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mcDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_COMPONENT_SERVICE_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mcDes, 0x00, sizeof(GOS_DESC_MULT_COMPONENT_S));
    pData = srcDes->Data;

    /*component tag*/
    mcDes->u8ComponentTag = pData[0];
    pData += 1;

    remainLen = srcDes->u16Size - 1;

    i = 0;
    while (remainLen > 0)
    {
        /*ISO 639 language code*/
        mcDes->astComponent[i].au8ISO639_Language_Code[0] = pData[0];
        mcDes->astComponent[i].au8ISO639_Language_Code[1] = pData[1];
        mcDes->astComponent[i].au8ISO639_Language_Code[2] = pData[2];
        pData += 3;

        /*txt description length*/
        mcDes->astComponent[i].u8NameLength = pData[0];
        pData += 1;

        /*txt char*/
        memcpy(mcDes->astComponent[i].au8Char, pData, mcDes->astComponent[i].u8NameLength);
        mcDes->astComponent[i].au8Char[sizeof(mcDes->astComponent[i].au8Char) - 1] = '\0';
        pData += mcDes->astComponent[i].u8NameLength;

        remainLen -= 4 + mcDes->astComponent[i].u8NameLength;
        i++;
    }
    mcDes->u8ComponentNum = (GOSTSR_U8)i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_MultilingualNetworkName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_NETWORK_NAME_S *mnnDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 remainLen = 0;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mnnDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MULTILINGUAL_NETWORK_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mnnDes, 0x00, sizeof(GOS_DESC_MULT_NETWORK_NAME_S));
    pData = srcDes->Data;

    remainLen = srcDes->u16Size;

    i = 0;
    while (remainLen > 0)
    {
        /*ISO 639 language code*/
        mnnDes->astNetworkName[i].au8ISO639_Language_Code[0] = pData[0];
        mnnDes->astNetworkName[i].au8ISO639_Language_Code[1] = pData[1];
        mnnDes->astNetworkName[i].au8ISO639_Language_Code[2] = pData[2];
        pData += 3;

        /*Network name length*/
        mnnDes->astNetworkName[i].u8NameLength = pData[0];
        pData += 1;

        /*Network name*/
        memcpy(mnnDes->astNetworkName[i].au8Char, pData, mnnDes->astNetworkName[i].u8NameLength);
        mnnDes->astNetworkName[i].au8Char[sizeof(mnnDes->astNetworkName[i].au8Char) - 1] = '\0';
        pData += mnnDes->astNetworkName[i].u8NameLength;

        remainLen -= 4 + mnnDes->astNetworkName[i].u8NameLength;
        i++;
    }
    mnnDes->u8NetworkNameNum = (GOSTSR_U8)i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_MultilingualServiceName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_SERVICE_NAME_S *msnDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_S32 remainLen = 0;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == msnDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MULTILINGUAL_SERVICE_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(msnDes, 0x00, sizeof(GOS_DESC_MULT_SERVICE_NAME_S));
    pData = srcDes->Data;

    remainLen = srcDes->u16Size;

    i = 0;
    while (remainLen > 0)
    {
        /*ISO 639 language code*/
        msnDes->astServiceProviderName[i].au8ISO639_Language_Code[0] = pData[0];
        msnDes->astServiceProviderName[i].au8ISO639_Language_Code[1] = pData[1];
        msnDes->astServiceProviderName[i].au8ISO639_Language_Code[2] = pData[2];
        msnDes->astServiceName[i].au8ISO639_Language_Code[0] = pData[0];
        msnDes->astServiceName[i].au8ISO639_Language_Code[1] = pData[1];
        msnDes->astServiceName[i].au8ISO639_Language_Code[2] = pData[2];
        pData += 3;

        /*service provider name length*/
        msnDes->astServiceProviderName[i].u8NameLength = pData[0];
        pData += 1;

        /*service provider name*/
        memcpy(msnDes->astServiceProviderName[i].au8Char, pData, msnDes->astServiceProviderName[i].u8NameLength);
        msnDes->astServiceProviderName[i].au8Char[sizeof(msnDes->astServiceProviderName[i].au8Char) - 1] = '\0';
        pData += msnDes->astServiceProviderName[i].u8NameLength;

        /*service name length*/
        msnDes->astServiceName[i].u8NameLength = pData[0];
        pData += 1;

        /*service name*/
        memcpy(msnDes->astServiceName[i].au8Char, pData, msnDes->astServiceName[i].u8NameLength);
        msnDes->astServiceName[i].au8Char[sizeof(msnDes->astServiceName[i].au8Char) - 1] = '\0';
        pData += msnDes->astServiceName[i].u8NameLength;

        remainLen -= 5 + msnDes->astServiceProviderName[i].u8NameLength + msnDes->astServiceName[i].u8NameLength;
        i++;
    }
    msnDes->u8ServiceNameNum = (GOSTSR_U8)i;
    msnDes->u8ServiceProviderNameNum = (GOSTSR_U8)i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_MultiplexBufferUtilization(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULBUFFERUTILIZATION_S *mbuDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == mbuDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_MULTIPLEX_BUFFER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(mbuDes, 0x00, sizeof(GOS_DESC_MULBUFFERUTILIZATION_S));
    pData = srcDes->Data;

    /*mdv valid flag*/
    mbuDes->u8MdvValidFlag = (pData[0] & BIT8) / BIT8;

    /*multiplex delay variation*/
    mbuDes->u16MultiplexDelayVariation = (pData[0] & (~(BIT8)))*256 + pData[1];
    pData += 2;

    /*multiplex strategy*/
    mbuDes->u8MultiplexStrategy = (pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6;

    /*reserved*/

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_NVODReference(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_NVODREF_S *nrDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == nrDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_NVOD_REFERENCE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(nrDes, 0x00, sizeof(GOS_DESC_NVODREF_S));
    pData = srcDes->Data;

    nrDes->u8NbService = srcDes->u16Size / 6;
    if (nrDes->u8NbService > GOS_MAX_NVODSERVICE_NUM)
    {
        nrDes->u8NbService = GOS_MAX_NVODSERVICE_NUM;
    }

    for (i = 0; i < nrDes->u8NbService; i++)
    {
        /*transport stream ID*/
        nrDes->astServiceList[i].u16TrasId = pData[0]*256 + pData[1];
        pData += 2;

        /*original network ID*/
        nrDes->astServiceList[i].u16OrId = pData[0]*256 + pData[1];
        pData += 2;

        /*service ID*/
        nrDes->astServiceList[i].u16ServiceId = pData[0]*256 + pData[1];
        pData += 2;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_NetworkName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_NETWORKNAME_S *netnDes)
{
    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == netnDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_NETWORK_NAME != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(netnDes, 0x00, sizeof(GOS_DESC_NETWORKNAME_S));
    netnDes->u8NameLen = (srcDes->u16Size < GOS_MAX_BOUQUET_NAME_LEN)? srcDes->u16Size : GOS_MAX_BOUQUET_NAME_LEN;
    memcpy(netnDes->au8NetworkName, srcDes->Data, netnDes->u8NameLen);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_ParentRating(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PARENTRATING_S *prDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == prDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_PARENTAL_RATING != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(prDes, 0x00, sizeof(GOS_DESC_PARENTRATING_S));
    pData = srcDes->Data;

    prDes->u32RatingNum = srcDes->u16Size / 4;
    if (prDes->u32RatingNum > GOS_MAX_PARENTALRATING_NUM)
    {
        prDes->u32RatingNum = GOS_MAX_PARENTALRATING_NUM;
    }

    for (i = 0; i < prDes->u32RatingNum; i++)
    {
        /*country code*/
        prDes->astRating[i].u32Lag_Code = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;

        /*rating*/
        prDes->astRating[i].u8Rating = pData[0];
        pData += 1;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_PrivateDataSpecifier(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PRIVATE_DATA_SPECIFIER_S *pdsDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == pdsDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_PRIVATE_DATA_SPECIFIER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(pdsDes, 0x00, sizeof(GOS_DESC_PRIVATE_DATA_SPECIFIER_S));
    pData = srcDes->Data;

    /*private data specifier*/
    pdsDes->u32PrivateData = (pData[0]<<24) + (pData[1]<<16) + (pData[0]<<8) + pData[0];
    pData += 4;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_PrivateDataIndicator(GOS_PSISI_Descriptor_S *srcDes,  GOS_DESC_PRIVATEDATAINDICATOR_S *pdiDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == pdiDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_PRIVATE_DATA_INDICATOR != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(pdiDes, 0x00, sizeof(GOS_DESC_PRIVATEDATAINDICATOR_S));
    pData = srcDes->Data;

    /*private data indicator*/
    pdiDes->u32PrivateDataIndicator = (pData[0]<<24) + (pData[1]<<16) + (pData[0]<<8) + pData[0];
    pData += 4;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Registration(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_REGISTRATION_S *rDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 remainLen = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == rDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_REGISTRATION != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(rDes, 0x00, sizeof(GOS_DESC_REGISTRATION_S));
    pData = srcDes->Data;

    /*format indentifier*/
    rDes->u32FormatIdentifier = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    pData += 4;

    /*additional identification info length*/
    remainLen = srcDes->u16Size - 4;
    rDes->u16AddIdentificationLen = remainLen;

    /*additional identification info*/
    memcpy(rDes->u8AddIdentificationInfo, pData, remainLen);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}
GOSTSR_S32 GosTsr_Descriptor_ShortSmoothingBuffer(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SHORT_SMOOTHING_BUFFER_S *ssbDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ssbDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SHORT_SMOOTHING_BUFFER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ssbDes, 0x00, sizeof(GOS_DESC_SHORT_SMOOTHING_BUFFER_S));
    pData = srcDes->Data;

    /*sb size*/
    ssbDes->u8SbSize = (pData[0] & (BIT8 | BIT7)) / BIT7;

    /*sb leak rate*/
    ssbDes->u8SbLeakRate = pData[0] & (~(BIT8 | BIT7));
    pData += 1;

    /*DVB reserved*/
    if (srcDes->u16Size > (sizeof(ssbDes->u8DVBReserved) + 1))
    {
        ssbDes->u16DVBReservedLen = sizeof(ssbDes->u8DVBReserved);
        memcpy(ssbDes->u8DVBReserved, pData, sizeof(ssbDes->u8DVBReserved));
    }
    else
    {
        ssbDes->u16DVBReservedLen =  srcDes->u16Size - 1;
        memcpy(ssbDes->u8DVBReserved, pData, srcDes->u16Size - 1);
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_SmoothingBuffer(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SMOOTHING_BUFFER_S *sbDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == sbDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SMOOTHING_BUFFER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(sbDes, 0x00, sizeof(GOS_DESC_SMOOTHING_BUFFER_S));
    pData = srcDes->Data;
    pData += 1;

    /*sb leak rate*/
    sbDes->u32SbLeakRate = (pData[0] << 8)+pData[1];
    pData += 3;

    /*sb size*/
    sbDes->u32SbSize = ((pData[0] & 0X0F) << 8) + pData[1];



    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Service(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVICE_S *sDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == sDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SERVICE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(sDes, 0x00, sizeof(GOS_DESC_SERVICE_S));
    pData = srcDes->Data;

    /*service type*/
    sDes->u8ServiceType = pData[0];
    pData += 1;

    /*service provider name length*/
    sDes->u8ServiceProviderNameLen = pData[0];
    pData += 1;

    /*service provider name*/
    memcpy(sDes->u8ServiceProvideName, pData, sDes->u8ServiceProviderNameLen);
    sDes->u8ServiceProvideName[sizeof(sDes->u8ServiceProvideName) - 1] = '\0';
    pData += sDes->u8ServiceProviderNameLen;

    /*service name length*/
    sDes->u8ServiceNameLen = pData[0];
    pData += 1;

    /*service name*/
    memcpy(sDes->u8ServiceName, pData, sDes->u8ServiceNameLen);
    sDes->u8ServiceName[sizeof(sDes->u8ServiceName) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_ServiceList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVLIST_S *serLDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;
    GOSTSR_S32 desTotalLen = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == serLDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SERVICE_LIST != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(serLDes, 0x00, sizeof(GOS_DESC_SERVLIST_S));

    desTotalLen = srcDes->u16Size;
    pData = srcDes->Data;

    i = 0;
    while(desTotalLen > 0)
    {
        /*service ID*/
        serLDes->astServiceList[i].U16ServiceId = pData[0]*256 + pData[1];
        pData += 2;

        /*service type*/
        serLDes->astServiceList[i].U8ServiceType = pData[0];
        pData += 1;

        desTotalLen -= 3;
        i++;
    }
    serLDes->u16NbService = i;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_ServiceMove(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVICE_MOVE_S *smDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == smDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SERVICE_MOVE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(smDes, 0x00, sizeof(GOS_DESC_SERVICE_MOVE_S));
    pData = srcDes->Data;

    /*new original network ID*/
    smDes->u16NewONID = pData[0]*256 + pData[1];
    pData += 2;

    /*new transport strean ID*/
    smDes->u16NewTSID = pData[0]*256 + pData[1];
    pData += 2;

    /*new service ID*/
    smDes->u16NewServiceID = pData[0]*256 + pData[1];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_SatelliteSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SATDELIVSYS_S *ssdes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ssdes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ssdes, 0x00, sizeof(GOS_DESC_SATDELIVSYS_S));
    pData = srcDes->Data;

    /*frequency*/
    ssdes->u32Freq =((pData[0] & 0xf0) / BIT5)*10000000 + (pData[1] & 0x0f)*1000000 + \
            ((pData[1] & 0xf0) / BIT5)*100000 + (pData[1] & 0x0f)*10000 + \
            ((pData[2] & 0xf0) / BIT5)*1000 + (pData[2] & 0x0f)*100 + \
            ((pData[3] & 0xf0) / BIT5)*10 + (pData[3] & 0x0f);
    pData += 4;

    /*orbital position*/
    ssdes->u160rbital_Position = ((pData[0] & 0xf0) / BIT5)*1000 + (pData[0] & 0x0f)*100 + \
            ((pData[1] & 0xf0) / BIT5)*10 + (pData[1] & 0x0f);
    pData += 2;

    /*west east flag*/
    ssdes->u8West_East_Flag = (pData[0] & BIT8) / BIT8;

    /*polarisation*/
    ssdes->u8Polarization = (pData[0] & (BIT7 | BIT6)) / BIT6;

    /*modulation*/
    ssdes->u8Modulation = pData[0] & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*symbol rate*/
    ssdes->u32Symbol_Rate = ((pData[0] & 0xf0) / BIT5)*1000000 + (pData[1] & 0x0f)*100000 + \
            ((pData[1] & 0xf0) / BIT5)*10000 + (pData[1] & 0x0f)*1000 + \
            ((pData[2] & 0xf0) / BIT5)*100 + (pData[2] & 0x0f)*10 + \
            ((pData[3] & 0xf0) / BIT5);
    pData += 3;

    /*FEC inner*/
    ssdes->u8FEC_Inner = pData[0] & (BIT8 | BIT7 | BIT6 | BIT5);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;

}

GOSTSR_S32 GosTsr_Descriptor_ShortEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SHORTEVT_S *seDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == seDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SHORT_EVENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(seDes, 0x00, sizeof(GOS_DESC_SHORTEVT_S));
    pData = srcDes->Data;

    /*ISO 639 language code*/
    seDes->u32LangCode = (pData[0]<<16) + (pData[1]<<8) + pData[2];
    pData += 3;

    /*event name length*/
    seDes->u16EvtNameLen = pData[0];
    if (seDes->u16EvtNameLen > GOS_MAX_EVENTNAME_LEN)
    {
        seDes->u16EvtNameLen = GOS_MAX_EVENTNAME_LEN;
    }
    pData += 1;

    /*event name*/
    memcpy(seDes->acEvtName, pData, seDes->u16EvtNameLen);
    seDes->acEvtName[sizeof(seDes->acEvtName) - 1] = '\0';
    pData += seDes->u16EvtNameLen;

    /*text length*/
    seDes->u16ShortEvtDesLen = pData[0];
    if (seDes->u16ShortEvtDesLen > GOS_MAX_EVENTSHORTEXTDES_LEN)
    {
        seDes->u16ShortEvtDesLen = GOS_MAX_EVENTSHORTEXTDES_LEN;
    }
    pData += 1;

    /*text*/
    memcpy(seDes->acShortEvtDes, pData, seDes->u16ShortEvtDesLen);
    seDes->acShortEvtDes[sizeof(seDes->acShortEvtDes) - 1] = '\0';

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_StreamIdentifier(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STREAM_IDENTIFIER_S *siDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == siDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_STREAM_IDENTIFIER != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(siDes, 0x00, sizeof(GOS_DESC_STREAM_IDENTIFIER_S));
    pData = srcDes->Data;

    /*component tag*/
    siDes->u8Component_tag = pData[0];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Stuffing(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STUFFING_S *sDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == sDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_STUFFING != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(sDes, 0x00, sizeof(GOS_DESC_STUFFING_S));
    pData = srcDes->Data;

    /*stuffing length*/
    sDes->u16stuffingLen = srcDes->u16Size;

    /*stuffing byte*/
    memcpy(sDes->u8stuffingByte, pData, sDes->u16stuffingLen);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Subtitling(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SUBT_S *sDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == sDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SUBTITLING != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(sDes, 0x00, sizeof(GOS_DESC_SUBT_S));
    pData = srcDes->Data;

    /*subtitling element number*/
    sDes->u32SubtitleEleNum = srcDes->u16Size / 8;
    if (sDes->u32SubtitleEleNum > GOS_MAX_SUBTITLE_ELENUM)
    {
        sDes->u32SubtitleEleNum = GOS_MAX_SUBTITLE_ELENUM;
    }

    for (i = 0; i < sDes->u32SubtitleEleNum; i++)
    {
        /*ISO 639 langguage code*/
        sDes->astSubtitleInfo[i].u32Country_Code = (pData[0]<<16) + (pData[1]<<8) + pData[2];


        pData += 3;

        /*subtitling code*/
        sDes->astSubtitleInfo[i].u8Substitling_Type = pData[0];
        pData += 1;

        /*composition page ID*/
        sDes->astSubtitleInfo[i].u16Composition_Page_Id = pData[0]*256 + pData[1];
        pData += 2;

        /*anclillary page ID*/
        sDes->astSubtitleInfo[i].u16Ancillary_Page_Id = pData[0]*256 + pData[1];
        pData += 2;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_SystemClock(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SYSTEMCLOCK_S *scDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == scDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_SYSTEM_CLOCK != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(scDes, 0x00, sizeof(GOS_DESC_SYSTEMCLOCK_S));
    pData = srcDes->Data;

    /*external clock reference indicator*/
    scDes->u8ExternClockRefIndicator = (pData[0] & BIT8) / BIT8;

    /*reserved*/

    /*clock accuracy integer*/
    scDes->u8ClockAccuracyInteger = pData[0] & (~(BIT8 | BIT7));
    pData += 1;

    /*clock accuracy exponent*/
    scDes->u8ClockAccuracyExponent = (pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6;

    /*reserved*/

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_TargetBackgroundGrid(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TARBACGRID_S *tbgDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tbgDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TARGET_BACKGROUND_GRID != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tbgDes, 0x00, sizeof(GOS_DESC_TARBACGRID_S));
    pData = srcDes->Data;

    /*horizontal size*/
    tbgDes->u16HorizontalSize = (pData[0]<<6) + (pData[1]>>2);
    pData += 1;

    /*vertical size*/
    tbgDes->u16VerticalSize = ((pData[0] & (BIT2 | BIT1))<<12) + (pData[1]<<4) + (pData[2] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;
    pData += 2;

    /*pel aspect ratio*/
    tbgDes->u8PelaspectRatio = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_TerrestrialSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TERDELIVSYS_S *tsdes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tsdes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tsdes, 0x00, sizeof(GOS_DESC_TERDELIVSYS_S));
    pData = srcDes->Data;

    /*centre frequency*/
    tsdes->u32Centre_Freq = (pData[0]<<24) + (pData[1]<<16) + (pData[2]<<8) + pData[3];
    pData += 4;

    /*bandwidth*/
    tsdes->u8BandWidth = (pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6;

    /*reserved future use*/
    pData += 1;

    /*constellation*/
    tsdes->u8Constellation = (pData[0] & (BIT8 | BIT7)) / BIT7;

    /*hierarchy information*/
    tsdes->u8Hierarchy_Infor = (pData[0] & (BIT6 | BIT5 | BIT4)) / BIT4;

    /*code rate HP stream*/
    tsdes->u8Code_rate_HP_stream = pData[0] & (BIT3 | BIT2 | BIT1);
    pData += 1;

    /*code rate LP stream*/
    tsdes->u8Code_rate_LP_stream = (pData[0] & (BIT8 | BIT7 | BIT6)) / BIT6;

    /*guard interval*/
    tsdes->u8Guard_Interval = (pData[0] & (BIT5 | BIT4)) / BIT4;

    /*transmission mode*/
    tsdes->u8Trans_Mode = (pData[0] & (BIT3 | BIT2)) / BIT2;

    /*other frequency flag*/
    tsdes->u8Other_Freq_Flag = pData[0] & BIT1;

    pData += 1;

    /*reserved future use*/

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Telephone(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TELEPHONE_S *tDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TELEPHONE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tDes, 0x00, sizeof(GOS_DESC_TELEPHONE_S));
    pData = srcDes->Data;

    /*reserved future use*/

    /*foregin availability*/
    tDes->u8ForeginAvailability = (pData[0] & BIT6) / BIT6;

    /*connection type*/
    tDes->u8ConnectType = pData[0] & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*reserved future use*/

    /*country prefix length*/
    tDes->u8CountryPrefixLen = (pData[0] & (BIT7 | BIT6)) / BIT6;

    /*international area code length*/
    tDes->u8InternationalAreaCodeLen = (pData[0] & (BIT5 | BIT4 | BIT3)) / BIT3;

    /*operator code length*/
    tDes->u8OperatorCodeLen = pData[0] & (BIT2 | BIT1);
    pData += 1;

    /*reserved future use*/

    /*national area code length*/
    tDes->u8NationAreaCodeLen = (pData[0] & (BIT7 | BIT6 | BIT5)) / BIT5;

    /*core number length*/
    tDes->u8CoreNumberCodeLen = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*country prefix*/
    memcpy(tDes->u8CountryPrefix, pData, tDes->u8CountryPrefixLen);
    pData += tDes->u8CountryPrefixLen;

    /*international area code*/
    memcpy(tDes->u8InternationaAreaCode, pData, tDes->u8InternationalAreaCodeLen);
    pData += tDes->u8InternationalAreaCodeLen;

    /*operator code*/
    memcpy(tDes->u8OperatorCode, pData, tDes->u8OperatorCodeLen);
    pData += tDes->u8OperatorCodeLen;

    /*nation area code*/
    memcpy(tDes->u8InternationaAreaCode, pData, tDes->u8InternationalAreaCodeLen);
    pData += tDes->u8InternationalAreaCodeLen;

    /*core number*/
    memcpy(tDes->u8CoreNumberCode, pData, tDes->u8CoreNumberCodeLen);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_Teletex(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TELETEXT_S *tDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TELETEXT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tDes, 0x00, sizeof(GOS_DESC_TELETEXT_S));
    pData = srcDes->Data;

    /*teletex element number*/
    tDes->u32TeletextNum = srcDes->u16Size / 5;

    /*teletex element*/
    for (i = 0; i < tDes->u32TeletextNum; i++)
    {
        /*ISO 639 language code*/
        tDes->astTeletextInfo[i].u32Lag_Code = (pData[0]<<16) + (pData[1]<<8) + pData[2];

        pData += 3;

        /*teletext type*/
        tDes->astTeletextInfo[i].u8Teletext_Type = (pData[0] & (BIT8 | BIT7 | BIT6 | BIT5 | BIT4)) / BIT4;

        /*teletext magazine number*/
        tDes->astTeletextInfo[i].u8Teletext_Magazine_Number = pData[0] & (BIT3 | BIT2 | BIT1);
        pData += 1;

        /*teletext page number*/
        tDes->astTeletextInfo[i].u8Teletext_Page_Number = pData[0];
        pData += 1;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_TimeShiftedEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TMSHIFTEVT_S *tseDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tseDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TIME_SHIFTED_EVENT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tseDes, 0x00, sizeof(GOS_DESC_TMSHIFTEVT_S));
    pData = srcDes->Data;

    /*reference service ID*/
    tseDes->u16Ref_Service_Id = pData[0]*256 + pData[1];
    pData += 2;

    /*reference event ID*/
    tseDes->u16Ref_Evt_Id = pData[0]*256 + pData[1];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_TimeShiftedService(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TIMESHIFTSERV_S *tssDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tssDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TIME_SHIFTEDSERVICE != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tssDes, 0x00, sizeof(GOS_DESC_TIMESHIFTSERV_S));
    pData = srcDes->Data;

    /*reference service ID*/
    tssDes->u16RefServiceId = pData[0]*256 + pData[1];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_VideoStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VIDEOSTREAM_S *vsDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == vsDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_VIDEO_STREAM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(vsDes, 0x00, sizeof(GOS_DESC_VIDEOSTREAM_S));
    pData = srcDes->Data;

    /*multiple frame rate flag*/
    vsDes->u8MultipleFrameRateFlag = (pData[0] & BIT8) / BIT8;

    /*frame rate code*/
    vsDes->u8FrameRateCode = (pData[0] & (BIT7 | BIT6 | BIT5 | BIT4)) / BIT4;

    /*MPEG-2 flag*/
    vsDes->u8MPEG2Flag = (pData[0] & BIT3) / BIT3;

    /*constrained parameter flag*/
    vsDes->u8ConstrainParameterFlag = (pData[0] & BIT2) / BIT2;

    /*still picture flag*/
    vsDes->u8StillPictureFlag = pData[0] & BIT1;
    pData += 1;

    if (vsDes->u8MPEG2Flag ==  0x01)
    {
        /*profile and level indication*/
        vsDes->u8ProfileAndLevelIndication = pData[0];
        pData += 1;

        /*chroma format*/
        vsDes->u8ChromaFormat = (pData[0] & (BIT8 | BIT7)) / BIT7;

        /*frame  rate extension flag*/
        vsDes->u8FrameRateExtensionFlag = (pData[0] & BIT6) / BIT6;

        /*reserved*/
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_VideoWindow(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VIDEOWINDOW_S *vwDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == vwDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_VIDEO_WINDOW != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(vwDes, 0x00, sizeof(GOS_DESC_VIDEOWINDOW_S));
    pData = srcDes->Data;

    /*horizontal offset*/
    vwDes->u16HorizontalZOffset = (pData[0]<<6) + (pData[1]>>2);
    pData += 1;

    /*vertical offset*/
    vwDes->u16VerticalOffset = ((pData[0] & (BIT2 | BIT1))<<12) + (pData[1]<<4) + (pData[2] & (BIT8 | BIT7 | BIT6 | BIT5)) / BIT5;
    pData += 2;

    /*window priority*/
    vwDes->u8WindowPriority = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_PDC(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PDC_S *pdcDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 tempData = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == pdcDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_PDC != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(pdcDes, 0x00, sizeof(GOS_DESC_PDC_S));
    pData = srcDes->Data;

    /*reserved future use*/
    tempData = (pData[0] & (BIT8 | BIT7 | BIT6 | BIT5) ) / BIT5;

    /*programme identification label*/
    pdcDes->u32ProgrammeIdentification = ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 16) + (pData[1] << 8) + pData[2];
    pData += 3;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}


GOSTSR_S32 GosTsr_Descriptor_STD(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STD_S *stdDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 tempData = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == stdDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_STD != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(stdDes, 0x00, sizeof(GOS_DESC_STD_S));
    pData = srcDes->Data;

    /*reserved*/
    tempData = (pData[0] & (~BIT1)) / BIT2;

    /*leak valid flag*/
    stdDes->u8ReakValidFlag = pData[0] & (BIT1);

    pData += 1;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_IBP(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_IBP_S *ibpDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ibpDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_IBP  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ibpDes, 0x00, sizeof(GOS_DESC_IBP_S));
    pData = srcDes->Data;

    /*closed gop flag*/
    ibpDes->u8ClosedGopFlag= (pData[0] & (BIT8)) / BIT8;

    /*identical gop flag*/
    ibpDes->u8IdenticalGopFlag= (pData[0] & (BIT7)) / BIT7;

    /*max gop length*/
    ibpDes->u16MaxGopLength =  (pData[0] & ((~ (BIT8 | BIT7)) << 8)) + pData[1];

    pData += 2;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_DSNG(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DSNG_S *dgDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == dgDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_DSNG != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(dgDes, 0x00, sizeof(GOS_DESC_DSNG_S));
    pData = srcDes->Data;

    /*DSNG byte length*/
    dgDes->u16ByteLen = srcDes->u16Size;

    for(i = 0; i < dgDes->u16ByteLen; i++)
    {
        /*DSNG byte*/
        dgDes->astByte[i].u8ByteData = pData[0];

    }

    pData += dgDes->u16ByteLen;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_TransportStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TPSTREAM_S *tpsDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == tpsDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_TRANSPORT_STREAM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(tpsDes, 0x00, sizeof(GOS_DESC_TPSTREAM_S));
    pData = srcDes->Data;

    /*TransportStream byte length*/
    tpsDes->u16ByteLen = srcDes->u16Size;


    for(i = 0; i < tpsDes->u16ByteLen; i++)
    {
        /*Transport Stream byte*/
        tpsDes->astByte[i].u8ByteData = pData[0];

    }
    pData += tpsDes->u16ByteLen;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_VBI_Data(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VBIDATA_S *vbiDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 tempDataInt = 0;
    GOSTSR_U8 tempDataChar[256] = {0};
    GOSTSR_U16 i = 0;
    GOSTSR_U16 j = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == vbiDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_VBI_DATA != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(vbiDes, 0x00, sizeof(GOS_DESC_VBIDATA_S));
    pData = srcDes->Data;

    vbiDes->u16VbiDataLen= srcDes->u16Size;

    for(i = 0; i < vbiDes->u16VbiDataLen; i++)
    {
        /*data service id*/
        vbiDes->astDataServ[i].u8DataServiceId= pData[0];
        pData += 1;

        /*data service descriptor length*/
        vbiDes->astDataServ[i].u8DataServDesLen = pData[0];
        pData += 1;

        if (	vbiDes->astDataServ[i].u8DataServiceId ==0x01 || 	\
                vbiDes->astDataServ[i].u8DataServiceId ==0x02 || 	\
                vbiDes->astDataServ[i].u8DataServiceId ==0x04 || 	\
                vbiDes->astDataServ[i].u8DataServiceId ==0x05 || 	\
                vbiDes->astDataServ[i].u8DataServiceId ==0x06 || 	\
                vbiDes->astDataServ[i].u8DataServiceId ==0x07	)
        {
            for (j = 0; j < vbiDes->astDataServ[i].u8DataServDesLen ; j++)
            {
                /*reserved*/
                tempDataInt = (pData[0] & (BIT8 | BIT7)) / BIT7;

                /*field parity*/
                vbiDes->astDataServ[i].astServDes[j].u8FieldParity =  (pData[0] & BIT6) / BIT6;

                /*line offset*/
                vbiDes->astDataServ[i].astServDes[j].u8LineOffset = pData[0] & (~(BIT8 | BIT7 | BIT6));
            }
        }
        else
        {
            /*reserved*/
            memcpy(tempDataChar, pData, vbiDes->astDataServ[i].u8DataServDesLen);

        }
    }

    pData += 1;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_VBI_Teletext(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VBITELETEXT_S *vttDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;


    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == vttDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_VBI_TELETEXT != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(vttDes, 0x00, sizeof(GOS_DESC_VBITELETEXT_S));
    pData = srcDes->Data;


    vttDes->u16VbiTeleTextLen= srcDes->u16Size;

    for (i = 0; i < vttDes->u16VbiTeleTextLen; i++)
    {
        /*ISO 639 language code */
        vttDes->astTeleText[i].u32LanguageCode = (pData[0]<<16) + (pData[1]<<8) + pData[2];
        pData += 3;

        /*teletext type */
        vttDes->astTeleText[i].u8TeletextType = (pData[0] & (~ (BIT3 | BIT2 | BIT1))) / BIT4;

        /*teletext magazine numbe*/
        vttDes->astTeleText[i].u8TeletextMagNum = pData[0] & (BIT3 | BIT2 | BIT1);
        pData += 1;

        /*teletext page numbe*/
        vttDes->astTeleText[i].u8TeletextPageNum = pData[0];
        pData += 1;
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_PartialTranStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PARTIAL_TRANSTREAM_S *ptsDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 tempData = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == ptsDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_PARTIAL_TRANSPORT_STREAM != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(ptsDes, 0x00, sizeof(GOS_DESC_PARTIAL_TRANSTREAM_S));
    pData = srcDes->Data;

    /*DVB reserved future use*/
    tempData = (pData[0] & (BIT8 | BIT7)) /BIT7;

    /*peak rate*/
    ptsDes->u32PeakRate = ((pData[0] & (~(BIT8 | BIT7))) << 16) + (pData[1] << 8) + pData[2];
    pData += 3;

    /*DVB reserved future use*/
    tempData = (pData[0] & (BIT8 | BIT7)) /BIT7;

    /*minimum overall smoothing rate*/
    ptsDes->u32MiniSmoothRate = ((pData[0] & (~(BIT8 | BIT7))) << 16) + (pData[1] << 8) + pData[2];
    pData += 3;

    /*DVB reserved future use*/
    tempData = (pData[0] & (BIT8 | BIT7)) /BIT7;

    /*maximum overall smoothing butter*/
    ptsDes->u16MaxiOverSmoothButte = ((pData[0] & (~(BIT8 | BIT7))) << 8) + pData[1];

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_AC3(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AC3_S *acDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 u16tempLen= 0;
    GOSTSR_U8 tempData = 0;
    GOSTSR_U8 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == acDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_AC3  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(acDes, 0x00, sizeof(GOS_DESC_AC3_S));
    pData = srcDes->Data;

    u16tempLen = srcDes->u16Size;

    /*AC-3 type flag*/
    acDes->u8AC3TypeFlag = (pData[0] & BIT8) /BIT8;

    /*bsid flag*/
    acDes->u8AC3TypeFlag = (pData[0] & BIT7) /BIT7;

    /*mainid flag*/
    acDes->u8AC3TypeFlag = (pData[0] & BIT6) /BIT6;

    /*asvc flag*/
    acDes->u8AC3TypeFlag = (pData[0] & BIT5) /BIT5;

    /*reserved*/
    tempData = pData[0] & (BIT4 | BIT3 | BIT2 | BIT1);
    pData += 1;

    /*AC-3 type */
    if(acDes->u8AC3TypeFlag == 0x01)
    {
        acDes->u8AC3Type = pData[0];
        pData += 1;
    }

    /*bsid */
    if(acDes->u8BsidFlag == 0x01)
    {
        acDes->u8Bsid = pData[0];
        pData += 1;
    }

    /*mainid */
    if(acDes->u8MainidFlag == 0x01)
    {
        acDes->u8Mainid = pData[0];
        pData += 1;
    }

    /*asvc */
    if(acDes->u8AsvcFlag == 0x01)
    {
        acDes->u8Asvc = pData[0];
        pData += 1;
    }
    for(i = 0; i < u16tempLen; i++)
    {
        /*additional_info */
        acDes->astAddInfo[i].u32AdditionalInfo = pData[0] * u16tempLen;
    }
    //pData += u16tempLen;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_AncillaryData(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ANCILLARRYDATA_S *anDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == anDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_ANCILLARY_DATA  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(anDes, 0x00, sizeof(GOS_DESC_ANCILLARRYDATA_S));
    pData = srcDes->Data;

    /*ancillary data identifier */
    anDes->u8AncillaryDataIdentifier = pData[0];
    pData += 1;

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CellList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CELLLIST_S *clDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;
    GOSTSR_U16 j = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == clDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CELL_LIST  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(clDes, 0x00, sizeof(GOS_DESC_CELLLIST_S));
    pData = srcDes->Data;

    clDes->u16CellListLen = srcDes->u16Size;
    for(i = 0; i < clDes->u16CellListLen; i++)
    {
        /*cell id */
        clDes->astCellInfo[i].u16CellId = (pData[0] << 8) + pData[1];
        pData += 2;

        /*cell latitude */
        clDes->astCellInfo[i].u16CellLatitude = (pData[0] << 8) + pData[1];
        pData += 2;

        /*cell longtitude */
        clDes->astCellInfo[i].u16CellLongtitude = (pData[0] << 8) + pData[1];
        pData += 2;

        /*cell extend of latitude */
        clDes->astCellInfo[i].u16CellExLatitude = (pData[0] << 8) + (pData[1] & (BIT8 | BIT7 | BIT6 | BIT5) / BIT5);
        pData += 1;

        /*cell extend of longitude */
        clDes->astCellInfo[i].u16CellExLongtitude = ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 8) + pData[1];
        pData += 2;

        /*subcell info loop length  */
        clDes->astCellInfo[i].u8SubCellInfoLen = pData[0] ;
        pData += 1;

        for(j = 0; j < clDes->astCellInfo[i].u8SubCellInfoLen; j++)
        {
            /*cell id extension  */
            clDes->astCellInfo[i].astSubCellInfo[j].u8CellIdExtension = pData[0];
            pData += 1;

            /*subcell latitude  */
            clDes->astCellInfo[i].astSubCellInfo[j].u16SubCellLatitude = (pData[0] << 8) + pData[1];
            pData += 2;

            /*subcell longitude  */
            clDes->astCellInfo[i].astSubCellInfo[j].u16SubCellLongtitude = (pData[0] << 8) + pData[1];
            pData += 2;

            /*subcell extend of latitude */
            clDes->astCellInfo[i].astSubCellInfo[j].u16SubCellExLatitude = (pData[0] << 8) + (pData[1] & (BIT8 | BIT7 | BIT6 | BIT5) / BIT5);
            pData += 1;

            /*subcell extend of longitude */
            clDes->astCellInfo[i].astSubCellInfo[j].u16SubCellExLongtitude = ((pData[0] & (BIT4 | BIT3 | BIT2 | BIT1)) << 8) + pData[1];
            pData += 2;
        }
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_CellFreLink(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CELLFRELINK_S *cflDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U16 i = 0;
    GOSTSR_U16 j = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == cflDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_CELL_FREQUENCY_LINK  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(cflDes, 0x00, sizeof(GOS_DESC_CELLFRELINK_S));
    pData = srcDes->Data;

    cflDes->u16CellFreLen = srcDes->u16Size;
    for(i = 0; i < cflDes->u16CellFreLen; i++)
    {
        /*cell id */
        cflDes->astCellFreInfo[i].u16CellId = (pData[0] << 8) + pData[1];
        pData += 2;

        /*frequency */
        cflDes->astCellFreInfo[i].u32Frequency =((pData[0] & 0xf0) / BIT5)*10000000 + (pData[0] & 0x0f)*1000000 + \
                ((pData[1] & 0xf0) / BIT5)*100000 + (pData[1] & 0x0f)*10000 + \
                ((pData[2] & 0xf0) / BIT5)*1000 + (pData[2] & 0x0f)*100 + \
                ((pData[3] & 0xf0) / BIT5)*10 + (pData[3] & 0x0f);
        pData += 3;

        /*subcell info loop length  */
        cflDes->astCellFreInfo[i].u8SubCellFreInfoLen = pData[0] ;
        pData += 1;

        for(j = 0; j < cflDes->astCellFreInfo[i].u8SubCellFreInfoLen; j++)
        {
            /*cell id extension  */
            cflDes->astCellFreInfo[i].astSubCellFreInfo[j].u8CellIdExten = pData[0];
            pData += 1;

            /*transposer frequency */
            cflDes->astCellFreInfo[i].astSubCellFreInfo[j].u32TransFrequency = ((pData[0] & 0xf0) / BIT5)*10000000 + (pData[0] & 0x0f)*1000000 + \
                    ((pData[1] & 0xf0) / BIT5)*100000 + (pData[1] & 0x0f)*10000 + \
                    ((pData[2] & 0xf0) / BIT5)*1000 + (pData[2] & 0x0f)*100 + \
                    ((pData[3] & 0xf0) / BIT5)*10 + (pData[3] & 0x0f);
            pData += 3;
        }
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 GosTsr_Descriptor_AnnounceSupport(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ANNOUNCE_S *anDes)
{
    GOSTSR_U8 *pData = GOSTSR_NULL;
    GOSTSR_U8 tmpData = 0;
    GOSTSR_U16 i = 0;

    if ((GOSTSR_NULL == srcDes) || (GOSTSR_NULL == anDes))
    {
        return GOSTSR_FAILURE;
    }

    if (GOS_PSISI_DESCRIPTOR_ANNOUNCEMENT_SUPPORT  != srcDes->Tag)
    {
        return GOSTSR_FAILURE;
    }

    memset(anDes, 0x00, sizeof(GOS_DESC_ANNOUNCE_S));
    pData = srcDes->Data;

    anDes->u16AnnounceLen = srcDes->u16Size;

    /*announcement support indicator  */
    anDes->u16AnSupportIndicator = (pData[0] << 8) + pData[1];
    pData += 2;

    for(i = 0; i < anDes->u16AnnounceLen; i++)
    {
        /*announcement typer  */
        anDes->astAddInfo[i].u8AnnounceType = (pData[0] & (BIT8 | BIT7 |BIT6 | BIT5)) / BIT5;

        /*reserved future use  */
        tmpData = (pData[0] & BIT4) / BIT4;

        /*reference type   */
        anDes->astAddInfo[i].u8ReferenceType = pData[0] & (BIT3 | BIT2 | BIT1);
        pData += 1;

        if (anDes->astAddInfo[i].u8ReferenceType  == 0x01		\
                || anDes->astAddInfo[i].u8ReferenceType  ==0x02	\
                || anDes->astAddInfo[i].u8ReferenceType  ==0x03)
        {
            /*original network id   */
            anDes->astAddInfo[i].u8OriginalNetworkId = (pData[0] << 8) + pData[1];
            pData += 2;

            /*transport stream id */
            anDes->astAddInfo[i].u8TransportStreamId = (pData[0] << 8) + pData[1];
            pData += 2;

            /*service id   */
            anDes->astAddInfo[i].u8ServiceId = (pData[0] << 8) + pData[1];
            pData += 2;

            /*component tag */
            anDes->astAddInfo[i].u8ComponentTag = pData[0];
            pData += 1;
        }
    }

    pData = GOSTSR_NULL;

    return GOSTSR_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
