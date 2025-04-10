// #include "stm32f0xx.h"  // Device header – adjust as needed

// /* 
//    Function prototypes
// */
// void TIM3_PWM_Init(void);
// void TIM3_GPIO_Init(void);

// int main(void)
// {
//     /* Initialize GPIO pins for TIM3 PWM outputs (PC6 and PC7) */
//     TIM3_GPIO_Init();

//     /* Initialize TIM3 for PWM operation */
//     TIM3_PWM_Init();

//     /* Start TIM3 counter */
//     TIM3->CR1 |= TIM_CR1_CEN;
    
//     /* Main loop – PWM is generated in hardware */
//     while (1)
//     {
//         // The PWM outputs on PC6 and PC7 will run continuously.
//         // You could update TIM3->CCR1 and TIM3->CCR2 here to adjust brightness.
//     }
// }

// /* 
//    TIM3_GPIO_Init
//    Configures PC6 and PC7 as alternate function pins for TIM3_CH1 and TIM3_CH2.
//    According to the STM32F072 datasheet, PC6 and PC7 can be configured with AF1 for TIM3.
// */
// void TIM3_GPIO_Init(void)
// {
//     /* 1. Enable clock for GPIOC */
//     RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
//     /* 2. Set PC6 and PC7 to Alternate Function mode.
//           Each pin uses two bits in the MODER register.
//           For alternate function mode, set MODER bits = 10.
//     */
//     GPIOC->MODER &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)));  
//     GPIOC->MODER |=  ((2UL << (6 * 2)) | (2UL << (7 * 2)));
    
//     /* 3. Configure output type as push-pull and set high speed if desired */
//     GPIOC->OTYPER &= ~((1UL << 6) | (1UL << 7));
//     GPIOC->OSPEEDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)));
//     GPIOC->OSPEEDR |=  ((3UL << (6 * 2)) | (3UL << (7 * 2)));
    
//     /* 4. No pull-up, no pull-down (modify if needed) */
//     GPIOC->PUPDR &= ~((3UL << (6 * 2)) | (3UL << (7 * 2)));
    
//     /* 5. Set the alternate function for PC6 and PC7 to AF1.
//           For pins 0–7, alternate functions are configured in AFRL.
//           Each pin gets 4 bits: 
//           PC6 uses bits [6*4+3:6*4] and PC7 uses bits [7*4+3:7*4].
//     */
//     GPIOC->AFR[0] &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
//     GPIOC->AFR[0] |=  ((1 << (6 * 4)) | (1 << (7 * 4))); // AF1 for TIM3
// }

// /*
//    TIM3_PWM_Init
//    Configures TIM3 to generate PWM outputs on channels 1 (PC6) and 2 (PC7) with a frequency of 800 Hz.
//    - Use a prescaler of 7 to have a timer tick frequency of 1 MHz (8MHz/8).
//    - Set the auto-reload register (ARR) to 1249 so that the period is 1250 ticks (1.25 ms).
//    - Configure Channel 1 in PWM Mode 2 and Channel 2 in PWM Mode 1.
//    - Preload the CCR registers with a value corresponding to 20% duty cycle (approximately 250).
// */
// void TIM3_PWM_Init(void)
// {
//     /* 1. Enable TIM3 clock in RCC (TIM3 is on APB1) */
//     RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    
//     /* 2. Configure prescaler and auto-reload for 800 Hz PWM.
//          Timer tick frequency = 8MHz / (PSC+1). To get 1MHz, set PSC = 7.
//          Then, period (in ticks) = 1MHz / 800 Hz = 1250 ticks.
//          Since ARR is zero-indexed, set ARR = 1249.
//     */
//     TIM3->PSC = 7;          // Prescaler = 7, so tick frequency = 8MHz/8 = 1MHz
//     TIM3->ARR = 1249;       // Auto-reload value for a period of 1250 ticks (1.25 ms)
    
//     /* 3. Set up capture/compare registers for a 20% duty cycle.
//          20% of 1250 = 250 (approximately)
//     */
//     TIM3->CCR1 = 250;       // Channel 1 compare value
//     TIM3->CCR2 = 250;       // Channel 2 compare value
    
//     /* 4. Configure PWM mode in the Capture/Compare Mode Register 1 (CCMR1) for channels 1 and 2.
//          For channel 1:
//            - Clear CC1S bits (set as output).
//            - Set OC1M bits to PWM mode 2 (110) and enable preload (OC1PE).
//          For channel 2:
//            - Clear CC2S bits.
//            - Set OC2M bits to PWM mode 1 (111) and enable preload (OC2PE).
//          Note: The bit positions for channel 1 are bits 4–6 for OC1M and bit 3 for OC1PE.
//                For channel 2, OC2M are bits 12–14 and OC2PE is bit 11.
//     */
//     // Clear and configure channel 1
//     TIM3->CCMR1 &= ~((0x7 << 4) | (0x3 << 0));  // Clear OC1M and CC1S
//     TIM3->CCMR1 |= ( (6 << 4) | (1 << 3) );       // Set OC1M = 110 (PWM mode 2) and enable preload
    
//     // Clear and configure channel 2   TIM3->CCMR1 &= ~(0x7 << 12);                  // Clear OC2M bits
//     TIM3->CCMR1 |= ( (7 << 12) | (1 << 11) );      // Set OC2M = 111 (PWM mode 1) and enable preload
    
//     /* 5. Enable the output channels in the Capture/Compare Enable Register (CCER).
//          For channel 1, enable the output (bit 0).
//          For channel 2, enable the output (bit 4).
//     */
//     TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
    
//     /* 6. Force an update to load the registers (optional) */
//     TIM3->EGR = TIM_EGR_UG;
    
//     /* Note: Do not enable update interrupts for this part.
//        The timer will start when CR1's CEN bit is set (done in main).
//     */
// }
