#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int early_board_init(void)
{
	printk("Board Initialized.\n");
	return 0;
}

SYS_INIT(early_board_init, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);