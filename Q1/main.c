// Initialization Functions //
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "main.h"

#define NAME DESD_Device

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAJOL");
MODULE_DESCRIPTION("DESD ELDD  Exam module Q1");

//#define STATUS _IOW('a', 'a', struct Status *)

// Funtion Prototypes //
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_read(struct file *filp,  char __user *Ubuff, size_t count, loff_t *offp);
static long NAME_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// Structure that defines the operation that the driver provides. //
struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open  = NAME_open,
	.read  = NAME_read,
	.write = NAME_write,
	.unlocked_ioctl= NAME_ioctl,
	.release = NAME_release,
};


// Structure for a Character driver //
struct cdev *my_cdev;

// Init module  //
static int __init CharDevice_init(void)
{
	int result;
	int MAJOR, MINOR;
	dev_t Mydev;
	Mydev = MKDEV(100, 0);    // create a device driver number
	MAJOR = MAJOR(Mydev);
	MINOR = MINOR(Mydev);
	printk("WE are in init function.\n");
	printk("\nThe MAjor number is %d\nThe Minor number is %d\n", MAJOR, MINOR);
	result= register_chrdev_region(Mydev, 1, "DESD_Device");   // register device region..
	if(result<0)
	{
		printk("\n Failed to obtain\n");
		return(-1);
	}

	my_cdev = cdev_alloc();		// allocate memory to Char Device structure
	my_cdev->ops = &fops; 		// link out file operation to the char device

	result = cdev_add(my_cdev, Mydev,1); // Notify the kernel about the new device
	if(result<0)
        {
                printk("\nThe char device not being created\n");
		unregister_chrdev_region(Mydev, 1);
                return(-1);
        }

	return 0;
}

// Cleanup module //
static void __exit CharDevice_exit(void)
{
	dev_t Mydev;
	int MAJOR, MINOR;
	Mydev = MKDEV(100, 0);    
        MAJOR = MAJOR(Mydev);
        MINOR = MINOR(Mydev);
	printk("We are exiting from the driver\n");
        printk("\nThe MAjor number is %d\nThe Minor number is %d\n", MAJOR, MINOR);
	unregister_chrdev_region(Mydev,1); // unregister the device number and the device created
	cdev_del(my_cdev);
	printk("\nUnregistered the stuff that was allocated... Bye Bye\n");
//	return ;
}


// Open system call //

int NAME_open(struct inode *inode, struct file *filp)
{
	printk("\n This is the kernel open call\n");
	return 0;
}

// Close system call //

int NAME_release(struct inode *inode, struct file *filp)
{
	printk("\n This is the kernel release system call\n");
        return 0;
}

// write functionality //
ssize_t NAME_write(struct file *filp, const char __user *Ubuff , size_t count, loff_t *offp)
{
	char Kbuff[100];
	unsigned long result;
	ssize_t retval;
	result = copy_from_user((char *)Kbuff, (char *)Ubuff, count);    //get user data

	if(result == 0)
	{
	printk("\nMsg from the user ==> %s\n", Kbuff);    // recieved data from user application
	printk("\nData successfully written.\n");
	retval = count;
	printk("\nNo. of bytes received from user to kenel : %d\n", retval);
	return retval;
	}

	else if (result > 0)
	{
	printk("\nMsg from the user ==> %s\n", Kbuff);
	printk("\nPart data copied from user application to kernel\n");
	retval = (count-result);
	printk("\nData bytes: %d\n", retval);
	return retval;
	}

	else
	{
	printk("\nError writing data\n");
	retval = -EFAULT;
	return retval;
	}

}

// read Functionallity //
 ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{

	char Kbuff[] = "This is some data from the kernel to the user.\n";
	unsigned long result;
	ssize_t retval;
	printk("The value of count : %ld\n", count);
	result = copy_to_user((char *)Ubuff, (char *)Kbuff, sizeof(Kbuff));  //copy to user

	if(result == 0)
	{
		printk("\nMessage from kernel ==> %s\n ", Kbuff);
		printk("\nData successfully read.\n");
		retval= count;
		return retval;
	}

	else if (result > 0)
	{
		printk("\nPart data available\n");
		retval = (count - result);
		printk("\nNo. of bytes recived from kernal : %d\n", retval);
		return retval;
	}

	else
	{
		printk("\nError writing data to user\n");
		retval = -EFAULT;
		return retval;
	}
}

// ioctl functionality //

static long NAME_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

		struct Status test;

		switch (cmd)
			{
			case STATUS:
 				if (copy_from_user(&test, (struct status *) arg, sizeof(test)))
				printk("error\n");
				else
				printk("ioctl status:\n size = %d\n char buff[] = %s\n", test.size, test.buff);
				break;
			default:
				printk("invalid command");
				break;

                       }

		return 0;
}


module_init(CharDevice_init);
module_exit(CharDevice_exit);
