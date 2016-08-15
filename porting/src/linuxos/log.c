#include "linuxos.h"
#include "log.h"

#define PRINTF_COL_GREEN	"\033[1;32m"
#define PRINTF_COL_BLUE		"\033[1;34m"
#define PRINTF_COL_RED		"\033[1;31m"
#define PRINTF_COL_NONE		"\033[0m"

#define LOG_PRINTF          printf

/* ��ӡ�ȼ������ļ�·�� */
#define LOG_CFG_FILE         "/tmp/log.ini"

/* ÿ����־������ֽ��� */
#define LOG_SIZE_MAX         256

/* ģ�����ֵ�����ֽ��� */
#define MODULE_NAME_LEN      16

/* ģ������ */
#define MODULE_ALL_NAME      "ALL"

typedef struct
{
    char         cModName[MODULE_NAME_LEN];
    LOG_LEVEL_E  eLogLevel;
} LOG_CFG_S;

static  FILE        *gLogFileptr = NULL;
static  GOS_BOOL     gLogModInit = GOS_FALSE;
static  LOG_CFG_S    stLogCfgInfo[eOS_MODULE_BUTT];

static GOS_S32 log_add_module(int mod_id, char *mod_name, LOG_LEVEL_E log_level)
{
    if (eOS_MODULE_BUTT <= mod_id)
    {
        LOG_PRINTF("[%s][%d]The Module ID is invaild...\n", __FUNCTION__, __LINE__);
        return GOS_FAILURE;
    }

    if ((!mod_name) || (log_level >= E_LOG_LEVELBUTT))
    {
        LOG_PRINTF("[%s][%d]Please check input param...\n", __FUNCTION__, __LINE__);
        return GOS_FAILURE;
    }

    strncpy(stLogCfgInfo[mod_id].cModName, mod_name, strlen(mod_name));
    stLogCfgInfo[mod_id].eLogLevel = log_level;

    return GOS_SUCCESS;
}

static GOS_S32 log_creat_cfg_file(char *cfg_path)
{
    int   index = 0;
    char  buffer[MODULE_NAME_LEN * 2];
    FILE *fp_log = NULL;

    if (NULL == (fp_log = fopen(cfg_path, "wb")))
    {
        LOG_PRINTF("[%s][%d]Open log cfg file failure\n", __FUNCTION__, __LINE__);
        return GOS_FAILURE;
    }

    for (index = 0; index < eOS_MODULE_BUTT; index++)
    {
        if (stLogCfgInfo[index].cModName)
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, 32, "%s=%d\n", stLogCfgInfo[index].cModName, stLogCfgInfo[index].eLogLevel);
            fputs(buffer, fp_log);
        }
    }

    fputs("\n*********Level*********\n", fp_log);
    fputs("Error:1 Waring:2 Info:3", fp_log);
    fputs("\n***********************\n", fp_log);

    fclose(fp_log);

    return GOS_SUCCESS;
}

static GOS_S32 log_parse_cfg_file(char *cfg_path)
{
    int   index = 0;
    char  buffer[MODULE_NAME_LEN * 2];
    FILE *fp_log = NULL;

    if (NULL == (fp_log = fopen(cfg_path, "rb")))
    {
        LOG_PRINTF("[%s][%d]Open log cfg file failure\n", __FUNCTION__, __LINE__);
        return GOS_FAILURE;
    }

    while (!feof(fp_log))
    {
        char *ptr = fgets(buffer, sizeof(buffer), fp_log);

        if (ptr)
        {
            for (index = 0; index < eOS_MODULE_BUTT; index++)
            {
                if (!memcmp(stLogCfgInfo[index].cModName, buffer, strlen(stLogCfgInfo[index].cModName)))
                {
                    stLogCfgInfo[index].eLogLevel = E_LOG_LEVELINFO;//Ĭ�ϴ�ӡ�ȼ�

                    char *ptr = strchr(buffer, '=');

                    if (ptr)
                    {
                        ptr++;

                        if (atoi(ptr) < E_LOG_LEVELBUTT)
                        {
                            stLogCfgInfo[index].eLogLevel = atoi(ptr);
                        }
                    }

                    break;
                }
            }
        }
    }

    fclose(fp_log);

    return GOS_SUCCESS;
}

static GOS_BOOL log_level_check(GOS_S32 s32Levle, GOS_U32 u32ModId)
{
    if (GOS_FALSE == gLogModInit)
    {
        return GOS_TRUE;
    }

    if (stLogCfgInfo[eOS_MODULE_NULL].eLogLevel >= E_LOG_LEVELINFO)
    {
        return GOS_TRUE;
    }

    if ((u32ModId >= eOS_MODULE_BUTT) ||
        (s32Levle > (GOS_S32)stLogCfgInfo[u32ModId].eLogLevel))
    {
        return GOS_FALSE;
    }

    return GOS_TRUE;
}

static void log_level_colour(GOS_S32 s32Levle)
{
	switch (s32Levle)
	{
		case E_LOG_LEVELERROR:
			LOG_PRINTF(PRINTF_COL_RED);
			break;

		case E_LOG_LEVELWARING:
			LOG_PRINTF(PRINTF_COL_GREEN);
			break;

		default:
			LOG_PRINTF(PRINTF_COL_NONE);
			break;
	}
}

static void log_output2usb(void *param)
{
    int    count = 0;

    while (gLogFileptr)
    {
        if (count++ > 5)
        {
            fflush(gLogFileptr);
            count = 0;
        }

        usleep(200 * 1000);
    }

    return;
}

/***********************************************************************************
* Function:     LOG_Init
* Description:  ��ʼ����־����
* Input:
* Output:
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Init(void)
{
    GOS_S32 s32Ret = GOS_SUCCESS;

    if (gLogModInit)
    {
        LOG_PRINTF("The Module of log have initializated...\n");
        return s32Ret;
    }

    /*ע�����ģ������ֺʹ�ӡ�ȼ�*/
    log_add_module(eOS_MODULE_NULL, MODULE_ALL_NAME, E_LOG_LEVELINFO);

    /*�ж������ļ��Ƿ���ڣ���������ֱ�Ӷ�ȡ�����������ؽ������ļ�*/
    if (GOS_SUCCESS == access(LOG_CFG_FILE, F_OK))
    {
        s32Ret = log_parse_cfg_file(LOG_CFG_FILE);
    }
    else
    {
        s32Ret = log_creat_cfg_file(LOG_CFG_FILE);
    }

    if (GOS_SUCCESS == s32Ret)
    {
        gLogModInit = GOS_TRUE;
        LOG_PRINTF("The Module of log initializate success...\n");
    }

    return s32Ret;
}

/***********************************************************************************
* Function:     LOG_DeInit
* Description:  ȥ��ʼ����־����
* Input:
* Output:
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_DeInit(void)
{
    int   index = 0;

    for (index = 0; index < eOS_MODULE_BUTT; index++)
    {
        if (stLogCfgInfo[index].cModName)
        {
            stLogCfgInfo[index].eLogLevel = E_LOG_LEVELINFO;
        }
    }

    gLogModInit = GOS_FALSE;

    return GOS_SUCCESS;
}

/***********************************************************************************
* Function:     LOG_Outputf
* Description:  �����־
* Input:    u32ModID:ģ��ID��
            enErrLevel:�����뼶��
            enoErrno:������
            pszProgramFileName:�����Դ��������ļ���
            pszProgramFunctionName:�����Դ�����������
            s32ProgramLineNo:�����Դ�����к�
            pszFormat:�����־�ַ�����ʽ���÷���printf��ͬ
* Output:
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Outputf(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel, GOS_S32 enoErrno,
                const char *pszProgramFileName, const char *pszProgramFunctionName, GOS_S32 s32ProgramLineNo,
                const char *pszFormat, ...)
{
    va_list args;
    GOS_S32  MsgLen = 0;
    GOS_BOOL bLevel = GOS_FALSE;
    char log_str[LOG_SIZE_MAX] = {0};
	
    if (GOS_FALSE != gLogModInit)
    {
        bLevel = log_level_check(enErrLevel, u32ModID);
    }

    if ((GOS_FALSE == gLogModInit) || bLevel)
    {
        va_start(args, pszFormat);
        MsgLen = vsnprintf(log_str, LOG_SIZE_MAX, pszFormat, args);
        va_end(args);

        if (MsgLen >= LOG_SIZE_MAX)
        {
            log_str[LOG_SIZE_MAX - 1] = '\0'; /* even the 'vsnprintf' commond will do it */
            log_str[LOG_SIZE_MAX - 2] = '\n';
            log_str[LOG_SIZE_MAX - 3] = '.';
            log_str[LOG_SIZE_MAX - 4] = '.';
            log_str[LOG_SIZE_MAX - 5] = '.';
        }

		log_level_colour(enErrLevel);
        LOG_PRINTF("%s %s[%d]:%s", pszProgramFileName, pszProgramFunctionName, s32ProgramLineNo, log_str);
		LOG_PRINTF(PRINTF_COL_NONE);

        if (GOS_FALSE == gLogModInit)
        {
            return GOS_FAILURE;
        }

        if (bLevel)
        {
            return GOS_SUCCESS;
        }
    }

    return GOS_FAILURE;
}

/***********************************************************************************
* Function:     LOG_SetAllowOutput
* Description:  �����Ƿ����������־
* Input:    u32ModID:ģ��ID��
            enErrLevel:�����뼶��
            UsbPath:USB·����Ϊ���ǲ������USB�豸��
            pFLog:�ض���֮����ļ����
* Output:   ��
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_SetAllowOutput(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel)
{
    if ((u32ModID >= eOS_MODULE_BUTT) || (enErrLevel >= E_LOG_LEVELBUTT))
    {
        LOG_PRINTF("[%s][%d]Please check input param...\n", __FUNCTION__, __LINE__);
        return GOS_FAILURE;
    }

    stLogCfgInfo[u32ModID].eLogLevel = enErrLevel;

    return GOS_SUCCESS;
}

/***********************************************************************************
* Function:     LOG_StartOutputToUSB
* Description:  ��ʼ����׼����ض���USB��
* Input:    UsbPath:USB·����Ϊ���ǲ������USB�豸��
* Output:   ��
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_StartOutputToUSB(char *UsbPath)
{
    GOS_S32    s32Ret = GOS_SUCCESS;
    pthread_t  logthread;

    if (UsbPath)
    {
        if ((gLogFileptr = freopen(UsbPath, "w+", stdout)) == NULL)
        {
            LOG_PRINTF("----------------io err----------------\n");
            return GOS_FAILURE;
        }

        s32Ret = pthread_create(&logthread, NULL, (void *)log_output2usb, UsbPath);

        LOG_PRINTF("start record log to log.txt...\n");
    }

    return s32Ret;
}

/***********************************************************************************
* Function:     LOG_StopOutputToUSB
* Description:  ֹͣ����׼����ض���USB��
* Input:    ��
* Output:   ��
* Return:   GOS_SUCCESS: success
*           GOS_FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_StopOutputToUSB(void)
{
    gLogFileptr = NULL;

    if (NULL == (freopen("/dev/console", "w", stdout)))
    {
        return GOS_FAILURE;
    }

    LOG_PRINTF("stop record log to log.txt...\n");

    return GOS_SUCCESS;
}

