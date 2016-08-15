/// @file user_define.h
/// @brief 
/// @author tengym <tengym@gospell.com>
/// 0.01_0
/// @date 2016-08-04

#ifndef __USER_DEFILE_H__                                                                                                                        
#define __USER_DEFILE_H__                                                                                                                        
#include <stdio.h>                                                                                                                               
#include <stdlib.h>                                                                                                                              
#include <string.h>                                                                                                                              
#include <unistd.h>                                                                                                                              
#include <fcntl.h>                                                                                                                          
#include <sys/socket.h>                                                                                                                     
#include <netinet/in.h>                                                                                                                     
#include <arpa/inet.h>                                                                                                                      
#include <errno.h>                                                                                                                          
#include <sys/types.h> 
                                                                                                                                                 
typedef unsigned char           GOS_U8;
typedef unsigned short          GOS_U16;
typedef unsigned int            GOS_U32;
typedef unsigned long long      GOS_U64;

typedef signed char             GOS_S8;
typedef short                   GOS_S16;
typedef int                     GOS_S32;
typedef long long               GOS_S64;

typedef char                    GOS_CHAR;
typedef char*                   GOS_PCHAR;

typedef float                   GOS_FLOAT;
typedef double                  GOS_DOUBLE;

#define GOS_VOID 				void

typedef GOS_U32                 GOS_HANDLE;

#define GOS_RECT_S				HI_RECT

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
typedef enum {
    GOS_FALSE    = 0,
    GOS_TRUE     = 1,
} GOS_BOOL;

#define GOS_NULL          0L

#define GOS_SUCCESS       0
#define GOS_FAILURE       (-1)

#define INVALID_HANDLE 0xFFFFFFFF

#ifndef Print
#define Print(fmt, arg  ... )  do{printf(fmt ,  ##arg );  }while(0);                                                                           
#endif

#ifndef DEBUG
#define DEBUG(fmt, arg  ... ) do{printf("[%s, %s: line %d]" fmt ,__FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);                        
#endif

#ifndef DEBUG_INFO
/*green*/                                                                                                                                        
#define DEBUG_INFO(fmt, arg  ... ) do{printf("\033[32m""[%s, %s: line %d]" fmt"\033[0m" ,__FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);
#endif

#ifndef DEBUG_ERROR
/*red*/                                                                                                                                          
#define DEBUG_ERROR(fmt, arg  ... ) do{printf("\033[31m""[%s, %s: line %d]" fmt"\033[0m" , __FILE__, __FUNCTION__, __LINE__,  ##arg );  }while(0);  
#endif

enum
{
    IPADDR_MULTICAST = 0,
    IPADDR_UNITCAST,
    IPADDR_MAX
};

#endif   
