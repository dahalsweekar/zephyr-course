#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

#define LED_NODE DT_ALIAS(warningled)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

bool led_state = false;

int our_driver_set_led_blinkrate(const struct device* dev, int rate){

    LOG_INF("The fake blink rate for this device is: %d", rate);
    return 0;
}

static int channel_fetch_implementation(const struct device *dev, enum sensor_channel chan){

    LOG_INF("Hello from Sensor Fetch");

    gpio_pin_set_dt(&led, 1);

    led_state = !led_state;

    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

    return 0;
}

static int channel_get_implementation(const struct device *dev,
                                        enum sensor_channel chan, 
                                        struct sensor_value *val)
{
    LOG_INF("Hello from channel Get, channel %d", chan);

    gpio_pin_set_dt(&led, 0);

    led_state = !led_state;

    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

    return 0;
}

static DEVICE_API(sensor, api_iomico_homework) = {
    .sample_fetch = channel_fetch_implementation,
    .channel_get = channel_get_implementation, 
};

static int init(const struct device* dev){
    LOG_INF("Device Initialized");

    return 0;
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_homework);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);