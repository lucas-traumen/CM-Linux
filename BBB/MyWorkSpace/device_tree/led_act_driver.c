#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/leds.h>

#define DRIVER_NAME "gpio_leds_act"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MInh Pham");
MODULE_DESCRIPTION("ACT LED Platform Driver for Pi Zero W");
MODULE_VERSION("1.0");

struct act_led_priv {
    struct led_classdev cdev; // Core structure from LED subsystem
    int gpio;
    bool active_low;
};

static void act_led_brightness_set(struct led_classdev *led_cdev,
                                   enum led_brightness brightness)
{
    struct act_led_priv *priv = container_of(led_cdev, struct act_led_priv, cdev);
    int value = priv->active_low ? (brightness == LED_OFF) : (brightness != LED_OFF);
    gpio_set_value(priv->gpio, value);
}

static enum led_brightness act_led_brightness_get(struct led_classdev *led_cdev)
{
    struct act_led_priv *priv = container_of(led_cdev, struct act_led_priv, cdev);
    int value = gpio_get_value(priv->gpio);
    return (priv->active_low ? !value : value) ? LED_FULL : LED_OFF;
}

static int act_led_parse_dt(struct platform_device *pdev, struct act_led_priv *priv)
{
    struct device_node *child;
    enum of_gpio_flags flags;
    const char *label, *trigger;
    int gpio;

    child = of_get_child_by_name(pdev->dev.of_node, "led-act");
    if (!child)
        return -ENODEV;

    gpio = of_get_named_gpio_flags(child, "gpios", 0, &flags);
    if (gpio < 0) {
        of_node_put(child);
        return gpio;
    }

    priv->gpio = gpio;
    priv->active_low = !!(flags & OF_GPIO_ACTIVE_LOW);

    /* Get label */
    if (of_property_read_string(child, "label", &label) == 0)
        priv->cdev.name = devm_kstrdup(&pdev->dev, label, GFP_KERNEL);
    else
        priv->cdev.name = "act";

    /* Get default trigger */
    if (of_property_read_string(child, "linux,default-trigger", &trigger) == 0)
        priv->cdev.default_trigger = devm_kstrdup(&pdev->dev, trigger, GFP_KERNEL);

    of_node_put(child);
    return 0;
}

static int act_led_probe(struct platform_device *pdev)
{
    struct act_led_priv *priv;
    int ret;

    priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
    if (!priv)
        return -ENOMEM;

    /* Associate private data priv with this device pdev */
    platform_set_drvdata(pdev, priv);

    /* Parse device tree */
    ret = act_led_parse_dt(pdev, priv);
    if (ret)
        return ret;

    /* Request and configure GPIO */
    ret = devm_gpio_request(&pdev->dev, priv->gpio, priv->cdev.name);
    if (ret)
        return ret;

    ret = gpio_direction_output(priv->gpio, priv->active_low ? 1 : 0);
    if (ret)
        return ret;

    /* Setup and register LED */
    priv->cdev.brightness_set = act_led_brightness_set;
    priv->cdev.brightness_get = act_led_brightness_get;
    priv->cdev.max_brightness = LED_FULL;
    priv->cdev.brightness = LED_OFF;

    ret = devm_led_classdev_register(&pdev->dev, &priv->cdev);
    if (ret)
        return ret;

    dev_info(&pdev->dev, "LED '%s' registered (GPIO %d, %s)\n",
             priv->cdev.name, priv->gpio, 
             priv->active_low ? "active-low" : "active-high");

    return 0;
}

static int act_led_remove(struct platform_device *pdev)
{
    struct act_led_priv *priv = platform_get_drvdata(pdev);
    
    /* Turn off LED */
    gpio_set_value(priv->gpio, priv->active_low ? 1 : 0);
    
    dev_info(&pdev->dev, "LED driver removed\n");
    return 0;
}

static const struct of_device_id act_led_of_match[] = {
    { .compatible = "custom,gpio-leds-act" },
    { }
};
MODULE_DEVICE_TABLE(of, act_led_of_match);

static struct platform_driver act_led_platform_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = act_led_of_match,
    },
    .probe = act_led_probe,
    .remove = act_led_remove,
};

/* Allows modprobe to auto-load driver when device is present */
module_platform_driver(act_led_platform_driver);



