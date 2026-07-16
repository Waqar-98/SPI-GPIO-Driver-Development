/*
 * gpio_driver.h
 *
 *  Created on: Dec 26, 2025
 *      Author: wiki
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_
/*
 * The GPIO peripheral is to be consisted of following configurable item for user application
 * GPIO part name
 * GPIo pin Number
 * GPIo mode
 * GPIO speed
 * GPIO outputType
 * GPIO Pullup-pulldown
 * GPIO Alternate function
 */
#include "stm32c031xx.h"
typedef struct
{
	uint8_t GPIO_PinNumber;//uint8_t kept based on the values its representing the pins are 15 so 8 bits are enough for the bits likewise based on registers
	uint8_t Mode;
	uint8_t Speed;
	uint8_t PuPdControl;
	uint8_t OPType;
	uint8_t AltFuncType;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_TypeDef *pGPIOx; //BaseAddress;//Holds the base address of the the port which the gpio pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;//Holds Pin Configuration Settings

}GPIO_Handle_t;

#include <stdio.h>

//macro for resetting the port using the rcc resgister
#define GPIOA_REG_RESET()   do{ (RCC->IOPRSTR |= (1<<0)); (RCC->IOPRSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RESET()   do{ (RCC->IOPRSTR |= (2<<0)); (RCC->IOPRSTR &= ~(2<<0));}while(0)
#define GPIOC_REG_RESET()   do{ (RCC->IOPRSTR |= (3<<0)); (RCC->IOPRSTR &= ~(3<<0));}while(0)
#define GPIOD_REG_RESET()   do{ (RCC->IOPRSTR |= (4<<0)); (RCC->IOPRSTR &= ~(4<<0));}while(0)
#define GPIOF_REG_RESET()   do{ (RCC->IOPRSTR |= (6<<0)); (RCC->IOPRSTR &= ~(6<<0));}while(0)

//GPIO PIN Numbers
#define GPIO_PIN_NO_0 0
#define GPIO_PIN_NO_1 1
#define GPIO_PIN_NO_2 2
#define GPIO_PIN_NO_3 3

#define GPIO_PIN_NO_4 4
#define GPIO_PIN_NO_5 5
#define GPIO_PIN_NO_6 6
#define GPIO_PIN_NO_7 7
#define GPIO_PIN_NO_8 8
#define GPIO_PIN_NO_9 9
#define GPIO_PIN_NO_10 10
#define GPIO_PIN_NO_11 11
#define GPIO_PIN_NO_12 12
#define GPIO_PIN_NO_13 13
#define GPIO_PIN_NO_14 14
#define GPIO_PIN_NO_15 15



//macro to setup the port address for
#define GPIO_BASEADDRESS_TO_CODE(x)  (  (x==GPIOA)? 0 :\
										(x==GPIOB)? 1 :\
										(x==GPIOC)? 2 :\
										(x==GPIOD)? 3 :\
										(x==GPIOF)? 5 :0)

/*Macros for gpio modes
 * taken from the dataseet
 */
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1
#define GPIO_MODE_ALTFN 2
#define GPIO_MODE_ANALOG 3

#define GPIO_MODE_IT_FT 4//our made based on the trigger of interrupt
#define GPIO_MODE_IT_RT 5
#define GPIO_MODE_IT_RTFT 6
//GPIO Pin possible output types
//from the datasheet
#define GPIO_OP_TYPE_PP 0;
#define GPIO_OP_TYPE_OD 1;


//GPIO pin possible output speeds
#define GPIO_SPEED_LOW 0;
#define GPIO_SPEED_MEDIUM 1;
#define GPIO_SPEED_HIGH 2;

//GPIO PULL UP PULL DOWN configurations

#define GPIO_NO_PUPD 0;
#define GPIO_PIN_PU 1;
#define GPIO_PIN_PD 2;


/*Generic Macros
 *
 *
 */
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 1


// Clock Enable Macros
#define GPIOA_PCLK_EN()        (RCC->IOPENR |= RCC_IOPENR_GPIOAEN)
#define GPIOB_PCLK_EN()        (RCC->IOPENR |= RCC_IOPENR_GPIOBEN)
#define GPIOC_PCLK_EN()        (RCC->IOPENR |= RCC_IOPENR_GPIOCEN)
#define GPIOD_PCLK_EN()        (RCC->IOPENR |= RCC_IOPENR_GPIODEN)
#define GPIOF_PCLK_EN()        (RCC->IOPENR |= RCC_IOPENR_GPIOFEN)

// Clock Disable Macros (You need these for your else block!)
#define GPIOA_PCLK_DI()        (RCC->IOPENR &= ~RCC_IOPENR_GPIOAEN)
#define GPIOB_PCLK_DI()        (RCC->IOPENR &= ~RCC_IOPENR_GPIOBEN)
#define GPIOC_PCLK_DI()        (RCC->IOPENR &= ~RCC_IOPENR_GPIOCEN)
#define GPIOD_PCLK_DI()        (RCC->IOPENR &= ~RCC_IOPENR_GPIODEN)
#define GPIOF_PCLK_DI()        (RCC->IOPENR &= ~RCC_IOPENR_GPIOFEN)

/*Driver APIs requirements
 * Initialisation
 * Enable Disable of GPIO port
 * read from gpio pin
 * write to gpio pin
 * Configure alternate functionality
 * Interrupt handling
 */
//Prototypes of the APIs

//init and deinit
void GPIO_Init(GPIO_Handle_t *pGPIO_Handle);//pointer to handle structure
void GPIO_DeInit(GPIO_TypeDef *pGPIOx);//to do important register in RCC register -Peripheral reset register what that need the base address of peripheral to know which port is to be

//clock setup enable or disable
void GPIO_PeriClockCont(GPIO_TypeDef *pGPIOx,uint8_t EnorDi);//enable or diable clock on given gpio base address

//read and write
int8_t GPIO_Read_InPin(GPIO_TypeDef *pGPIOx, uint8_t Pin);//return will be 0 or 1 so bool or int8_t is enough
int16_t GPIO_Read_InPort(GPIO_TypeDef *pGPIOx);//port is of 16 bits so return is set accordingly
void GPIO_Write_OutPin(GPIO_TypeDef *pGPIOx, uint8_t Pin, uint8_t value);
void GPIO_Write_OutPort(GPIO_TypeDef *pGPIOx, uint16_t value);
void GPIO_Toggle_OutPin(GPIO_TypeDef *pGPIOx, uint8_t Pin);

//interrupt handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);//to setup irq number of gpio pin and setting the iser and icer
void GPIO_IRQHandler(uint8_t Pin);//When irq happen to handle the trigger of which pin
void IRQ_PriorConfig(uint8_t IRQNumber,uint8_t IRQPriority);



#endif /* INC_GPIO_DRIVER_H_ */
