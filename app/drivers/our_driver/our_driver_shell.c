#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

static int cmd_channel_fetch_handler(const struct shell *sh, int argc, char** argv){
    //shell_info(sh, "Hello from Fetch channel");

    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver1));

    int ret = sensor_sample_fetch(driver);

    if (ret != 0){
         shell_error(sh, "Cannot read drive for this device");
    } 
    else
    {
        shell_print(sh, "Success with return value: %d", ret);
    }

    return 0;
}

static int cmd_channel_read_handler(const struct shell *sh, int argc, char** argv){
    //shell_print(sh, "Hello from Get channel");

    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver1));

    struct sensor_value val;

    int ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);

    if (ret != 0){
         shell_error(sh, "Cannot read drive for this device");
    }
    else
    {
        shell_print(sh, "Success with return value: %d", ret);
    }

    return 0;
}

static int cmd_channel_info_handler(const struct shell *sh, int argc, char** argv){

    const char *dev_name = "STM32_UART"; 

    const struct device *dev = device_get_binding(dev_name);

    if (dev == NULL) {
        shell_error(sh, "Device '%s' not found or failed to initialize.", dev_name);
        return -ENODEV;
    }

    bool is_ready = device_is_ready(dev);

    shell_print(sh, "Device Name: %s | Ready State: %s", 
                dev->name, 
                is_ready ? "READY" : "NOT READY");

    return 0;
}

static int cmd_sensor_set_handler(const struct shell *sh, int argc, char **argv) {

    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver1)); 
    
    if (!device_is_ready(dev)) {
        shell_error(sh, "Error: Sensor/LED device driver not ready.");
        return -ENODEV;
    }

    char *str = argv[1];
    long value = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            shell_error(sh, "Error: Invalid argument format. Expected an integer.");
            return -EINVAL;
        }

        value = (value * 10) + (str[i] - '0');
        i++;
    }
    
    if (value < 10 || value > 500) {
        shell_error(sh, "Error: Value %ld is out of range [%d, %d].", 
                    value, 10, 500);
        return -ERANGE;
    }

    int ret = our_driver_set_led_blinkrate(dev, (int)value);

    if (ret < 0) {
        shell_error(sh, "Error: Driver failed to apply setting (Error code: %d).", ret);
        return ret;
    }

    shell_print(sh, "Successfully set driver blink rate to %ld ms.", value);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd, 
    SHELL_CMD_ARG(fetch, NULL, "Fetch channel of my driver", cmd_channel_fetch_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Get channel of my driver", cmd_channel_read_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Show device Information", cmd_channel_info_handler, 1, 0),
    SHELL_CMD_ARG(set, NULL, "Set sensor blink rate", cmd_sensor_set_handler, 2, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &our_driver_subcmd, "Our driver set of commands", NULL);