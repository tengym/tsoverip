#include <dirent.h>

#include "user_define.h"
#include "tsparse_api.h"
#include "tsparse_search.h"
#include "tsparse_update.h"

#include "GosTsr_Crc.h"
#include "GosTsr_AnalysisData.h"

#include "TsErrorCheck_OneLevel.h"
#include "TsErrorCheck_TwoLevel.h"
#include "TsErrorCheck_ThreeLevel.h"

#include "udp_unicast_api.h"
#include "udp_multicast_api.h"
#include "rtp_unicast_api.h"
#include "rtp_multicast_api.h"

#include "web.h"
#include "usbdesk.h"
#include "threadpool.h"
#include "user_define.h"
#include "tsparse_file.h"

enum
{
	GOS_UDP_MULTICAST = 0,		 /*UDP ×é2￥*/
	GOS_UDP_UNICAST, 		 /*UDP μ￥2￥*/
	GOS_RTP_MULTICAST, 		 /*RTP ×é2￥*/
	GOS_RTP_UNICAST, 		 /*RTP μ￥2￥*/
	
	GOS_UDPRTP_BUTT
};

typedef struct
{
    char filepath[256];
    char addr[32];

    GOSTSR_U8 useflag;
    GOSTSR_U8 protocol;

    GOSTSR_U32 handle;
    GOSTSR_U32 port;
    GOSTSR_U32 bitrate;
}THREAD_POOL_PARA;

typedef struct
{
    int used;
    int fd;
    int flag;
    GOSTSR_U64 distance;
    struct sockaddr_in dest_addr;
}SOCKET_PARA;

static THREAD_POOL_PARA gstTask_Para[MAX_THREAD_NUM];
static SOCKET_PARA gstsocket[MAX_THREAD_NUM];
static GOSTSR_U32 gPacketNumber = 0;

static GOSTSR_BOOL gb_startflag = GOSTSR_FALSE;
static GOS_SVC_TsParse_BitRate_t gst_bitrate;

static inline GOSTSR_U32 tsparse_time_gettimenow()
{
    GOSTSR_U32 timeValue = 0;
    struct  timeval time_value; 
    gettimeofday(&time_value, NULL); 
    timeValue = time_value.tv_sec * 1000 + time_value.tv_usec / 1000;   

    return (timeValue);
}

static inline GOSTSR_S32 tsparse_time_waittime(int handle, GOSTSR_U64 value)
{
    GOSTSR_U32 nWait = 0;
    GOSTSR_U64 ntime = tsparse_time_gettimenow();

    GOSTSR_U64 nSystime = 27*1000*ntime;//把当前时间转换成27M时钟

    if(gstsocket[handle].flag == 0)
    {
        gstsocket[handle].distance = abs(nSystime - value); 
        gstsocket[handle].flag = 1;
    }
    else
    {
        nWait = (value - (nSystime - gstsocket[handle].distance))/ (27*1000);
        if(nWait < 0)
        {
            gstsocket[handle].distance = nSystime - value; 
            return GOSTSR_SUCCESS;
        }
        if(nWait <= 5000)
        {
            usleep(nWait*1000);
        }
        else
        {
            gstsocket[handle].distance = nSystime - value; 
        }
    }

    return GOSTSR_SUCCESS;
}

/************* ************************************************
 * 判断一个IP地址是否为组播地址,只需要判断该地址是否在224.0.0.255(不含)到239.255.255.255之间,

　　IP地址本身是一个32位的数值，即0x00000000-0xffffffff之间，所以只需判断IP对应的Long值是否大于0xE00000FF且小于等于0xEFFFFFFF即可
*************************************************************/
static GOSTSR_S32 tsparse_socket_gettype_byipaddr(int *mode, GOSTSR_U8 protocol, char *paddr)
{
    if((GOSTSR_NULL == mode) || (GOSTSR_NULL == paddr))
    {
        return GOSTSR_FAILURE;
    }

    GOSTSR_S32 s32Ret = GOSTSR_FAILURE;
    GOSTSR_U32 ip_value = inet_addr(paddr);

    if((ip_value > 0xe00000ff) || (ip_value <= 0xefffffff))
    {
        if(protocol == 0) /*UDP*/
        {
            *mode = GOS_UDP_MULTICAST;
        }
        else if(protocol == 1) /*RTP*/
        {
            *mode = GOS_RTP_MULTICAST;
        }
        s32Ret = GOSTSR_SUCCESS;
    }
    else
    {
        if(protocol == 0) /*UDP*/
        {
            *mode = GOS_UDP_UNICAST;
        }
        else if(protocol == 1) /*RTP*/
        {
            *mode = GOS_RTP_UNICAST;
        }
        s32Ret = GOSTSR_SUCCESS;
    }

    return s32Ret;
}

static GOSTSR_S32 tsparse_socket_creatsock(int mode, int port, char *paddr, int *handle)
{
    int s32Ret = -1;

    char cmd[256];
    sprintf(cmd, "route add -net %s netmask 255.255.255.255 eth1", paddr);
    system(cmd);

	switch (mode)
	{
		case GOS_UDP_MULTICAST:
			s32Ret = udp_multicast_create(UDP_MULTICAST_SEND, port, paddr, handle);
			break;

		case GOS_UDP_UNICAST:
			//s32Ret = udp_unicast_creat(port, ipaddr, tsbuffer_socket_callback);
			break;

		case GOS_RTP_MULTICAST:
			s32Ret = rtp_multicast_create(RTP_MULTICAST_SEND,33, port, paddr, handle);
			break;

		case GOS_RTP_UNICAST:
			//s32Ret = rtp_unicast_creat(port, ipaddr, tsbuffer_socket_callback);
			break;

		default:
			break;
	}
    return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_socket_senddata(int mode, int handle, void *data, int datalen)
{
    int s32Ret = -1;

	switch (mode)
	{
		case GOS_UDP_MULTICAST:
			s32Ret = udp_multicast_sendmsg(handle, (const char *)data, datalen);
			break;

		case GOS_UDP_UNICAST:
			//s32Ret = udp_unicast_creat(port, ipaddr, tsbuffer_socket_callback);
			break;

		case GOS_RTP_MULTICAST:
			s32Ret = rtp_multicast_sendmsg(handle, (const char*)data, datalen, 33);
			break;

		case GOS_RTP_UNICAST:
			//s32Ret = rtp_unicast_creat(port, ipaddr, tsbuffer_socket_callback);
			break;

		default:
			break;
	}
    return s32Ret;
}

static GOSTSR_S32 tsparse_api_sendtspacket(int handle, char *pfilePath, int tsrate, int mode, int port, char *addr) 
{
    FILE *sFp = GOSTSR_NULL;
    GOSTSR_U32 packageLen = 0;
    GOSTSR_U8 *tsData = GOSTSR_NULL;	

    if((GOSTSR_NULL == pfilePath) || (GOSTSR_NULL == addr))
    {
        return -1;
    }
    int port_udp = port;
    int bitrate = tsrate;
    char filePath[256];
    char paddr[32];

    if (GOSTSR_NULL == pfilePath)
    {
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    strcpy(filePath, pfilePath);
    strcpy(paddr, addr);

    int socket_handle = -1;
    if(0 != tsparse_socket_creatsock(mode, port_udp, paddr,  &socket_handle))
    {
        printf("creat Socket Failed\n");
        return -1;
    }
    printf("File:%s--bitrate:%.4f--handle:%d--port:%d--addr:%s\n", filePath, (float)bitrate/1000/1000,handle, port_udp, paddr);

    if(GOSTSR_FAILURE == GosTsr_AnalysisData_PacketType(&packageLen,filePath))
    {
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    sFp = fopen(filePath, "rb");
    if (GOSTSR_NULL == sFp)
    {
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    fseek(sFp, 0, SEEK_SET);

    GOSTSR_U64 nCountTotal = 0;
    GOSTSR_U64 nSendTotal = 0;
    GOSTSR_U64 nPCRBase = 0;
    int     nDataLen = 100 * TS_OVER_IP_SIZE; 
    int     nSendLen = 0;
    int     nLen = 0;

    tsData = (GOSTSR_U8 *)malloc(nDataLen);
    if (GOSTSR_NULL == tsData)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        printf("failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    GOSTSR_U64 nTime_tmp = 0;
    GOSTSR_U64 nTime_begin = 0;
    GOSTSR_U64 dwTime = tsparse_time_gettimenow();
    gstsocket[handle].flag = 0;
    nTime_begin = dwTime;

    printf("\n\nHandle[%d] begin to send ....\n\n", handle);

    while(gb_startflag)
    {
        nLen = fread(tsData, 1, nDataLen, sFp);
        if (nLen < nDataLen)
        {
            nSendLen = 0;
            while (nSendLen < nLen)
            {
                tsparse_socket_senddata(mode, socket_handle, tsData+nSendLen, TS_OVER_IP_SIZE);
                nSendLen += TS_OVER_IP_SIZE;
            }

            nCountTotal++;
            dwTime = tsparse_time_gettimenow() - dwTime;
            DEBUG_INFO("handle:%d--nSendNum:%lld, bitrate[%.2fMbps]\n", handle, nCountTotal, (1.0 * nSendTotal * 1000 * 8) / dwTime/1000/1000);

            nSendTotal = 0;
            fseek(sFp, 0, SEEK_SET);
            dwTime = tsparse_time_gettimenow(); 
            gstsocket[handle].flag = 0;
            nTime_begin = dwTime;
            continue;
        }

        nSendLen = 0;
        while (nSendLen < nDataLen)
        {
            tsparse_socket_senddata(mode, socket_handle, tsData+nSendLen, TS_OVER_IP_SIZE);
            nSendLen += TS_OVER_IP_SIZE;
        }
        nSendTotal += nSendLen;

        /*获取实时速率*/
        nTime_tmp = tsparse_time_gettimenow() - nTime_begin;
        if(nTime_tmp != 0)
        {
            gst_bitrate.bitrate[handle]  = (1.0 * nSendTotal * 1000 * 8) / nTime_tmp;
        }

        /*计算此时的PCR*/
        nPCRBase = 27*1000*1000 *(1.0 * nSendTotal * 8) / bitrate;

        tsparse_time_waittime(handle, nPCRBase);
    }

    fseek(sFp, 0, SEEK_SET);
    fclose(sFp);
    sFp = GOSTSR_NULL;

    printf("\n\nend to send(Handle[%d]) ....\n\n",handle);

    return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_api_threadpool_callback(void *argv)
{
    if(GOSTSR_NULL == argv)
    {
        return -1;
    }

    int mode = 0;
    THREAD_POOL_PARA *pPara = (THREAD_POOL_PARA *)argv;

    if(GOSTSR_SUCCESS != tsparse_socket_gettype_byipaddr(&mode, pPara->protocol, pPara->addr))
    {
        DEBUG_ERROR("tsparse_socket_gettype_byipaddr error");
        return -1;
    }

    tsparse_api_sendtspacket(pPara->handle, pPara->filepath, pPara->bitrate, mode,  pPara->port, pPara->addr);

    return 0;
}

static GOSTSR_S32 tsparse_api_analysispacket(GOSTSR_BOOL bCarryFlag,GOSTSR_U32 u32BytePos,GOSTSR_U8 *pPacketData, GOSTSR_U32 u32DataLen)
{
    GOSTSR_U32 startTime = 0;
    GOSTSR_U32 endTime = 0;
    TS_HEAD_INFO tsHeadInfo;
    TS_ADAPT_INFO adaptInfo;
    TS_PES_INFO  pesInfo;

    if (GOSTSR_NULL == pPacketData)
    {
        return GOSTSR_FAILURE;
    }

    memset(&tsHeadInfo, 0, sizeof(tsHeadInfo));
    memset(&adaptInfo, 0, sizeof(adaptInfo));
    memset(&pesInfo, 0, sizeof(pesInfo));

    if(TsErrorCheck_TwoLevel_getAvTransportRate() != 0)
    {
        startTime = TsErrorCheck_TwoLevel_getTimeUs_byBytePos(u32BytePos);
        endTime = TsErrorCheck_TwoLevel_getTimeUs_byBytePos(u32BytePos+188);
    }

    tsHeadInfo.stErrorInfo.bytePos = u32BytePos;
    tsHeadInfo.stErrorInfo.bCarryFlag = bCarryFlag;
    tsHeadInfo.stErrorInfo.startTime = startTime;
    tsHeadInfo.stErrorInfo.endTime = endTime;
    if(GOSTSR_FAILURE == GosTsr_AnalysisData_TSHeadInfo((void *)pPacketData, &tsHeadInfo))
    {
        return GOSTSR_FAILURE;
    }

    if(GOSTSR_FAILURE == GosTsr_AnalysisData_AdaptationInfo(&tsHeadInfo, (void *)(&pPacketData[TS_HEAD_LENGTH]), &adaptInfo))			
    {
        return GOSTSR_FAILURE;
    }

    if (GOSTSR_FAILURE == GosTsr_AnalysisData_PSI((void *)pPacketData, tsHeadInfo))
    {
        if (GOSTSR_FAILURE == GosTsr_AnalysisData_SI((void *)pPacketData, tsHeadInfo))
        {
            GosTsr_AnalysisData_PESInfo(&tsHeadInfo,(void *)pPacketData, &pesInfo);		
        }
    }

    return GOSTSR_SUCCESS;
}

static GOSTSR_S32 tsparse_api_get_filename(TSPARSE_API_INFO_t *tsparse_info)  
{  
    DIR *dp ;  
    struct dirent *dirp ;                         
    GOSTSR_U8 firstflag = 0;
    GOSTSR_U32 filelen = 0;
    DEV_ATTRIBUTE   stUdeskDevAttr; 

    if((tsparse_info == GOSTSR_NULL))
    {
        return GOSTSR_FAILURE;
    }
    
    if(USB_GetDiskPartNum() <= 0)
    {
        return GOSTSR_FAILURE;
    }

    if(USB_GetAttribute(0, &stUdeskDevAttr, 0) != GOSTSR_SUCCESS) 
    {
        DEBUG_ERROR("USB_GetAttribute failed\n");
        return GOSTSR_FAILURE;
    }

    DEBUG_INFO("Mount Name is: %s\n", stUdeskDevAttr.mount_name);

    /*仅仅遍历根路径*/
    if((dp = opendir(stUdeskDevAttr.mount_name)) == NULL)  
    {  
        perror("opendir");  
        return GOSTSR_FAILURE;
    }  

    while((dirp = readdir(dp)) != NULL)  
    {  
        if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..") == 0)  
        {
            continue;
        }

        int size = strlen(dirp->d_name);  
        if(size < 3)     
        {
            continue;
        }
        if(strcmp((dirp->d_name + (size - 3)) , ".ts") == 0)  
        {
            if(filelen >= 4*1024)
            {
                break;
            }
            if(0 != firstflag)
            {
                strcat(tsparse_info->pfilename, "-");
            }

            strcat(tsparse_info->pfilename, dirp->d_name);
            filelen += strlen(dirp->d_name);
            firstflag = 1;
        }
    }  
    closedir(dp);  

    return GOSTSR_SUCCESS;  
}  
GOSTSR_S32 TsParse_Api_ReadStaticTs(char *pfilepath, int *bitrate)
{
    FILE *sFp = GOSTSR_NULL;
    GOSTSR_U32 fileLen = 0;
    GOSTSR_U32 u32BytePos = 0;
    GOSTSR_U32 i = 0;
    GOSTSR_U32 offsetPos = 0;
    GOSTSR_U32 packageLen = 0;
    GOSTSR_U8 offsetPos_Carry = 0;
    GOSTSR_U8 chr = '\0';
    GOSTSR_U8 *tsData = GOSTSR_NULL;	
    GOSTSR_BOOL endState = GOSTSR_TRUE;
    GOSTSR_U32 curSyncOffset = 0;
    GOSTSR_U32 outSyncOffset = 0;
    GOSTSR_U32 tmpSyncOffset = 0;
    GOSTSR_U32 tmpSyncOffsetbak = 0;
    GOSTSR_BOOL bCarryFlag = GOSTSR_FALSE;

    if ((GOSTSR_NULL == pfilepath) || (GOSTSR_NULL == bitrate)) 
    {
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    if(GOSTSR_FAILURE == GosTsr_AnalysisData_PacketType(&packageLen,pfilepath))
    {
        printf("return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }
    sFp = fopen(pfilepath, "rb");
    if (GOSTSR_NULL == sFp)
    {
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    tsData = (GOSTSR_U8 *)malloc(packageLen);
    if (GOSTSR_NULL == tsData)
    {
        fclose(sFp);
        sFp = GOSTSR_NULL;
        printf("  return failure %d \n", __LINE__);
        return GOSTSR_FAILURE;
    }

    fseek(sFp, 0, SEEK_END);
    fileLen = ftell(sFp);
    fseek(sFp, 0, SEEK_SET);
    printf("______ TS File size = %dM, packageLen:%d_____\n",fileLen / 1024 /1024, packageLen);

    int coun_Len = 0;
    while(endState)
    {
        offsetPos = ftell(sFp);
        do
        {
            fseek(sFp, offsetPos, SEEK_SET);
            chr = fgetc(sFp);
            offsetPos++;
            if(offsetPos >= TWOLEVEL_BYTEPOS_MAX)
            {
                offsetPos_Carry++;
                offsetPos = 0;	
                bCarryFlag = GOSTSR_TRUE;
            }

            if (offsetPos_Carry*TWOLEVEL_BYTEPOS_MAX+offsetPos >= (fileLen - 1))
            {
                endState = GOSTSR_FALSE;
            }

        }while(chr != SYNC_BYTE);

        if (chr == SYNC_BYTE)
        {
            offsetPos = offsetPos - 1;
            curSyncOffset = offsetPos;
            fseek(sFp, offsetPos, SEEK_SET);

            for(i = 0;i < 5;i++)
            {
                if(fread(tsData, 1, packageLen, sFp) < packageLen)
                {
                    endState = GOSTSR_FALSE;
                    break;//文件结尾处理
                }

                if(tsData[0] != SYNC_BYTE)
                {
                    break;
                }
            }

            if(i != 5)
            {
                fseek(sFp, curSyncOffset + 1, SEEK_SET);
                continue;
            }
            else
            {
                if((curSyncOffset > outSyncOffset) && outSyncOffset != 0)
                {
                    tmpSyncOffset = outSyncOffset;
                    if((curSyncOffset - outSyncOffset) > packageLen)
                    {
                        while(tmpSyncOffset <= curSyncOffset)
                        {
                            do
                            {
                                fseek(sFp, tmpSyncOffset, SEEK_SET);
                                chr = fgetc(sFp);
                                tmpSyncOffset++;

                                if(tmpSyncOffset > curSyncOffset)
                                {break;}
                            }while(chr != SYNC_BYTE);

                            if(chr == SYNC_BYTE)
                            {
                                tmpSyncOffset -= 1;
                                if(tmpSyncOffset + packageLen < curSyncOffset)
                                {
                                    fseek(sFp, tmpSyncOffset + packageLen, SEEK_SET);
                                    chr = fgetc(sFp);
                                    if(chr == SYNC_BYTE)
                                    {
                                        tmpSyncOffsetbak = tmpSyncOffset;
                                        tmpSyncOffset += packageLen;
                                    }
                                    else
                                    {
                                        tmpSyncOffset++;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }

                fseek(sFp, curSyncOffset, SEEK_SET);
                do
                {
                    offsetPos = ftell(sFp);
                    if(fread(tsData, 1, packageLen, sFp) < packageLen)
                    {
                        endState = GOSTSR_FALSE;
                        break;
                    }
                    if(tsData[0] == SYNC_BYTE)
                    {
                        u32BytePos = offsetPos;

                        tsparse_api_analysispacket(bCarryFlag,u32BytePos,tsData, packageLen);

                        coun_Len += packageLen;
                        if(coun_Len >= 10*1024*1024)
                        {
                            coun_Len = 0;
                            goto LOOP_FILE_END;
                        }

                        if(bCarryFlag)/*将进位标志置为初始化状态*/
                        {
                            bCarryFlag = GOSTSR_FALSE;
                        }
                        gPacketNumber++;
                        if(gPacketNumber >= TWOLEVEL_BYTEPOS_MAX)
                        {
                            gPacketNumber = 0;
                        }
                    }
                    else
                    {
                        outSyncOffset = offsetPos;
                        fseek(sFp, outSyncOffset, SEEK_SET);
                        break;
                    }
                }while(1);
            }
        }
    }

LOOP_FILE_END:
    free(tsData);
    tsData = GOSTSR_NULL;

    if (GOSTSR_SUCCESS != fclose(sFp))
    {
        return GOSTSR_FAILURE;
    }
    sFp = GOSTSR_NULL;

    *bitrate  = TsErrorCheck_TwoLevel_getAvTransportRate();
    if(*bitrate == 0)
    {
        return GOSTSR_FAILURE;
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_Threadpool_AddTask(int handle, char *pfilepath, int bit_rate, int protocol, int port, char *paddr)
{
    GOSTSR_S32 i = 0;
    GOSTSR_S32 ret = -1;

    if((GOSTSR_NULL == pfilepath) || (GOSTSR_NULL == paddr))
    {
        return GOSTSR_FAILURE;
    }

    for(i = 0; i < MAX_THREAD_NUM; i++)
    {
        if(gstTask_Para[i].useflag == 0)
        {
            break;
        }
    }
    if(i == MAX_THREAD_NUM)
    {
        return GOSTSR_FAILURE;
    }

    memset(&gstTask_Para[i], 0, sizeof(gstTask_Para[i]));

    gstTask_Para[i].handle = handle;
    gstTask_Para[i].bitrate = bit_rate;
    gstTask_Para[i].port = port;
    gstTask_Para[i].protocol = protocol;
    strcpy(gstTask_Para[i].filepath, pfilepath);
    strcpy(gstTask_Para[i].addr, paddr);
    ret = threadpool_api_addtask(tsparse_api_threadpool_callback, (void *)&gstTask_Para[i]);
    if(ret != 0)
    {
        return GOSTSR_FAILURE;
    }

    usleep(500 * 1000);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_TsParseProcess(TSPARSE_API_INFO_t *tsparse_info)
{
    if(GOSTSR_NULL == tsparse_info)
    {
        return GOSTSR_FAILURE;
    }
#if 0
    int index = 0;
    printf("=============================\n");
    for(index = 0; index < 5; index++)
    {
        printf("index: %d\n", index);
        printf("FileName: %s\n", tsparse_info->unit_info[index].pfilename);
        printf("protocol: %d\n", tsparse_info->unit_info[index].protocol);
        printf("port: %d\n", tsparse_info->unit_info[index].port);
        printf("ip: %s\n", tsparse_info->unit_info[index].paddr);
        printf("packet: %d\n", tsparse_info->unit_info[index].packet_num);
        printf("state: %d\n\n", tsparse_info->unit_info[index].state);
    }
    printf("=============================\n");
#endif
    
    GOSTSR_S32 i = 0;
    int bitrate[MAX_THREAD_NUM];
    char filepath[MAX_THREAD_NUM][256];
    DEV_ATTRIBUTE   stUdeskDevAttr; 

    if(USB_GetDiskPartNum() == 1)
    {
        if(USB_GetAttribute(0, &stUdeskDevAttr, 0) != GOSTSR_SUCCESS) 
        {
            DEBUG_ERROR("USB_GetAttribute failed\n");
            return GOSTSR_FAILURE;
        }
    }
    char *path = stUdeskDevAttr.mount_name; 

    memset(&gst_bitrate, 0, sizeof(gst_bitrate));

    for(i = 0; i < MAX_THREAD_NUM; i++)
    {
        if((tsparse_info->unit_info[i].state == 0) || (strcmp(tsparse_info->unit_info[i].pfilename, "0") == 0))
        {
            continue;
        }

        bitrate[i] = 0;
        memset(filepath[i], 0, sizeof(filepath[i]));

        sprintf(filepath[i], "%s/%s", path, tsparse_info->unit_info[i].pfilename);

        TsParse_Api_ResInit();

        if(GOSTSR_SUCCESS != TsParse_Api_ReadStaticTs(filepath[i], &bitrate[i]))
        {
            DEBUG_ERROR("Parse file[%s] failed, num:%d\n", filepath[i], i);
        }

        gst_bitrate.bitrate[i] = bitrate[i];

        DEBUG_INFO("filename[%d] = %s--bit_rate:%.2f\n", i, filepath[i], (float)bitrate[i]/1000/1000);
    }

    TsParse_Api_DeInit();

    gb_startflag = GOSTSR_TRUE;
    for(i = 0; i < MAX_THREAD_NUM; i++)
    {
        if((tsparse_info->unit_info[i].state == 0) || (strcmp(tsparse_info->unit_info[i].pfilename, "0") == 0))
        {
            continue;
        }
        if(GOSTSR_SUCCESS != TsParse_Api_Threadpool_AddTask(i, filepath[i], bitrate[i], tsparse_info->unit_info[i].protocol, 
                                    tsparse_info->unit_info[i].port, tsparse_info->unit_info[i].paddr))                   
        {
            DEBUG_ERROR("add task failed, file[%s], num:%d\n", filepath[i], i);
        }
    }

    /*Store to DB*/
    tsparse_info->chip_state  = 1;
    TsParse_File_Write_TsInfoFile((void *)tsparse_info);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_Get_TsInfo(TSPARSE_API_INFO_t *tsparse_info)  
{  
    GOSTSR_S32 ret = -1;

    if((tsparse_info == GOSTSR_NULL))
    {
        return GOSTSR_FAILURE;
    }

    ret = TsParse_File_Read_TsInfoFile(tsparse_info);
    if(ret != GOSTSR_SUCCESS)
    {
        tsparse_info->chip_state = 0;
        return GOSTSR_FAILURE;
    }

    ret = tsparse_api_get_filename(tsparse_info);
    if(ret != GOSTSR_SUCCESS)
    {
        strcpy(tsparse_info->pfilename, "0");
        printf("\nUSB not insert...\n");
        tsparse_info->chip_state = 0;
        return GOSTSR_FAILURE;
    }

    return GOSTSR_SUCCESS;  
}  

GOSTSR_S32 TsParse_Api_Get_Bitrate(GOS_SVC_TsParse_BitRate_t *stbitrate)
{
    if(stbitrate == GOSTSR_NULL)
    {
        return GOSTSR_FAILURE;
    }

    memcpy(stbitrate->bitrate, gst_bitrate.bitrate, sizeof(gst_bitrate.bitrate));

    int i = 0;
    for(i = 0; i < MAX_TASK_PERCHIP; i++)
    {
       stbitrate->total_bitrate += stbitrate->bitrate[i];
    }

    //printf("totbit_rate:%d\n", stbitrate->total_bitrate);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_Restart(void)
{
    TSPARSE_API_INFO_t tsparse_info;

    int time_count = 0;
    while(1)
    {
        if((USB_GetDiskPartNum() > 0) || (time_count > 100)) 
        {
            break;
        }
        
        usleep(50*1000);
        time_count++;
    }

    TsParse_File_Read_TsInfoFile((void *)&tsparse_info);
    if(tsparse_info.chip_state == 1)
    {
        TsParse_Api_TsParseProcess(&tsparse_info);
    }

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_StopSend(void)
{
    gb_startflag = GOSTSR_FALSE;

    TSPARSE_API_INFO_t tsparse_info;

    TsParse_File_Read_TsInfoFile((void *)&tsparse_info);
    tsparse_info.chip_state  = 0;

    TsParse_File_Write_TsInfoFile((void *)&tsparse_info);

    memset(gst_bitrate.bitrate, 0, sizeof(gst_bitrate.bitrate));

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_Init()
{
    memset(&gstTask_Para, 0, sizeof(gstTask_Para));
    memset(&gstsocket, 0, sizeof(gstsocket));
    memset(&gst_bitrate, 0, sizeof(gst_bitrate));

    GosTsr_AnalysisData_TsEnvInit();
    GosTsr_AnalysisData_CRCInit();
    TsParse_Update_ProgSearch_Init();

    TsErrorCheck_OneLevel_Init();
    TsErrorCheck_TwoLevel_Init();
    TsErrorCheck_ThreeLevel_Init();

    udp_multicast_init();
    udp_unicast_init();
    rtp_unicast_init();
    rtp_multicast_init();   
    threadpool_api_create(MAX_THREAD_NUM);

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_DeInit()
{
    GosTsr_AnalysisData_TsEnvDeInit();
    TsParse_Update_ProgSearch_DeInit();

    TsErrorCheck_OneLevel_DeInit();
    TsErrorCheck_TwoLevel_DeInit();
    TsErrorCheck_ThreeLevel_DeInit();

    return GOSTSR_SUCCESS;
}

GOSTSR_S32 TsParse_Api_ResInit()
{
    GosTsr_AnalysisData_TsEnvDeInit();
    TsParse_Update_ProgSearch_DeInit();

    TsErrorCheck_OneLevel_DeInit();
    TsErrorCheck_TwoLevel_DeInit();
    TsErrorCheck_ThreeLevel_DeInit();

    GosTsr_AnalysisData_TsEnvInit();
    GosTsr_AnalysisData_CRCInit();
    TsParse_Update_ProgSearch_Init();

    TsErrorCheck_OneLevel_Init();
    TsErrorCheck_TwoLevel_Init();
    TsErrorCheck_ThreeLevel_Init();

    return GOSTSR_SUCCESS;
}
