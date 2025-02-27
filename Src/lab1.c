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
    //RCC-> AHBENR |=  (1 << 19);
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable peripheral clock to TIMER2
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
                                GPIO_MODE_OUTPUT_PP,
                                GPIO_SPEED_FREQ_LOW,
                                GPIO_NOPULL};
    // HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
    My_HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9

   // HAL_GPIO_WritePin(GPIOC, 1 << 8 ); // Start PC8 high
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high
    //My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    while (1) {
        HAL_Delay(200); // Delay 200ms
        // Toggle the output state of both PC8 and PC9
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);

        
          }
        }
       // uint32_t debouncer = 0;
// while(1) {

// // When button is bouncing the bit-vector value is random since bits are set when
// //the button is high and not when it bounces low.
// }


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
//   GPIOC->MODER &= ~(0b11 << (6 * 2)); // Clear mode bits for PA5
  //   GPIOC->MODER |=  (0b01 << (6 * 2)); // Set PA5 to output mode (01)
  //   assert(GPIOC->MODER ==  0x1000);
  //   GPIOC->OTYPER &=  ~(1 << 6);
  //   GPIOC->OTYPER |=  (0 << 6);
  //   assert(GPIOC->OTYPER ==  0x0000);
  //   GPIOC->OSPEEDR &=  ~(0b11 << (6 * 2));
  //   GPIOC->OSPEEDR |=  (0b11 << (6 * 2));
  //   assert(GPIOC->OSPEEDR ==  0x3000);
  //   GPIOC->PUPDR &= ~(0b11 << (6 * 2));
  //   GPIOC->PUPDR |= (0b11 << (6 * 2));
  //   assert(GPIOC->PUPDR ==  0x3000);
  //   GPIOC->ODR &=  ~(1 << 6);
  //   GPIOC->ODR |=  (1 << 6);
  //   assert(GPIOC->ODR ==  0x0040);
   

  //   GPIOC->MODER &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
  //   GPIOC->MODER |=  (0b01 << (7 * 2)); // Set PA5 to output mode (01)
  //   assert(GPIOC->MODER ==  0x5000);
  //   GPIOC->OTYPER &=  ~(1 << 7);
  //   GPIOC->OTYPER |=  (0 << 7);
  //   assert(GPIOC->OTYPER ==  0x0000);
  //   GPIOC->OSPEEDR &=  ~(0b11 << (7 * 2));
  //   GPIOC->OSPEEDR |=  (0b11 << (7 * 2));
  //   assert(GPIOC->OSPEEDR ==  0xf000);
  //   GPIOC->PUPDR &= ~(0b11 << (7 * 2)); // Clear mode bits for PA5
  //   GPIOC->PUPDR |= (0b11 << (7 * 2)); // Clear mode bits for PA5
  //   assert(GPIOC->PUPDR ==  0xf000);
  //   GPIOC->ODR &=  ~(1 << 7);
  //   GPIOC->ODR |=  (1 << 7);
  //   assert(GPIOC->ODR ==  0x00c0);


  //   GPIOC->MODER &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
  //   GPIOC->MODER |=  (0b01 << (8 * 2)); // Set PA5 to output mode (01)
  //   assert(GPIOC->MODER ==  0x15000);
  //   GPIOC->OTYPER &=  ~(1 << 8);
  //   GPIOC->OTYPER |=  (0 << 8);
  //   assert(GPIOC->OTYPER ==  0x0000);
  //   GPIOC->OSPEEDR &=  ~(0b11 << (8 * 2));
  //   GPIOC->OSPEEDR |=  (0b11 << (8 * 2));
  //   assert(GPIOC->OSPEEDR ==  0x3f000);
  //   GPIOC->PUPDR &= ~(0b11 << (8 * 2)); // Clear mode bits for PA5
  //   GPIOC->PUPDR |= (0b11 << (8 * 2)); // Clear mode bits for PA5
  //   assert(GPIOC->PUPDR ==  0x3f000);
  //   GPIOC->ODR &=  ~(1 << 8);
  //   GPIOC->ODR |=  (1 << 8);
  //   assert(GPIOC->ODR ==  0x01c0);

  //   GPIOC->MODER &= ~(0b11 << (9* 2)); // Clear mode bits for PA5
  //   GPIOC->MODER |=  (0b01 << (9 * 2)); // Set PA5 to output mode (01)
  //   assert(GPIOC->MODER ==  0x55000);
  //   GPIOC->OTYPER &=  ~(1 << 9);
  //   GPIOC->OTYPER |=  (0 << 9);
  //   assert(GPIOC->OTYPER ==  0x0000);
  //   GPIOC->OSPEEDR &=  ~(0b11 << (9 * 2));
  //   GPIOC->OSPEEDR |=  (0b11 << (9 * 2));
  //   assert(GPIOC->OSPEEDR ==  0xff000);
  //   GPIOC->PUPDR &= ~(0b11 << (9 * 2)); // Clear mode bits for PA5
  //   GPIOC->PUPDR |= (0b11 << (9 * 2)); // Clear mode bits for PA5
  //   assert(GPIOC->PUPDR ==  0xff000);
  //   GPIOC->ODR &=  ~(1 << 9);
  //   GPIOC->ODR |=  (1 << 9);
  //   assert(GPIOC->ODR ==  0x03c0);
  //   GPIO_TypeDef *a = GPIOA;
  //   uint32_t *b = (uint32_t*) GPIOA;
  //   assert(a->MODER ==  0x28000000);
  //   GPIOA->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
  //   GPIOA->MODER |=  1; // Set PA5 to output mode (01)

  //  // assert(GPIOA->MODER & 1);
  //   GPIOA->OSPEEDR &=  ~(0b11 << 0);
  //   GPIOA->OSPEEDR |=  (0b11 << 0);
  //  // assert(GPIOA->OSPEEDR ==  0x0C000003);
  //   GPIOA->PUPDR &= ~(0b10 << 0);
  //   GPIOA->PUPDR |= (0b10 << 0);
    //assert(GPIOA->PUPDR ==  0x24000002);
    
    
    // assert(GPIOC->MODER ==  0x55000);
    // assert(GPIOC->PUPDR ==  0xff000);
    // assert(GPIOC->OSPEEDR ==  0xff000);

    // assert(GPIOC->ODR ==  0x03c0);
    // assert(GPIOC->OTYPER ==  0x3c0);