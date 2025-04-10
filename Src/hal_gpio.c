#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>
#include <assert.h>
#pragma GCC optimize ("O0")
 void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    // Enable clock for GPIOC.
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Configure PC6, PC7, PC8, and PC9 as general-purpose outputs.
    GPIOC->MODER &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));
    GPIOC->MODER |=  ((1UL << (6 * 2)) | (1UL << (7 * 2)) | (1UL << (8 * 2)) | (1UL << (9 * 2)));
    
    // Configure output type as push-pull (default) and low speed, no pull-up/pull-down.
    GPIOC->OTYPER &= ~((1UL << 6) | (1UL << 7) | (1UL << 8) | (1UL << 9));
    GPIOC->OSPEEDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));
    GPIOC->PUPDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));

    GPIOA->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
        //GPIOA->MODER |=  1; // Set PA5 to output mode (01)
        GPIOA->MODER |=  (0b00 << (0));
       // assert(GPIOA->MODER & 1);
        GPIOA->OSPEEDR &=  ~(0b11 << 0);
        GPIOA->OSPEEDR |=  (0b0 << 0);
       // assert(GPIOA->OSPEEDR ==  0x0C000003);
        GPIOA->PUPDR &= ~(0b10 << 0);
        GPIOA->PUPDR |= (0b10 << 0);
        //assert(GPIOA->PUPDR ==  0x24000002);
        // GPIOA->IDR &= ~(0b10 << 0);
        // GPIOA->IDR |= (0b10 << 0);
}

/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

     return GPIOA->IDR & 0x1;

}


// void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
//     if (PinState == GPIO_PIN_SET) {
//         GPIOx->ODR |= GPIO_Pin;  // Set pin HIGH
//     } else {
//         GPIOx->ODR &= ~GPIO_Pin; // Set pin LOW
//     }
// }
void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    GPIOC->ODR &=  ~(1 << 6);
    GPIOC->ODR |=  (0 << 6);
    //assert(GPIOC->ODR ==  0x01c0);

    GPIOC->ODR &=  ~(1 << 7);
    GPIOC->ODR |=  (0 << 7);
}

// /**
//   * @brief  Toggle the specified GPIO pin.
//   * @param  GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
//   * @param  GPIO_Pin specifies the pin to be toggled.
//   * @retval None
//   */
//  #define GPIO_NUMBER           16U


void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{       
   
    GPIOC->ODR ^=  (1 << 6);
    GPIOC->ODR ^=  (1 << 7);

    }    
//     // //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
//     // uint32_t odr;

//     // /* Check the parameters */
//     // assert_param(IS_GPIO_PIN(GPIO_Pin));
  
//     // /* get current Output Data Register value */
//     // odr = GPIOx->ODR;
    
//     // /* Set selected pins that were at low level, and reset ones that were high */
//     // GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);
    
// }

// section 2.2
// void configure_EXTI0_for_PA0(void)
// {
//     // Enable/unmask interrupt generation on EXTI line 0.
//     EXTI->IMR |= (1 << 0);
    
//     // Configure EXTI line 0 to trigger on the rising edge.
//     EXTI->RTSR |= (1 << 0);
// }

void configure_SYSCFG_EXTI_PA0(void)
{
        // Enable/unmask interrupt generation on EXTI line 0.
    EXTI->IMR |= (1 << 0);
    
    // Configure EXTI line 0 to trigger on the rising edge.
    EXTI->RTSR |= (1 << 0);
    // 1. Enable the SYSCFG peripheral clock.
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
    // 2 & 3. For EXTI line 0, the configuration is in SYSCFG->EXTICR[0] (bits [3:0]).
    // Clear the lower 4 bits (for EXTI0)...
    SYSCFG->EXTICR[0] &= ~(0xF);
    // ...and set them to 0 to select port A (PA0). (0 represents Port A per reference manual.)
    SYSCFG->EXTICR[0] |= (0 << 0);
}


// {
    
//     GPIOC->MODER &= ~(0b11 << (6 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0b01 << (6 * 2)); // Set PA5 to output mode (01)
//     assert(GPIOC->MODER ==  0x1000);
//     GPIOC->OTYPER &=  ~(1 << 6);
//     // GPIOC->OTYPER |=  (0 << 6);
//     // assert(GPIOC->OTYPER ==  0x0000);
//     GPIOC->OTYPER |=  (1 << 6);
//     // GPIOC->OSPEEDR &=  ~(0b11 << (6 * 2));
//     // GPIOC->OSPEEDR |=  (0b11 << (6 * 2));
//     // assert(GPIOC->OSPEEDR ==  0x3000);
//     GPIOC->OSPEEDR &=  ~(0b11 << (6 * 2));
//     GPIOC->OSPEEDR |=  (0b0 << (6 * 2));
//     assert(GPIOC->OSPEEDR ==  0x0000);
//     GPIOC->PUPDR &= ~(0b11 << (6 * 2));
//     GPIOC->PUPDR |= (0b0 << (6 * 2));
//     assert(GPIOC->PUPDR ==  0x0000);
//     GPIOC->ODR &=  ~(1 << 6);
//     GPIOC->ODR |=  (0 << 6);
//     assert(GPIOC->ODR ==  0x0040);
   

//     GPIOC->MODER &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0b01 << (7 * 2)); // Set PA5 to output mode (01)
//     assert(GPIOC->MODER ==  0x5000);
//     GPIOC->OTYPER &=  ~(1 << 7);
//     // GPIOC->OTYPER |=  (0 << 7);
//     // assert(GPIOC->OTYPER ==  0x0000);
//     GPIOC->OTYPER |=  (1 << 7);
//     // GPIOC->OSPEEDR &=  ~(0b11 << (7 * 2));
//     // GPIOC->OSPEEDR |=  (0b11 << (7 * 2));
//     // assert(GPIOC->OSPEEDR ==  0xf000);
//     GPIOC->OSPEEDR &=  ~(0b11 << (7 * 2));
//     GPIOC->OSPEEDR |=  (0b0 << (7 * 2));
//     assert(GPIOC->OSPEEDR ==  0x0000);
//     GPIOC->PUPDR &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
//     GPIOC->PUPDR |= (0b0 << (7 * 2)); // Clear mode bits for PA5
//     assert(GPIOC->PUPDR ==  0x0000);
//     GPIOC->ODR &=  ~(1 << 7);
//     GPIOC->ODR |=  (0 << 7);
//     assert(GPIOC->ODR ==  0x00c0);


//     GPIOC->MODER &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0b00 << (8 * 2)); // Set PA5 to output mode (01)
//     assert(GPIOC->MODER ==  0x15000);
//     GPIOC->OTYPER &=  ~(1 << 8);
//     GPIOC->OTYPER |=  (0 << 8);
//     assert(GPIOC->OTYPER ==  0x0000);
//     GPIOC->OSPEEDR &=  ~(0b11 << (8 * 2));
//     GPIOC->OSPEEDR |=  (0b0 << (8 * 2));
//     assert(GPIOC->OSPEEDR ==  0x0000);
//     GPIOC->PUPDR &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
//     GPIOC->PUPDR |= (0b0 << (8 * 2)); // Clear mode bits for PA5
//     assert(GPIOC->PUPDR ==  0x0000);
    

//     GPIOC->MODER &= ~(0b11 << (9* 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0b01 << (9 * 2)); // Set PA5 to output mode (01)
//     assert(GPIOC->MODER ==  0x55000);
//     GPIOC->OTYPER &=  ~(1 << 9);
//     GPIOC->OTYPER |=  (0 << 9);
//     assert(GPIOC->OTYPER ==  0x0000);
//     GPIOC->OSPEEDR &=  ~(0b11 << (9 * 2));
//     GPIOC->OSPEEDR |=  (0b0 << (9 * 2));
//     assert(GPIOC->OSPEEDR ==  0x0000);
//     GPIOC->PUPDR &= ~(0b11 << (9 * 2)); // Clear mode bits for PA5
//     GPIOC->PUPDR |= (0b0 << (9 * 2)); // Clear mode bits for PA5
//     assert(GPIOC->PUPDR ==  0x0000);
   

//     // GPIO_TypeDef *a = GPIOA;
//     // uint32_t *b = (uint32_t*) GPIOA;
//     // assert(a->MODER ==  0x28000000);
//     GPIOA->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
//     //GPIOA->MODER |=  1; // Set PA5 to output mode (01)
//     GPIOA->MODER |=  (0b00 << (0));
//    // assert(GPIOA->MODER & 1);
//     GPIOA->OSPEEDR &=  ~(0b11 << 0);
//     GPIOA->OSPEEDR |=  (0b0 << 0);
//    // assert(GPIOA->OSPEEDR ==  0x0C000003);
//     GPIOA->PUPDR &= ~(0b10 << 0);
//     GPIOA->PUPDR |= (0b10 << 0);
//     //assert(GPIOA->PUPDR ==  0x24000002);
//     // GPIOA->IDR &= ~(0b10 << 0);
//     // GPIOA->IDR |= (0b10 << 0);
// } 
