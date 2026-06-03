#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define MY_DEVICE_NAME "my_platform_device"

static struct platform_device *my_pdev;

static int my_pdrv_probe(struct platform_device *pdev)
{
    pr_info("Hello! device probed!\n");
    pr_info("Device name: %s\n", dev_name(&pdev->dev));
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    pr_info("Good bye reader!\n");
    return 0;
}

static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = MY_DEVICE_NAME,
        .owner = THIS_MODULE,
    },
};

static int __init my_module_init(void)
{
    int ret;

    pr_info("Loading platform driver module\n");

    ret = platform_driver_register(&mypdrv);
    if (ret) {
        pr_err("Failed to register platform driver\n");
        return ret;
    }

    my_pdev = platform_device_register_simple(MY_DEVICE_NAME, -1, NULL, 0);
    if (IS_ERR(my_pdev)) {
        pr_err("Failed to register platform device\n");
        platform_driver_unregister(&mypdrv);
        return PTR_ERR(my_pdev);
    }

    return 0;
}

static void __exit my_module_exit(void)
{
    pr_info("Unloading platform driver module\n");

    platform_device_unregister(my_pdev);
    platform_driver_unregister(&mypdrv);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Madieu");
MODULE_DESCRIPTION("My platform Hello World module");