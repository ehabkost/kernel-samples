/* Driver para dispositivo FISL
 */
#include <linux/module.h>
#include <asm/io.h>

MODULE_AUTHOR("Eduardo Habkost");
MODULE_LICENSE("GPL");

#define PORT_ADDRESS 120
#define PORT_DATA_ADDRESS 124
#define ID_FISL_DEV 0xdead

#define FISL_INICIALIZAR 1
#define FISL_DESLIGA 2
#define FISL_BUFFER_PRONTO 0xff

char texto[100] = "hello world";

module_param_string(texto, texto, 20, 0400);

static void fisl_mostra(const char *buf, size_t len)
{
	int i;
	for (i = 0; i < len; i++)
		outl(buf[i], PORT_DATA_ADDRESS);

	outl(FISL_BUFFER_PRONTO, PORT_ADDRESS);
}

static int __init fisl_init(void)
{
	u16 data;

	printk("Procurando dispositivo na porta %d\n", PORT_ADDRESS);
	data = inl(PORT_ADDRESS);
	if (data != ID_FISL_DEV) {
		printk("Dispositivo FISL nao encontrado\n");
		return -ENODEV;
	}

	outl(FISL_INICIALIZAR, PORT_ADDRESS);
	printk("Dispositivo inicializado\n");

	fisl_mostra(texto, strlen(texto));

	return 0;
}

static void __exit fisl_exit(void)
{
	outl(FISL_DESLIGA, PORT_ADDRESS);
	printk("Dispositivo desligado\n");
}

module_init(fisl_init);
module_exit(fisl_exit);
