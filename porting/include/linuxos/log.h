/***********************************************************************************
*             Copyright 2009 - 2050, Gospell Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
******************************************************************************
* FileName: log.h
* Description:S2项目公共日志头文件
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
	eOS_MODULE_NULL = 0,/*代表未定义模块或者所有模块*/
	eOS_MODULE_BUTT
};

/* 定义4个等级*/
typedef enum e_LOG_LEVEL_E
{
    E_LOG_LEVELDEBUG = 0,  /* debug-level               */
    E_LOG_LEVELERROR,      /* error conditions       	*/
    E_LOG_LEVELWARING,     /* waring conditions      	*/
    E_LOG_LEVELINFO,       /* informational             */
    E_LOG_LEVELBUTT
}LOG_LEVEL_E;

/*-----------------------------------------------------------------
无模块的日志输出宏:
LOG_DEBUG、LOG_INFO一般用于调试，没有错误码，
LOG_ERROR和LOG_FATAL一般用于日志，有错误码
LOG_OUTOUTF通用于所有情况
-----------------------------------------------------------------*/
#ifndef     __THIS_LOG_MOD_ID
#define     __THIS_LOG_MOD_ID   eOS_MODULE_NULL
#endif

/* 通用输出信息函数*/
#define LOG_OUTPUTF(enErrLevel, pszFormat...)                           \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, (enErrLevel), (GOS_FAILURE),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

/* 错误信息输出，带指定错误码*/
#define LOG_ERROR_NO(ErrNumber , pszFormat...)                            \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELERROR, (ErrNumber),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

// 错误信息输出
#define LOG_ERROR( pszFormat...)                                            \
    (HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELERROR, (GOS_FAILURE),\
                         __FILE__, __FUNCTION__, __LINE__, pszFormat)

/*错误信息输出*/
#define LOG_DEBUG( pszFormat...)                                            \
		(HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELDEBUG, (GOS_FAILURE),\
							 __FILE__, __FUNCTION__, __LINE__, pszFormat)

/* 错误信息输出*/
#define LOG_INFO( pszFormat...)                                            \
		(HI_VOID)LOG_Outputf(__THIS_LOG_MOD_ID, E_LOG_LEVELINFO, (GOS_FAILURE),\
							 __FILE__, __FUNCTION__, __LINE__, pszFormat)


#define dump()  //LOG_OUTPUTF(E_LOG_LEVELERROR, "%s, %d\n", __FUNCTION__, __LINE__)


/*函数参数检查*/
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
/*检查期望值，如果不对，返回该指定错误码 , val要写错误情况的判断*/
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
/* 检查期望值，如果不对，打印错误信息, val要写错误情况的判断*/
#define LOG_CHK_PRINT_ERR(val, ret)                     	\
    do                                                      \
    {                                                       \
        if ((val))                                          \
        {                                                   \
            LOG_ERROR_NO(ret , "");                         \
        }                                                   \
    } while (0)

/*-----------------------------------------------------------------*/
/* 如果func执行失败，只打印出错信息，不返回 */
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
/* 如果func执行失败，则直接返回被调函数的返回值 */
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
/* 如果func执行失败，则返回指定的错误码 */
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
/* 如果func执行失败，则返回void */
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
* Description:  初始化日志功能
* Input:
* Output:
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Init(void);

/***********************************************************************************
* Function:     LOG_DeInit
* Description:  去初始化日志功能
* Input:
* Output:
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_DeInit(void);

/***********************************************************************************
* Function:     LOG_Outputf
* Description:  输出日志
* Input:    u32ModID:模块ID号
            enErrLevel:错误码级别
            enoErrno:错误码
            pszProgramFileName:出错的源代码程序文件名
            pszProgramFunctionName:出错的源代码程序函数名
            s32ProgramLineNo:出错的源代码行号
            pszFormat:输出日志字符串格式，用法与printf相同
* Output:
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_Outputf(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel, GOS_S32 enoErrno,
                const char *pszProgramFileName, const char *pszProgramFunctionName, GOS_S32 s32ProgramLineNo,
                const char *pszFormat, ...);

/***********************************************************************************
* Function:     LOG_SetAllowOutput
* Description:  设置是否允许输出日志
* Input:    u32ModID:模块ID号
            enErrLevel:错误码级别
            UsbPath:USB路径，为空是不输出到USB设备中
            pFLog:重定向之后的文件句柄
* Output:   无
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_SetAllowOutput(GOS_U32 u32ModID, LOG_LEVEL_E enErrLevel);

/***********************************************************************************
* Function:     LOG_StartOutputToUSB
* Description:  开始讲标准输出重定向到USB中
* Input:    UsbPath:USB路径，为空是不输出到USB设备中
* Output:   无
* Return:   SUCCESS: success
*           FAILURE: failed
***********************************************************************************/
GOS_S32 LOG_StartOutputToUSB(char *UsbPath);

/***********************************************************************************
* Function:     LOG_StopOutputToUSB
* Description:  停止讲标准输出重定向到USB中
* Input:    无
* Output:   无
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
