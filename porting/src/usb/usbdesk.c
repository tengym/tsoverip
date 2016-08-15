/* Include files ------------------------------------------------------------*/
#include "usbdesk.h"

/* Definition ---------------------------------------------------------------*/

#define USB_DETECT_DIR              "/dev"
#define MIN_SUPPORT_CAPACITY        32//容量少于10M的USB设备不支持
#define MSCALE(b,bs)                (((b) * (long long) (bs)) / Msize)

static int GosCfg_USB_Print        = 0; 
/* Private Variant ----------------------------------------------------------*/
static int          PartitionNum = 0;
static int          usb_sdX_number = 0;
static int          usb_current_disk = -1;
static int          usb_partition_to_dev[USB_MAX_DIR];
static char         dev_usb_dir[USB_MAX_DIR][50];
static void         (*lv_pfNotifier)(UsbStatus_t status) = NULL;
static pthread_t    hUsbTaskHandle;
static UsbStatus_t  lv_UsbConStatus = USB_STATUS_UNKNOWN;
DEV_ATTRIBUTE       dev_attribute[USB_MAX_DIR];
USB_Partition_Type  usb_partition_type[USB_MAX_DIR];

static FILE        *stream = NULL;
static char         debug_path[128];
static char         to_usb_log = 0;
static int          number = 0;

/* Function Prototype -------------------------------------------------------*/
void               *UsbCon_Task                 (void *param);
int                 USB_Umount                  (void);
static int          UsbCon_DetectConnection     (void);
static int          UsbCon_Mount                (void);
void                UsbCon_SortAllDevice  (void);


/*===========================================================================*/
// Function Declaration
/*===========================================================================*/
/*---------------------------------------------------------------------------
Function:   USB_Init
-----------------------------------------------------------------------------*/
int USB_Init(void (*pfNotifier)(UsbStatus_t status))
{
    lv_pfNotifier   = pfNotifier;
    PartitionNum    = 0;
    usb_sdX_number  = 0;
    usb_current_disk = -1;
    memset(dev_usb_dir, 0, 50 * USB_MAX_DIR);
    memset(usb_partition_to_dev, 0, sizeof(usb_partition_to_dev));
    memset(dev_attribute, 0, sizeof(DEV_ATTRIBUTE)*USB_MAX_DIR);

    if (pthread_create(&hUsbTaskHandle, NULL, UsbCon_Task, NULL) != 0)
    {
        printf("Failed to create UsbCon_Task !\n");
        return -1;
    }

    return 0;
}

/*---------------------------------------------------------------------------
Function:   UsbCon_Task
-----------------------------------------------------------------------------*/
void *UsbCon_Task(void *param)
{
    int retvalue;

    while (1)
    {
        usleep(200 * 1000);
        retvalue = UsbCon_DetectConnection();

        if (retvalue == USB_INSERT)//add a dev
        {
            usleep(500 * 1000);
            printf("insert a usb device ....\n");

            if (UsbCon_Mount() == 0)
            {
                printf("usb mount ok\n");

                if (usb_current_disk < 0)
                {
                    usb_current_disk = 0;
                }

                lv_UsbConStatus = USB_STATUS_MOUNTED;

                if (lv_pfNotifier != NULL)
                {
                    lv_pfNotifier(USB_STATUS_MOUNTED);
                }

                UsbCon_SortAllDevice();
            }
        }
        else if (retvalue == USB_REMOVE)//remove a dev
        {
            printf("remove a usb device ....\n");

            if (lv_UsbConStatus == USB_STATUS_MOUNTED)
            {
                if (USB_Umount() == 0)
                {
                    printf("usb ummount ok\n");

                    if (USB_GetDiskPartNum() <= 0)
                    {
                        usb_current_disk = -1;
                    }

                    if (PartitionNum <= 0)
                    {
                        lv_UsbConStatus = USB_STATUS_DISCONNECT;
                    }

                    if (lv_pfNotifier != NULL)
                    {
                        lv_pfNotifier(USB_STATUS_DISCONNECT);
                    }

                    UsbCon_SortAllDevice();
                }
            }
        }

        if (GosCfg_USB_Print)
        {
            if (to_usb_log)
            {
                number ++;
            }

            if (number >= 5 && to_usb_log == 1 && stream != NULL)
            {
                fflush(stream);
				sync();
                number = 0;
            }
        }
    }

    param = NULL;
    return (NULL);
}

//-----------------------------------------------------------------------------
//  FUNCTION:   UsbCon_DetectConnection()
//  DESRIPTION: Detect if usb device is connected.
//  RETURN:     1, if connected;
//              0, if disconnected.
//              -1,if error encountered.
//-----------------------------------------------------------------------------
static int UsbCon_DetectConnection( void )
{
    int             i = 0;
    DIR             *pDir = NULL;
    struct dirent   *pDirEnt = NULL;

    if ((pDir = opendir(USB_DETECT_DIR)) == NULL)
    {
        return USB_ERROR;
    }

    while ((pDirEnt = readdir(pDir)) != NULL)
    {
        if ( pDirEnt->d_name[0] == 's' && pDirEnt->d_name[1] == 'd' )  // add a node.
        {
            i++;
        }
    }

    closedir(pDir);

    if (i != usb_sdX_number)
    {
        printf("i:%d usb_sdX_number:%d\n", i, usb_sdX_number);

        if (i > usb_sdX_number)//insert a usb disk
        {
            usb_sdX_number = i;
            return USB_INSERT;
        }
        else//remove a usb disk
        {
            usb_sdX_number = i;
            return USB_REMOVE;
        }
    }

    return USB_NONE;
}

//-----------------------------------------------------------------------------
//  FUNCTION:   UsbCon_Chkntfs()
//  DESRIPTION: Check the NTFS USB storage device
//  RETURN:     0 success.
//              -1 failed.
//-----------------------------------------------------------------------------
int UsbCon_Chkntfs(const char *pcExtName)
{
    int s32Status;
    pid_t  child_pid;
    char *argv_list[] =
    {
        (char *)"chkntfs",
        NULL,
        (char *)"-fa",
        NULL
    };

    if (NULL == pcExtName)
    {
        return -1;
    }

    argv_list[1] = (char *)pcExtName;
    child_pid = fork();

    if (0 == child_pid)
    {
        execvp((const char *)argv_list[0], (char * const *)argv_list);
        printf("on error occurred in execvp!\r\n");
        abort();
    }

    if (-1 == child_pid)
    {
        return -1;
    }

    waitpid(child_pid, &s32Status, 0);

    if (WIFEXITED(s32Status))
    {
        return (WEXITSTATUS(s32Status));
    }

    return -1;
}

//-----------------------------------------------------------------------------
//  FUNCTION:   UsbCon_Mount()
//  DESRIPTION: Mount the USB storage device
//  RETURN:     0 success.
//              -1 failed.
//-----------------------------------------------------------------------------
static int UsbCon_Mount( void )
{
    int                 i = 0, k = 0;
    int                 Result = 0;
    int                 tmp_pn = 0;
    int                 is_sata_dev = -1;
    char                sata_name[8] = {0};
    char                usb_dir_buf[USB_MAX_DIR][32];
    char                ShellCommand[256] = {0};
    char                source[128] = {0}, target[128] = {0};
    DIR                *pDir = NULL;
    struct statfs       sfs ;
    struct dirent      *pDirEnt = NULL;
    unsigned char       bGotDevFileName = 0;
    unsigned long int   size = 0;

    if ((pDir = opendir(USB_DETECT_DIR)) == NULL)
    {
        printf("[UsbCon_Mount]:GetUsbDevFileName(): Failed to open dir.\n");
        return -1;
    }

    while ((pDirEnt = readdir(pDir)) != NULL)
    {
        //printf("pDirEnt->d_name:%s\n",pDirEnt->d_name);
        if ((pDirEnt->d_name[0] == 's') && (pDirEnt->d_name[1] == 'd'))
        {
        	memset(usb_dir_buf[tmp_pn], 0, 32);
            strncpy(usb_dir_buf[tmp_pn], pDirEnt->d_name, strlen(pDirEnt->d_name));
            tmp_pn++;
        }
    }

    closedir(pDir);

    if (tmp_pn == 0)
    {
        printf("[UsbCon_Mount] fail. PartitionNum == 0 \n");
        PartitionNum = 0;
        return -1;
    }

    printf("[UsbCon_Mount] patition = %d tmp_pn = %d\n", PartitionNum, tmp_pn);

    if ((pDir = opendir(USB_MOUNT_DIR)) == NULL)
    {
        printf("[USB_Umount]: Failed to open home dir.\n");
        Result = mkdir(USB_MOUNT_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    else
    {
        if (PartitionNum == 0)
        {
            while ( (pDirEnt = readdir(pDir)) != NULL )
            {
                if ( pDirEnt->d_name[0] == 'p' && pDirEnt->d_name[1] == 'a' && pDirEnt->d_name[2] == 'r' )
                {
                    memset(ShellCommand, 0, sizeof(ShellCommand));
                    snprintf(ShellCommand, sizeof(ShellCommand), "%s/%s", USB_MOUNT_DIR, pDirEnt->d_name);
                    umount(ShellCommand);
                }
            }
        }
    }

    closedir(pDir);

    char old_partition_num = PartitionNum;

    if (PartitionNum == 0)
    {
        bGotDevFileName = 0;

        for (i = 0; (i < tmp_pn) && (tmp_pn < USB_MAX_DIR); i++)
        {
            printf(">>> usb dir buf[%d]:%s \n", i, usb_dir_buf[i]);
            memset(target, 0, sizeof(target));
            snprintf(target, sizeof(target), "%s/partition%d", USB_MOUNT_DIR, bGotDevFileName);
            printf("[UsbCon_Mount 0] partition name : %s\n", target);

            if (access(target, F_OK) != 0)  //测试文件是否存在
            {
                Result = mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

                if (Result != 0)
                {
                    printf("mkdir error!!!");
                    bGotDevFileName ++;
                    continue;
                }
            }

            memset(source, 0, sizeof(source));
            snprintf(source, 128, "/dev/%s", usb_dir_buf[i]);
            printf("[UsbCon_Mount 0]source:%s target:%s\n", source, target);

            memset(ShellCommand, 0, sizeof(ShellCommand));
            snprintf(ShellCommand, sizeof(ShellCommand), "mount %s %s", source, target);

#if 0
            Result = mount(source, target, "vfat", MS_MGC_VAL, MOUNT_CMD_PARAM); //NTFS格式

#else
            Result = system(ShellCommand);
#endif

            if (Result != 0)
            {
                Result = mount(source, target, "ext2", MS_MGC_VAL, MOUNT_CMD_PARAM);

                if (Result != 0)
                {
                    if (USB_CHK_NTFS_OK == UsbCon_Chkntfs((char *)source))
                    {
                        Result = mount(source, target, "ufsd", MS_MGC_VAL, MOUNT_CMD_PARAM);
                        if (Result != 0)
                        {
                            printf("mount ext2,ntfs,vfat err....\n");
                            Result = system(ShellCommand);
                            printf("--->Result :%d ShellCommand:%s\n", Result, ShellCommand);
                        }
                    }
                    else
                    {
                        Result = mount(source, target, "ufsd", MS_MGC_VAL, MOUNT_CMD_PARAM);
                        printf("--->the result is %d\n", Result);
                    }
                }
            }

            printf("mount result is %d the target is %s\n", Result, target);

            if (Result == 0)
            {
                if (statfs(target, &sfs) != -1)
                {
                    size = MSCALE(sfs.f_blocks, sfs.f_bsize);
                    printf("[UsbCon_Mount 0] file size:%lld, f_bsize:%lld, f_blocks:%lld \n", (long long int)size, (long long int)sfs.f_bsize,
                           (long long int)sfs.f_blocks);

                    if (size <= MIN_SUPPORT_CAPACITY)
                    {
                        umount(target);
                    }
                    else
                    {
                        strncpy(dev_attribute[PartitionNum].mount_name, target, strlen(target));

                        if (sfs.f_type == 0x4d44)
                        {
                            dev_attribute[PartitionNum].partition_type = DOS;
                        }
                        else if (sfs.f_type == 0x5346544e)
                        {
                            dev_attribute[PartitionNum].partition_type = NTFS;
                        }
                        else if (sfs.f_type == 0xef53)
                        {
                            dev_attribute[PartitionNum].partition_type = EXT2;
                        }
                        else
                        {
                            dev_attribute[PartitionNum].partition_type = UNKNOW;
                        }

                        if (is_sata_dev == 0 && strstr(source, sata_name) != 0)
                        {
                            dev_attribute[PartitionNum].dev_type = SATA_DEV;
                        }
                        else
                        {
                            dev_attribute[PartitionNum].dev_type = USB_DEV;
                        }

                        dev_attribute[PartitionNum].status = DEV_READY;
                        dev_attribute[PartitionNum].size = size;
                        dev_attribute[PartitionNum].free_size = MSCALE(sfs.f_bfree, sfs.f_bsize);

                        strncpy(dev_attribute[PartitionNum].original_name, source, strlen(source));
                        printf("----0----mount ok,sourc:%s target:%s----------\n", source, target);
                        PartitionNum ++;
                        bGotDevFileName ++;
                    }
                }
                else
                {
                    bGotDevFileName ++;
                    printf("[UsbCon_Mount 0] can't get file info \n");
                    umount(target);
                }
            }
            else
            {
                bGotDevFileName ++;
                perror("mount0:");
            }
        }

        printf("bGotDevFileName:%d\n", PartitionNum);

        if (PartitionNum > 0)
        {
            if (GosCfg_USB_Print)
            {
                snprintf(debug_path, sizeof(debug_path), "%s/log.txt", dev_attribute[0].mount_name);
                printf("log file is:%s\n", debug_path);

                if ((stream = freopen(debug_path, "a+", stdout)) == NULL)
                {
                    printf("----------------io err........----------------\n");
                }

                to_usb_log = 1;
            }

            return 0;
        }
        else
        {
            printf("[UsbCon_Mount 0] fail. can not get dev file name \n");
            return -1;
        }
    }
    else// add a usb dev
    {
        int     get_mounted_number = 0;
        char    mounted_sdx_name[16] = {0};
        char    used_partition_name[USB_MAX_DIR] = {0};

        for (i = 0; i < USB_MAX_DIR; i++)
        {
            if (dev_attribute[i].status == DEV_READY)
            {
                memcpy(mounted_sdx_name, &(dev_attribute[i].original_name[5]), 3);
                used_partition_name[get_mounted_number++] =
                    dev_attribute[i].mount_name[strlen(dev_attribute[i].mount_name) - 1] - '0';
            }
        }

        bGotDevFileName = 0;

        for (i = 0; (i < tmp_pn) && (tmp_pn < USB_MAX_DIR); i++)
        {
            if (strstr(usb_dir_buf[i], mounted_sdx_name))
            {
                continue;
            }

            printf(">>> usb dir buf[%d]:%s mounted_sdx_name:%s\n", i, usb_dir_buf[i], mounted_sdx_name);

            for (bGotDevFileName = 0; bGotDevFileName < USB_MAX_DIR; bGotDevFileName ++)
            {
                for (k = 0; k < get_mounted_number; k++)
                {
                    if (used_partition_name[k] == bGotDevFileName)
                    {
                        break;
                    }
                }

                if (k == get_mounted_number)
                {
                    break;
                }
            }

            printf("2bGotDevFileName:%d\n", bGotDevFileName);
            memset(target, 0, sizeof(target));
            snprintf(target, sizeof(target), "%s/partition%d", USB_MOUNT_DIR, bGotDevFileName);
            printf("[UsbCon_Mount 1] partition name : %s\n", ShellCommand);

            if (access(target, F_OK) != 0)  //测试文件是否存在
            {
                Result = mkdir(target, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

                if (Result != 0)
                {
                    printf("mkdir error!!!");
                    bGotDevFileName++;
                    continue;
                }
            }

            memset(source, 0, sizeof(source));
            snprintf(source, sizeof(source), "/dev/%s", usb_dir_buf[i]);
            printf("[UsbCon_Mount 1]source:%s target:%s\n", source, target);

            memset(ShellCommand, 0, sizeof(ShellCommand));
            snprintf(ShellCommand, sizeof(ShellCommand), "mount %s %s", source, target);

#if 0
            Result = mount(source, target, "vfat", MS_MGC_VAL, MOUNT_CMD_PARAM); //NTFS格式
#else
            Result = system(ShellCommand);
            if (Result != 0)
            {
                Result = mount(source, target, "ext2", MS_MGC_VAL, MOUNT_CMD_PARAM);

                if (Result != 0)
                {
                    if (USB_CHK_NTFS_OK == UsbCon_Chkntfs((char *)source))
                    {
                        Result = mount(source, target, "ufsd", MS_MGC_VAL, MOUNT_CMD_PARAM);

                        if (Result != 0)
                        {
                            printf("mount ext2,ntfs,vfat err....\n");
                            Result = system(ShellCommand);
                            printf("--->Result :%d ShellCommand:%s\n", Result, ShellCommand);
                        }
                    }
                }
            }

            if (Result == 0)
            {
                if (statfs(target, &sfs) != -1)
                {
                    size = MSCALE(sfs.f_blocks, sfs.f_bsize);
                    printf("[UsbCon_Mount 1] file size, f_bsize:%lld, f_blocks:%lld \n", (long long int)sfs.f_bsize,
                           (long long int)sfs.f_blocks);

                    if (size <= MIN_SUPPORT_CAPACITY)
                    {
                        umount(target);
                    }
                    else
                    {
                        int free_attribute_index;

                        for (free_attribute_index = 0; free_attribute_index < USB_MAX_DIR; free_attribute_index++)
                        {
                            if (dev_attribute[free_attribute_index].status == DEV_NO_READY)
                            {
                                break;
                            }
                        }

                        if (free_attribute_index == USB_MAX_DIR)
                        {
                            umount(target);
                            break;
                        }

                        if (sfs.f_type == 0x4d44)
                        {
                            dev_attribute[free_attribute_index].partition_type = DOS;
                        }
                        else if (sfs.f_type == 0x5346544e)
                        {
                            dev_attribute[free_attribute_index].partition_type = NTFS;
                        }
                        else if (sfs.f_type == 0xef53)
                        {
                            dev_attribute[free_attribute_index].partition_type = EXT2;
                        }
                        else
                        {
                            dev_attribute[free_attribute_index].partition_type = UNKNOW;
                        }

                        if (is_sata_dev == 0 && strstr(source, sata_name) != 0)
                        {
                            dev_attribute[free_attribute_index].dev_type = SATA_DEV;
                        }
                        else
                        {
                            dev_attribute[free_attribute_index].dev_type = USB_DEV;
                        }

                        dev_attribute[free_attribute_index].status = DEV_READY;
                        dev_attribute[free_attribute_index].size = size;
                        dev_attribute[free_attribute_index].free_size = MSCALE(sfs.f_bfree, sfs.f_bsize);
                        strncpy(dev_attribute[free_attribute_index].mount_name, target, strlen(target));
                        strncpy(dev_attribute[free_attribute_index].original_name, source, strlen(source));
                        used_partition_name[get_mounted_number++] = bGotDevFileName;
                        PartitionNum ++;
                    }
                }
                else
                {
                    bGotDevFileName ++;
                    printf("[UsbCon_Mount 1] can't get file info \n");
                    umount(target);
                }
            }
            else
            {
                bGotDevFileName ++;
                perror("mount1:");
            }
#endif
        }

        printf("bGotDevFileName:%d\n", PartitionNum);
    }

    if (old_partition_num  < PartitionNum)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void UsbCon_SortAllDevice(void)
{
    int i = 0, j = 0;
    DEV_ATTRIBUTE temp;

    for (i = 0; i < USB_MAX_DIR; i++)
    {
        for (j = i + 1; j < USB_MAX_DIR; j++)
        {
            if (dev_attribute[i].status == DEV_READY && dev_attribute[j].status == DEV_READY)
            {
                if (dev_attribute[i].size >= dev_attribute[j].size)
                {
                    temp = dev_attribute[i];
                    dev_attribute[i] = dev_attribute[j];
                    dev_attribute[j] = temp;
                }
            }
        }
    }

    for (i = 0; i < USB_MAX_DIR; i++)
    {
        for (j = i + 1; j < USB_MAX_DIR; j++)
        {
            if (dev_attribute[i].status == DEV_NO_READY && dev_attribute[j].status == DEV_READY)
            {
                temp = dev_attribute[i];
                dev_attribute[i] = dev_attribute[j];
                dev_attribute[j] = temp;
            }
        }
    }
}

//-----------------------------------------------------------------------------
//  FUNCTION:   USB_Umount()
//  DESRIPTION: umount the USB storage device
//  RETURN:     0 success.
//              -1 failed.
//-----------------------------------------------------------------------------
int USB_Umount(void)
{
    int             i = 0, k = 0;
    int             tmp_pn = 0;
    DIR            *pDir = NULL;
    char            usb_dir_buf[USB_MAX_DIR][32];
    struct dirent  *pDirEnt = NULL;

    if ((pDir = opendir(USB_DETECT_DIR)) == NULL)
    {
        printf("[USB_Umount]: Failed to home dir.\n");
        return -1;
    }

    while ((pDirEnt = readdir(pDir)) != NULL)
    {
        if ((pDirEnt->d_name[0] == 's') && (pDirEnt->d_name[1] == 'd'))
        {
            snprintf(usb_dir_buf[tmp_pn], 32, "/dev/%s", pDirEnt->d_name);
            tmp_pn++;
        }
    }

    closedir(pDir);

    if (tmp_pn == 0)
    {
        for (i = 0; i < USB_MAX_DIR; i++)
        {
            dev_attribute[i].status = DEV_NO_READY;
            dev_attribute[i].sync = 0;
			printf("umount a dev:%s\n", dev_attribute[i].mount_name);
            umount(dev_attribute[i].mount_name);
        }

        usb_sdX_number = PartitionNum = 0;

        if (GosCfg_USB_Print)
        {
            freopen("/dev/console", "w", stdout);
            stream      = NULL;
            number      = 0;
            to_usb_log  = 0;
            printf("And now back to the console once again\n");
        }

        return 0;
    }

    //first,find and umount remove usb dev
    for (i = 0; i < USB_MAX_DIR; i++)
    {
        if (dev_attribute[i].status == DEV_READY)
        {
            for (k = 0; k < tmp_pn; k++)
            {
                if (strncmp(dev_attribute[i].original_name, usb_dir_buf[k], strlen(usb_dir_buf[k])) == 0)
                {
                    break;
                    //not remove
                }
            }

            if (k == tmp_pn)
            {
                //a remove dev
                dev_attribute[i].status = DEV_NO_READY;
                dev_attribute[i].sync = 0;
                printf("umount a dev:%s\n", dev_attribute[i].mount_name);
                umount(dev_attribute[i].mount_name);
                PartitionNum --;
            }
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
//  FUNCTION:   USB_GetStatus()
//  DESRIPTION:
//  RETURN:
//
//-----------------------------------------------------------------------------
int USB_GetStatus(void)
{
    return (lv_UsbConStatus);
}

int USB_GetCurDisk(void)
{
    return usb_current_disk;
}

int USB_SetCurDisk(int Curdisk)
{
    if (Curdisk >= USB_MAX_DIR)
    {
        return -1;
    }

    usb_current_disk = Curdisk;

    return 0;
}

int USB_GetExitedDisk(void)
{
    int disk_index = -1;

    for (disk_index = 0; disk_index < USB_MAX_DIR; disk_index++)
    {
        if (dev_attribute[disk_index].status == DEV_READY)
        {
            break;
        }
    }

    if (disk_index >= USB_MAX_DIR)
    {
        disk_index = -1;
    }

    return disk_index;
}

int USB_SetNextValidDisk(int current_disk)
{
    int i;

    for (i = current_disk + 1; i < USB_MAX_DIR; i++)
    {
        if (dev_attribute[i].status == DEV_READY)
        {
            return i;
        }
    }

    if ( i >=  USB_MAX_DIR)
    {
        for (i = 0; i < current_disk ; i++)
        {
            if (dev_attribute[i].status == DEV_READY)
            {
                return i;
            }
        }
    }

    return -1;//没有找到上一个
}

int USB_SetPrevValidDisk(int current_disk)
{
    int i;

    for (i = current_disk - 1; i >= 0; i--)
    {
        if (dev_attribute[i].status == DEV_READY)
        {
            return i;
        }
    }

    if (i < 0)
    {
        for (i = USB_MAX_DIR - 1; i > current_disk ; i--)
        {
            if (dev_attribute[i].status == DEV_READY)
            {
                return i;
            }
        }
    }

    return -1;//没有找到下一个
}

int USB_GetAttribute(int dev_index, DEV_ATTRIBUTE *dev_attr, char get_size_info)
{
    struct statfs sfs;

    if (dev_index >= USB_MAX_DIR || dev_index < 0 || dev_attr == NULL)
    {
        return -1;
    }

    if (get_size_info == 1)
    {
        if (dev_attribute[dev_index].status == DEV_READY)
        {
            if (statfs(dev_attribute[dev_index].mount_name, &sfs) != -1)
            {
                dev_attribute[dev_index].size = MSCALE(sfs.f_blocks, sfs.f_bsize);
                dev_attribute[dev_index].free_size = MSCALE(sfs.f_bfree, sfs.f_bsize);
                printf("dev_attribute[%d].free_size:%ld\n", dev_index, dev_attribute[dev_index].free_size);
                printf("sfs.f_bfree=%lld sfs.f_block=%lld sfs.f_bsize=%lld\n", (long long int)sfs.f_bfree,
                       (long long int)sfs.f_blocks, (long long int)sfs.f_bsize);
            }
            else
            {
                return -1;
            }
        }
    }

    memcpy(dev_attr, &dev_attribute[dev_index], sizeof(DEV_ATTRIBUTE));

    return 0;
}

int USB_GetAttribute_In_Use(int dev_in_use_index, DEV_ATTRIBUTE *dev_attr, int *dev_index, char get_size_info)
{
    int i, k = 0;

    if (dev_in_use_index >= PartitionNum || dev_attr == NULL)
    {
        return -1;
    }

    for (i = 0; i < USB_MAX_DIR; i++)
    {
        if (dev_attribute[i].status == DEV_READY)
        {
            if (k == dev_in_use_index)
            {
                if (get_size_info == 1)
                {
                    struct statfs sfs;

                    if (statfs(dev_attribute[i].mount_name, &sfs) != -1)
                    {
                        dev_attribute[i].size = MSCALE(sfs.f_blocks, sfs.f_bsize);
                        dev_attribute[i].free_size = MSCALE(sfs.f_bfree, sfs.f_bsize);
                    }
                }

                memset(dev_attr, 0, sizeof(DEV_ATTRIBUTE));
                memcpy(dev_attr, &dev_attribute[i], sizeof(DEV_ATTRIBUTE));

                if (dev_index)
                {
                    *dev_index = i;
                }

                return 0;
            }

            k ++;
        }
    }

    return -1;
}

int USB_GetPreferenceDevAttribute(int *dev_index, DEV_ATTRIBUTE *dev_attr, char get_size_info)
{
    int i;
    int max_free_size_dev = 0;
    unsigned long int max_free_size = 0;

    if (dev_index == NULL || dev_attr == NULL)
    {
        return -1;
    }

    for (i = 0; i < USB_MAX_DIR; i++)
    {
        if ((dev_attribute[i].status == DEV_READY)
		 && ((dev_attribute[i].partition_type == EXT2)
         || (dev_attribute[i].partition_type == DOS)
         || (dev_attribute[i].partition_type == NTFS)))
        {
            if (get_size_info == 1)
            {
                struct statfs sfs;

                if (statfs(dev_attribute[i].mount_name, &sfs) != -1)
                {
                    dev_attribute[i].size = MSCALE(sfs.f_blocks, sfs.f_bsize);
                    dev_attribute[i].free_size = MSCALE(sfs.f_bfree, sfs.f_bsize);
                }
            }

            if (dev_attribute[i].dev_type == SATA_DEV)
            {
                *dev_index = i;
                memcpy(dev_attr, &dev_attribute[i], sizeof(DEV_ATTRIBUTE));
                return 0;
            }

            if (dev_attribute[i].free_size > max_free_size)
            {
                max_free_size = dev_attribute[i].free_size;
                max_free_size_dev = i;
            }
        }
    }

    if (max_free_size != 0)
    {
        *dev_index = max_free_size_dev;
        memcpy(dev_attr, &dev_attribute[max_free_size_dev], sizeof(DEV_ATTRIBUTE));
        return 0;
    }

    return -1;
}

int USB_Judge_DevType(void)
{
    int     i = 0;
    int     ret = -1;

    for (i = 0; i < PartitionNum; i++)
    {
        if (dev_attribute[i].dev_type == USB_DEV)
        {
            ret = i;
            break;
        }
    }

    return ret;
}

int USB_SetAttribute(int dev_index, DEV_ATTRIBUTE *dev_attr)
{
    if (dev_index >= USB_MAX_DIR || dev_attr == NULL)
    {
        return -1;
    }

    memcpy(&dev_attribute[dev_index], dev_attr, sizeof(DEV_ATTRIBUTE));
    return 0;
}

int USB_SetSyncFlag(int dev_index)
{
    if (dev_index >= USB_MAX_DIR)
    {
        return -1;
    }

    dev_attribute[dev_index].sync = 0;

    return 0;
}

int USB_GetDiskPartNum(void)
{
	return PartitionNum;
}

/* End ----------------------------------------------------------------------*/
