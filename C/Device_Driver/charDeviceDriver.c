#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>    // for put_user
#include <charDeviceDriver.h>

MODULE_AUTHOR ("Iniyan Kanmani <ixk990@student.bham.ac.uk>");
MODULE_DESCRIPTION ("Simple Character Device Driver");
MODULE_LICENSE ("GPL");

DEFINE_MUTEX  (devLock);

List* insertNode(List*, char*);
List* searchNode(List*);
void cpy(char*, char*);
List* freeNode(List*);

/*
 * This function is called when the module is loaded
 * You will not need to modify it for this assignment
 */

int init_module(void) {

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


/*
 * This function is called when the module is unloaded
 */

void cleanup_module(void) {

    /*
     * Unregister the device 
     */

    root = freeNode(root);
    unregister_chrdev(Major, DEVICE_NAME);

}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */

static int device_open(struct inode *inode, struct file *file) {
    
    mutex_lock (&devLock);

    if (Device_Open) {

        mutex_unlock (&devLock);
        return -EBUSY;

    }

    Device_Open++;
    mutex_unlock (&devLock);
    try_module_get(THIS_MODULE);
    
    return SUCCESS;

}

/*
 * Called when a process closes the device file.
 */

static int device_release(struct inode *inode, struct file *file) {

    mutex_lock (&devLock);
    Device_Open--;      // We're now ready for our next caller
    mutex_unlock (&devLock);

    /* 
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module. 
     */

    module_put(THIS_MODULE);

    return SUCCESS;

}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 * This is just an example about how to copy a message in the user space
 * You will need to modify this function
 */

static ssize_t device_read(struct file *filp,   // see include/linux/fs.h
                           char *buffer,       // buffer to fill with data
                           size_t length,     // length of the buffer
                           loff_t *offset) {

    char msg[BUF_LEN] = "";
    size_t bytes_read = 0;
    char *msg_Ptr = NULL;

    if (*offset != 0) {

        return 0;

    }

    if (root == NULL) {

        return -EAGAIN;

    } else {

        root = searchNode(root);
        cpy(msg, found_node->data);
        found_node = freeNode(found_node);

    }

    msg_Ptr = msg;

    while(length && *msg_Ptr) {

        put_user(*(msg_Ptr++), buffer++);

        length--;
        bytes_read++;

    }

    *offset += bytes_read;

    return bytes_read;

}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */

static ssize_t device_write(struct file *filp,
                            const char *buff, 
                            size_t len, 
                            loff_t *off) {

    char msg[BUF_LEN] = "";

    if (copy_from_user(msg, buff, len)) {

        return -EFAULT;

    }

    count_bytes += strlen(msg);

    if (count_bytes >= (4*1024*1024)) {

        return -EAGAIN;

    }

    if (strlen(msg) >= (6*1024)) {

        return -EINVAL;

    }

    root = insertNode(root, msg);

    return len;
    
}

List* insertNode(List* node, char *dat) {

    if (node == NULL) {

        node = (struct List*)kmalloc(sizeof(struct List), GFP_KERNEL);
        cpy(node->data, dat);
        node->next = NULL;

        return node;

    } else {

        node->next = insertNode(node->next, dat);

    }

    return node;
}

List* searchNode(List *node) {


    if (node->next == NULL) {

        found_node = (struct List*)kmalloc(sizeof(struct List), GFP_KERNEL);
        cpy(found_node->data, node->data);
        found_node->next = NULL;

        node = freeNode(node);

        return node;

    } else {

        node->next = searchNode(node->next);

    }

    return node;

}

void cpy(char* str1, char* str2) {

    int i;

    for (i = 0; str2[i] != '\0'; i++) {

        str1[i] = str2[i];

    }

    str1[i] = '\0';

}

List* freeNode(List* node) {

    if (node != NULL) {

        node->next = freeNode(node->next);
        kfree(node);

    }

    return NULL;

}
