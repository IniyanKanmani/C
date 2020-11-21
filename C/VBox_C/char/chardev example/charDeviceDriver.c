/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <charDeviceDriver.h>
#include <stdlib.h>
#include "ioctl.h"

MODULE_LICENSE("GPL");

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */


DEFINE_MUTEX  (devLock);
static int counter = 0;

static long device_ioctl(struct file *file,	/* see include/linux/fs.h */
         unsigned int ioctl_num,	/* number and param for ioctl */
         unsigned long ioctl_param)
{

    /* 
     * Switch according to the ioctl called 
     */
    if (ioctl_num == RESET_COUNTER) {
        counter = 0; 
        /* 	    return 0; */
        return 5; /* can pass integer as return value */
    }

    else {
        /* no operation defined - return failure */
        return -EINVAL;

    }
}


/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}

void freeNode(List* node) {

    if (node != NULL) {

        freeNode(node->next);
        free(node);

    }

    return NULL;

}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
    /*  Unregister the device */
    printk(KERN_ALERT "Device unregister success\n");

    freeNode(root);

    unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
    mutex_lock (&devLock);
    if (Device_Open) {
    mutex_unlock (&devLock);
    return -EBUSY;
    }
    Device_Open++;
    printk(KERN_ALERT "Device opened\n");
    mutex_unlock (&devLock);
    try_module_get(THIS_MODULE);
    
    return SUCCESS;
}

/* Called when a process closes the device file. */
static int device_release(struct inode *inode, struct file *file)
{
    mutex_lock (&devLock);
    Device_Open--;		/* We're now ready for our next caller */
    mutex_unlock (&devLock);
    /* 
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module. 
     */

    printk(KERN_ALERT "Device closed\n");
    module_put(THIS_MODULE);

    return 0;
}

List* searchNode(List *node) {

    if (node == NULL) {

        return NULL;

    } else if (node->next == NULL) {

        return node;

    } else if (node->next != NULL) {

        node = searchNode(node->next);

    }

    return node;

}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
               char *buffer,	/* buffer to fill with data */
               size_t length,	/* length of the buffer     */
               loff_t * offset)
{

    char msg[BUF_LEN];
    int bytes_read = 0;
    //int result = 0;
    int res = 0;
    List *lis;
    lis = searchNode(root);

    sprintf(msg, lis->dat);
    mesg_Ptr = msg;

    //result = copy_to_user(buffer, mesg, size_of_mesg);

    while (length && *mesg_Ptr) {

		/* 
		 * The buffer is in the user data segment, not the kernel 
		 * segment so "*" assignment won't work.  We have to use 
		 * put_user which copies data from the kernel data segment to
		 * the user data segment. 
		 */
		res = put_user(*(mesg_Ptr++), buffer++);

		if (res != 0) {
		         return -EFAULT;
		}
		    
		length--;
        bytes_read++;
		
	}

    printk (KERN_ALERT "READ SUCCESS\n");

    /*
    if (result == 0) {

        return (size_of_mesg = 0);

    } else {

        return -EFAULT;

    }
    */
    
   return bytes_read;

}

List* insertNode(List* node, char *data) {

    if (node == NULL) {

        node = (List *)malloc(sizeof(struct List));
        sprintf(node->dat, data);
        node->next = NULL;

        return node;

    } else {

        node->next = insertNode(node->next, data);

    }

    return node;
}

/* Called when a process writes to dev file: echo "hi" > /dev/hello  */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{   

    if (copy_from_user(mesg, buff, len)) {

        return -EFAULT;

    }
    
    size_of_mesg = strlen(mesg);
    printk(KERN_ALERT "COPY SUCCESS");
    root = insertNode(root, mesg);

    printk(KERN_ALERT "WRITE SUCCESS [%d]\n", size_of_mesg);

    return size_of_mesg;
}
