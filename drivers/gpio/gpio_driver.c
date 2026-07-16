/*
 * gpio_driver.c
 *
 *  Created on: Dec 26, 2025
 *      Author: wiki
 */
#include "gpio_driver.h"


void GPIO_PeriClockCont(GPIO_TypeDef *pGPIOx, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(pGPIOx == GPIOA) { GPIOA_PCLK_EN(); }
        else if(pGPIOx == GPIOB) { GPIOB_PCLK_EN(); }
        else if(pGPIOx == GPIOC) { GPIOC_PCLK_EN(); }
        else if(pGPIOx == GPIOD) { GPIOD_PCLK_EN(); }
        else if(pGPIOx == GPIOF) { GPIOF_PCLK_EN(); }
    }
    else
    {
        if(pGPIOx == GPIOA) { GPIOA_PCLK_DI(); }
        else if(pGPIOx == GPIOB) { GPIOB_PCLK_DI(); }
        else if(pGPIOx == GPIOC) { GPIOC_PCLK_DI(); }
        else if(pGPIOx == GPIOD) { GPIOD_PCLK_DI(); }
        else if(pGPIOx == GPIOF) { GPIOF_PCLK_DI(); }
    }
}

void GPIO_Init(GPIO_Handle_t *pGPIO_Handle)
{
    uint32_t temp = 0;

    // 1. Configure mode
    if(pGPIO_Handle->GPIO_PinConfig.Mode <= MY_GPIO_MODE_ANALOG)
    {
        temp = (pGPIO_Handle->GPIO_PinConfig.Mode << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIO_Handle->pGPIOx->MODER &= ~(0x03u << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
        pGPIO_Handle->pGPIOx->MODER |= temp;
    }
    else
    {
        // This part is for the interrupt
        if(pGPIO_Handle->GPIO_PinConfig.Mode == GPIO_MODE_IT_FT)
        {
            EXTI->FTSR1 |= (1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->RTSR1 &= ~(1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIO_Handle->GPIO_PinConfig.Mode == GPIO_MODE_IT_RT)
        {
            EXTI->RTSR1 |= (1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR1 &= ~(1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
        }
        else if(pGPIO_Handle->GPIO_PinConfig.Mode == GPIO_MODE_IT_RTFT)
        {
            EXTI->RTSR1 |= (1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR1 |= (1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
        }

        // Configure gpio port selection in EXTICR
        uint8_t temp1 = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber / 4;
        uint8_t temp2 = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber % 4;
        uint8_t portcode = GPIO_BASEADDRESS_TO_CODE(pGPIO_Handle->pGPIOx);

        // Clear the specific EXTI bits first, then set them
        // On Cortex-M0+ (STM32C0), EXTI line selection takes 8 bits per pin!
        EXTI->EXTICR[temp1] &= ~(0xFFu << (temp2 * 8));
        EXTI->EXTICR[temp1] |=  (portcode << (temp2 * 8));

        // Enable exti interrupt delivery using IMR
        EXTI->IMR1 |= (1 << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
    }

    // 2. Configure speed
    temp = 0;
    temp = (pGPIO_Handle->GPIO_PinConfig.Speed << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIO_Handle->pGPIOx->OSPEEDR &= ~(0x03u << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIO_Handle->pGPIOx->OSPEEDR |= temp;

    // 3. Configure pupd register
    temp = 0;
    temp = (pGPIO_Handle->GPIO_PinConfig.PuPdControl << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIO_Handle->pGPIOx->PUPDR &= ~(0x03u << (2 * pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIO_Handle->pGPIOx->PUPDR |= temp;

    // 4. Configure optype register (NOTE: OTYPER is 1 bit per pin, NOT 2!)
    temp = 0;
    temp = (pGPIO_Handle->GPIO_PinConfig.OPType << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIO_Handle->pGPIOx->OTYPER &= ~(0x01u << pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIO_Handle->pGPIOx->OTYPER |= temp;

    // 5. Configure alt functionality
    if (pGPIO_Handle->GPIO_PinConfig.Mode == GPIO_MODE_ALTFN)
    {
        uint8_t temp1 = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber / 8;
        uint8_t temp2 = pGPIO_Handle->GPIO_PinConfig.GPIO_PinNumber % 8;

        pGPIO_Handle->pGPIOx->AFR[temp1] &= ~(0x0Fu << (4 * temp2));
        pGPIO_Handle->pGPIOx->AFR[temp1] |= (pGPIO_Handle->GPIO_PinConfig.AltFuncType << (4 * temp2));
    }
}

void GPIO_DeInit(GPIO_TypeDef *pGPIOx)
{
    if(pGPIOx == GPIOA) { GPIOA_REG_RESET(); }
    else if(pGPIOx == GPIOB) { GPIOB_REG_RESET(); }
    else if(pGPIOx == GPIOC) { GPIOC_REG_RESET(); }
    else if(pGPIOx == GPIOD) { GPIOD_REG_RESET(); }
    else if(pGPIOx == GPIOF) { GPIOF_REG_RESET(); }
}


// Read and write
int8_t GPIO_Read_InPin(GPIO_TypeDef *pGPIOx, uint8_t Pin)
{
    uint8_t value;
    value = (uint8_t)((pGPIOx->IDR >> Pin) & 0x01u);
    return value;
}


int16_t GPIO_Read_InPort(GPIO_TypeDef *pGPIOx)
{
    uint16_t value;
    value = (uint16_t)(pGPIOx->IDR);
    return value;
}


void GPIO_Write_OutPin(GPIO_TypeDef *pGPIOx, uint8_t Pin, uint8_t value)
{
    if (value == GPIO_PIN_SET)
    {
        pGPIOx->ODR |= (1u << Pin);
    }
    else
    {
        pGPIOx->ODR &= ~(1u << Pin);
    }
}

void GPIO_Write_OutPort(GPIO_TypeDef *pGPIOx, uint16_t value)
{
    pGPIOx->ODR = value;
}

void GPIO_Toggle_OutPin(GPIO_TypeDef *pGPIOx, uint8_t Pin)
{
    pGPIOx->ODR ^= (1u << Pin); // XOR for toggling
}


// Interrupt handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == 1) // 1 means ENABLE
    {
        // Enable the interrupt in ISER0. (Cortex-M0+ only has 1 register)
        *(volatile uint32_t*)0xE000E100 |= (1 << IRQNumber);
    }
    else
    {
        // Disable the interrupt in ICER0.
        *(volatile uint32_t*)0xE000E180 |= (1 << IRQNumber);
    }
}

void GPIO_IRQHandler(uint8_t Pin)
{
    // 1. Check if the interrupt was triggered by a RISING edge on this pin
    if (EXTI->RPR1 & (1 << Pin))
    {
        // Clear the EXTI PR bit by writing '1' to it
        EXTI->RPR1 = (1 << Pin);
    }

    // 2. Check if the interrupt was triggered by a FALLING edge on this pin
    if (EXTI->FPR1 & (1 << Pin))
    {
        // Clear the EXTI PR bit by writing '1' to it
        EXTI->FPR1 = (1 << Pin);
    }
}
