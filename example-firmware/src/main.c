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
static struct device *tx_eq0;
static struct device *tx_eq1;

#define TX_EQ0_PORT DT_GPIO_GECKO_PORTA_NAME
#define TX_EQ0_PIN 9
#define TX_EQ1_PORT DT_GPIO_GECKO_PORTA_NAME
#define TX_EQ1_PIN 10

static int i2c_cb(struct i2c_slave_config *config, uint8_t val)
{
	if (val == 0x0) {
		gpio_pin_write(tx_eq0, TX_EQ0_PIN, 0);
		gpio_pin_write(tx_eq1, TX_EQ1_PIN, 0);
		gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 0);
		gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 0);
	} else if (val == 0x1) {
		gpio_pin_write(tx_eq0, TX_EQ0_PIN, 1);
		gpio_pin_write(tx_eq1, TX_EQ1_PIN, 0);
		gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 1);
		gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 0);
	} else if (val == 0x2) {
		gpio_pin_write(tx_eq0, TX_EQ0_PIN, 0);
		gpio_pin_write(tx_eq1, TX_EQ1_PIN, 1);
		gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 0);
		gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 1);
	} else if (val == 0x3) {
		gpio_pin_write(tx_eq0, TX_EQ0_PIN, 1);
		gpio_pin_write(tx_eq1, TX_EQ1_PIN, 1);
		gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 1);
		gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 1);
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

	tx_eq0 = device_get_binding(TX_EQ0_PORT);
	tx_eq1 = device_get_binding(TX_EQ1_PORT);

	if (!tx_eq0 && !tx_eq1)
		printf("binding equalizer setting pins failed\n");

        gpio_pin_configure(led0, DT_ALIAS_LED0_GPIOS_PIN, GPIO_DIR_OUT);
        gpio_pin_write(led0, DT_ALIAS_LED0_GPIOS_PIN, 0);
        gpio_pin_configure(led1, DT_ALIAS_LED1_GPIOS_PIN, GPIO_DIR_OUT);
        gpio_pin_write(led1, DT_ALIAS_LED1_GPIOS_PIN, 0);

	gpio_pin_configure(tx_eq0, TX_EQ0_PIN, GPIO_DIR_OUT);
	gpio_pin_write(tx_eq0, TX_EQ0_PIN, 0);
	gpio_pin_configure(tx_eq1, TX_EQ1_PIN, GPIO_DIR_OUT);
	gpio_pin_write(tx_eq1, TX_EQ1_PIN, 0);

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
