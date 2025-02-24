//#include "\Users\Clay Stanley\6780_clay_stanley\Drivers\STM32F0xx_HAL_Driver\Inc\stm32f0xx_hal.h"
  #include <stm32f0xx_hal.h>
  #include <main.h>
  #include <assert.h>
  #include <hal_gpio.h>
int lab1_main(void) {
    HAL_Init(); // Reset of all peripherals, init the Flash and Systick
    SystemClock_Config();//Configure the system clock
    /* This example uses HAL library calls to control
    the GPIOC peripheral. You’ll be redoing this code
    with hardware register access. */
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};
    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high
    
    while (1) {
        HAL_Delay(200); // Delay 200ms
        // Toggle the output state of both PC8 and PC9
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    }
}

// int lab1_main(void) {
//   HAL_Init(); // Reset of all peripherals, init the Flash and Systick
//   SystemClock_Config();//Configure the system clock
//   /* This example uses HAL library calls to control
//   the GPIOC peripheral. You’ll be redoing this code
//   with hardware register access. */
//   __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
//   // Set up a configuration struct to pass to the initialization function
//   GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
//                               GPIO_MODE_OUTPUT_PP,
//                               GPIO_SPEED_FREQ_LOW,
//                               GPIO_NOPULL};
//   My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
//   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high
//     assert(GPIOC->MODER &= ~(0x11 << (6 * 2))); // Clear mode bits for PA5
//     assert(GPIOC->MODER |=  (0x01 << (6 * 2))); // Set PA5 to output mode (01)
//     assert(GPIOC->OTYPER &=  ~(1 << 6));
//     assert(GPIOC->OSPEEDR &=  ~(0x11 << (6 * 2)));
//     assert(GPIOC->PUPDR &= ~(0x11 << (6 * 2)));

//     GPIOC->MODER &= ~(0x11 << (7 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0x01 << (7 * 2)); // Set PA5 to output mode (01)
//     GPIOC->OTYPER &=  ~(1 << 7);
//     GPIOC->OSPEEDR &=  ~(0x11 << (7 * 2));
//     GPIOC->PUPDR &= ~(0x11 << (7 * 2)); // Clear mode bits for PA5

//     GPIOC->MODER &= ~(0x11 << (8 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0x01 << (8 * 2)); // Set PA5 to output mode (01)
//     GPIOC->OTYPER &=  ~(1 << 8);
//     GPIOC->OSPEEDR &=  ~(0x11 << (8 * 2));
//     GPIOC->PUPDR &= ~(0x11 << (8 * 2));

//     GPIOC->MODER &= ~(0x11 << (9 * 2)); // Clear mode bits for PA5
//     GPIOC->MODER |=  (0x01 << (9 * 2)); // Set PA5 to output mode (01)
//     GPIOC->OTYPER &=  ~(1 << 9);
//     GPIOC->OSPEEDR &=  ~(0x11 << (9 * 2));
//     GPIOC->PUPDR &= ~(0x11 << (9 * 2));

//     GPIOA->MODER &= ~(0x11 << (0)); // Clear mode bits for PA5
//     GPIOA->MODER |=  (0x01 << (0)); // Set PA5 to output mode (01)
//     GPIOC->OSPEEDR &=  ~(0x11 << 0);
//     GPIOC->PUPDR &= ~(0x10 << 0);
//   while (1) {
//       HAL_Delay(200); // Delay 200ms
//       // Toggle the output state of both PC8 and PC9
//       HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
//   }
// }