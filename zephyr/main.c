/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

#ifdef BUILD_QEMU
void main(void)
{
    while(1) {
        k_sleep(SLEEP_TIME);
    }
}
#else
/* Change this if you have an LED connected to a custom port */
#ifndef LED0_GPIO_CONTROLLER
#define LED0_GPIO_CONTROLLER 	LED0_GPIO_PORT
#endif

#define LED_PORT LED0_GPIO_CONTROLLER

/* Change this if you have an LED connected to a custom pin */
#define LED	LED0_GPIO_PIN

void main(void)
{
	int cnt = 0;
	struct device *dev;

	dev = device_get_binding(LED_PORT);
	/* Set LED pin as output */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		gpio_pin_write(dev, LED, cnt % 2);
		cnt++;
		k_sleep(SLEEP_TIME);
	}
}
#endif
