#include "stm32f0xx.h"
#include "hal_gpio.h"   // Assume this header provides your custom HAL functions
#include "stm32f0xx_hal.h"  // If using HAL_Delay and HAL_Init
#include <assert.h>
#include <main.h>
// Forward declaration of system clock configuration function
void SystemClock_Config(void);

// Custom function to initialize LED pins (assume LEDs on PC6, PC7, PC8, PC9)
void LED_Init(void) {
    // Enable clock for GPIOC.
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // Configure PC6, PC7, PC8, and PC9 as general-purpose outputs.
    GPIOC->MODER &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));
    GPIOC->MODER |=  ((1UL << (6 * 2)) | (1UL << (7 * 2)) | (1UL << (8 * 2)) | (1UL << (9 * 2)));
    
    // Configure output type as push-pull (default) and low speed, no pull-up/pull-down.
    GPIOC->OTYPER &= ~((1UL << 6) | (1UL << 7) | (1UL << 8) | (1UL << 9));
    GPIOC->OSPEEDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));
    GPIOC->PUPDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)) | (3UL << (8 * 2)) | (3UL << (9 * 2)));
     GPIOA->MODER &= ~(3UL << (0 * 2));
    
    // Set low speed for PA0.
    GPIOA->OSPEEDR &= ~(3UL << (0 * 2));
    
    // Enable internal pull-down resistor on PA0 (set PUPDR bits to '10').
    GPIOA->PUPDR &= ~(3UL << (0 * 2));
    GPIOA->PUPDR |=  (2UL << (0 * 2));
}


//TEST Code

void Configure_SYSCFG_EXTI0(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->EXTICR[0] &= ~(0xF << 0); // Map EXTI0 to PA0 (0000)
}

void EXTI0_1_IRQHandler(void)
{
    
    // Toggle green (PC9) and orange (PC8) LEDs.
    GPIOC->ODR ^= ((1 << 9) | (1 << 8));
    HAL_Delay(2000); // Delay for 2 seconds
    GPIOC->ODR ^= ((1 << 9) | (1 << 8));

    // Clear the pending flag for EXTI line 0.
    EXTI->PR |= (1 << 0);
}

int lab2_main(void)
{   HAL_Init(); // Reset of all peripherals, init the Flash and Systick

    // NVIC_SetPriority(EXTI0_1_IRQn, 1);
    //  NVIC_EnableIRQ(EXTI0_1_IRQn); // Enable EXTI0 interrupt in NVIC.
    LED_Init();
    //EXTI0_1_IRQHandler();
    Configure_SYSCFG_EXTI0(); // Configure SYSCFG for EXTI line 0
    // Assume Button_Init() and EXTI0_Init() from Section 2.4 are called here.
    configure_SYSCFG_EXTI_PA0(); // Configure SYSCFG for EXTI line 0
    //NVIC_EnableIRQ(EXTI0_1_IRQn);  // Enable EXTI0 interrupt in NVIC.
    NVIC_SetPriority(SysTick_IRQn, 2);
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
        HAL_Delay(500); // Delay of ~500 ms (between 400 and 600ms)
        // __WFI();
    }
}

//TEST Code


//2.7
// int lab2_main(void)
// {
//     LED_Init();
//     // Assume Button_Init() and EXTI0_Init() are called to configure PA0 and EXTI0.
    
//     // Set SysTick priority to 2 (medium priority).
//     NVIC_SetPriority(SysTick_IRQn, 2);
//     // Initially, set EXTI0 interrupt priority to 1 (high priority) so it preempts SysTick.
//     NVIC_SetPriority(EXTI0_1_IRQn, 1);
//     NVIC_EnableIRQ(EXTI0_1_IRQn);
    
//     // For demonstration, wait a short period then change EXTI0 priority to 3 (low priority).
//     for(volatile uint32_t i = 0; i < 1000000; i++);
//     Hal_Delay(2000); // Wait for 2 seconds
//     NVIC_SetPriority(EXTI0_1_IRQn, 3);
    
//     while(1)
//     {
//          __WFI();
//     }
// }
//Section 2.1
// int lab2_main(void)
// {
//     // Standard HAL initialization (if using the HAL library)
//     HAL_Init();
//     SystemClock_Config(); // Configure system clock as needed
    
//     // 1. Initialize all LED pins (PC6: Red, PC7: Blue, PC8: Orange, PC9: Green)
//     LED_Init();
    
//     // 2. Set the green LED (PC9) high.
//     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    
//     // Main application loop:
//     // 3. Toggle the red LED (PC6) with a delay of about 500ms to indicate main loop execution.
//     while (1)
//     {
//         HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
//         HAL_Delay(500); // Delay of ~500 ms (between 400 and 600ms)
//     }
// }

//section2.2
// int lab2_main(void)
// {
//     // 1. Enable clock for GPIOA to configure PA0.
//     RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    
//     // Configure PA0 to input mode:
//     // Clear MODER bits for PA0 to set it as input (00).
//     GPIOA->MODER &= ~(3UL << (0 * 2));
    
//     // Set low speed for PA0.
//     GPIOA->OSPEEDR &= ~(3UL << (0 * 2));
    
//     // Enable internal pull-down resistor on PA0 (set PUPDR bits to '10').
//     GPIOA->PUPDR &= ~(3UL << (0 * 2));
//     GPIOA->PUPDR |=  (2UL << (0 * 2));
    
//     // 4. Add assertions before calling our EXTI configuration function.
//     // Typically, if nothing has been configured yet, EXTI_IMR and EXTI_RTSR for line 0 should be 0.
//     assert((EXTI->IMR & (1 << 0)) == 0);
//     assert((EXTI->RTSR & (1 << 0)) == 0);
    
//     // 6 & 7. Call the function to unmask EXTI line 0 and set rising-edge trigger.
//     configure_EXTI0_for_PA0();
    
//     // 5. Add assertions after the function call to verify that the bits are set.
//     assert((EXTI->IMR & (1 << 0)) != 0);
//     assert((EXTI->RTSR & (1 << 0)) != 0);
    
//     // Main infinite loop.
//     while(1)
//     {
//         // Optionally, put the MCU in low-power mode while waiting for interrupts.
//         __WFI();
//     }
    
//     // This return statement will never be reached.
//     return 0;
// }
















// void EXTI0_1_IRQHandler(void)
//     {
//     // Toggle green LED on PC9 and orange LED on PC8.
//     GPIOC->ODR ^= ((1 << 9) | (1 << 8));
    
//     // Clear the pending flag for EXTI line 0.
//     // Writing a '1' to the corresponding bit clears the flag.
//     EXTI->PR |= (1 << 0);
//     }

    //section2.6
//     void EXTI0_1_IRQHandler(void)
// {
//     // Toggle LEDs before delay.
//     GPIOC->ODR ^= ((1 << 9) | (1 << 8));
    
//     // Long delay loop (~1.5 million iterations).
//     for (volatile uint32_t i = 0; i < 1500000; i++);
    
//     // Toggle LEDs after delay.
//     GPIOC->ODR ^= ((1 << 9) | (1 << 8));
    
//     // Clear the pending flag.
//     EXTI->PR |= (1 << 0);
// }
// void Configure_EXTI_PA0(void)
// {
//     __HAL_RCC_GPIOA_CLK_ENABLE();

//     GPIO_InitTypeDef gpio = {0};
//     gpio.Pin = GPIO_PIN_0;
//     gpio.Mode = GPIO_MODE_IT_RISING;
//     gpio.Pull = GPIO_PULLDOWN;
//     HAL_GPIO_Init(GPIOA, &gpio);

//     // Enable EXTI line 0 interrupt
//     HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
//     HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
// }


// //section 2.7
// void EXTI0_1_IRQHandler(void)
// {
//     // Toggle orange LED on PC8.
//     GPIOC->ODR ^= (1 << 8);
    
//     // Simulate a long-running process (delay loop).
//     for (volatile uint32_t i = 0; i < 1000000; i++);
    
//     // Toggle orange LED again (for visual indication).
//     GPIOC->ODR ^= (1 << 8);
    
//     // Clear the pending flag for EXTI line 0.
//     EXTI->PR |= (1 << 0);
// }


// int lab2_main(void)
// {
//         // Standard HAL initialization (if using the HAL library)
//     HAL_Init();
//     SystemClock_Config(); // Configure system clock as needed
    
//     // 1. Initialize all LED pins (PC6: Red, PC7: Blue, PC8: Orange, PC9: Green)
//     LED_Init();
    
//     // 2. Set the green LED (PC9) high.
//     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    
//     // Main application loop:
//     // 3. Toggle the red LED (PC6) with a delay of about 500ms to indicate main loop execution.
  
       
    
//     // (Assume that GPIO initialization for the button and other peripherals has already been done.)
    
//     // 4. Assertion before configuring: Check that the bits for EXTI line 0 are not already set
//     // to route PA0. For this example, the expected value is 0 (for Port A).
//    // uint32_t exti_config_before = SYSCFG->EXTICR[0] & 0xF;
//     // For demonstration, if exti_config_before is not 0, we know a change will occur.
//     // (If the default is already 0, this assertion will pass anyway.)
//     //assert(exti_config_before != 0); // Optional: if you expect a nonzero default.
    
//     // Call our helper function to set the multiplexer.
//     Configure_EXTI_PA0();
    
//     // 5. Assertion after configuring: Verify that the lower 4 bits of EXTICR[0] are now 0 (i.e., PA0 is selected)
//     uint32_t exti_config_after = SYSCFG->EXTICR[0] & 0xF;
//     assert(exti_config_after == 0);

//     HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
//     HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

//     //section 2.7
//     // Change SysTick priority to 2 (medium priority).
//      NVIC_SetPriority(SysTick_IRQn, 2);
//      // Ensure EXTI (EXTI0_1_IRQn) is initially set to priority 1 (high priority) to simulate starvation.
//      NVIC_SetPriority(EXTI0_1_IRQn, 1);
//     NVIC_EnableIRQ(EXTI0_1_IRQn);
//     uint32_t elapsed = 0;
//     while (1)
//     {
//         // Toggle red LED to indicate main loop execution.
//         GPIOC->ODR ^= (1 << 6);
//         HAL_Delay(500);
//         elapsed += 500;
        
//         // After 10 seconds, adjust EXTI priority to 3 (lowest) to allow SysTick (priority 2) to run.
//         if (elapsed >= 10000) {
//             NVIC_SetPriority(EXTI0_1_IRQn, 3);
//         }
//     }
    
//     return 0;
// }
    
// //     while (1)
// //     {
// //         HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
// //         HAL_Delay(500); // Delay of ~500 ms (between 400 and 600ms)
// //        // __WFI(); // Wait for interrupt
// //     }
    
// //     return 0;
// // }
