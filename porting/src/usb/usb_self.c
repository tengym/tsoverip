#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>

#define USB_MAX_NUM				10
#define USB_CHECK_STR			"1234567890"
#define _MAXDIR_   256
#define DEV_DIR     "/dev" 
#define MOUNT_DIR_  "/home/data/usb/" 

static int USB_Count	= 0;

static int mount_USB()
{
	DIR                *pDir = NULL;
	struct dirent      *pDirEnt = NULL;
	char                usb_name[USB_MAX_NUM][32];
	int      			dev_count = 0,mounted_conut = 0,i=0,ret;
	char				usb_mount_name[USB_MAX_NUM][256];
	char 				SYS_CMD[1024] = {0};
	memset(usb_name,0,sizeof(usb_name));
	memset(usb_mount_name,0,sizeof(usb_mount_name));

	if ((pDir = opendir(DEV_DIR)) == NULL) 
    {
        return -1;
    }
	while ((pDirEnt = readdir(pDir)) != NULL)
    {
        if ((pDirEnt->d_name[0] == 's') && (pDirEnt->d_name[1] == 'd'))  
        {
            strncpy(usb_name[dev_count], pDirEnt->d_name, strlen(pDirEnt->d_name));
            dev_count++;
        }
    }
	closedir(pDir);

	if(USB_Count > 0) 
	{
		for(i = 0; i < USB_Count; i++)
		{
			sprintf(usb_mount_name[i],"%s/usb%d",MOUNT_DIR_,i);
			umount(usb_mount_name[i]);//umount 已经存在的
		}
	}

	for(i = 0; i < dev_count; i++)     //挂载
	{
		sprintf(usb_mount_name[mounted_conut],"%s/usb%d",MOUNT_DIR_,mounted_conut);
		sprintf(SYS_CMD,"mount %s/%s %s >/dev/null 2>/dev/null",DEV_DIR,usb_name[i],usb_mount_name[mounted_conut]);
		ret = system(SYS_CMD); 
		if(ret == 0)
        {
			mounted_conut++;
        }
	}

	return mounted_conut ;
}

//0成功，-1失败
static int  check_one_usbByDIR(char *usb_mount_name)
{

	int 				fd = -1;
	char 				read_buff[1024] = {0};
	char 				usb_check_file[1024] = {0};

	sprintf(usb_check_file,"%s/check_usb",usb_mount_name);
	remove(usb_check_file);

	fd = open(usb_check_file,O_CREAT | O_RDWR);
	if(fd < 0)
    {
        return -1;
    }

	write(fd,USB_CHECK_STR,strlen(USB_CHECK_STR));
	lseek(fd, 0L, SEEK_SET);
	read(fd,read_buff,sizeof(read_buff));
	close(fd);
	if(strncmp(read_buff,USB_CHECK_STR,strlen(USB_CHECK_STR))== 0)
    {
        return 0;
    }
	else
    {
        return -1;
    }
}

//返回值为USB 接口正常的个数
int check_USB()
{
	int PartitionNum = 0;
	int i = 0 ;
	int mounted_USB_NUM = -1 ;
	char				usb_mount_name[256]={0};

	mounted_USB_NUM = mount_USB();
	if(mounted_USB_NUM <= 0)
	{
		USB_Count = 0;
		return 0;
	}
	if(USB_Count == mounted_USB_NUM)
	{
		return USB_Count;
	}
	else
		USB_Count = mounted_USB_NUM ;
	for(i = 0;i < USB_Count;i++)
	{
		memset(usb_mount_name,0,sizeof(usb_mount_name));
		sprintf(usb_mount_name,"%s/usb%d",MOUNT_DIR_,i);
		if(check_one_usbByDIR(usb_mount_name) == 0)
			PartitionNum++;
	}
	return PartitionNum ;
}

void umount_USB()
{
	char				usb_mount_name[USB_MAX_NUM][256];
	int i,ret=-1;

	for(i = 0;i < USB_Count;i++)
	{
		sprintf(usb_mount_name[i],"%s/usb%d",MOUNT_DIR_,i);
		ret= umount(usb_mount_name[i]);//umount 已经存在的
		if(ret == -1)
			perror("------>umount Failed \n");
		else
		{
			printf("umount success\n");
		}
		//rmdir(usb_mount_name[i]);
	}
}

void umount_USB_Pre()
{
	char				usb_mount_name[USB_MAX_NUM][256];
	int i,ret=-1;

	for(i = 0;i < 5;i++)
	{
		sprintf(usb_mount_name[i],"%s/usb%d",MOUNT_DIR_,i);
		ret= umount(usb_mount_name[i]);//umount 已经存在的
		if(ret == -1)
			perror("------>umount Failed \n");
		else
		{
			printf("umount success\n");
		}
	}
}

