#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

MODULE_AUTHOR("Eduardo Habkost");
MODULE_LICENSE("MIT");


#ifdef USE_CDEV
static int major;
static struct class *mostrador_class;
#endif

static ssize_t mostrador_write(struct file *f, const char __user *buf, size_t len, loff_t *loff)
{
	char c, i;
	printk("texto: [");
	for (i = 0 ; i < len; i++) {
		if (get_user(c, buf + i))
			return -EFAULT;
		printk("%c", c);
	}
	printk("]\n");
	return len;
}

static const struct file_operations mostrador_fops = {
	.owner = THIS_MODULE,
	.write = mostrador_write,
};

#ifndef USE_CDEV
static struct miscdevice mostrador_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mostrador",
	.fops = &mostrador_fops,
};
#endif

static int __init cdev_init(void)
{
#ifdef USE_CDEV
	mostrador_class = class_create(THIS_MODULE, "mostrador");
	if (IS_ERR(mostrador_class))
		return PTR_ERR(mostrador_class);

	major = register_chrdev(0, "mostrador", &mostrador_fops);
	if (major < 0) {
		printk("Error registrando chardev");
		return major;
	}

	device_create(...);
#else
	int r;
	r = misc_register(&mostrador_dev);
	if (r < 0)
		return r;
#endif

	return 0;
}

static void __exit cdev_exit(void)
{
#ifdef USE_CDEV
	unregister_chrdev(major, DEVNAME);
#else
	misc_deregister(&mostrador_dev);
#endif
}

module_init(cdev_init);
module_exit(cdev_exit);
