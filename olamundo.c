/* Módulo "Olá Mundo"
 */
#include <linux/module.h>

MODULE_AUTHOR("Fulano de Tal");
MODULE_LICENSE("GPL");

static int __init ola_init(void)
{
	printk("Olá, mundo!\n");
	return 0;
}

static void __exit ola_exit(void)
{
	printk("Adeus, mundo!\n");
}

module_init(ola_init);
module_exit(ola_exit);
