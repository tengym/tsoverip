/***********************************************************************************
*             Copyright 2009 - 2050, Gospell Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: log.h
* Description:S2��Ŀ������־ͷ�ļ�
*
* History:
* Version  Date        Author                               DefectNum  Description
* 1.0      2013/08/13  Gospell STB Step-1 software group  NULL       Create this file.
***********************************************************************************/

#ifndef __OS_LOG_H__
#define __OS_LOG_H__

#include "linuxos.h"
#include "user_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#define ERROR_HEAD			"%s-%s-%d:"
#define ERROR_HEAD_MEMBER	__FILE__,__FUNCTION__,__LINE__

#define HI_VOID void
enum
{
	eOS_MODULE_NULL = 0,/*����δ����ģ���������ģ��*/
	eOS_MODULE_BUTT
};

/* ����4���ȼ�*/
typedef enum e_LOG_LEVEL_E
{
    E_LOG_LEVELDEBUG = 0,  /* debug-level               */
    E_LOG_LEVELERROR,      /* error conditions       	*/
    E_LOG_LEVELWARING,     /* waring conditions      	*/
    E_LOG_LEVELINFO,       /* informational             */
    E_LOG_LEVELBUTT
}LOG_LEVEL_E;

/*-----------------------------------------------------------------
��ģ�����־�����:
LOG_DEBUG��LOG_INFOһ�����ڵ��ԣ�û�д����룬
LOG_ERROR��LOG_FATALһ��������־���д�����
LOG_OUTOUTFͨ�����������
-----------------------------------------------------------------*/
#ifndef     __THIS_LOG_MOD_ID
#define     __THIS_LOG_MOD_ID   eOS_MODULE_NULL
#endif

/* ͨ�������Ϣ����*/
#define LOG_OUTPUTF(enErrLevel, pszFormat...)                           \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, (enErrLevel), (GOS_FAILURE),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

/* ������Ϣ�������ָ��������*/
#define LOG_ERROR_NO(ErrNumber , pszFormat...)                            \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELERROR, (ErrNumber),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

// ������Ϣ���
#define LOG_ERROR( pszFormat...)                                            \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELERROR, (GOS_FAILURE),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

/*������Ϣ���*/
#define LOG_DEBUG( pszFormat...)                                            \
		(HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELDEBUG, (GOS_FAILURE),\
							 __FILE__, __FUNCTION__, __LINE__, pszFormat)

/* ������Ϣ���*/
#define LOG_INFO( pszFormat...)                                            \
		(HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELINFO, (GOS_FAILURE),\
							 __FILE__, __FUNCTION__, __LINE__, pszFormat)


#define dump()  //LOG_OUTPUTF(E_LOG_LEVELERROR, "%s, %d\n", __FUNCTION__, __LINE__)


/*�����������*/
#define LOG_CHK_PARA( val  )                            	\
    do                                                      \
    {                                                       \
        if ((val))                                          \
        {                                                   \
            LOG_OUTPUTF(E_LOG_LEVELERROR ,"");              \
            return HI_EINVAL;                               \
        };                                                  \
    } while (0)

/*-----------------------------------------------------------------*/
/*�������ֵ��������ԣ����ظ�ָ�������� , valҪд����������ж�*/
#define LOG_CHK_RETURN_ERR(val, ret )                   	\
    do                                                      \
    {                                                       \
        if ((val))                                          \
        {                                                   \
            LOG_ERROR_NO(ret , "");                         \
            return ret;                                     \
        }                                                   \
    } while (0)

/*-----------------------------------------------------------------*/
/* �������ֵ��������ԣ���ӡ������Ϣ, valҪд����������ж�*/
#define LOG_CHK_PRINT_ERR(val, ret)                     	\
    do                                                      \
    {                                                       \
        if ((val))                                          \
        {                                                   \
            LOG_ERROR_NO(ret , "");                         \
        }                                                   \
    } while (0)

/*-----------------------------------------------------------------*/
/* ���funcִ��ʧ�ܣ�ֻ��ӡ������Ϣ�������� */
#define LOG_DOFUNC( func )                              	\
    do{                                                     \
        GOS_S32 ret = 0 ;                                       \
        ret = func ;                                        \
        if (ret != SUCCESS)                                 \
        {                                                   \
            LOG_ERROR_NO(ret, "CALL %s", #func);            \
        };                                                  \
    }while(0)

/*-----------------------------------------------------------------*/
/* ���funcִ��ʧ�ܣ���ֱ�ӷ��ر��������ķ���ֵ */
#define LOG_DOFUNC_RETURN( func )                       	\
    do{                                                     \
        GOS_S32 ret = 0 ;                                       \
        ret = func ;                                        \
        if (ret != SUCCESS)                                 \
        {                                                   \
            LOG_ERROR_NO(ret, "CALL %s", #func);            \
            return ret;                                     \
        };                                                  \
    }while(0)

/*-----------------------------------------------------------------*/
/* ���funcִ��ʧ�ܣ��򷵻�ָ���Ĵ����� */
#define LOG_DOFUNC_RETURN_ERR( func ,err)               	\
    do{                                                     \
        GOS_S32 ret = 0 ;                                       \
        ret = func ;                                        \
        if (ret != SUCCESS)                                 \
        {                                                   \
            LOG_ERROR_NO(ret, "CALL %s", #func);            \
            return err;                                     \
        };                                                  \
    }while(0)

/*-----------------------------------------------------------------*/
/* ���funcִ��ʧ�ܣ��򷵻�void */
#define LOG_DOFUNC_RETURN_VOID(func)                    	\
    do{                                                     \
        GOS_S32 ret = func;                                     \
        if (ret != SUCCESS)                                 \
        {                                                   \
            LOG_ERROR_NO(ret, "CALL %s", #func);            \
            return ;                                        \
        }                                                   \
    }while(0)


#define LOG_DOFUNC_GOTO(func, errhandle)								  \
    do{                                                                    \
        GOS_S32 s32Ret = func;                                                 \
        if (s32Ret != SUCCESS )                                            \
        {                                                                  \
            LOG_ERROR("%s fail:0x%x, line:%d\n", #func, s32Ret, __LINE__); \
            goto errhandle;                                                \
        }                                                                  \
    }while(0)



/***********************************************************************************
* Function:     LOG_Init
* Description:  ��ʼ����־����
* Input:
* Output:
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Init(void);

/***********************************************************************************
* Function:     LOG_DeInit
* Description:  ȥ��ʼ����־����
* Input:
* Output:
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_DeInit(void);

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
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Outputf(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel, GOS_S32 enoErrno,
                const char *pszProgramFileName, const char *pszProgramFunctionName, GOS_S32 s32ProgramLineNo,
                const char *pszFormat, ...);

/***********************************************************************************
* Function:     LOG_SetAllowOutput
* Description:  �����Ƿ����������־
* Input:    u32ModID:ģ��ID��
            enErrLevel:�����뼶��
            UsbPath:USB·����Ϊ���ǲ������USB�豸��
            pFLog:�ض���֮����ļ����
* Output:   ��
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_SetAllowOutput(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel);

/***********************************************************************************
* Function:     LOG_StartOutputToUSB
* Description:  ��ʼ����׼����ض���USB��
* Input:    UsbPath:USB·����Ϊ���ǲ������USB�豸��
* Output:   ��
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_StartOutputToUSB(char *UsbPath);

/***********************************************************************************
* Function:     LOG_StopOutputToUSB
* Description:  ֹͣ����׼����ض���USB��
* Input:    ��
* Output:   ��
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_StopOutputToUSB(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /*__LOG_H__*/
