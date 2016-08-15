#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "GosTsr_Common.h"
#include "GosTsr_AnalysisData.h"

#ifndef __GOSTSR_DESCRIPTOR_H__
#define __GOSTSR_DESCRIPTOR_H__

#define GOS_CODE_RATIO   						   (2)
#define GOS_MAX_BOUQUET_NAME_LEN                   (32 * GOS_CODE_RATIO)
#define GOS_MAX_CA_NUM    						   (8)
#define GOS_MAX_CONTENT_NUM  					   (10)
#define GOS_MAX_COUNTRY_NUM                        (16)
#define GOS_MAX_EXTENDEDEVENT_ITEMDES_LEN          (128 * GOS_CODE_RATIO)
#define GOS_MAX_EXTENDEDEVENT_ITEMCHAR_LEN         (128 * GOS_CODE_RATIO)
#define GOS_MAX_EXTENDEDEVENT_ITEM_NUM             (20)
#define GOS_MAX_EXTENDEDEVENT_TXT_LEN              (256 * GOS_CODE_RATIO)
#define GOS_MAX_FREQUENCY_NUM                      (100)
#define GOS_MAX_LOCALTIME_ELENUM                   (32)
#define GOS_MAX_MOSAIC_ElE_CELL_NUM   			   (6*6)
#define GOS_MAX_BOUQUET_NAME_NUM                   (64)
#define GOS_MAX_COMPONENT_NUM                      (32)
#define GOS_MAX_NETWORK_NAME_NUM                   (32)
#define GOS_MAX_SERVICE_NAME_NUM                   (32)
#define GOS_MAX_NVODSERVICE_NUM                    (100)
#define GOS_MAX_PARENTALRATING_NUM                 (32)
#define GOS_MAX_EVENTNAME_LEN                      (64 * GOS_CODE_RATIO)
#define GOS_MAX_EVENTSHORTEXTDES_LEN               (256 * GOS_CODE_RATIO)
#define GOS_MAX_STUFFING_LEN					   (256 * GOS_CODE_RATIO)
#define GOS_MAX_SUBTITLE_ELENUM                    (16)
#define GOS_MAX_TELETEXT_NUM                       (16)
#define GOS_MAX_LANGUAGECODE_NUM				   (64)
#define GOS_MAX_ADDCOPYRIGHTINFO_NUM			   (256)
#define GOS_MAX_AVCVIDEO_NUM					   (256)
#define GOS_MAX_RESERVED_NUM					   (256)
#define GOS_MAX_VBIDATA_DES_NUM					   (256)
#define GOS_MAX_VBIDATA_NUM					   	   (32)
#define GOS_MAX_DSNG_NUM					   	   (32)
#define GOS_MAX_TPSTREAM_NUM					   (32)
#define GOS_MAX_VBITELETEXT_NUM					   (32)
#define GOS_MAX_LINKAGEDATA_NUM                    (256)
#define GOS_MAX_LCNSREVICE_NUM                     (256)
#define GOS_MAX_CHANNELLIST_NUM                    (256)
#define GOS_MAX_SUBTITLE_NUM                       (16)

#define GOS_MAX_MOSAIC_LOGIC_CELL_NUM   GOS_MAX_MOSAIC_ElE_CELL_NUM

/*network name descriptor struct */
typedef struct
{
    GOSTSR_U8   u8NameLen;
    GOSTSR_U8   au8NetworkName[GOS_MAX_BOUQUET_NAME_LEN];
} GOS_DESC_NETWORKNAME_S;

/*servicelist descriptor struct */
typedef struct
{
    GOSTSR_U16  U16ServiceId;
    GOSTSR_U8   U8ServiceType;
}GOS_DESC_SERVLISTSERV_S;

typedef struct
{
    GOSTSR_U16   u16NbService;
    GOS_DESC_SERVLISTSERV_S   astServiceList[GOS_MAX_SERVICE_NUM];
} GOS_DESC_SERVLIST_S;

/*cable system  descriptor struct */
typedef struct
{
    GOSTSR_U32 u32Freq;         /**<in 100 Hz*/
    GOSTSR_U32 u32Symbol_Rate;  /**<in 100 bit/s*/
    GOSTSR_U8  u8Modulation;
    GOSTSR_U8  u8FEC_Outer;
    GOSTSR_U8  u8FEC_Inner;
}GOS_DESC_CABDELIVSYS_S;

/*sat system  descriptor struct */
typedef struct
{
    GOSTSR_U32 u32Freq;           /**<in 10K Hz*/
    GOSTSR_U32 u32Symbol_Rate;    /**<in 100 bit/s*/
    GOSTSR_U16 u160rbital_Position;
    GOSTSR_U8  u8Modulation;
    GOSTSR_U8  u8West_East_Flag;
    GOSTSR_U8  u8FEC_Inner;
    GOSTSR_U8  u8Polarization;
}GOS_DESC_SATDELIVSYS_S;

/*ter system  descriptor struct */
typedef struct
{
    GOSTSR_U32  u32Centre_Freq;   /**<in 10 Hz*/
    GOSTSR_U8   u8BandWidth;
    GOSTSR_U8   u8Constellation;
    GOSTSR_U8   u8Hierarchy_Infor;
    GOSTSR_U8   u8Code_rate_HP_stream;
    GOSTSR_U8   u8Code_rate_LP_stream;
    GOSTSR_U8   u8Guard_Interval;
    GOSTSR_U8   u8Trans_Mode ;
    GOSTSR_U8   u8Other_Freq_Flag;
}GOS_DESC_TERDELIVSYS_S;

/*bouquet name descriptor struct */
typedef struct
{
    GOSTSR_U8   u8NameLen;
    GOSTSR_U8   au8BouqName[GOS_MAX_BOUQUET_NAME_LEN];
} GOS_DESC_BOUQUETNAME_S;

/*ca identifier descriptor struct */
typedef struct
{
    GOSTSR_U16   au16CA_Sys_ID[GOS_MAX_CA_NUM];
    GOSTSR_U8    u8CANum;
} GOS_DESC_CAIDF_S;

/*Component descriptor struct */
typedef struct
{
    GOSTSR_U8 u8StreamContent;
    GOSTSR_U8 u8ComponentType;
    GOSTSR_U8 u8ComponentTag;
    GOSTSR_U8 au8ISO639_Language_Code[3];
    GOSTSR_U8 acTextChar[256];
} GOS_DESC_COMPONENT_S;

/*Component Service Name descriptor struct*/
typedef struct
{
    
} GOS_DESC_COMPONENT_SERVICE_NAME_S;

/*content descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8NibbleLevel1;
    GOSTSR_U8 u8NibbleLevel2;
    GOSTSR_U8 u8UserNibble1;
    GOSTSR_U8 u8UserNibble2;
}GOS_DESC_CONTENTDETAIL_S;

typedef struct
{
    GOSTSR_U16   u16NbContent;
    GOS_DESC_CONTENTDETAIL_S   astDetailList[GOS_MAX_CONTENT_NUM];
}GOS_DESC_CONTENT_S;

/*country availability descriptor struct */
typedef struct
{
    GOSTSR_U32 u32CountryNum;
    GOSTSR_U32 au32CountryCode[GOS_MAX_COUNTRY_NUM];
    GOSTSR_U8  u8Country_Availability_Flag;
} GOS_DESC_COUNTRY_AVAILABILITY_S;

/*Data broadcast descriptor struct */
typedef struct
{
    GOSTSR_U16 u16DataBroadcastId;
    GOSTSR_U8  u8ComponentTag;
    GOSTSR_U8  u8SelectorLength;
    GOSTSR_U8  au8SelectorByte[256];
    GOSTSR_U8  au8ISO639_Language_Code[3];
    GOSTSR_U8  u8TextLength;
    GOSTSR_U8  au8TextChar[256];
} GOS_DESC_DATA_BROADCAST_S;

/*Data broadcast id descriptor struct */
typedef struct
{
    GOSTSR_U16 u16DataBroadcastId;
    GOSTSR_U8  au8IdSelectorByte[256];
} GOS_DESC_DATA_BROADCAST_ID_S;

/*extended event descriptor struct */
typedef struct
{
    GOSTSR_U32 u8ItemDescLen;
    GOSTSR_U32 u8ItemCharLen;
    GOSTSR_U8  ItemDesc[GOS_MAX_EXTENDEDEVENT_ITEMDES_LEN];
    GOSTSR_U8  ItemChar[GOS_MAX_EXTENDEDEVENT_ITEMCHAR_LEN];
} GOS_DESC_EXTEVTELE_S;

typedef struct
{
    GOSTSR_U32  u32Lag_Code;
    GOSTSR_U32  u32Txt_Length;
    GOSTSR_U8   u8Desc_Num;
    GOSTSR_U8   u8Last_Desc_Num;
    GOSTSR_U8   u8ItemNum;
    GOS_DESC_EXTEVTELE_S   astItemEle[GOS_MAX_EXTENDEDEVENT_ITEM_NUM];
    GOSTSR_U8 	acTxt_Char[GOS_MAX_EXTENDEDEVENT_TXT_LEN];
} GOS_DESC_EXTEVT_S;

/*frequency list  descriptor struct */
typedef struct
{
    GOSTSR_U32 u32FreNum;
    GOSTSR_U32 au32FrequencyValue[GOS_MAX_FREQUENCY_NUM];
    GOSTSR_U8  u8Coding_Type;
} GOS_DESC_FREQLST_S;

/*linkage descriptor struct */
typedef struct
{
    GOSTSR_U16  u16Orid;
    GOSTSR_U16  u16TsId;
    GOSTSR_U16  u16ServiceId;
    GOSTSR_U16  u16NetworkID;
    GOSTSR_U16  u16InitSerID;
    GOSTSR_U16  u16TargetEventID;
    GOSTSR_U16  u16PrivDataLen;
    GOSTSR_U8   u8LinkType;
    GOSTSR_U8   u8HandOverType;
    GOSTSR_U8   u8OriginType;
    GOSTSR_U8   u8TargetListed;
    GOSTSR_U8   u8EventSimulcast;
    GOSTSR_U8   u8PrivData[GOS_MAX_LINKAGEDATA_NUM];
}GOS_DESC_LINKAGE_S;

/*local time offset descriptor struct */
typedef struct  
{
    GOSTSR_U32   u32Country_Code;
    GOSTSR_U32   u32Time_Of_Change_UTC;
    GOSTSR_U16   u16Local_Time_Offset;
    GOSTSR_U16   u16time_Of_Change_MJD;
    GOSTSR_U16   u16Next_Time_Offset;
    GOSTSR_U8    u8Country_Region_Id;
    GOSTSR_U8    u8Local_Time_Offset_Polarity;
} GOS_DESC_LOCALTMOFFELE_S;

typedef struct
{
    GOSTSR_U32 u32TimeOffsetEleNum;
    GOS_DESC_LOCALTMOFFELE_S astTimeOffsetInfo[GOS_MAX_LOCALTIME_ELENUM];
} GOS_DESC_LOCALTMOFFSET_S;

/** mosaic descriptor struct */
typedef struct 
{
    GOSTSR_U16 u16BouquetId ;
    GOSTSR_U16 u16OrId;
    GOSTSR_U16 u16TsId;
    GOSTSR_U16 u16ServiceId;
    GOSTSR_U16 u16EventId;
    GOSTSR_U8  u8LogicalCellId;
    GOSTSR_U8  u8LogicalCellInfo;
    GOSTSR_U8  u8EleCellNum;
    GOSTSR_U8  astEleCellId[GOS_MAX_MOSAIC_ElE_CELL_NUM];
    GOSTSR_U8  u8CellLinkageInfo;
} GOS_DESC_MOSAIC_LOGICCELL_S;

typedef struct
{
    GOSTSR_U8  u8Mosaic_entry_point;
    GOSTSR_U8  u8NumberHElementaryCells;
    GOSTSR_U8  u8NumberVElementaryCells;
    GOSTSR_U8  u8NumberLogicCells;
    GOS_DESC_MOSAIC_LOGICCELL_S  astLogicCellInfo[GOS_MAX_MOSAIC_LOGIC_CELL_NUM];
} GOS_DESC_MOSAIC_S;

/*Multilingual bouquet name descriptor struct */
typedef struct
{
    GOSTSR_U8 au8ISO639_Language_Code[3];
    GOSTSR_U8 u8NameLength;
    GOSTSR_U8 au8Char[256];
} GOS_DESC_NAME_S;

typedef struct
{
    GOSTSR_U8 u8BouquetNameNum;
    GOS_DESC_NAME_S astBouquetName[GOS_MAX_BOUQUET_NAME_NUM];
} GOS_DESC_MULT_BOUQUET_NAME_S;

/*Multilingual component descriptor struct */
typedef struct
{
    GOSTSR_U8 u8ComponentTag;
    GOSTSR_U8 u8ComponentNum;
    GOS_DESC_NAME_S astComponent[GOS_MAX_COMPONENT_NUM];
} GOS_DESC_MULT_COMPONENT_S;

/*Multilingual network name descriptor struct */
typedef struct
{
    GOSTSR_U8 u8NetworkNameNum;
    GOS_DESC_NAME_S astNetworkName[GOS_MAX_NETWORK_NAME_NUM];
} GOS_DESC_MULT_NETWORK_NAME_S;

/*Multilingual service name descriptor struct */
typedef struct
{
    GOSTSR_U8 u8ServiceNameNum;
    GOSTSR_U8 u8ServiceProviderNameNum;
    GOS_DESC_NAME_S astServiceName[GOS_MAX_SERVICE_NAME_NUM];
    GOS_DESC_NAME_S astServiceProviderName[GOS_MAX_SERVICE_NAME_NUM];
} GOS_DESC_MULT_SERVICE_NAME_S;

/*nvod reference descriptor struct */
typedef struct
{
    GOSTSR_U16  u16TrasId;
    GOSTSR_U16  u16OrId;
    GOSTSR_U16  u16ServiceId;
}GOS_DESC_NVODSERV_S;

typedef struct
{
    GOSTSR_U8   u8NbService;
    GOS_DESC_NVODSERV_S   astServiceList[GOS_MAX_NVODSERVICE_NUM];
} GOS_DESC_NVODREF_S;

/*Parental Rating descriptor struct */
typedef struct
{
    GOSTSR_U32 u32Lag_Code;
    GOSTSR_U8  u8Rating;
} GOS_DESC_RATING_S;

typedef struct
{
    GOSTSR_U32 u32RatingNum;
    GOS_DESC_RATING_S astRating[GOS_MAX_PARENTALRATING_NUM];
} GOS_DESC_PARENTRATING_S;

/*Private data specifier descriptor struct */
typedef struct
{
    GOSTSR_U32 u32PrivateData;
} GOS_DESC_PRIVATE_DATA_SPECIFIER_S;

/*Short smoothing buffer descriptor struct */
typedef struct
{
    GOSTSR_U16 u16DVBReservedLen;
    GOSTSR_U8  u8SbSize;
    GOSTSR_U8  u8SbLeakRate;
    GOSTSR_U8  u8DVBReserved[256];
} GOS_DESC_SHORT_SMOOTHING_BUFFER_S;

/*smoothing buffer descriptor*/
typedef struct
{
    GOSTSR_U32 u32SbLeakRate;
    GOSTSR_U32 u32SbSize;
}GOS_DESC_SMOOTHING_BUFFER_S;

/*Service descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8ServiceType;
    GOSTSR_U8 u8ServiceProviderNameLen;
    GOSTSR_U8 u8ServiceProvideName[256];
    GOSTSR_U8 u8ServiceNameLen;
    GOSTSR_U8 u8ServiceName[256];
}GOS_DESC_SERVICE_S;

/*Service move descriptor struct */
typedef struct
{
    GOSTSR_U16 u16NewONID;
    GOSTSR_U16 u16NewTSID;
    GOSTSR_U16 u16NewServiceID;
} GOS_DESC_SERVICE_MOVE_S;

/*short event descriptor struct */
typedef struct 
{
    GOSTSR_U32  u32LangCode;
    GOSTSR_U16  u16EvtNameLen;
    GOSTSR_U16  u16ShortEvtDesLen;
    GOSTSR_U8  	acEvtName[GOS_MAX_EVENTNAME_LEN];
    GOSTSR_U8   acShortEvtDes[GOS_MAX_EVENTSHORTEXTDES_LEN];
} GOS_DESC_SHORTEVT_S;

/*Stream identifier descriptor struct */
typedef struct
{
    GOSTSR_U8   u8Component_tag;
} GOS_DESC_STREAM_IDENTIFIER_S;

/*Stuffing descriptor struct */
typedef struct
{
    GOSTSR_U16 u16stuffingLen;
    GOSTSR_U8  u8stuffingByte[GOS_MAX_STUFFING_LEN];
} GOS_DESC_STUFFING_S;

/*subtitling  descriptor struct */
typedef struct
{
    GOSTSR_U32   u32Country_Code;
    GOSTSR_U16   u16Composition_Page_Id;
    GOSTSR_U16   u16Ancillary_Page_Id;
    GOSTSR_U8    u8Substitling_Type;
} GOS_DESC_SUBTELE_S;

typedef struct
{
    GOSTSR_U32 u32SubtitleEleNum;
    GOS_DESC_SUBTELE_S astSubtitleInfo[GOS_MAX_SUBTITLE_ELENUM];
} GOS_DESC_SUBT_S;

/*Telephone descriptor struct */
typedef struct
{
    GOSTSR_U8 u8ForeginAvailability;
    GOSTSR_U8 u8ConnectType;
    GOSTSR_U8 u8CountryPrefixLen;
    GOSTSR_U8 u8CountryPrefix[4];
    GOSTSR_U8 u8InternationalAreaCodeLen;
    GOSTSR_U8 u8InternationaAreaCode[8];
    GOSTSR_U8 u8OperatorCodeLen;
    GOSTSR_U8 u8OperatorCode[4];
    GOSTSR_U8 u8NationAreaCodeLen;
    GOSTSR_U8 u8NationAreaCode[8];
    GOSTSR_U8 u8CoreNumberCodeLen;
    GOSTSR_U8 u8CoreNumberCode[16];
} GOS_DESC_TELEPHONE_S;

/*teletext descriptor struct */
typedef struct
{
    GOSTSR_U32 u32Lag_Code;
    GOSTSR_U8  u8Teletext_Type;
    GOSTSR_U8  u8Teletext_Magazine_Number;
    GOSTSR_U8  u8Teletext_Page_Number;
} GOS_DESC_TELETEXTELE_S;

typedef struct
{
    GOSTSR_U32 u32TeletextNum;
    GOS_DESC_TELETEXTELE_S astTeletextInfo[GOS_MAX_TELETEXT_NUM];
} GOS_DESC_TELETEXT_S;

/*time shifted event descriptor struct */
typedef struct 
{
    GOSTSR_U16    u16Ref_Service_Id;
    GOSTSR_U16    u16Ref_Evt_Id;
} GOS_DESC_TMSHIFTEVT_S;

/*time shift servie descriptor struct */
typedef struct
{
    GOSTSR_U16   u16RefServiceId;
} GOS_DESC_TIMESHIFTSERV_S;

/*video stream descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8MultipleFrameRateFlag;
    GOSTSR_U8 u8FrameRateCode;
    GOSTSR_U8 u8MPEG2Flag;
    GOSTSR_U8 u8ConstrainParameterFlag;
    GOSTSR_U8 u8StillPictureFlag;
    GOSTSR_U8 u8ProfileAndLevelIndication;
    GOSTSR_U8 u8ChromaFormat;
    GOSTSR_U8 u8FrameRateExtensionFlag;
}GOS_DESC_VIDEOSTREAM_S;

/*audio stream descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8FreeFormatFlag;
    GOSTSR_U8 u8ID;
    GOSTSR_U8 u8Layer;
}GOS_DESC_AUDIOSTRAM_S;

/*hierarchy descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8HierarchyType;
    GOSTSR_U8 u8HierarchyLayerIndex;
    GOSTSR_U8 u8HierarchyEmbeddedLayer;
    GOSTSR_U8 u8HierarchyPriority;
}GOS_DESC_HIERARCHY_S;

/*registration descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32FormatIdentifier;
    GOSTSR_U16 u16AddIdentificationLen;
    GOSTSR_U8  u8AddIdentificationInfo[256];
}GOS_DESC_REGISTRATION_S;

/*data stream alignment descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8AlignmentType;
}GOS_DESC_DATASTRAMALIGNMENT_S;

/*target background grid descriptor struct*/
typedef struct
{
    GOSTSR_U16 u16HorizontalSize;
    GOSTSR_U16 u16VerticalSize;
    GOSTSR_U8  u8PelaspectRatio;
}GOS_DESC_TARBACGRID_S;

/*video window descriptor struct*/
typedef struct
{
    GOSTSR_U16 u16HorizontalZOffset;
    GOSTSR_U16 u16VerticalOffset;
    GOSTSR_U8  u8WindowPriority;
}GOS_DESC_VIDEOWINDOW_S;

/*ca descriptor struct*/
typedef struct
{
    GOSTSR_U16  u16CaSystemId;
    GOSTSR_U16  u16CaPId;
    GOSTSR_U8   u8PivateDataLen;
    GOSTSR_U8   *pu8Data;
} GOS_DESC_CA_S;

/*ca System descriptor struct*/
typedef struct
{
    
} GOS_DESC_CA_SYSTEM_S;

/*ISO 639 language descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32LanguageCode[GOS_MAX_LANGUAGECODE_NUM];
    GOSTSR_U16 u16LanguageCodeLen;
    GOSTSR_U8  u8AudioType;
}GOS_DESC_ISOLANGUAGECODE_S;

/*system clock descriptor struct*/
typedef struct
{
    GOSTSR_U8 u8ExternClockRefIndicator;
    GOSTSR_U8 u8ClockAccuracyInteger;
    GOSTSR_U8 u8ClockAccuracyExponent;
}GOS_DESC_SYSTEMCLOCK_S;

/*multiplex buffer utilization descriptor struct*/
typedef struct
{
    GOSTSR_U16 u16MultiplexDelayVariation;
    GOSTSR_U8  u8MdvValidFlag;
    GOSTSR_U8  u8MultiplexStrategy;
}GOS_DESC_MULBUFFERUTILIZATION_S;

/*copyright descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32CopyrightIdentifier;
    GOSTSR_U16 u32AddCopyrightInfoLen;
    GOSTSR_U8  u8AddCopyrightInfo[GOS_MAX_ADDCOPYRIGHTINFO_NUM];
}GOS_DESC_COPYRIGHT_S;

/*maximum bitrate descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32MaxBitrate;
}GOS_DESC_MAXBITRATE_S;

/*private data indicator descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32PrivateDataIndicator;
}GOS_DESC_PRIVATEDATAINDICATOR_S;

/*AVC video descriptor struct*/
typedef struct  
{
    GOSTSR_U16 u16AVCVideoLen;
    GOSTSR_U8  u8AVCVideoData[GOS_MAX_AVCVIDEO_NUM];
}GOS_DESC_AVCVIDEO_S;

/*LCN 0x83 descriptor struct*/
typedef struct
{
    GOSTSR_U16 u16ServiceId;
    GOSTSR_U16 u16LCN;
    GOSTSR_U8  u8VisFlag;
}GOS_DESC_LCNINFO_S;

typedef struct  
{
    GOSTSR_U16 u16LCNServiceNum;
    GOS_DESC_LCNINFO_S  LCNServiceInfo[GOS_MAX_LCNSREVICE_NUM];
}GOS_DESC_LCN83_S;

/*LCN 0x87 descriptor struct*/
typedef struct
{
    GOSTSR_U32 u32Country_Code;
    GOSTSR_U16 u16ChannelListNameLen;
    GOSTSR_U16 u16LCNServiceNum;
    GOSTSR_U8  u8ChannelListId;
    GOSTSR_U8  u16ChannelListName[256];
    GOS_DESC_LCNINFO_S  LCNServiceInfo[GOS_MAX_LCNSREVICE_NUM];
}GOS_DESC_CHANNELLISTINFO_S;

typedef struct
{
    GOSTSR_U16 u16ChannelListNum;
    GOS_DESC_CHANNELLISTINFO_S channelListInfo[GOS_MAX_CHANNELLIST_NUM];
}GOS_DESC_LCN87_S;

/*Transport stream descriptor struct*/
typedef struct  
{
    GOSTSR_U32 u32ProgrammeIdentification;
}GOS_DESC_PDC_S;

/*Transport stream descriptor struct*/
typedef struct  
{

}GOS_DESC_TRANSPORT_STREAM_S;

/*STD Descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8ReakValidFlag;
}GOS_DESC_STD_S;

/*IBP Descriptor struct*/
typedef struct  
{
    GOSTSR_U16 u16MaxGopLength;
    GOSTSR_U8 u8ClosedGopFlag;
    GOSTSR_U8 u8IdenticalGopFlag;
}GOS_DESC_IBP_S;

/*DSNG Descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8ByteData;
}GOS_DESC_DGBYTE_S;

typedef struct  
{
    GOSTSR_U16 u16ByteLen;
    GOS_DESC_DGBYTE_S astByte[GOS_MAX_DSNG_NUM];
}GOS_DESC_DSNG_S;

/*Transport Stream Descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8ByteData;
}GOS_DESC_TSBYTE_S;

typedef struct  
{
    GOSTSR_U16 u16ByteLen;
    GOS_DESC_TSBYTE_S astByte[GOS_MAX_TPSTREAM_NUM];
}GOS_DESC_TPSTREAM_S;

/*VBI Data Descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8FieldParity;
    GOSTSR_U8 u8LineOffset;
}GOS_DESC_SERVDES_S;

typedef struct  
{
    GOSTSR_U8 u8DataServiceId;
    GOSTSR_U8 u8DataServDesLen;
    GOS_DESC_SERVDES_S astServDes[GOS_MAX_VBIDATA_DES_NUM];
}GOS_DESC_DATASERV_S;

typedef struct  
{
    GOSTSR_U16 u16VbiDataLen;
    GOS_DESC_DATASERV_S astDataServ[GOS_MAX_VBIDATA_NUM];
}GOS_DESC_VBIDATA_S;

/*VBI Teletext Descriptor struct*/
typedef struct  
{
    GOSTSR_U32 u32LanguageCode;
    GOSTSR_U8 u8TeletextType;
    GOSTSR_U8 u8TeletextMagNum;
    GOSTSR_U8 u8TeletextPageNum;
}GOS_DESC_TELETEXTDES_S;

typedef struct  
{
    GOSTSR_U16 u16VbiTeleTextLen;
    GOS_DESC_TELETEXTDES_S astTeleText[GOS_MAX_VBITELETEXT_NUM];
}GOS_DESC_VBITELETEXT_S;

/*partial transport stream descriptor struct*/
typedef struct  
{
    GOSTSR_U32 u32PeakRate;
    GOSTSR_U32 u32MiniSmoothRate;
    GOSTSR_U16 u16MaxiOverSmoothButte;
}GOS_DESC_PARTIAL_TRANSTREAM_S;

/*AC-3 descriptor struct*/
typedef struct  
{
    GOSTSR_U32 u32AdditionalInfo;
}GOS_DESC_ADDINFO_S;

typedef struct  
{
    GOSTSR_U8 u8AC3TypeFlag;
    GOSTSR_U8 u8BsidFlag;
    GOSTSR_U8 u8MainidFlag;
    GOSTSR_U8 u8AsvcFlag;
    GOSTSR_U8 u8AC3Type;
    GOSTSR_U8 u8Bsid;
    GOSTSR_U8 u8Mainid;
    GOSTSR_U8 u8Asvc;
    GOS_DESC_ADDINFO_S astAddInfo[32];
}GOS_DESC_AC3_S;

/*ancillary data descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8AncillaryDataIdentifier;

}GOS_DESC_ANCILLARRYDATA_S;

/*cell list descriptor struct*/
typedef struct  
{
    GOSTSR_U16 u16SubCellLatitude ;
    GOSTSR_U16 u16SubCellLongtitude;
    GOSTSR_U16 u16SubCellExLatitude;
    GOSTSR_U16 u16SubCellExLongtitude;
    GOSTSR_U8  u8CellIdExtension ;
}GOS_DESC_SUBCELLINFO_S;

typedef struct  
{
    GOSTSR_U16 u16CellId;
    GOSTSR_U16 u16CellLatitude ;
    GOSTSR_U16 u16CellLongtitude;
    GOSTSR_U16 u16CellExLatitude;
    GOSTSR_U16 u16CellExLongtitude;
    GOSTSR_U8 u8SubCellInfoLen;
    GOS_DESC_SUBCELLINFO_S astSubCellInfo[256];
}GOS_DESC_CELLINFO_S;

typedef struct  
{
    GOSTSR_U16 u16CellListLen;
    GOS_DESC_CELLINFO_S astCellInfo[256];
}GOS_DESC_CELLLIST_S;

/*cell frequency linkdescriptor struct*/
typedef struct  
{
    GOSTSR_U32 u32TransFrequency;
    GOSTSR_U8  u8CellIdExten ;
}GOS_DESC_SUBCELLFREINFO_S;

typedef struct  
{
    GOSTSR_U32 u32Frequency ;
    GOSTSR_U16 u16CellId;
    GOSTSR_U8 u8SubCellFreInfoLen;
    GOS_DESC_SUBCELLFREINFO_S astSubCellFreInfo[256];
}GOS_DESC_CELLFREINFO_S;

typedef struct  
{
    GOSTSR_U16 u16CellFreLen;
    GOS_DESC_CELLFREINFO_S astCellFreInfo[256];
}GOS_DESC_CELLFRELINK_S;

/*announcement support descriptor struct*/
typedef struct  
{
    GOSTSR_U8 u8AnnounceType;
    GOSTSR_U8 u8ReferenceType;
    GOSTSR_U8 u8OriginalNetworkId;
    GOSTSR_U8 u8TransportStreamId ;
    GOSTSR_U8 u8ServiceId;
    GOSTSR_U8 u8ComponentTag ;
}GOS_DESC_ANNOUNCEINFO_S;

typedef struct  
{
    GOSTSR_U16 u16AnnounceLen;
    GOSTSR_U16 u16AnSupportIndicator;
    GOS_DESC_ANNOUNCEINFO_S astAddInfo[32];
}GOS_DESC_ANNOUNCE_S;

/*3D service descriptor struct*/
typedef enum
{
    DESC_3DSERVICE_STREAMCONTENT_AVS = 0x01,
    DESC_3DSERVICE_STREAMCONTENT_H264 = 0x05
}GOS_DESC_3DSERVICE_STREAMCONTENT_E;

typedef enum 
{
    DESC_3DSERVICE_CONTENTTYPE_AVS_25_LR = 0xD0,
    DESC_3DSERVICE_CONTENTTYPE_AVS_25_BT = 0xD1,
    DESC_3DSERVICE_CONTENTTYPE_AVS_30_LR = 0xD2,
    DESC_3DSERVICE_CONTENTTYPE_AVS_30_BT = 0xD3,

    DESC_3DSERVICE_CONTENTTYPE_H264_25_LR = 0x80,
    DESC_3DSERVICE_CONTENTTYPE_H264_25_BT = 0x81,
    DESC_3DSERVICE_CONTENTTYPE_H264_30_LR = 0x82,
    DESC_3DSERVICE_CONTENTTYPE_H264_30_BT = 0x83
}GOS_DESC_3DSERVICE_CONTENTTYPE_E;

typedef struct
{
    GOS_DESC_3DSERVICE_STREAMCONTENT_E   enTvStreamContent_3D;
    GOS_DESC_3DSERVICE_CONTENTTYPE_E   enTvComponentType_3D;
} GOS_DESC_3DSERVICE_PRIVATE_S;

/*extern function*/
extern GOSTSR_S32 GosTsr_Descriptor_AC3(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AC3_S *acDes);
extern GOSTSR_S32 GosTsr_Descriptor_AudioStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AUDIOSTRAM_S *asDes);
extern GOSTSR_S32 GosTsr_Descriptor_AVCVideo(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_AVCVIDEO_S *avDes);
extern GOSTSR_S32 GosTsr_Descriptor_AncillaryData(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ANCILLARRYDATA_S *anDes);
extern GOSTSR_S32 GosTsr_Descriptor_AnnounceSupport(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ANNOUNCE_S *anDes);
extern GOSTSR_S32 GosTsr_Descriptor_Bouquetname(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_BOUQUETNAME_S *bndes);
extern GOSTSR_S32 GosTsr_Descriptor_CableSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CABDELIVSYS_S *csDes);
extern GOSTSR_S32 GosTsr_Descriptor_CaIdentify(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CAIDF_S *ciDes);
extern GOSTSR_S32 GosTsr_Descriptor_CA(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CA_S *caDes);
extern GOSTSR_S32 GosTsr_Descriptor_CA_System(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CA_SYSTEM_S *caDes);
extern GOSTSR_S32 GosTsr_Descriptor_CellFreLink(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CELLFRELINK_S *cflDes);
extern GOSTSR_S32 GosTsr_Descriptor_CellList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CELLLIST_S *clDes);
extern GOSTSR_S32 GosTsr_Descriptor_CopyRight(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COPYRIGHT_S *cDes);
extern GOSTSR_S32 GosTsr_Descriptor_Component(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COMPONENT_S *cDes);
extern GOSTSR_S32 GosTsr_Descriptor_Component_Service_Name(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COMPONENT_SERVICE_NAME_S *cDes);
extern GOSTSR_S32 GosTsr_Descriptor_Content(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_CONTENT_S *cDes);
extern GOSTSR_S32 GosTsr_Descriptor_CountryAvailability(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_COUNTRY_AVAILABILITY_S *caDes);
extern GOSTSR_S32 GosTsr_Descriptor_DataStreamAlignment(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATASTRAMALIGNMENT_S *dsaDes);
extern GOSTSR_S32 GosTsr_Descriptor_DataBroadcast(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATA_BROADCAST_S *dbDes);
extern GOSTSR_S32 GosTsr_Descriptor_DataBroadcastID(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DATA_BROADCAST_ID_S *dIDDes);
extern GOSTSR_S32 GosTsr_Descriptor_DSNG(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_DSNG_S *dgDes);
extern GOSTSR_S32 GosTsr_Descriptor_ExtendEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_EXTEVT_S *eeDes);
extern GOSTSR_S32 GosTsr_Descriptor_FrequencyList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_FREQLST_S *flDes);
extern GOSTSR_S32 GosTsr_Descriptor_Hierarchy(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_HIERARCHY_S *hDes);
extern GOSTSR_S32 GosTsr_Descriptor_IBP(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_IBP_S *ibpDes);
extern GOSTSR_S32 GosTsr_Descriptor_ISO639LanguageCode(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_ISOLANGUAGECODE_S *lcDes);
extern GOSTSR_S32 GosTsr_Descriptor_Linkage(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LINKAGE_S *lDes);
extern GOSTSR_S32 GosTsr_Descriptor_LocalTimeOffset(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LOCALTMOFFSET_S *ltoDes);
extern GOSTSR_S32 GosTsr_Descriptor_LCN_83(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LCN83_S *lcn83Des);
extern GOSTSR_S32 GosTsr_Descriptor_LCN_87(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_LCN87_S *lcn87Des);
extern GOSTSR_S32 GosTsr_Descriptor_Mosaic(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MOSAIC_S *mDes);
extern GOSTSR_S32 GosTsr_Descriptor_MaximumBitrate(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MAXBITRATE_S *mbDes);
extern GOSTSR_S32 GosTsr_Descriptor_MultilingualBouquetName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_BOUQUET_NAME_S *mbnDes);
extern GOSTSR_S32 GosTsr_Descriptor_MultilingualComponent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_COMPONENT_S *mcDes);
extern GOSTSR_S32 GosTsr_Descriptor_MultilingualNetworkName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_NETWORK_NAME_S *mnnDes);
extern GOSTSR_S32 GosTsr_Descriptor_MultilingualServiceName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULT_SERVICE_NAME_S *msnDes);
extern GOSTSR_S32 GosTsr_Descriptor_MultiplexBufferUtilization(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_MULBUFFERUTILIZATION_S *mbuDes);
extern GOSTSR_S32 GosTsr_Descriptor_NVODReference(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_NVODREF_S *nrDes);
extern GOSTSR_S32 GosTsr_Descriptor_NetworkName(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_NETWORKNAME_S *netnDes);
extern GOSTSR_S32 GosTsr_Descriptor_ParentRating(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PARENTRATING_S *prDes);
extern GOSTSR_S32 GosTsr_Descriptor_PartialTranStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PARTIAL_TRANSTREAM_S *ptsDes);
extern GOSTSR_S32 GosTsr_Descriptor_PDC(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PDC_S *pdcDes);
extern GOSTSR_S32 GosTsr_Descriptor_PrivateDataSpecifier(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_PRIVATE_DATA_SPECIFIER_S *pdsDes);
extern GOSTSR_S32 GosTsr_Descriptor_PrivateDataIndicator(GOS_PSISI_Descriptor_S *srcDes,  GOS_DESC_PRIVATEDATAINDICATOR_S *pdiDes);
extern GOSTSR_S32 GosTsr_Descriptor_Registration(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_REGISTRATION_S *rDes);
extern GOSTSR_S32 GosTsr_Descriptor_ShortSmoothingBuffer(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SHORT_SMOOTHING_BUFFER_S *ssbDes);
extern GOSTSR_S32 GosTsr_Descriptor_SmoothingBuffer(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SMOOTHING_BUFFER_S *sbDes);
extern GOSTSR_S32 GosTsr_Descriptor_Service(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVICE_S *sDes);
extern GOSTSR_S32 GosTsr_Descriptor_ServiceList(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVLIST_S *serLDes);
extern GOSTSR_S32 GosTsr_Descriptor_ServiceMove(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SERVICE_MOVE_S *smDes);
extern GOSTSR_S32 GosTsr_Descriptor_SatelliteSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SATDELIVSYS_S *ssdes);
extern GOSTSR_S32 GosTsr_Descriptor_ShortEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SHORTEVT_S *seDes);
extern GOSTSR_S32 GosTsr_Descriptor_StreamIdentifier(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STREAM_IDENTIFIER_S *siDes);
extern GOSTSR_S32 GosTsr_Descriptor_Stuffing(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STUFFING_S *sDes);
extern GOSTSR_S32 GosTsr_Descriptor_STD(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_STD_S *stdDes);
extern GOSTSR_S32 GosTsr_Descriptor_Subtitling(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SUBT_S *sDes);
extern GOSTSR_S32 GosTsr_Descriptor_SystemClock(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_SYSTEMCLOCK_S *scDes);
extern GOSTSR_S32 GosTsr_Descriptor_TargetBackgroundGrid(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TARBACGRID_S *tbgDes);
extern GOSTSR_S32 GosTsr_Descriptor_TerrestrialSystem(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TERDELIVSYS_S *tsdes);
extern GOSTSR_S32 GosTsr_Descriptor_Telephone(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TELEPHONE_S *tDes);
extern GOSTSR_S32 GosTsr_Descriptor_Teletex(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TELETEXT_S *tDes);
extern GOSTSR_S32 GosTsr_Descriptor_TimeShiftedEvent(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TMSHIFTEVT_S *tseDes);
extern GOSTSR_S32 GosTsr_Descriptor_TimeShiftedService(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TIMESHIFTSERV_S *tssDes);
extern GOSTSR_S32 GosTsr_Descriptor_TransportStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_TPSTREAM_S *tpsDes);
extern GOSTSR_S32 GosTsr_Descriptor_VBI_Data(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VBIDATA_S *vbiDes);
extern GOSTSR_S32 GosTsr_Descriptor_VBI_Teletext(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VBITELETEXT_S *vttDes);
extern GOSTSR_S32 GosTsr_Descriptor_VideoStream(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VIDEOSTREAM_S *vsDes);
extern GOSTSR_S32 GosTsr_Descriptor_VideoWindow(GOS_PSISI_Descriptor_S *srcDes, GOS_DESC_VIDEOWINDOW_S *vwDes);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
