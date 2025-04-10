// #include "stm32f0xx.h"  // Include the appropriate header for your STM32 series

// void USART3_Init(void) {
//     uint32_t hclk;
//     uint32_t baud_div;

//     // 1. Enable the system clock for USART3 via the RCC peripheral.
//     // Using direct register access; alternatively, you could use the HAL macro: __HAL_RCC_USART3_CLK_ENABLE();
//     RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

//     // 2. Set the Baud rate for communication to 115200 bps.
//     // Get the system clock frequency using HAL_RCC_GetHCLKFreq() or similar function.
//     hclk = HAL_RCC_GetHCLKFreq();  // This returns the HCLK frequency in Hz.
//     // For 16x oversampling, the baud rate divider is:
//     baud_div = hclk / 115200;
//     // Set the baud rate register (BRR) to the calculated divider.
//     USART3->BRR = baud_div;

//     // 3. Enable the transmitter and receiver hardware.
//     // Set the TE (Transmitter Enable) and RE (Receiver Enable) bits in the Control Register 1.
//     USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;

//     // 4. Enable the USART peripheral.
//     // Set the UE (USART Enable) bit in the Control Register 1.
//     USART3->CR1 |= USART_CR1_UE;

//     // Optional: Wait until the USART is ready (e.g., check the TEACK and REACK flags).
//     while (!(USART3->ISR & (USART_ISR_TEACK | USART_ISR_REACK))) {
//         // Wait until both transmitter and receiver are enabled
//     }
// }


// Blocking Reception:
// #include "stm32f0xx.h"     // Device header (adjust as needed)
// #include "stm32f0xx_hal.h" // HAL header for functions like HAL_RCC_GetHCLKFreq()

// /* Function prototypes */
// void USART3_Init(void);
// void LED_Init(void);
// void Toggle_Red(void);
// void Toggle_Green(void);
// void Toggle_Blue(void);
// void USART3_Transmit(const char *str);

// int main(void)
// {
//     char rx_char;

//     /* Initialize LED GPIO pins and USART3 */
//     LED_Init();
//     USART3_Init();

//     while (1)
//     {
//         /* 1. Wait for a character to be received.
//            The RXNE flag in USART3->ISR is set when a byte is available in the Receive Data Register (RDR) */
//         while (!(USART3->ISR & USART_ISR_RXNE))
//         {
//             // Wait until data is received
//         }

//         /* 2. Read the received data.
//            (Note: reading the RDR clears the RXNE flag) */
//         rx_char = (char)(USART3->RDR & 0xFF);

//         /* 3. Test the received data and toggle the appropriate LED.
//            If the received character is 'r'/'R', 'g'/'G', or 'b'/'B' toggle the corresponding LED.
//            Otherwise, print an error message via USART3. */
//         switch(rx_char)
//         {
//             case 'r':
//             case 'R':
//                 Toggle_Red();
//                 break;
//             case 'g':
//             case 'G':
//                 Toggle_Green();
//                 break;
//             case 'b':
//             case 'B':
//                 Toggle_Blue();
//                 break;
//             default:
//                 USART3_Transmit("Error: Unrecognized command.\r\n");
//                 break;
//         }
//     }
// }

// /* USART3 initialization:
//    1. Enable the system clock to USART3.
//    2. Set the baud rate to 115200 using the system clock frequency from HAL_RCC_GetHCLKFreq().
//    3. Enable transmitter and receiver hardware.
//    4. Enable the USART peripheral.
// */
// void USART3_Init(void)
// {
//     uint32_t hclk, baud_div;

//     /* 1. Enable clock for USART3. */
//     RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

//     /* 2. Set the baud rate to 115200 bps.
//        Assume 16x oversampling. Get the system clock frequency (HCLK) via HAL_RCC_GetHCLKFreq(). */
//     hclk = HAL_RCC_GetHCLKFreq();
//     baud_div = hclk / 115200;
//     USART3->BRR = baud_div;

//     /* 3. Enable transmitter (TE) and receiver (RE) in Control Register 1. */
//     USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;

//     /* 4. Enable the USART peripheral by setting UE bit.
//        Once enabled, many configuration bits become read-only. */
//     USART3->CR1 |= USART_CR1_UE;

//     /* Optional: wait until both transmitter and receiver are acknowledged as enabled. */
//     while (!(USART3->ISR & (USART_ISR_TEACK | USART_ISR_REACK)))
//     {
//         // Wait for TEACK and REACK flags.
//     }
// }

// /* Initialize LED GPIO pins.
//    This example assumes three LEDs are connected to GPIOA pins 5 (Red), 6 (Green), and 7 (Blue). */
// void LED_Init(void)
// {
//     /* Enable clock for GPIOA */
//     RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

//     /* Configure PA5, PA6, and PA7 as general purpose output.
//        Clear the mode bits for each pin then set them as output (01). */
//     GPIOA->MODER &= ~((3 << (5*2)) | (3 << (6*2)) | (3 << (7*2)));
//     GPIOA->MODER |= ((1 << (5*2)) | (1 << (6*2)) | (1 << (7*2)));
// }

// /* Toggle functions for each LED */
// void Toggle_Red(void)
// {
//     GPIOA->ODR ^= (1 << 5);  // Toggle PA5 (Red LED)
// }

// void Toggle_Green(void)
// {
//     GPIOA->ODR ^= (1 << 6);  // Toggle PA6 (Green LED)
// }

// void Toggle_Blue(void)
// {
//     GPIOA->ODR ^= (1 << 7);  // Toggle PA7 (Blue LED)
// }

// /* Simple blocking transmit function for USART3.
//    It sends a null-terminated string over USART using the TXE flag to indicate readiness. */
// void USART3_Transmit(const char *str)
// {
//     while (*str)
//     {
//         /* Wait until Transmit Data Register is empty (TXE flag is set) */
//         while (!(USART3->ISR & USART_ISR_TXE))
//         {
//             // Wait for TXE
//         }
//         USART3->TDR = *str++;  // Send the character and move to the next
//     }
// }

// 4.3 Using Interrupts:
// #include "stm32f0xx.h"      // Adjust header file as needed for your STM32 series
// #include "stm32f0xx_hal.h"  // For HAL_RCC_GetHCLKFreq(), etc.

// // Global variables for USART reception via interrupt
// volatile uint8_t rx_data = 0;
// volatile uint8_t rx_flag = 0;

// // Function prototypes
// void USART3_Init(void);
// void LED_Init(void);
// void LED_Red_On(void);
// void LED_Red_Off(void);
// void LED_Green_On(void);
// void LED_Green_Off(void);
// void LED_Blue_On(void);
// void LED_Blue_Off(void);
// void Toggle_Red(void);
// void Toggle_Green(void);
// void Toggle_Blue(void);
// void USART3_Transmit(const char *str);

// // Interrupt handler for USART3 reception
// void USART3_IRQHandler(void)
// {
//     if (USART3->ISR & USART_ISR_RXNE)
//     {
//         rx_data = (uint8_t)(USART3->RDR & 0xFF); // Read the received byte (clears RXNE)
//         rx_flag = 1;
//     }
// }

// int main(void)
// {
//     uint8_t command[2];
//     uint8_t command_index = 0;

//     // Initialize peripherals
//     LED_Init();
//     USART3_Init();

//     // Enable USART3 interrupt in NVIC
//     NVIC_EnableIRQ(USART3_IRQn);

//     // Main loop: process two-character commands
//     while (1)
//     {
//         // If waiting for a new command, prompt the user
//         if (command_index == 0)
//         {
//             USART3_Transmit("CMD? ");
//         }
        
//         // Check if new data was received by the interrupt handler
//         if (rx_flag)
//         {
//             rx_flag = 0; // Clear flag
//             command[command_index] = rx_data;
//             command_index++;

//             // Once two characters have been received, process the command
//             if (command_index == 2)
//             {
//                 char color = command[0];
//                 char op    = command[1];

//                 // Validate command: first char must be one of r, g, b (case-insensitive)
//                 // and second char must be between '0' and '2'
//                 if ((color == 'r' || color == 'R' || color == 'g' || color == 'G' || color == 'b' || color == 'B') &&
//                     (op >= '0' && op <= '2'))
//                 {
//                     // Process command based on LED color and operation
//                     if (op == '0')
//                     {
//                         // Turn LED off
//                         if (color == 'r' || color == 'R') { LED_Red_Off(); }
//                         else if (color == 'g' || color == 'G') { LED_Green_Off(); }
//                         else if (color == 'b' || color == 'B') { LED_Blue_Off(); }
//                         USART3_Transmit("\r\nCommand recognized: Turn Off LED\r\n");
//                     }
//                     else if (op == '1')
//                     {
//                         // Turn LED on
//                         if (color == 'r' || color == 'R') { LED_Red_On(); }
//                         else if (color == 'g' || color == 'G') { LED_Green_On(); }
//                         else if (color == 'b' || color == 'B') { LED_Blue_On(); }
//                         USART3_Transmit("\r\nCommand recognized: Turn On LED\r\n");
//                     }
//                     else if (op == '2')
//                     {
//                         // Toggle LED
//                         if (color == 'r' || color == 'R') { Toggle_Red(); }
//                         else if (color == 'g' || color == 'G') { Toggle_Green(); }
//                         else if (color == 'b' || color == 'B') { Toggle_Blue(); }
//                         USART3_Transmit("\r\nCommand recognized: Toggle LED\r\n");
//                     }
//                 }
//                 else
//                 {
//                     // Unknown command: print error and restart command processing
//                     USART3_Transmit("\r\nError: Invalid Command\r\n");
//                 }
//                 command_index = 0; // Reset command index for the next command
//             }
//         }
//     }
// }

// /* USART3 Initialization:
//    1. Enable the system clock to USART3.
//    2. Set the baud rate to 115200 bps (assuming 16x oversampling).
//    3. Enable transmitter and receiver hardware.
//    4. Enable the USART peripheral.
// */
// void USART3_Init(void)
// {
//     uint32_t hclk, baud_div;

//     // 1. Enable clock for USART3 (adjust if using a different USART)
//     RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

//     // 2. Configure baud rate using HCLK (assume 16x oversampling)
//     hclk = HAL_RCC_GetHCLKFreq(); // Get system clock frequency in Hz
//     baud_div = hclk / 115200;
//     USART3->BRR = baud_div;

//     // 3. Enable transmitter and receiver (set TE and RE bits)
//     USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;

//     // 4. Enable USART peripheral (set UE bit)
//     USART3->CR1 |= USART_CR1_UE;

//     // Optional: wait until the hardware acknowledges transmitter and receiver are enabled
//     while (!(USART3->ISR & (USART_ISR_TEACK | USART_ISR_REACK)))
//     {
//         // Wait for acknowledgment
//     }
// }

// /* LED Initialization:
//    Assume three LEDs are connected to GPIOA pins 5 (Red), 6 (Green), and 7 (Blue).
// */
// void LED_Init(void)
// {
//     // Enable clock for GPIOA
//     RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

//     // Configure PA5, PA6, and PA7 as general-purpose output (set MODER bits to '01')
//     GPIOA->MODER &= ~((3 << (5 * 2)) | (3 << (6 * 2)) | (3 << (7 * 2)));
//     GPIOA->MODER |= ((1 << (5 * 2)) | (1 << (6 * 2)) | (1 << (7 * 2)));
// }

// /* LED control functions for Red LED on PA5 */
// void LED_Red_On(void)
// {
//     GPIOA->ODR |= (1 << 5);
// }

// void LED_Red_Off(void)
// {
//     GPIOA->ODR &= ~(1 << 5);
// }

// void Toggle_Red(void)
// {
//     GPIOA->ODR ^= (1 << 5);
// }

// /* LED control functions for Green LED on PA6 */
// void LED_Green_On(void)
// {
//     GPIOA->ODR |= (1 << 6);
// }

// void LED_Green_Off(void)
// {
//     GPIOA->ODR &= ~(1 << 6);
// }

// void Toggle_Green(void)
// {
//     GPIOA->ODR ^= (1 << 6);
// }

// /* LED control functions for Blue LED on PA7 */
// void LED_Blue_On(void)
// {
//     GPIOA->ODR |= (1 << 7);
// }

// void LED_Blue_Off(void)
// {
//     GPIOA->ODR &= ~(1 << 7);
// }

// void Toggle_Blue(void)
// {
//     GPIOA->ODR ^= (1 << 7);
// }

// /* Blocking transmit function for USART3.
//    This function sends a null-terminated string over USART.
// */
// void USART3_Transmit(const char *str)
// {
//     while (*str)
//     {
//         // Wait until the Transmit Data Register is empty
//         while (!(USART3->ISR & USART_ISR_TXE))
//         {
//             // Waiting for TXE flag
//         }
//         USART3->TDR = *str++;  // Transmit next character
//     }
// }