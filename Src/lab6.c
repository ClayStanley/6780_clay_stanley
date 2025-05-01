#include "stm32f0xx.h"
#include "hal_gpio.h"   // Assume this header provides your custom HAL functions
#include "stm32f0xx_hal.h"  // If using HAL_Delay and HAL_Init
#include <assert.h>
#include <main.h>
// LED pins on Discovery: PE8 (LED1), PE9 (LED2), PE10 (LED3), PE11 (LED4)
#define LED_GREEN_PIN   6  // PC6
#define LED_ORANGE_PIN  7  // PC7
#define LED_RED_PIN     8  // PC8
#define LED_BLUE_PIN    9  // PC9


// // ADC channel for PC0
// #define ADC_CHANNEL       10U

// // Thresholds for 8-bit ADC (0–255)
// #define THRESH1  64U
// #define THRESH2  128U
// #define THRESH3  192U
// #define THRESH4  240U

// void delay_ms(uint32_t ms) {
//     // crude busy-wait @≈48 MHz
//     for(uint32_t i=0; i<ms*8000; i++) __NOP();
// }
// void GPIO_Init6(void) {
// RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN; 

// uint32_t mask = (3U << (LED_GREEN_PIN*2))
// | (3U << (LED_ORANGE_PIN*2))
// | (3U << (LED_RED_PIN*2))
// | (3U << (LED_BLUE_PIN*2));
// GPIOC->MODER &= ~mask;
// GPIOC->MODER |= ((1U << (LED_GREEN_PIN*2))
//  | (1U << (LED_ORANGE_PIN*2))
//  | (1U << (LED_RED_PIN*2))
//  | (1U << (LED_BLUE_PIN*2)));
// // 3) Configure PC0 as analog mode (11 at bits [1:0])
// GPIOC->MODER |= (3U<<0);
// //GPIOC->ODR |= (1<<LED_GREEN_PIN); // turn off all LEDs
// }

// //part 1
// int lab6_main(void) {
//     // 1) Enable GPIOC (LEDs) and GPIOC (PC0) clocks
//     GPIO_Init6();
//     // 2) Configure PE8–PE11 as general-purpose outputs (00=IN, 01=OUT, 10=AF, 11=AN)
//     GPIOC->MODER &= ~((3U<<6)|(3U<<7)|(3U<<8)|(3U<<9));
//     GPIOC->MODER |=  ((1U<<6)|(1U<<7)|(1U<<8)|(1U<<9));
   
//     // 4) Enable ADC1 clock
//     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 
//     GPIOC->ODR |= (1<<LED_GREEN_PIN);
//     // 5) ADC1 configuration: 8-bit resolution, continuous mode, software trigger
//     ADC1->CFGR1 = (1<<4)    // 8-bit
//                   | ADC_CFGR1_CONT;     // continuous conversions 
//                   ADC1->CFGR1 &= ~(3UL<<10); //clearing 11 and 10 bit to disable hardware trigger detections
//                   //ADC1->CHSELR = ADC_CHANNEL_10;
//                   //ADC1->CHSELR = ADC_IN10;
//     // 6) Select channel 10 (PC0)
//     ADC1->CHSELR |= (1U << ADC_CHANNEL);

//     // // 7) Calibrate ADC
//     // ADC1->CR |= ADC_CR_ADCAL;                // start calibration
//     // while(ADC1->CR & ADC_CR_ADCAL) { }       // wait until cleared

//     // // 8) Enable ADC
//     // ADC1->CR |= ADC_CR_ADEN;                 // enable
//     // while(!(ADC1->ISR & ADC_ISR_ADRDY)) { }  // wait for ready :contentReference[oaicite:6]{index=6}&#8203;:contentReference[oaicite:7]{index=7}

//     // // 9) Start regular conversion
//     // ADC1->CR |= ADC_CR_ADSTART;              // software trigger
//     adc_self_calibrate_enable_and_start(); // call the function to calibrate and start ADC
//     // 10) Main loop: read ADC, update LEDs
//     GPIOC->ODR &= ~ ((1<<LED_ORANGE_PIN)| (1<<LED_RED_PIN)|(1<<LED_BLUE_PIN)|(1<<LED_GREEN_PIN)); // turn off all LEDs
//     while(1) {
//         // wait for end-of-conversion
//         while(!(ADC1->ISR & ADC_ISR_EOC)) { }
//         uint8_t val = (uint8_t)(ADC1->DR & 0xFF);
//         // clear EOC flag
//         ADC1->ISR |= ADC_ISR_EOC;

//         // // turn LEDs off
//         // GPIOC->ODR &= ~(LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN);
//         // GPIOC->ODR |= (LED_GREEN_PIN|LED_ORANGE_PIN|LED_RED_PIN|LED_BLUE_PIN);

//         // light up LEDs in sequence
//         if (val < THRESH1) {
//             GPIOC->ODR &= ~ ((1<<LED_ORANGE_PIN)| (1<<LED_RED_PIN)|(1<<LED_BLUE_PIN)|(1<<LED_GREEN_PIN)); // turn off all LEDs
//         }
//         if (val > THRESH1){
//             GPIOC->ODR |= (1<<LED_GREEN_PIN);
//             GPIOC->ODR &= ~ ((1<<LED_ORANGE_PIN)| (1<<LED_RED_PIN)|(1<<LED_BLUE_PIN)); 
//         }
//         if (val > THRESH2){
//             GPIOC->ODR |= ((1<<LED_BLUE_PIN)|(1<<LED_GREEN_PIN));
//             GPIOC->ODR &= ~ ((1<<LED_ORANGE_PIN)| (1<<LED_RED_PIN)); 
//         }
//         if (val > THRESH3){
//             GPIOC->ODR |= ((1<<LED_BLUE_PIN)|(1<<LED_GREEN_PIN)|(1<<LED_RED_PIN));
//             GPIOC->ODR &= ~ ((1<<LED_ORANGE_PIN)); 
//         }

//         if (val > THRESH4) GPIOC->ODR |= ((1<<LED_ORANGE_PIN)|(1<<LED_BLUE_PIN)|(1<<LED_GREEN_PIN)|(1<<LED_RED_PIN)); // all LEDs on

//         delay_ms(50); // debounce noise
//     }
// }


//part 2

#include "stm32f0xx.h"

static const uint8_t sine32[32] = {127,151,175,197,216,232,244,251,254,251,244,
    232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};
//3//2‐point 8‐bit sine wave LUT
// static const uint8_t sine32[32] = {
//     128, 152, 176, 198, 218, 234, 246, 253,
//     255, 253, 246, 234, 218, 198, 176, 152,
//     128, 104,  80,  58,  38,  22,  10,   3,
//     1,   3,  10,  22,  38,  58,  80, 104
// };

void delay_ms6(uint32_t ms) {
    // rough busy‐wait @48MHz
    for(uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

int lab6_main(void) {
    // 1) Enable GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;              
    // 2) Set PA4 to analog mode (MODER4 = 11), no pull‐up/down
    GPIOA->MODER   |=  (3U << (4 * 2));
    //GPIOA->PUPDR   &= ~(3U << (4 * 2));

    // 3) Enable DAC1 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;            

    // 4) Configure DAC channel1:
    //    – Enable channel1 (EN1 bit0)
    //    – Enable trigger (TEN1 bit2)
    //    – Leave TSEL1=111 for software trigger
    DAC1->CR |= (0x7<<3);
    DAC1->CR |= DAC_CR_EN1;  // bit 0: enable output on DAC_OUT1 (PA4)
            // | DAC_CR_TEN1; // bit 2: enable trigger
    DAC1->SWTRIGR = DAC_SWTRIGR_SWTRIG1;
    // 5) Main loop: output waveform
    int idx = 0;
    while (1) {
        // Write next 8‐bit value, right-aligned, to DHR8R1 (offset 0x08)
        DAC1->DHR8R1 = sine32[idx];


        if (idx == 31){
            idx = 0;
        }
        else{
            idx++;
        }

        // 1 ms delay → ~31 Hz output over 32 samples
        delay_ms6(1);
    }
}


// static const uint8_t sine32[32] = {
//     128, 152, 176, 198, 218, 234, 246, 253,
//     255, 253, 246, 234, 218, 198, 176, 152,
//     128, 104,  80,  58,  38,  22,  10,   3,
//       1,   3,  10,  22,  38,  58,  80, 104
// };

// DAC_HandleTypeDef hdac;

// void SystemClock_Config(void);
// void MX_GPIO_Init(void);
// void MX_DAC_Init(void);

// int lab6_main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_GPIO_Init();
//     MX_DAC_Init();

//     // Start DAC Channel 1
//     HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

//     uint32_t idx = 0;
//     while (1)
//     {
//         // Write next point (8-bit right aligned) and trigger
//         HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, sine32[idx]);
//         HAL_DAC_SoftwareTrigger(&hdac, DAC_CHANNEL_1);

//         // Advance and wrap index
//         if (++idx >= 32) idx = 0;

//         // 1 ms delay → 1 kHz/32 ≈ 31 Hz waveform
//         HAL_Delay(1);
//     }
// }

// /** Configure PA4 as analog, no pull **/
// void MX_GPIO_Init(void)
// {
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     GPIO_InitStruct.Pin  = GPIO_PIN_4;          // DAC_OUT1
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// }

// /** DAC1 Channel1 init for software trigger **/
// void MX_DAC_Init(void)
// {
//     __HAL_RCC_DAC1_CLK_ENABLE();

//     hdac.Instance = DAC1;
//     HAL_DAC_Init(&hdac);

//     DAC_ChannelConfTypeDef sConfig = {0};
//     sConfig.DAC_Trigger      = DAC_TRIGGER_SOFTWARE;
//     sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
//     HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
// }

// /** Standard 48 MHz HSI system clock setup **/
// void SystemClock_Config(void)
// {
//     RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};

//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//     RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
//     RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//     RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
//     RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
//     RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL6;
//     HAL_RCC_OscConfig(&RCC_OscInitStruct);

//     RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK
//                                      | RCC_CLOCKTYPE_HCLK
//                                      | RCC_CLOCKTYPE_PCLK1;
//     RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
//     RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//     HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
// }
// // int lab6_main(void) {
// // }


void adc_self_calibrate_enable_and_start(void) {
    // 1) Ensure ADC is powered off (ADEN=0) before calibration
    if (ADC1->CR & ADC_CR_ADEN) {
        ADC1->CR |= ADC_CR_ADDIS;               // disable ADC
        while (ADC1->CR & ADC_CR_ADEN) { }      // wait until fully disabled
    }

    // 2) Start calibration
    ADC1->CR |= ADC_CR_ADCAL;                   // set ADCAL bit
    while (ADC1->CR & ADC_CR_ADCAL) { }         // wait until ADCAL bit clears

    // 3) Enable the ADC
    ADC1->CR |= ADC_CR_ADEN;                    // set ADEN bit
    // 4) Wait for ADC ready (ADRDY flag)
    while (!(ADC1->ISR & ADC_ISR_ADRDY)) { }
    // Clear ADRDY by writing 1 (clears the flag in ISR)
    ADC1->ISR |= ADC_ISR_ADRDY;

    // 5) Start conversions (software trigger)
    ADC1->CR |= ADC_CR_ADSTART;                 // set ADSTART bit
    // From here, ADC will continuously convert if in continuous mode,
    // or perform one conversion if in single mode.
}



// #include "stm32f0xx.h"

// #define LED_PORT       GPIOC
// #define LED_PINS       (GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9)

// #define POT_PORT       GPIOA
// #define POT_PIN        GPIO_PIN_1    // ADC_IN1

// // ADC thresholds (0..255)
// #define THR0  64
// #define THR1  128
// #define THR2  192

// void init_leds(void) {
//     // 1) Enable GPIOB clock
//     RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//     // 2) Set PB0–PB3 to general-purpose output (01)
//     GPIOC->MODER &= ~(0xFFU);           // clear MODER0..MODER3
//     GPIOC->MODER |=  (0x55U);           // 01 for pins 0..3
//     // 3) Push-pull, no pull-up/down, medium speed
//     GPIOC->OTYPER &= ~LED_PINS;
//     GPIOC->PUPDR   &= ~LED_PINS;
//     GPIOC->OSPEEDR |= (LED_PINS << 0);   // medium speed
// }

// void init_adc(void) {
//     // 2) Configure PA1 as analog, no pulls
//     RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//     POT_PORT->MODER   |=  (3U << (1*2));   // analog mode = 11
//     POT_PORT->PUPDR   &= ~(3U << (1*2));   // no pull

//     // 3) Enable ADC1 clock
//     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

//     // 4) ADC config: 8-bit, continuous, software trigger
//     //      RES[1:0]=10 for 8-bit, CONT=1, EXTEN=00 disable HW triggers
//     ADC1->CFGR1 = (2U << ADC_CFGR1_RES_Pos)
//                 |  ADC_CFGR1_CONT;

//     // 5) Select channel 1
//     ADC1->CHSELR = ADC_CHSELR_CHSEL1;

//     // 6a) Calibrate: set ADCAL, wait for it to clear
//     ADC1->CR |= ADC_CR_ADCAL;
//     while (ADC1->CR & ADC_CR_ADCAL) { }

//     // 6b) Enable ADC, wait for ready
//     ADC1->CR |= ADC_CR_ADEN;
//     while (!(ADC1->ISR & ADC_ISR_ADRDY)) { }

//     // 6c) Start continuous conversions
//     ADC1->CR |= ADC_CR_ADSTART;
// }

// int main(void) {
//     uint8_t value;
    
//     init_leds();
//     init_adc();

//     while (1) {
//         // 7a) Read 8-bit result (right-aligned)
//         value = (uint8_t)(ADC1->DR & 0xFF);

//         // 7b) Update LEDs
//         // LED0 on PB0 if value >= THR0, LED1 on PB1 if >=THR1, etc.
//         LED_PORT->ODR = 0;
//         if (value >= THR0) LED_PORT->ODR |= GPIO_PIN_6;
//         if (value >= THR1) LED_PORT->ODR |= GPIO_PIN_7;
//         if (value >= THR2) LED_PORT->ODR |= GPIO_PIN_8;
//         if (value == 255) LED_PORT->ODR |= GPIO_PIN_9;  // top level
//     }
// }