#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DRIVER_AUTHOR "Pablo Oliveira <pablo.oliveira@uvsq.fr>"
#define DRIVER_DESC "GPIO Interrupt Kernel Module Test"

#define GPIO_ANY_GPIO 18
#define GPIO_ANY_GPIO_DESC "GPIO 18 Interrupt Test"
#define GPIO_ANY_GPIO_DEVICE_DESC ""

/* Cette variable va permettre de sauvegarder le numéro IRQ qui nous sera attribué
par le noyau */

short int irq_any_gpio = 0;
char my_tasklet_data[]="Appel de la fonction tasklet";




// Fonction tasklet
void my_tasklet_function( unsigned long data )
{
  printk( "Printk de la fonction tasklet\n");
}

DECLARE_TASKLET( my_tasklet, my_tasklet_function, (unsigned long) &my_tasklet_data );



/* Le gestionnaire d'interruption */
static irqreturn_t modtest_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    long unsigned int flags;
    local_irq_save(flags);

    /* Complétez */
    // printk("Les fils se touchent !!!\n");
    tasklet_schedule( &my_tasklet );


    local_irq_restore(flags);
    return IRQ_HANDLED;
}

int modtest_init(void)
{
    printk(KERN_NOTICE "Bonjour!\n");

    /* On demande l'accès au GPIO 18 */
    if (gpio_request(GPIO_ANY_GPIO, GPIO_ANY_GPIO_DESC)) {
        printk("GPIO request failure: %s\n", GPIO_ANY_GPIO_DESC);
        return 1;
    }

    /* On demande une ligne IRQ sur le GPIO 18 */
    if ((irq_any_gpio = gpio_to_irq(GPIO_ANY_GPIO)) < 0) {
        printk("GPIO to IRQ mapping failure %s\n", GPIO_ANY_GPIO_DESC);
        return 1;
    }

    printk(KERN_NOTICE "Mapped int %d\n", irq_any_gpio);

    /* On configure le gestionnaire d'IRQ */
    if (request_irq(irq_any_gpio,
            (irq_handler_t) modtest_irq_handler,
            IRQF_TRIGGER_FALLING,
            GPIO_ANY_GPIO_DESC, GPIO_ANY_GPIO_DEVICE_DESC)) {
        printk("Irq Request failure\n");
        return 1;
    }

    return 0;
}

void modtest_exit(void)
{
    /* Libère l'IRQ */
    if (irq_any_gpio >= 0) {
        free_irq(irq_any_gpio, GPIO_ANY_GPIO_DEVICE_DESC);
    }
    /* Libère le GPIO */
    gpio_free(GPIO_ANY_GPIO);
    tasklet_kill( &my_tasklet );
    printk(KERN_NOTICE "Au revoir!\n");
}

module_init(modtest_init);
module_exit(modtest_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);