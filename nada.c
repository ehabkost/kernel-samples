/* Módulo de kernel mínimo
 */
#include <linux/module.h>

MODULE_AUTHOR("Fulano de Tal");
MODULE_LICENSE("GPL");

/* Módulo vazio, sem nenhuma função */

/* Um módulo, apesar de não ter função de inicialização e de saída, pode
 * exportar funções para que outros módulos possam usá-las. Exemplo abaixo:
 */

void faz_alguma_coisa(void)
{
	/* alguma coisa aqui ... */
}
EXPORT_SYMBOL(faz_alguma_coisa);
