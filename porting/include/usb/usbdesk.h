#ifndef _USB_MOUNT_H_
#define _USB_MOUNT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
#include <sched.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <semaphore.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>

#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <net/if.h>
#include <net/route.h>				/* network_parseRoutes*/
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/fb.h>
#include <linux/mii.h>				/*miire_read*/
#include <linux/sockios.h>			/*network_get_interface_name SIOCGIFCONF*/
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>		/*for rtnetlink*/

#define USB_MAX_DIR             16
#define MAX_NAME_LEN            128

#define Gsize                   (1024.0*1024.0*1024.0)
#define Msize                   (1024.0*1024.0)

#define USB_MOUNT_DIR           "/home/data/usb"
#define MOUNT_CMD_PARAM         "iocharset=cp936"
#define REPAIR_NTFS_USB         "chkntfs -fa"
#define USB_CHK_NTFS_OK         0x1

enum
{
    USB_NONE,
    USB_INSERT,
    USB_REMOVE,
    USB_ERROR
};

typedef enum
{
    SATA_DEV,
    USB_DEV
} DEVICE_TYPE;

typedef enum
{
    USB_STATUS_UNKNOWN      = 0,
    USB_STATUS_DISCONNECT   = 1,
    USB_STATUS_CONNECTED    = 2,
    USB_STATUS_MOUNTED      = 3,
    USB_STATUS_MOUNT_FAILED = 4,
    USB_STATUS_NOMAX        = 5,
    USB_DLGMSG_HIDE         = 6
} UsbStatus_t;

typedef enum
{
    DOS = 5,
    NTFS,
    EXT2,
    UNKNOW,
} USB_Partition_Type;

typedef enum
{
    DEV_NO_READY,
    DEV_READY,
} DEV_STATUS;

typedef struct
{
    char                sync;
    char                mount_name[MAX_NAME_LEN];
    char                original_name[16];
    DEV_STATUS          status;
    DEVICE_TYPE         dev_type;
    USB_Partition_Type  partition_type;
    unsigned long int   size;//M
    unsigned long int   free_size;//M
} DEV_ATTRIBUTE;

int USB_Init(void(*pfNotifier)(UsbStatus_t status));
int USB_Umount(void);
int USB_GetStatus(void);
int USB_GetAttribute(int dev_index, DEV_ATTRIBUTE *dev_attr, char get_size_info);
int USB_GetAttribute_In_Use(int dev_in_use_index, DEV_ATTRIBUTE *dev_attr, int *dev_index, char get_size_info);
int USB_GetPreferenceDevAttribute(int *dev_index, DEV_ATTRIBUTE *dev_attr, char get_size_info);
int USB_Judge_DevType(void);
int USB_SetAttribute(int dev_index, DEV_ATTRIBUTE *dev_attr);
int USB_SetSyncFlag(int dev_index);
int USB_GetCurDisk(void);
int USB_SetCurDisk(int Curdisk);
int USB_GetDiskPartNum(void);
int USB_GetExitedDisk(void);
int USB_SetNextValidDisk(int current_disk);
int USB_SetPrevValidDisk(int current_disk);

#ifdef __cplusplus
}
#endif
#endif
