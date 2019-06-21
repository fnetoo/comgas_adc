/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    ADC_KSDK.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

/*
 * @brief   Application entry point.
 */
int main(void) {

	adc16_config_t adcCfg;
	adc16_channel_config_t adcChCfg;

	port_pin_config_t config = {0};

	gpio_pin_config_t gpioled1 = {

			kGPIO_DigitalOutput, 1 // configura como saída
	};
	gpio_pin_config_t gpioled2 = {

				kGPIO_DigitalOutput, 1 // configura como saída
	};
	gpio_pin_config_t gpioled3 = {

				kGPIO_DigitalOutput, 1 // configura como saída
	};

	/* Enable the clock for port B */
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTC, 2, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 3, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 4, kPORT_MuxAsGpio);

	config.mux = kPORT_MuxAsGpio;
	config.pullSelect = kPORT_PullUp;

	GPIO_PinInit(GPIOC, 2, &gpioled1);
	GPIO_PinInit(GPIOC, 3, &gpioled2);
	GPIO_PinInit(GPIOC, 4, &gpioled3);

	adcChCfg.channelNumber = 13; //PORTB3
	adcChCfg.enableInterruptOnConversionCompleted = false;

	/* Getting default config. */
	ADC16_GetDefaultConfig(&adcCfg);

	/* Inicialization ADC */
	ADC16_Init(ADC0, &adcCfg);

	while(1){

		uint32_t data;

		/*Configure the channel and starts the conversion. */
		ADC16_SetChannelConfig(ADC0, 0, &adcChCfg);

		while(ADC16_GetChannelStatusFlags(ADC0, 0) != kADC16_ChannelConversionDoneFlag);

		data = ADC16_GetChannelConversionValue(ADC0, 0);
		printf("ADC value : %01x\n", data);

		if(data <= 25){ // trocar para valores coerentes com os valores da tensão lida da bateria.

			GPIO_PortSet(GPIOC,(1 << 2));
			PRINTF("LED1 ACESO!");

		}else if((data > 25) && (data <= 50)){ // trocar para valores coerentes com os valores da tensão lida da bateria.

			GPIO_PortSet(GPIOC,(1 << 2));
			GPIO_PortSet(GPIOC,(1 << 3));
			PRINTF("leD1 e led2 ACESOS!");

		}else if(data > 75){ // trocar para valores coerentes com os valores da tensão lida da bateria.

			GPIO_PortSet(GPIOC,(1 << 2));
			GPIO_PortSet(GPIOC,(1 << 3));
			GPIO_PortSet(GPIOC,(1 << 4));
			PRINTF("led1, led2 e led3 acesos!");

		}else {

			GPIO_PortClear(GPIOC,(1 << 2));
			GPIO_PortClear(GPIOC,(1 << 3));
			GPIO_PortClear(GPIOC,(1 << 4));
			printf("Todos os leds apagados! ");
		}

	}
    return 0;
}
