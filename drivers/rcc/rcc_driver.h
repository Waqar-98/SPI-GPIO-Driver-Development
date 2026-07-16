/*
 * rcc_driver.h
 *
 *  Created on: Dec 6, 2025
 *      Author: wiki
 */

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_

#include "stm32c0xx.h"

void RCC_Config(void);          // Configure STM32C031 to run at 48 MHz using HSI
void SysTick_Init(uint32_t);    // Configure SysTick to generate 1 ms interrupts
void Delay_ms(uint32_t);        // Non-blocking delay using SysTick counter
uint32_t GetTick(void);         // Returns elapsed milliseconds since SysTick_Init()


#endif /* RCC_DRIVER_H_ */
