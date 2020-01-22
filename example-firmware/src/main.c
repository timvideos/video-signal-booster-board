/*
 * Copyright (c) 2019, Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <stdio.h>
#include <drivers/i2c.h>
#include <drivers/uart.h>
#include <stdlib.h>
#include <kernel.h>
#include <drivers/gpio.h>

static struct device *i2c_dev;
static struct device *led0;
static struct device *led1;

static int i2c_cb(struct i2c_slave_config *config, uint8_t val)
{
        static uint8_t cnt;

        if (val == 0xAA) {
                gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, cnt % 2);
                gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, cnt % 2);
                cnt++;
        }

        return 0;
}

static const struct i2c_slave_callbacks callbacks = {
        .write_received = i2c_cb,
};

void main(void)
{
	i2c_dev = device_get_binding("I2C_0");

	if (!i2c_dev)
		printf("binding failed\n");


        /* Disable watchdog timer */
        *(uint32_t *)0x40088000UL = 0;

        led0 = device_get_binding(DT_ALIAS_LED0_GPIOS_CONTROLLER);
        led1 = device_get_binding(DT_ALIAS_LED1_GPIOS_CONTROLLER);

        if (!led0 && !led1)
		printf("binding leds failed\n");

        gpio_pin_configure(led0, DT_ALIAS_LED0_GPIOS_PIN, GPIO_DIR_OUT);
        gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 0);
        gpio_pin_configure(led1, DT_ALIAS_LED1_GPIOS_PIN, GPIO_DIR_OUT);
        gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 0);

        struct i2c_slave_config cfg;
        cfg.address = 0x74;
        cfg.callbacks = &callbacks;

        int ret = i2c_slave_register(i2c_dev, &cfg);

        if (ret) {
                printf("Couldn't register as slave! \n");
        }

        while(1) {
                k_sleep(1000);
        }
}
