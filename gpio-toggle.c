#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/byteorder/generic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chau Thanh Hai");
MODULE_DESCRIPTION("A GPIO Toggle Module");

/* Tegra X1 SoC Technical Reference Manual, version 1.3
 *
 * See Chapter 9 "Multi-Purpose I/O Pins", section 9.13 "GPIO Registers"
 * (table 32: GPIO Register Address Map)
 *
 * The GPIO hardware shares PinMux with up to 4 Special Function I/O per
 * pin, and only one of those five functions (SFIO plus GPIO) can be routed to
 * a pin at a time, using the PixMux.
 *
 * In turn, the PinMux outputs signals to Pads using Pad Control Groups. Pad
 * control groups control things like "drive strength" and "slew rate," and
 * need to be reset after deep sleep. Also, different pads have different
 * voltage tolerance. Pads marked "CZ" can be configured to be 3.3V tolerant
 * and driving; and pads marked "DD" can be 3.3V tolerant when in open-drain
 * mode (only.)
 *
 * The CNF register selects GPIO or SFIO, so setting it to 1 forces the GPIO
 * function. This is convenient for those who have a different pinmux at boot.
 */
static const struct of_device_id led_id[] = {
	{ .compatible = "microchip,mcp2515", },
	{ /* sentinel */ },
    };

MODULE_DEVICE_TABLE(of, led_id);

static int led_probe(struct platform_device *pdev)
{
	printk(KERN_INFO "Hello!!! This is my probe");
	
    struct device_node *np;
	int value;
	void *ptr;
	struct resource *res;
    np = pdev->dev.of_node;    
	int err, irq;

  	ptr = of_get_property(np, "phandle", NULL);

  	value = be32_to_cpup(ptr);

	printk(KERN_INFO "phandle: %x\n",  value);

  	if (!ptr) {
		  printk(KERN_INFO "Can't match device\n");
  	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (irq <= 0)
		return -ENODEV;

	return 0;
}

static int led_remove(struct platform_device *pdev)
{

	printk(KERN_INFO "Remove device\n");
	return 0;
}

static struct platform_driver led_driver = {
	.driver = {
		.name = "mcp2515",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(led_id),
	},
	.probe = led_probe,
	.remove = led_remove,
};

//module_platform_driver(led_driver);

static int __init hello_init(void)
{
 	printk(KERN_INFO "Regstrating Led Blink Device Driver\n");
 	return platform_driver_register(&led_driver);
}

static void __exit hello_exit(void)
{
 	printk(KERN_INFO "Un-Regstrating Led Blink Device Driver\n");
 	platform_driver_unregister(&led_driver);
}

module_init(hello_init);
module_exit(hello_exit);
