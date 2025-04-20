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
    // __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
    // __HAL_RCC_GPIOA_CLK_ENABLE();
    // Set up a configuration struct to pass to the initialization function
//     GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7,
//                                 GPIO_MODE_OUTPUT_PP,
//                                 GPIO_SPEED_FREQ_LOW,
//                                 GPIO_NOPULL};
     //HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
My_HAL_GPIO_Init(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Initialize pins PC8 & PC9
//     // My_HAL_GPIO_Init(GPIOA, GPIO_PIN_0);
//    //HAL_GPIO_WritePin(GPIOC, 1 << 8 ); // Start PC8 high
 My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PC8 high
 My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // Start PC8 high

//    // My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_7);
   My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
//     HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
   uint32_t history=0;
//     // 
    while (1) {

        HAL_Delay(1); // Delay 200ms
        //read pin with buton
        int32_t ButtonPress= My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        //int32_t ButtonPress= 1;

        //add state to history
        //move history left 

        history = history << 1;
        //add button press to history

        if (ButtonPress) {
           history = history | 0x01;
        }
        //check steady state
        if (history == 0xFFFFFFFF) {
            // This code triggers repeatedly when button is steady high!
        }
        // check srteady state low
        else if (history == 0x00000000) {
            // This code triggers repeatedly when button is steady low!
        }
        //check transition state
        //if so, toggle leds 
        else if (history == 0x7FFFFFFF) {
            // This code triggers only once when transitioning to steady high!
            
            //My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Toggle LEDs
            My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
          }
        //noise state

        else  {
            //do nothing
        }
    }

    return -1;
}


// GPIO_InitTypeDef GPIO_InitStruct = {1};
//     GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//     // Configure GPIOA PIN 0 as Input with Pull-Down
//     GPIO_InitStruct.Pin = GPIO_PIN_0;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     while (1) {
//         // Check if button at PA0 is pressed
//         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
//             HAL_Delay(1); // Debounce delay
//             if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) { // Check again
//                 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Toggle PC6 & PC7

//                 while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET); // Wait for release
//             }
//         }
//     }
// }

// while (1) {
//   // Check if button at PA0 is pressed
//   if (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
//       HAL_Delay(1); // Debounce delay
//       if (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) { // Check again
//           My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Toggle PC6 & PC7

//           while (My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET); // Wait for release
//       }
//   }
// }
// }












    //     uint32_t debouncer = 0;
    //     debouncer = (debouncer << 1); // Always shift every loop iteration
    //     if(GPIOA->IDR & 0x1) { { // If input signal is set/high
    //     debouncer |= 0x01; // Set lowest bit of bit-vector
    //     }
    //     if (debouncer == 0xFFFFFFFF) {
    //     // This code triggers repeatedly when button is steady high!
    //     }
    //     if (debouncer == 0x00000000) {
    //     // This code triggers repeatedly when button is steady low!
    //     }
    //     if (debouncer == 0x7FFFFFFF) {
    //     // This code triggers only once when transitioning to steady high!
        
    //     My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Toggle PC6 & PC7
    //     }
    //     } // Triggers if bit 6 is set
    // return -1;
    //     // Toggle the output state of both PC8 and PC9
    //     //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    //     My_HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    //       }
    //     }
       // uint32_t debouncer = 0;
// while(1) {

// // When button is bouncing the bit-vector value is random since bits are set when
// //the button is high and not when it bounces low.
// }


//while (1) {
  //   // Check if button at PA0 is pressed
  //   if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
  //       HAL_Delay(50); // Debounce delay
  //       if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) { // Check again
  //           My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7); // Toggle PC6 & PC7

  //           while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET); // Wait for release
  //       }
  //   }
  //           } 
  //         }

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