#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef __GOSTSR_COMMON_H__
#define __GOSTSR_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*无符号数据类型定义*/
#define GOSTSR_U8  unsigned char
#define GOSTSR_U16 unsigned short
#define GOSTSR_U32 unsigned int
#define GOSTSR_U64 unsigned long long

/*有符号数据类型定义*/
#define GOSTSR_S8  signed char
#define GOSTSR_S16 signed short
#define GOSTSR_S32 signed int
#define GOSTSR_S64 signed long

#define GOSTSR_D64 double
#define GOSTSR_F32 float


/*定义返回的两种状态*/
#define GOSTSR_SUCCESS 0
#define GOSTSR_FAILURE -1

/*定义空指针*/
#define GOSTSR_NULL (0L)

#define GOSTSR_BOOL signed char

#define GOSTSR_FALSE 	0
#define GOSTSR_TRUE 	1

/*定义位值*/
#define BIT0 0x00
#define BIT1 0x01
#define BIT2 0x02
#define BIT3 0x04
#define BIT4 0x08
#define BIT5 0x10
#define BIT6 0x20
#define BIT7 0x40
#define BIT8 0x80


#define PRINT_DEBUG(str,pos) printf("%-30s\tBytePos = %-20d\tPkt = %d\n",str,pos,pos/188);

typedef struct
{
	GOSTSR_U32 bytePos;
	GOSTSR_U32 startTime;
	GOSTSR_U32 endTime;
	GOSTSR_BOOL bCarryFlag;	/*进位标志，若bytePos大于0xFFFFFFFF时，则为True*/
}TR101290_ERROR_S;

typedef enum
{
	TYPE_ERROR_ONE = 0x01,
	TYPE_ERROR_TWO = 0x02,
	TYPE_ERROR_THREE = 0x04,
	
	TR101290_ERROR_MAX	
}TR101290_TYPE_ERROR_E;


typedef enum
{
	/*ONE_ERROR number:6*/
	TR101290_ONE_ERROR_SYNCLOSS 		= 0x000001,
	TR101290_ONE_ERROR_SYNCBYTE 		= 0x000002,
	TR101290_ONE_ERROR_PAT 				= 0x000004,
	TR101290_ONE_ERROR_PMT	 			= 0x000008,
	TR101290_ONE_ERROR_CONTINUTYCOUNT 	= 0x000010,
	TR101290_ONE_ERROR_PID 				= 0x000020,

	/*TWO_ERROR number:6*/
	TR101290_TWO_ERROR_TRANSPORT 		= 0x000040,
	TR101290_TWO_ERROR_CRC 				= 0x000080,
	TR101290_TWO_ERROR_PCRDISCONT 		= 0x000100,	/*PCR间隔错误*/
	TR101290_TWO_ERROR_ACCURACY 		= 0x000200,	/*PCR精度*/
	TR101290_TWO_ERROR_PTS 				= 0x000400,
	TR101290_TWO_ERROR_CAT 				= 0x000800,

	/*THREE_ERROR number:11*/
	TR101290_THREE_ERROR_NITACTUAL 		= 0x001000,
	TR101290_THREE_ERROR_NITOTHER 		= 0x002000,
	TR101290_THREE_ERROR_SIREPETITION 	= 0x004000,
	TR101290_THREE_ERROR_UNREFERENCEPID  = 0x008000,
	TR101290_THREE_ERROR_SDTACTUAL 		= 0x010000,
	TR101290_THREE_ERROR_SDTOTHER 		= 0x020000,
	TR101290_THREE_ERROR_EITACTUAL 		= 0x040000,
	TR101290_THREE_ERROR_EITOTHER 		= 0x080000,
	TR101290_THREE_ERROR_EITPF 			= 0x100000,
	TR101290_THREE_ERROR_RST 			= 0x200000,
	TR101290_THREE_ERROR_TDT 			= 0x400000,

	TR101290_THREE_ERROR_MAX
}TR101290_ERROR_LEVEL_E;

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
