#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "GosTsr_Common.h"
#include "GosTsr_Crc.h"

#ifndef __GOSTSR_ANALYSISDATA_H__
#define __GOSTSR_ANALYSISDATA_H__

/*定义TS流相关信息*/
#define SYNC_BYTE  0x47  			/*起始字节*/
#define TS_LENGTH_188 188   		/*TS流分组包的大小(188)*/
#define TS_LENGTH_204 204			/*TS流分组包的大小(204)*/
#define TS_HEAD_LENGTH 4 			/*TS流分组包头部大小*/
#define TS_MAX_SECTIONLEN (4*1024)  /*最大的section长度*/
#define TS_MAX_SECTIONNUM (256)     /*最大的section个数*/
#define TS_MAX_SERVICEIDNUM (500)   /*最大的节目个数*/

/*PSI PID*/
#define PAT_PID	0x0000
#define PMT_PID_MIN 0x0020
#define PMT_PID_MAX 0x1FFE
#define CAT_PID 0x0001
#define NIT_PID 0x0010

/*SI PID*/
#define BAT_SDT_PID 0x0011
#define EIT_PID 0x0012
#define RST_PID 0x0013
#define TDT_TOT_PID 0x0014
#define SI_PID_MIN 0x0010
#define SI_PID_MAX 0x0014

/*PSI Table ID*/
#define PAT_TABLE_ID 0x00
#define PMT_TABLE_ID 0x02
#define CAT_TABLE_ID 0x01
#define NIT_TABLE_ID_ACTUAL 0x40
#define NIT_TABLE_ID_OTHER  0x41

/*SI Table ID*/
#define BAT_TABLE_ID 0x4A
#define SDT_TABLE_ID_ACTUAL 0x42
#define SDT_TABLE_ID_OTEHR  0x46
#define EIT_TABLE_ID_ACTUAL 0x4E
#define EIT_TABLE_ID_ACTUAL_SHEDULE 0x50
#define EIT_TABLE_ID_OTHER  0x4F
#define EIT_TABLE_ID_OTHER_SHEDULE  0x60
#define RST_TABLE_ID 0x71
#define TDT_TABLE_ID 0x70
#define TOT_TABLE_ID 0x73
#define SI_TABLE_ID  0x72
#define BULT_TABLE_ID 0xFF

/*define table number*/
#define TABLE_NUMBER   15

/*invaild version num*/
#define INVALID_VERSION_NUM 0xff

#define GOS_MAX_SERVICE_NUM  500     /*the max programs number at one section*/
#define GOS_MAX_CAT_DESCRIPTOR 256   /*the max descriptor number at one CAT section*/
#define GOS_MAX_NIT_DESCIP_LOOP (256)/*the max one level descriptor number at one NIT section*/
#define GOS_MAX_NIT_ELEMENT (100)    /*the max frequency number at one NIT section*/ 
#define GOS_MAX_NIT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one frequency element*/
#define GOS_MAX_PMT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one PMT element*/
#define GOS_MAX_PMT_DESCIP_LOOP1 (256)/*the max one level descriptor number at one PMT section*/
#define GOS_MAX_PMT_ELEMENT (20)	  /*the max frequency number at one PMT section*/
#define GOS_MAX_SDT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one SDT element*/
#define GOS_MAX_SDT_ELEMENT (100)	  /*the max element number at one SDT section*/
#define GOS_MAX_BAT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one BAT element*/
#define GOS_MAX_BAT_DESCIP_LOOP1 (256)/*the max one level descriptor number at one BAT section*/
#define GOS_MAX_BAT_ELEMENT (100)	  /*the max element number at one BAT section*/
#define GOS_MAX_EIT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one EIT element*/
#define GOS_MAX_EIT_ELEMENT (256)	  /*the max element number at one EIT section*/
#define GOS_MAX_TOT_DESCRIPTOR (256)  /*the max descriptor number at one TOT section*/
#define GOS_MAX_RTT_ELEMENT (100)     /*the max element number at one RST section*/
#define GOS_MAX_SIT_ELEMENT_DESCRIPTOR (256) /*the max descriptor number at one SIT element*/
#define GOS_MAX_SIT_DESCIP_LOOP1 (256)/*the max one level descriptor number at one SIT section*/
#define GOS_MAX_SIT_ELEMENT (100)	  /*the max element number at one SIT section*/

/** Descriptor tag value */
typedef enum
{
    GOS_PSISI_DESCRIPTOR_VIDEO_STREAM                = 0x02,
    GOS_PSISI_DESCRIPTOR_AUDIO_STREAM                = 0x03,
    GOS_PSISI_DESCRIPTOR_HIERARCHY                   = 0x04,
    GOS_PSISI_DESCRIPTOR_REGISTRATION                = 0x05,
    GOS_PSISI_DESCRIPTOR_DATA_STREAM_ALIGNMENT       = 0x06,
    GOS_PSISI_DESCRIPTOR_TARGET_BACKGROUND_GRID      = 0x07,
    GOS_PSISI_DESCRIPTOR_VIDEO_WINDOW                = 0x08,
    GOS_PSISI_DESCRIPTOR_CA                          = 0x09,
    GOS_PSISI_DESCRIPTOR_LANGUAGE_NAME               = 0x0A,
    GOS_PSISI_DESCRIPTOR_SYSTEM_CLOCK                = 0x0B,
    GOS_PSISI_DESCRIPTOR_MULTIPLEX_BUFFER            = 0x0C,
    GOS_PSISI_DESCRIPTOR_COPYRIGHT                   = 0x0D,
    GOS_PSISI_DESCRIPTOR_MAX_BITRATE                 = 0x0E,
    GOS_PSISI_DESCRIPTOR_PRIVATE_DATA_INDICATOR      = 0x0F,
    GOS_PSISI_DESCRIPTOR_SMOOTHING_BUFFER            = 0x10,
    GOS_PSISI_DESCRIPTOR_STD                         = 0x11,
    GOS_PSISI_DESCRIPTOR_IBP                         = 0x12,
    GOS_PSISI_DESCRIPTOR_CAROUSEL_IDENTIFIER         = 0x13,
    GOS_PSISI_DESCRIPTOR_ASSOCIATION_TAG             = 0x14,
    GOS_PSISI_DESCRIPTOR_DEFERRED_ASSOCIATION_TAG    = 0x15,
    GOS_PSISI_DESCRIPTOR_RESERVED_16                 = 0x16,
    GOS_PSISI_DESCRIPTOR_NPT_REFERENCE               = 0x17,
    GOS_PSISI_DESCRIPTOR_NPT_ENDPOINT                = 0x18,
    GOS_PSISI_DESCRIPTOR_STREAM_MODE                 = 0x19,
    GOS_PSISI_DESCRIPTOR_STREAM_EVENT                = 0x1A,
    GOS_PSISI_DESCRIPTOR_MPEG4_VIDEO                 = 0x1B,
    GOS_PSISI_DESCRIPTOR_MPEG4_AUDIO                 = 0x1C,
    GOS_PSISI_DESCRIPTOR_MPEG4_IOD                   = 0x1D,
    GOS_PSISI_DESCRIPTOR_MPEG4_SL                    = 0x1E,
    GOS_PSISI_DESCRIPTOR_MPEG4_FMC                   = 0x1F,
    GOS_PSISI_DESCRIPTOR_MPEG4_MUX_CODE              = 0x21,
    GOS_PSISI_DESCRIPTOR_MPEG4_FMX_BUFFER_SIZE       = 0x22,
    GOS_PSISI_DESCRIPTOR_MPEG4_MULTIPLEX_BUFFER      = 0x23,
    GOS_PSISI_DESCRIPTOR_MPEG4_CONTENT_LABELING      = 0x24,
    GOS_PSISI_DESCRIPTOR_TVA_METADATA_POINTER        = 0x25,
    GOS_PSISI_DESCRIPTOR_TVA_METADATA                = 0x26,
    GOS_PSISI_DESCRIPTOR_TVA_METADATA_STD            = 0x27,
    GOS_PSISI_DESCRIPTOR_AVC_VIDEO                   = 0x28,
    GOS_PSISI_DESCRIPTOR_AVC_IPMP                    = 0x29,
    GOS_PSISI_DESCRIPTOR_AVC_TIMING_AND_HRD          = 0x2A,
    GOS_PSISI_DESCRIPTOR_NETWORK_NAME                = 0x40,
    GOS_PSISI_DESCRIPTOR_SERVICE_LIST                = 0x41,
    GOS_PSISI_DESCRIPTOR_STUFFING                    = 0x42,
    GOS_PSISI_DESCRIPTOR_SATELLITE_DELIVERY_SYSTEM   = 0x43,
    GOS_PSISI_DESCRIPTOR_CABLE_DELIVERY_SYSTEM       = 0x44,
    GOS_PSISI_DESCRIPTOR_VBI_DATA                    = 0x45,
    GOS_PSISI_DESCRIPTOR_VBI_TELETEXT                = 0x46,
    GOS_PSISI_DESCRIPTOR_BOUQUET_NAME                = 0x47,
    GOS_PSISI_DESCRIPTOR_SERVICE                     = 0x48,
    GOS_PSISI_DESCRIPTOR_COUNTRY_AVAIBILITY          = 0x49,
    GOS_PSISI_DESCRIPTOR_LINKAGE                     = 0x4A,
    GOS_PSISI_DESCRIPTOR_NVOD_REFERENCE              = 0x4B,
    GOS_PSISI_DESCRIPTOR_TIME_SHIFTEDSERVICE         = 0x4C,
    GOS_PSISI_DESCRIPTOR_SHORT_EVENT                 = 0x4D,
    GOS_PSISI_DESCRIPTOR_EXTENDED_EVENT              = 0x4E,
    GOS_PSISI_DESCRIPTOR_TIME_SHIFTED_EVENT          = 0x4F,
    GOS_PSISI_DESCRIPTOR_COMPONENT                   = 0x50,
    GOS_PSISI_DESCRIPTOR_MOSAIC                      = 0x51,
    GOS_PSISI_DESCRIPTOR_STREAM_IDENTIFIER           = 0x52,
    GOS_PSISI_DESCRIPTOR_CA_IDENTIFIER               = 0x53,
    GOS_PSISI_DESCRIPTOR_CONTENT                     = 0x54,
    GOS_PSISI_DESCRIPTOR_PARENTAL_RATING             = 0x55,
    GOS_PSISI_DESCRIPTOR_TELETEXT                    = 0x56,
    GOS_PSISI_DESCRIPTOR_TELEPHONE                   = 0x57,
    GOS_PSISI_DESCRIPTOR_LOCAL_TIME_OFFSET           = 0x58,
    GOS_PSISI_DESCRIPTOR_SUBTITLING                  = 0x59,
    GOS_PSISI_DESCRIPTOR_TERRESTRIAL_DELIVERY_SYSTEM = 0x5A,
    GOS_PSISI_DESCRIPTOR_MULTILINGUAL_NETWORK_NAME   = 0x5B,
    GOS_PSISI_DESCRIPTOR_MULTILINGUAL_BOUQUET_NAME   = 0x5C,
    GOS_PSISI_DESCRIPTOR_MULTILINGUAL_SERVICE_NAME   = 0x5D,
    GOS_PSISI_DESCRIPTOR_COMPONENT_SERVICE_NAME      = 0x5E,
    GOS_PSISI_DESCRIPTOR_PRIVATE_DATA_SPECIFIER      = 0x5F,
    GOS_PSISI_DESCRIPTOR_SERVICE_MOVE                = 0x60,
    GOS_PSISI_DESCRIPTOR_SHORT_SMOOTHING_BUFFER      = 0x61,
    GOS_PSISI_DESCRIPTOR_FREQUENCY_LIST              = 0x62,
    GOS_PSISI_DESCRIPTOR_PARTIAL_TRANSPORT_STREAM    = 0x63,
    GOS_PSISI_DESCRIPTOR_DATA_BROADCAST              = 0x64,
    GOS_PSISI_DESCRIPTOR_CA_SYSTEM                   = 0x65,
    GOS_PSISI_DESCRIPTOR_DATA_BROADCAST_ID           = 0x66,
    GOS_PSISI_DESCRIPTOR_TRANSPORT_STREAM            = 0x67,
    GOS_PSISI_DESCRIPTOR_DSNG                        = 0x68,
    GOS_PSISI_DESCRIPTOR_PDC                         = 0x69,
    GOS_PSISI_DESCRIPTOR_AC3                         = 0x6A,
    GOS_PSISI_DESCRIPTOR_ANCILLARY_DATA              = 0x6B,
    GOS_PSISI_DESCRIPTOR_CELL_LIST                   = 0x6C,
    GOS_PSISI_DESCRIPTOR_CELL_FREQUENCY_LINK         = 0x6D,
    GOS_PSISI_DESCRIPTOR_ANNOUNCEMENT_SUPPORT        = 0x6E,
    GOS_PSISI_DESCRIPTOR_APPLICATION_SIGNALLING      = 0x6F,
    GOS_PSISI_DESCRIPTOR_ADAPTATION_FIELD_DATA       = 0x70,
    GOS_PSISI_DESCRIPTOR_SERVICE_IDENTIFIER          = 0x71,
    GOS_PSISI_DESCRIPTOR_SERVICE_AVAILABILITY        = 0x72,
    GOS_PSISI_DESCRIPTOR_TVA_DEFAULT_AUTHORITY       = 0x73,
    GOS_PSISI_DESCRIPTOR_TVA_RELATED_CONTENT         = 0x74,
    GOS_PSISI_DESCRIPTOR_TVA_ID                      = 0x75,
    GOS_PSISI_DESCRIPTOR_TVA_CONTENT_IDENTIFIER      = 0x76,
    GOS_PSISI_DESCRIPTOR_TIMESLICING_FEC_IDENTIFIER  = 0x77,
    GOS_PSISI_DESCRIPTOR_ECM_REPETITION_RATE         = 0x78,
    GOS_PSISI_DESCRIPTOR_S2_SATELLITE_DELIVERY_SYSTEM= 0x79,
    GOS_PSISI_DESCRIPTOR_ENHANCED_AC3                = 0x7A,
    GOS_PSISI_DESCRIPTOR_DTS                         = 0x7B,
    GOS_PSISI_DESCRIPTOR_AAC                         = 0x7C,
    GOS_PSISI_DESCRIPTOR_0X7D                        = 0x7D,
    GOS_PSISI_DESCRIPTOR_0X7E                        = 0x7E,
    GOS_PSISI_DESCRIPTOR_0X7F                        = 0x7F,
    GOS_PSISI_DESCRIPTOR_0X80                        = 0x80,
    GOS_PSISI_DESCRIPTOR_0X81                        = 0x81,
    GOS_PSISI_DESCRIPTOR_0X82                        = 0x82,
    GOS_PSISI_DESCRIPTOR_LCN_0X83                    = 0x83,
    GOS_PSISI_DESCRIPTOR_0X84                        = 0x84,
    GOS_PSISI_DESCRIPTOR_0X85                        = 0x85,
    GOS_PSISI_DESCRIPTOR_0X86                        = 0x86,
    GOS_PSISI_DESCRIPTOR_LCN_0X87                    = 0x87,
    GOS_PSISI_DESCRIPTOR_0X88                        = 0x88,
    GOS_PSISI_DESCRIPTOR_0X89                        = 0x89,
    GOS_PSISI_DESCRIPTOR_GOSCA_AREA_CONTROL          = 0x8A,

    GOS_PSISI_DESCRIPTOR_SERVICE_3D                  = 0x90, /**<3D tv SDT descriptor_tag*/

    GOS_PSISI_DESCRIPTOR_BUFF
} GOS_PSISI_DESCRIPTORTYPE_E;

/** stream type, see ISO/IEC 13818-1:2000 Table - Stream type assignments*/
typedef enum
{
    GOS_PSISI_STREAM_RESERVED         = 0x00, /**<ITU-T I ISOhEC Reserved*/
    GOS_PSISI_STREAM_VIDEO_MPEG1      = 0x01, /**<ISO/IEC 11172-2 Video*/
    GOS_PSISI_STREAM_VIDEO_MPEG2      = 0x02, /**<ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream*/
    GOS_PSISI_STREAM_AUDIO_MPEG1      = 0x03, /**<ISO/IEC 11172-3 Audio*/
    GOS_PSISI_STREAM_AUDIO_MPEG2      = 0x04, /**<ISO/IEC 13818-3 Audio*/
    GOS_PSISI_STREAM_PRIVATE_SECTIONS = 0x05, /**<ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections*/
    GOS_PSISI_STREAM_PRIVATE_PES      = 0x06, /**<ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data*/
    GOS_PSISI_STREAM_MHEG             = 0x07, /**<ISO/IEC 13522 MHEG*/
    GOS_PSISI_STREAM_DSM_CC           = 0x08, /**<ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC*/
    GOS_PSISI_STREAM_TYPE_H2221       = 0x09, /**<ITU-T Rec. H.222.1*/
    GOS_PSISI_STREAM_DSM_CC_TYPE_A    = 0x0A, /**<ISO/IEC 13818-6 type A*/
    GOS_PSISI_STREAM_DSM_CC_TYPE_B    = 0x0B, /**<ISO/IEC 13818-6 type B*/
    GOS_PSISI_STREAM_DSM_CC_TYPE_C    = 0x0C, /**<ISO/IEC 13818-6 type C*/
    GOS_PSISI_STREAM_DSM_CC_TYPE_D    = 0x0D, /**<ISO/IEC 13818-6 type D*/
    GOS_PSISI_STREAM_TYPE_AUX         = 0x0E, /**<ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary*/
    GOS_PSISI_STREAM_AUDIO_AAC_ADTS   = 0x0F, /**<ISO/IEC 13818-7 Audio with ADTS transport syntax*/
    GOS_PSISI_STREAM_VIDEO_MPEG4      = 0x10, /**<ISO/IEC 14496-2 Visual*/
    GOS_PSISI_STREAM_AUDIO_AAC_LATM   = 0x11, /**<ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3*/
    GOS_PSISI_STREAM_AUDIO_AAC_RAW    = 0x12, /**<ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets*/
    GOS_PSISI_STREAM_MPEG4_SECTION    = 0x13, /**<ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC 14496_sections*/
    GOS_PSISI_STREAM_DSM_CC_SDP       = 0x14, /**<ISO/IEC 13818-6 Synchronized Download Protocol*/
    GOS_PSISI_STREAM_METADATA_PES     = 0x15, /**<Metadata carried in PES packets*/
    GOS_PSISI_STREAM_METADATA_SECTION = 0x16, /**<Metadata carried in metadata_sections*/
    GOS_PSISI_STREAM_METADATA_DC      = 0x17, /**<Metadata carried in ISO/IEC 13818-6 Data Carousel*/
    GOS_PSISI_STREAM_METADATA_OC      = 0x18, /**<Metadata carried in ISO/IEC 13818-6 Object Carousel*/
    GOS_PSISI_STREAM_METADATA_SDP     = 0x19, /**<Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol*/
    GOS_PSISI_STREAM_IPMP_STREAM_MPEG2= 0x1A, /**<IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)*/
    GOS_PSISI_STREAM_VIDEO_H264       = 0x1B, /**<AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video*/
    /**0x1C-0x7E ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved*/
    GOS_PSISI_STREAM_VIDEO_AVS        = 0x42,
    GOS_PSISI_STREAM_AUDIO_AVS        = 0x43,
    GOS_PSISI_STREAM_VIDEO_WM9        = 0xEA,
    GOS_PSISI_STREAM_AUDIO_WM9        = 0xE6,
    GOS_PSISI_STREAM_IPMP_STREAM      = 0x7F, /*<IPMP stream*/
    GOS_PSISI_STREAM_VIDEO_USR        = 0x80, /*0x80-0xFF User Private*/
    GOS_PSISI_STREAM_AUDIO_AC3        = 0x81,
    GOS_PSISI_STREAM_SCTE_SUBT        = 0x82, /*SCTE Subtitle*/
    GOS_PSISI_STREAM_AUDIO_DTS        = 0x85,

    GOS_PSISI_STREAM_TELETEXT         = 0xB0,
    GOS_PSISI_STREAM_SUBTITLE         = 0xB1,
    GOS_PSISI_STREAM_CC               = 0xB2,
    GOS_PSISI_STREAM_UNKNOWN          = 0xB3,

    GOS_PSISI_STREAM_BUTT             = 0x100
} GOS_PSISI_STREAMTYPE_E;

typedef enum
{
    VIDEO_STREAM            = 0x00,
    AUDIO_STREAM            = 0x01,
    SCTE_SUBTITLE_STREAM    = 0x02,
    PRIVATE_PES_STREAM      = 0x03,
    BULT_STREAM
}GOS_STREAMTYPE;

/**table running status */
typedef enum 
{
    GOS_PSISI_TABLE_UNDEFINED            = 0,
    GOS_PSISI_TABLE_NOT_RUNNING          = 1,
    GOS_PSISI_TABLE_START_IN_FEW_SECONDS = 2,
    GOS_PSISI_TABLE_PAUSING              = 3,
    GOS_PSISI_TABLE_RUNNING              = 4,
    GOS_PSISI_TABLE_RESERVED_5           = 5,
    GOS_PSISI_TABLE_RESERVED_6           = 6,
    GOS_PSISI_TABLE_RESERVED_7           = 7
} GOS_PSISI_TABLE_STATUS_E;

/*package type*/
typedef enum
{
    PACKAGE_188 = 0x00,
    PACKAGE_204 = 0x01,
    PACKAGE_BULT
}GOS_TSPACKAGE_TYPE;


/*ts首部结构体*/
typedef struct 
{
	TR101290_ERROR_S stErrorInfo;
    GOSTSR_U32 number;
    GOSTSR_U16 ts_pid;
    GOSTSR_U8  error_indicater;
    GOSTSR_U8  load_indicater;
    GOSTSR_U8  scramble_control;
    GOSTSR_U8  adapter_control;
    GOSTSR_U8  counter;
    GOS_TSPACKAGE_TYPE packageType;
}TS_HEAD_INFO;

/*pcr info*/
typedef struct
{
    GOSTSR_U64 pgClockRfBase;
    GOSTSR_U16 pgClockRfExten;
}TS_PCR_INFO;

/*transport private data info*/
typedef struct
{
    GOSTSR_U8 priDataLen;
    GOSTSR_U8 priData[256];
}TS_PRIVATEDATA_INFO;

/*adaptation field extension info*/
typedef struct
{
    GOSTSR_U32 adaptExtenLen;
    GOSTSR_U8  ltwFlag;
    GOSTSR_U8  piecewiseRateFlag;
    GOSTSR_U8  seamlessSpliceFlag;
    GOSTSR_U8  ltwValidFlag;
    GOSTSR_U16 ltwOffset;
    GOSTSR_U32 piecewiseRate;
    GOSTSR_U8  spliceType;
    GOSTSR_U64 DTSNextAU;
}TS_ADAPTEXTEN_INFO;

/*adaptation field struct*/
typedef struct
{
    GOSTSR_U32 adaptLen;
    GOSTSR_U8  disContinuityIndicator;
    GOSTSR_U8  randomAccessIndicator;
    GOSTSR_U8  esPriorityIndicator;
    GOSTSR_U8  pcrFlag;
    GOSTSR_U8  opcrFlag;
    GOSTSR_U8  splicingPointFlag;
    GOSTSR_U8  tsPrivateFlag;
    GOSTSR_U8  adaptFieldExtenFlag;
    TS_PCR_INFO pcrInfo;
    TS_PCR_INFO opcrInfo;
    GOSTSR_U8  spliceCountDown;
    TS_PRIVATEDATA_INFO privateInfo;
    TS_ADAPTEXTEN_INFO extensionInfo;
}TS_ADAPT_INFO;

/*stream id*/
typedef enum
{
    PROGRAM_STREAM_MAP  = 0xbc,
    PRIVATE_STREAM_ONE  = 0xbd,
    PADDING_STREAM      = 0xbe,
    PRIVATE_STREAM_TWO  = 0xbf,
    AUDIO_STREAM_NUM    = 0xc0, /*0xc0 - 0xdf*/
    VIDEO_STREAM_NUM    = 0xe0, /*0xe0 - 0xef*/
    ECM_STREAM          = 0xf0,
    EMM_STREAM          = 0xf1,
    DSM_CC_STREAM       = 0xf2,
    ISOIEC_13522_STREAM = 0xf3,
    RESERVED_STREAM     = 0xf4, /*0xf4 - 0xfe*/
    PROGRAM_STREAM_DIR  = 0xff
}TS_STREAM_ID;

/*pes extension flag*/
typedef struct
{
    GOSTSR_U8  PESPrivateDataFlag;
    GOSTSR_U8  packHeaderFieldFlag;
    GOSTSR_U8  programPacketSequenceCounterFlag;
    GOSTSR_U8  PSTDBufferFlag;
    GOSTSR_U8  PESExtensionFlag2;
    GOSTSR_U8  packFieldLen;
    GOSTSR_U8  programPacketSequenceCounter;
    GOSTSR_U8  MPEG1MPEG2Identifier;
    GOSTSR_U8  originalStuffLen;
    GOSTSR_U8  PSTDBufferScale;
    GOSTSR_U16 PSTDBufferSize;
    GOSTSR_U8  PESExtensionFieldLen;
}TS_PESEXTENSION_INFO;

/*pes info*/
typedef struct
{
    GOSTSR_U32 pscp;
    GOSTSR_U8  streamId;
    GOSTSR_U16 pesPacketLen;
    GOSTSR_U8  pesScrambleCtr;
    GOSTSR_U8  pesPriorty;
    GOSTSR_U8  dataAlignmentIndicator;
    GOSTSR_U8  copyright;
    GOSTSR_U8  originalOrCopy;
    GOSTSR_U8  PTSDTSFlag;
    GOSTSR_U8  ESCRFlag;
    GOSTSR_U8  EsRateFlag;
    GOSTSR_U8  DSMTrickModeFlag;
    GOSTSR_U8  additionalCopyInfoFlag;
    GOSTSR_U8  PESCRCFlag;
    GOSTSR_U8  PESExtensionFlag;
    GOSTSR_U8  PESHeaderDataLen;
    GOSTSR_U64 PTS;
    GOSTSR_U64 DTS;
    GOSTSR_U64 ESCRBase;
    GOSTSR_U16 ESCRExten;
    GOSTSR_U32 ESRate;
    GOSTSR_U8  trickModeCtr;
    GOSTSR_U8  fieldId;
    GOSTSR_U8  intraSliceRefresh;
    GOSTSR_U8  frequencyTruncation;
    GOSTSR_U8  fieldRepCntrl;
    GOSTSR_U8  additionalCopyInfo;
    GOSTSR_U16 previousPESPacketCRC;
    TS_PESEXTENSION_INFO pesExtensionInfo;
}TS_PES_INFO;

/*ts sections info*/
struct SECTION_INFO
{
	TR101290_ERROR_S stErrorInfo;
    GOSTSR_U16 sectionLength;
    GOSTSR_U16 PID;
    GOSTSR_U8  tableID;
    GOSTSR_U8 *sectionData;
};
typedef struct SECTION_INFO  TS_SECTION_INFO;

/*ts package info*/
struct PACKAGE_INFO
{
	TR101290_ERROR_S stErrorInfo;
    GOSTSR_U16 sectionLen;
    GOSTSR_U16 packageLen;
    GOSTSR_U16 PID;
    GOSTSR_U8  packageData[256];
    GOSTSR_U8  number;
    GOSTSR_U8  tableID;
    struct PACKAGE_INFO *next;
};
typedef struct PACKAGE_INFO TS_PACKAGE_INFO;

/*Public Descriptor*/
typedef struct
{
    GOS_PSISI_DESCRIPTORTYPE_E   Tag;       /**<descriptor tag value*/
    GOSTSR_U16                   u16Size;   /**<the length of descriptor data*//**<CNcomment:从tag开始的描述子数据长度 */
    GOSTSR_U8                    *Data;     /**<descriptor data*//**<CNcomment:指针指向tag开始位置 */
} GOS_PSISI_Descriptor_S;

/*PAT struct*/
typedef struct
{
    GOSTSR_U16 u16ProgramNumber;
    GOSTSR_U16 u16Pid;
} GOS_PSISI_PAT_Element_S;

typedef struct
{
    GOSTSR_U32 u32CrcValue;
    GOSTSR_U16 u16NbElements;//the number of program
    GOS_PSISI_PAT_Element_S astElement[GOS_MAX_SERVICE_NUM];
    GOSTSR_U16 u16TransportStreamId;
    GOSTSR_U8  u8TableId;
    GOSTSR_U8  u8Version;
    GOSTSR_U8  u8SectionNum;
    GOSTSR_U8  u8LastSectionNum;

} GOSTSR_PSISI_PAT_S;

/*CAT struct*/
typedef struct
{
    GOSTSR_U32 u32CrcValue;
    GOSTSR_U16 u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_CAT_DESCRIPTOR];
    GOSTSR_U8 u8TableId;
    GOSTSR_U8 u8Version;
    GOSTSR_U8 u8SectionNum;
    GOSTSR_U8 u8LastSectionNum;
} GOSTSR_PSISI_CAT_S;

/*NIT struct*/
typedef struct
{
    GOSTSR_U16    u16TransportStreamId;
    GOSTSR_U16    u16OriginalNetWorkId;
    GOSTSR_U16    u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_NIT_ELEMENT_DESCRIPTOR];
} GOS_PSISI_NIT_Element_S;

typedef struct
{
    GOSTSR_U32  u32CrcValue;
    GOSTSR_U16  u16NetWorkId;
    GOSTSR_U16  u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_NIT_DESCIP_LOOP];
    GOSTSR_U16  u16NbElements;
    GOS_PSISI_NIT_Element_S astElement[GOS_MAX_NIT_ELEMENT];
    GOSTSR_U8   u8TableId;
    GOSTSR_U8   u8Version;
    GOSTSR_U8   u8SectionNum;
    GOSTSR_U8   u8LastSectionNum;
} GOSTSR_PSISI_NIT_S;

/*PMT struct*/
typedef struct
{
    GOSTSR_U16   u16Pid;
    GOSTSR_U16   u16NbDescriptors;
    GOS_PSISI_STREAMTYPE_E  eType;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_PMT_ELEMENT_DESCRIPTOR];
} GOS_PSISI_PMT_Element_S;

typedef struct 
{
    GOSTSR_U32 u32CrcValue;
    GOSTSR_U16   u16PcrPid;
    GOSTSR_U16   u16ProgramNumber;
    GOSTSR_U16   u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_PMT_DESCIP_LOOP1];
    GOSTSR_U16   u16NbElements;
    GOS_PSISI_PMT_Element_S astElement[GOS_MAX_PMT_ELEMENT];
    GOSTSR_U16   bScrambled;
    GOSTSR_U8    u8TableId;
    GOSTSR_U8    u8Version;
    GOSTSR_U8    u8SectionNum;
    GOSTSR_U8    u8LastSectionNum;
} GOSTSR_PSISI_PMT_S;

/*SDT struct*/
typedef struct 
{
    GOSTSR_U16    u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_SDT_ELEMENT_DESCRIPTOR];
    GOSTSR_U16    u16ServiceId;
    GOSTSR_U8     bEITScheduleFlag;
    GOSTSR_U8     bEITPresentFollowingFlag;
    GOS_PSISI_TABLE_STATUS_E   eStatus;
    GOSTSR_U8     bScrambled;
} GOS_PSISI_SDT_Element_S;

typedef struct
{
    GOSTSR_U32  u32CrcValue;
    GOSTSR_U16  u16TransportStreamId;
    GOSTSR_U16  u16OriginalNetWorkdId;
    GOSTSR_U16  u16NbElements;
    GOSTSR_U8   u8TableId;
    GOSTSR_U8   u8Version;
    GOSTSR_U8   u8SectionNum;
    GOSTSR_U8   u8LastSectionNum;
    GOS_PSISI_SDT_Element_S astElement[GOS_MAX_SDT_ELEMENT];
} GOSTSR_PSISI_SDT_S;

/*BAT struct*/
typedef struct
{
    GOSTSR_U16   u16TransportStreamId;
    GOSTSR_U16   u16OriginalNetworkId;
    GOSTSR_U16   u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_BAT_ELEMENT_DESCRIPTOR];
} GOS_PSISI_BAT_Element_S;

typedef struct
{
    GOSTSR_U32 u32CrcValue;
    GOSTSR_U16  u16Bouquet_id;
    GOSTSR_U16  u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_BAT_DESCIP_LOOP1];
    GOSTSR_U16  u16NbElements;
    GOS_PSISI_BAT_Element_S astElement[GOS_MAX_BAT_ELEMENT];
    GOSTSR_U8   u8TableId;
    GOSTSR_U8   u8Version;
    GOSTSR_U8   u8SectionNum;
    GOSTSR_U8   u8LastSectionNum;
} GOSTSR_PSISI_BAT_S;

/*EIT struct*/
typedef struct
{
    GOSTSR_U32   u32Mjd;
    GOSTSR_U32   u32StartTimeWord[2];
    GOSTSR_U32   u32DurationTimeWord;
    GOSTSR_U16   u16EventId;
    GOSTSR_U16   u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_EIT_ELEMENT_DESCRIPTOR];
    GOS_PSISI_TABLE_STATUS_E   eStatus;
    GOSTSR_U8    bScrambled;
} GOS_PSISI_EIT_Element_S;

typedef struct
{
    GOSTSR_U32  u32CrcValue;
    GOSTSR_U16  u16ServiceId;
    GOSTSR_U16  u16TransportStreamId;
    GOSTSR_U16  u16OriginalNetWorkdId;
    GOSTSR_U16  u16NbElements;
    GOSTSR_U8   u8TableId;
    GOSTSR_U8   u8Version;
    GOSTSR_U8   u8SectionNum;
    GOSTSR_U8   u8LastSectionNum;
    GOSTSR_U8   u8SegLastSectionNum;
    GOSTSR_U8   u8LastTabeId;
    GOS_PSISI_EIT_Element_S astElement[GOS_MAX_EIT_ELEMENT];
} GOSTSR_PSISI_EIT_S;

/*TDT struct*/
typedef struct
{
    GOSTSR_U8  u8Hour;
    GOSTSR_U8  u8Minute;
    GOSTSR_U8  u8Second;
} UTCtime;

typedef struct
{
    GOSTSR_U32  u32UTCtimeDWord;
    GOSTSR_U16  u16MJDtime;
    UTCtime     utcTime;
} GOSTSR_PSISI_TDT_S;

/*TOT struct*/
typedef struct 
{
    GOSTSR_U32  u32CrcValue;
    GOSTSR_U32	u32UTCtimeDWord;
    GOSTSR_U16  u16MJDtime;
    GOSTSR_U16  u16NbDescriptors;
    GOS_PSISI_Descriptor_S stLocalTimeOffDesc[GOS_MAX_TOT_DESCRIPTOR];
} GOSTSR_PSISI_TOT_S;

/*RST struct*/
typedef struct  
{
    GOSTSR_U16  u16TransportStreamId;
    GOSTSR_U16  u16OriginalNetWorkdId;
    GOSTSR_U16  u16ServiceId;
    GOSTSR_U16  u16EventId;
    GOSTSR_U8   u8RunStatus;
}GOS_PSISI_RST_Element_S;

typedef struct 
{
    GOSTSR_U16  u16NbElements;
    GOS_PSISI_RST_Element_S astElement[GOS_MAX_RTT_ELEMENT];
}GOSTSR_PSISI_RST_S;

/*ST struct*/
typedef struct  
{
    GOSTSR_U16  u8TableId;
}GOSTSR_PSISI_ST_S;

/*DIT struct*/
typedef struct  
{
    GOSTSR_U16  u8TableId;
    GOSTSR_U16  u8TransitionFlag;
}GOSTSR_PSISI_DIT_S;

/*SIT struct*/
typedef struct
{
    GOSTSR_U16   u16ServiceId;
    GOSTSR_U16   u8RunStatus;
    GOSTSR_U16   u16NbDescriptors;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_SIT_ELEMENT_DESCRIPTOR];
} GOS_PSISI_SIT_Element_S;

typedef struct 
{
    GOSTSR_U16  u16NbDescriptors;
    GOSTSR_U16  u16NbElements;
    GOSTSR_U8   u8TableId;
    GOSTSR_U8   u8VersionNum;
    GOSTSR_U8   u8SectionNum;
    GOSTSR_U8   u8LastSectionNum;
    GOS_PSISI_Descriptor_S astDescriptor[GOS_MAX_SIT_DESCIP_LOOP1];
    GOS_PSISI_SIT_Element_S astElement[GOS_MAX_SIT_ELEMENT];
}GOSTSR_PSISI_SIT_S;

/*date struct*/
typedef struct 
{
    GOSTSR_U16 u16year;
    GOSTSR_U8  u8month;
    GOSTSR_U8  u8day;
}GOSTSR_DATE_S;

/*time struct*/
typedef struct
{
    GOSTSR_U8 u8hour;  	/* 0~23 */
    GOSTSR_U8 u8minute;
    GOSTSR_U8 u8second;
}GOSTSR_TIME_S;

/*OTA private data struct*/
typedef struct
{
    GOSTSR_U32 u32OUI;
    GOSTSR_U32 u32Frequency;
    GOSTSR_U16 u16OADPid;
    GOSTSR_U16 u16Symbolrate;
    GOSTSR_U8  u8DscLen;
    GOSTSR_U8  u8SelectorLength;
    GOSTSR_U8  u8Selector[12];
    GOSTSR_U8  u8SoftVer	;
    GOSTSR_U8  u8UpdateMode	;
    GOSTSR_U8  u8SoftMode	;
    GOSTSR_U8  u8BandWidth	;
    GOSTSR_U8  u8Qam;
    GOSTSR_U8  u8Polarity;
}GOSTSR_PSISI_OTAINFO;

/*ISO 8859 encode enum*/
typedef enum
{
    FRIST_ISO8859_5     = 0x01,
    FRIST_ISO8859_6     = 0x02,
    FRIST_ISO8859_7     = 0x03,
    FRIST_ISO8859_8     = 0x04,
    FRIST_ISO8859_9     = 0x05,
    FRIST_ISO8859_10    = 0x06,
    FRIST_ISO8859_11    = 0x07, //0x08 reserved for future use
    FRIST_ISO8859_13    = 0x09,
    FRIST_ISO8859_14    = 0x0A,
    FRIST_ISO8859_15    = 0x0B, //0x0c - 0x0f reserved for future use
    FRIST_ISO8859       = 0x10,
    FRIST_ISO10646      = 0x11,
    FRIST_KSX1001       = 0x12,
    FRIST_GB2312        = 0x13,
    FRIST_ISO10646_TC   = 0x14,
    FRIST_ISO10646_UTF8 = 0x15, //0x16 - 0x1e reserved for future use
    FRIST_ENCODE_TYPEID = 0x1f,
    FRIST_BULT
}EPGHEAD_FRISTBYTE;

typedef enum
{
    SECOND_ISO8859   = 0x00,
    SECOND_BULT
}EPGHEAD_SECONDBYTE;

typedef enum
{
    THIRD_ISO8859_1   = 0x01, //0x00 reserved for future use
    THIRD_ISO8859_2   = 0x02,
    THIRD_ISO8859_3   = 0x03,
    THIRD_ISO8859_4   = 0x04,
    THIRD_ISO8859_5   = 0x05,
    THIRD_ISO8859_6   = 0x06,
    THIRD_ISO8859_7   = 0x07,
    THIRD_ISO8859_8   = 0x08,
    THIRD_ISO8859_9   = 0x09,
    THIRD_ISO8859_10  = 0x0A,
    THIRD_ISO8859_11  = 0x0B, //0x0c reserved for future use
    THIRD_ISO8859_13  = 0x0D,
    THIRD_ISO8859_14  = 0x0E,
    THIRD_ISO8859_15  = 0x0F,
    THIRD_BULT
}EPGHEAD_THIRDBYTE;

typedef enum
{
    ENCODE_ASCII = 0x00,
    ENCODE_UNICODE,
    ENCODE_ISO8859_1,
    ENCODE_ISO8859_2,
    ENCODE_ISO8859_3,
    ENCODE_ISO8859_4,
    ENCODE_ISO8859_5,
    ENCODE_ISO8859_6,
    ENCODE_ISO8859_7,
    ENCODE_ISO8859_8,
    ENCODE_ISO8859_9,
    ENCODE_ISO8859_10,
    ENCODE_ISO8859_11,
    ENCODE_ISO8859_13,
    ENCODE_ISO8859_14,
    ENCODE_ISO8859_15,
    ENCODE_ISO8859_16,
    ENCODE_GB2312,
    ENCODE_UTF8,
    ENCODE_BULT
}ENCODE_TYPE;

/*static ts stream hander functions*/
#define ANALYSIS_CALLBACKFUNCTION_NUM   0x10
typedef void (*AnalysisData_FUN)(TS_SECTION_INFO *tsSectionInfo);
GOSTSR_S32 GosTsr_AnalysisData_PacketType(GOSTSR_U32 *pu32PacketLen, char *filePath);
extern GOSTSR_S32 GosTsr_AnalysisData_TSHeadInfo(const void *srcData, TS_HEAD_INFO *desData);
extern GOSTSR_S32 GosTsr_AnalysisData_AdaptationInfo(TS_HEAD_INFO *desData, const void *data, TS_ADAPT_INFO *adaptInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_PESInfo(TS_HEAD_INFO *desData,const void *data, TS_PES_INFO *pesInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_CheckTsHeadInfo(TS_HEAD_INFO tsHeadInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_PSI(const void *data, TS_HEAD_INFO tsHeadInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_SI(const void *data, TS_HEAD_INFO tsHeadInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_GetPMTID(GOSTSR_PSISI_PAT_S *patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_ResetPMTID();
extern GOSTSR_S32 GosTsr_AnalysisData_TsEnvInit();
extern GOSTSR_S32 GosTsr_AnalysisData_TsEnvDeInit();
extern GOSTSR_S32 GosTsr_AnalysisData_RegisterCallBack(AnalysisData_FUN function, GOSTSR_U8 *index);
extern GOSTSR_S32 GosTsr_AnalysisData_UnRegisterCallBack(GOSTSR_U8 index);
extern GOSTSR_S32 GosTsr_AnalysisData_CallBack(TS_SECTION_INFO *tsSectionInfo);

/*table analysis functions*/
extern GOSTSR_S32 GosTsr_AnalysisData_PAT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_PAT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_CAT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_CAT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_NIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_NIT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_PMT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_PMT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_BAT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_BAT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_SDT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_SDT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_EIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_EIT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_TDT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_TDT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_TOT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_TOT_S * patInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_RST(TS_SECTION_INFO *sectionInfo, GOSTSR_PSISI_RST_S * rstInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_ST (TS_SECTION_INFO *sectionData, GOSTSR_PSISI_ST_S  * stInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_DIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_DIT_S * ditInfo);
extern GOSTSR_S32 GosTsr_AnalysisData_SIT(TS_SECTION_INFO *sectionData, GOSTSR_PSISI_SIT_S * sitInfo);

/*analysis extend functions*/
extern GOSTSR_DATE_S GosTsr_TIME_MJDtoYMD(GOSTSR_U16 mjd);
extern GOSTSR_TIME_S GosTsr_TIME_UTCtoHMS(GOSTSR_U32 utc);
extern GOS_STREAMTYPE GosTsr_AnalysisData_StreamType(GOS_PSISI_STREAMTYPE_E StreamData);
extern GOSTSR_S32 GosTsr_AnalysisData_OTAPrivate(GOSTSR_U8 *privateData, GOSTSR_PSISI_OTAINFO *otaInfo);

/*encode switch hander functions*/
extern GOSTSR_S32 GosTsr_UnicodeString_to_UTF8String(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_1String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_2String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_3String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_4String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_5String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_6String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_7String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_8String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_9String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_10String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_11String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_13String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_14String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_15String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern GOSTSR_S32 GosTsr_ISO8859_16String_to_UnicodeString(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);
extern ENCODE_TYPE GosTsr_CheckLanguageHeadCode(GOSTSR_U8 headCode, GOSTSR_U8 secondCode, GOSTSR_U8 thridCode);
extern GOSTSR_S32 GosTsr_SourceString_to_UTF8String(GOSTSR_U8 *sour_string, GOSTSR_U16 sour_len, GOSTSR_U8 *des_string, GOSTSR_U16 *des_len);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */


