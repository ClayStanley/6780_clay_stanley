#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <assert.h>
 void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    GPIOC->MODER &= ~(0b11 << (6 * 2)); // Clear mode bits for PA5
    GPIOC->MODER |=  (0b01 << (6 * 2)); // Set PA5 to output mode (01)
    assert(GPIOC->MODER ==  0x1000);
    GPIOC->OTYPER &=  ~(1 << 6);
    GPIOC->OTYPER |=  (1 << 6);
    assert(GPIOC->OTYPER ==  0x0040);
    GPIOC->OSPEEDR &=  ~(0b11 << (6 * 2));
    GPIOC->OSPEEDR |=  (0b11 << (6 * 2));
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->PUPDR &= ~(0b11 << (6 * 2));
    GPIOC->PUPDR |= (0b11 << (6 * 2));
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->ODR &=  ~(1 << 6);
    GPIOC->ODR |=  (1 << 6);
    assert(GPIOC->ODR ==  0x0040);

    GPIOC->MODER &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
    GPIOC->MODER |=  (0b01 << (7 * 2)); // Set PA5 to output mode (01)
    assert(GPIOC->MODER ==  0x1000);
    GPIOC->OTYPER &=  ~(1 << 7);
    GPIOC->OTYPER |=  (1 << 7);
    assert(GPIOC->OTYPER ==  0x0080);
    GPIOC->OSPEEDR &=  ~(0b11 << (7 * 2));
    GPIOC->OSPEEDR |=  (0b11 << (7 * 2));
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->PUPDR &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
    GPIOC->PUPDR |= (0b11 << (7 * 2)); // Clear mode bits for PA5
    assert(GPIOC->PUPDR ==  0xC000);
    GPIOC->ODR &=  ~(1 << 7);
    GPIOC->ODR |=  (1 << 7);
    assert(GPIOC->ODR ==  0x0080);


    GPIOC->MODER &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
    GPIOC->MODER |=  (0b01 << (8 * 2)); // Set PA5 to output mode (01)
    assert(GPIOC->MODER ==  0x1000);
    GPIOC->OTYPER &=  ~(1 << 8);
    GPIOC->OTYPER |=  (1 << 8);
    assert(GPIOC->OTYPER ==  0x0100);
    GPIOC->OSPEEDR &=  ~(0b11 << (8 * 2));
    GPIOC->OSPEEDR |=  (0b11 << (8 * 2));
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->PUPDR &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
    GPIOC->PUPDR |= (0b11 << (8 * 2)); // Clear mode bits for PA5
    assert(GPIOC->PUPDR ==  0x3000);
    GPIOC->ODR &=  ~(1 << 8);
    GPIOC->ODR |=  (1 << 8);
    assert(GPIOC->ODR ==  0x0100);

    GPIOC->MODER &= ~(0b11 << (9* 2)); // Clear mode bits for PA5
    GPIOC->MODER |=  (0b01 << (9 * 2)); // Set PA5 to output mode (01)
    assert(GPIOC->MODER ==  0x1000);
    GPIOC->OTYPER &=  ~(1 << 9);
    GPIOC->OTYPER |=  (1 << 9);
    assert(GPIOC->OTYPER ==  0x0200);
    GPIOC->OSPEEDR &=  ~(0b11 << (9 * 2));
    GPIOC->OSPEEDR |=  (0b11 << (9 * 2));
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->PUPDR &= ~(0b11 << (9 * 2)); // Clear mode bits for PA5
    GPIOC->PUPDR |= (0b11 << (9 * 2)); // Clear mode bits for PA5
    assert(GPIOC->PUPDR ==  0x6000);
    GPIOC->ODR &=  ~(1 << 9);
    GPIOC->ODR |=  (1 << 9);
    assert(GPIOC->ODR ==  0x0200);


    GPIOA->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
    GPIOA->MODER |=  (0b01 << (0)); // Set PA5 to output mode (01)
    assert(GPIOA->MODER ==  0x1000);
    GPIOC->OSPEEDR &=  ~(0b11 << 0);
    GPIOC->OSPEEDR |=  (0b11 << 0);
    assert(GPIOC->OSPEEDR ==  0x3000);
    GPIOC->PUPDR &= ~(0b10 << 0);
    GPIOC->PUPDR |= (0b10 << 0);
    assert(GPIOC->PUPDR ==  0x2000);
} 

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/

/*
void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
}
*/

/*
void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}
*/
