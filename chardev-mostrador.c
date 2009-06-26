#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

#include <asm/io.h>

MODULE_AUTHOR("Eduardo Habkost");
MODULE_LICENSE("GPL");

#define PORT_ADDRESS 120
#define PORT_DATA_ADDRESS 124
#define ID_FISL_DEV 0xdead

#define FISL_INICIALIZAR 1
#define FISL_DESLIGA 2
#define FISL_BUFFER_PRONTO 0xff


static ssize_t mostrador_write(struct file *f, const char __user *buf, size_t len, loff_t *loff)
{
	char c, i;
	for (i = 0 ; i < len; i++) {
		if (get_user(c, buf + i))
			return -EFAULT;

		if (c == '\n')
			outl(FISL_BUFFER_PRONTO, PORT_ADDRESS);
		else
			outl(c, PORT_DATA_ADDRESS);
	}
	return len;
}

static const struct file_operations mostrador_fops = {
	.owner = THIS_MODULE,
	.write = mostrador_write,
};

static struct miscdevice mostrador_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mostrador",
	.fops = &mostrador_fops,
};

static int __init cdev_init(void)
{
	u16 data;
	int r;

	printk("Procurando dispositivo na porta %d\n", PORT_ADDRESS);
	data = inl(PORT_ADDRESS);
	if (data != ID_FISL_DEV) {
		printk("Dispositivo FISL nao encontrado\n");
		return -ENODEV;
	}

	outl(FISL_INICIALIZAR, PORT_ADDRESS);
	printk("Dispositivo inicializado\n");

	r = misc_register(&mostrador_dev);
	if (r < 0)
		return r;

	return 0;
}

static void __exit cdev_exit(void)
{
	misc_deregister(&mostrador_dev);
	outl(FISL_DESLIGA, PORT_ADDRESS);
	printk("Dispositivo FISL desligado\n");
}

module_init(cdev_init);
module_exit(cdev_exit);
