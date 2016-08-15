#include "hi_common.h"
#include "hi_unf_gpio.h"
#include "gos_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HI_TYPE_ETH        "eth1"
#define HI_DEFAULTE_IP     "192.168.100.50"

static GOS_U32 gu32GpioChipValue = 0;
static char gp_AddrHost[32] = HI_DEFAULTE_IP;

#define SWAP32(x) ((((x)&0xFF)<<24)|(((x)>>24)&0xFF)|(((x)&0x0000FF00)<<8)|(((x)&0x00FF0000)>>8))

static void ip2stringex(GOS_U32 u32IP, char *pcDotIp)
{   
    u32IP = ntohl (u32IP);
    snprintf (pcDotIp, 20, "%d.%d.%d.%d",
                      (int) ((u32IP >> 24) & 0xff),
                      (int) ((u32IP >> 16) & 0xff),
                      (int) ((u32IP >> 8) & 0xff), (int) ((u32IP >> 0) & 0xff));
    return;
}

static int strtok_ip_str2ul(char *pAddr,int *pAddrNum)                                                                                                  
{                                                                                                                                                
    char string[32] = {0};                                                                                                                       
    char *tokseps = ".";                                                                                                                         
    char *pt;                                                                                                                                    
    int i = 0;                                                                                                                                   
    if((NULL == pAddr) || (NULL == pAddrNum))                                                                                                    
    {                                                                                                                                            
        return -1;                                                                                                                               
    }                                                                                                                                            
                                                                                                                                                 
    strcpy(string, pAddr);                                                                                                                       
    pt = strtok(string,tokseps);                                                                                                                 
    while(pt)                                                                                                                                    
    {                                                                                                                                            
        pAddrNum[i] = atoi(pt);                                                                                                                  
        pt = strtok(NULL,tokseps);                                                                                                               
        i++;                                                                                                                                     
    }                                                                                                                                            
                                                                                                                                                 
    return 0;                                                                                                                                    
} 

static int get_desip_bysrcip(char *desIP, char *srcIP, int chipID)
{
    unsigned long uNativeIP;
    char pName[32] = {0};
        
    if((NULL == desIP) || (NULL == srcIP) )
    {
        return -1;
    }   

    uNativeIP = ntohl(inet_addr(srcIP));
    uNativeIP += chipID;
    uNativeIP = SWAP32(uNativeIP);

    ip2stringex(uNativeIP, pName);

    strcpy(desIP, pName);

    return 0;
}

static void system_network_perpear(void)
{
	int chipID = 0;
	char cmd[256] = {0};
	char MAC[18] = {0};
	char pLocalIP[32] = {0};

	chipID = gu32GpioChipValue;
	
	int pAddrNum[4] = {0};
	if (GOS_SUCCESS != strtok_ip_str2ul(gp_AddrHost,pAddrNum))
	{
		printf( "strtok_ip_str2ul error!\n");
		return;
	}
	
	sprintf(MAC, "00:1C:1D:00:%2x", pAddrNum[3]);
	
	MAC[14] = ':';
	MAC[15] = '0' + (chipID * 2)/10;
	MAC[16] = '0' + (chipID * 2)%10;
	MAC[17] = '\0';

	/*eth0 down and set MAC*/
	sprintf(cmd,"ifconfig eth1 down");
	system(cmd);
	sprintf(cmd,"ifconfig eth1 hw ether %s",MAC);
	system(cmd);

	if(GOS_SUCCESS !=  get_desip_bysrcip(pLocalIP, gp_AddrHost, chipID))
	{
		printf("PORT_SOCKET_GetIP_ByChipID Error\n");
		return;
	}
	
    sprintf(cmd,"ifconfig %s up", HI_TYPE_ETH);
    system(cmd);
    sprintf(cmd,"ifconfig %s %s", HI_TYPE_ETH, pLocalIP);
    system(cmd);

    printf("\n\nIP:\n%s\n\n", pLocalIP);

	return;
}

#define GPIO_MAX_NUM                4
#define HI_IO_BASE_ADDR             (0xf8a21000)
static void system_gpio_getvalue(void)
{
	int i = 0;
	int Ret = GOS_FAILURE;
	GOS_U32  Reg_Addr[GPIO_MAX_NUM] = {(HI_IO_BASE_ADDR + 0x074), (HI_IO_BASE_ADDR + 0x094), (HI_IO_BASE_ADDR + 0x080), (HI_IO_BASE_ADDR + 0x08C)};	
	GOS_U32  Reg_Mask[GPIO_MAX_NUM] = { 0xfffffff8, 0xfffffff8, 0xfffffff8, 0xfffffff8};
	GOS_U32  Reg_Value[GPIO_MAX_NUM] = { 0, 0, 0, 0,};
	GOS_U32  Gpio_Port[GPIO_MAX_NUM] = { 3*8+5, 4*8+5, 4*8+0, 4*8+3}; 
	GOS_U32  u32Value_read = 0;
	GOS_U32  u32Value_write = 0;

	
	/*identification slave number, you can set by toggle switch*/
	for(i = 0; i < GPIO_MAX_NUM; i++)
	{
		HI_SYS_ReadRegister(Reg_Addr[i], &u32Value_read);
		u32Value_write = (u32Value_read & Reg_Mask[i]) | Reg_Value[i];
		HI_SYS_WriteRegister(Reg_Addr[i], u32Value_write);
		Ret = HI_UNF_GPIO_SetDirBit(Gpio_Port[i], GOS_TRUE);
		if(GOS_SUCCESS != Ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
			return;
		}
	}

	GOS_U32 ReadBitVal[GPIO_MAX_NUM];
	for(i = 0; i < GPIO_MAX_NUM; i++)
	{
		Ret = HI_UNF_GPIO_ReadBit(Gpio_Port[i], (HI_BOOL *)&ReadBitVal[i]);
		if(GOS_SUCCESS != Ret)
		{
			printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
			return;
		}
	}

	gu32GpioChipValue = (ReadBitVal[0] * 8 + ReadBitVal[1] * 4 + ReadBitVal[2] * 2 + ReadBitVal[3]);
	
	printf("###---gu32GpioChipValue = %#x\n",gu32GpioChipValue);

	return;
}

/*get chip value(0x0~0xf)*/
GOS_U32 GOS_SYS_Get_ChipID()
{
	return gu32GpioChipValue;
}

GOS_S32 GOS_SYS_Init(void)
{
	if (HI_SYS_Init() != GOS_SUCCESS)
	{
		LOG_ERROR("HI_SYS_Init Error!\n");
		return GOS_FAILURE;
	}

	if (HI_UNF_GPIO_Init() != GOS_SUCCESS)
	{
		LOG_ERROR("HI_UNF_GPIO_Init Error\n");
		return GOS_FAILURE;
	}

	system_gpio_getvalue();
	system_network_perpear();

	LOG_INFO("PORT_SYS_Init success!!!!!!!\n");
	
	return GOS_SUCCESS;
}

GOS_S32 GOS_SYS_DeInit(void)
{
	if (HI_SYS_DeInit() != GOS_SUCCESS)
	{
		LOG_ERROR("HI_SYS_DeInit Error!\n");
		return GOS_FAILURE;
	}

	return GOS_SUCCESS;
}

#ifdef __cplusplus
}
#endif



