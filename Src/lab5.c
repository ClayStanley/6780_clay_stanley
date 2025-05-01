#include "stm32f0xx.h"
#include <stdint.h>


#define L3GD20_ADDR       0x69
#define CTRL_REG1         0x20
#define OUT_X_L           0x28
//#define OUT_X_L           0xA8
#define OUT_X_H           0x29
#define OUT_Y_L           0x2A
//#define OUT_Y_L           0xAA
#define OUT_Y_H           0x2B
#define THRESHOLD         100
#define MULTI_READ     0x80
uint8_t ctrl1 = (1<<3) | (1<<1) | (1<<0);  // 0b00001011 = 0x0B

volatile int16_t globe_x = 0;
volatile int16_t globe_y = 0;
uint8_t i2c_read(uint8_t addr, uint8_t reg);
void    i2c_write(uint8_t addr, uint8_t reg, uint8_t value);

/* 
  Pin connections (Discovery board wiring):
    PB11 — I2C2_SDA (jumpered to PB15)
    PB13 — I2C2_SCL
    PB14 — SDO/SA1 pin (drive high for 0x6Bh address)
    PC0  — SPI/I2C mode select (drive high for I2C)
*/
void LED_Init55(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~((0b11 << (6 * 2)) | (0b11 << (7 * 2)) | (0b11 << (8 * 2)) | (0b11 << (9 * 2)));
    GPIOC->MODER |= (0b01 << (6 * 2)) | (0b01 << (7 * 2)) | (0b01 << (8 * 2)) | (0b01 << (9 * 2));
}

void Gyro_Init(void) {
    // Enable X and Y axis, set PD = 1, others = 0
    // CTRL_REG1 = 0b00000011 = 0x03 (Y enabled, X enabled, PD = 1)
    //I2C2_WriteRegister(CTRL_REG1, 0x03);
//slave address #bytes
i2c_write(L3GD20_ADDR, CTRL_REG1, ctrl1);
}

/* 1) GPIO + AF setup for I2C2 */
static void I2C2_GPIO_Init(void) {
    /* Enable GPIOB & GPIOC clocks */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
    
    /* PB11 & PB13: AF mode (10), open-drain, no pull, AF1=I2C2_SDA / AF5=I2C2_SCL */
    GPIOB->MODER &= ~((3UL<<22)|(3UL<<26));
    GPIOB->MODER |=  ((2UL<<22)|(2UL<<26));
    GPIOB->OTYPER |=  (1UL<<11)|(1UL<<13);
    GPIOB->OSPEEDR &= ~((3UL<<22)|(3UL<<26));
    GPIOB->PUPDR   &= ~((3UL<<22)|(3UL<<26));
    /* AF selection on AFR[1] (pins 8–15) */
    GPIOB->AFR[1] &= ~((0xFUL<<12)|(0xFUL<<20));
    GPIOB->AFR[1] |=  ((1UL<<12)|(5UL<<20));  // PB11→AF1, PB13→AF5 :contentReference[oaicite:0]{index=0}&#8203;:contentReference[oaicite:1]{index=1}

    /* PB14: push-pull output, high (SDO high → 0x6Bh) */
    GPIOB->MODER   &= ~(3UL<<(14*2));
    GPIOB->MODER   |=  (1UL<<(14*2));
    GPIOB->OTYPER  &= ~(1UL<<14);
    GPIOB->ODR     |=  (1UL<<14);

    /* PC0: push-pull output, high (I2C mode select) */
    GPIOC->MODER   &= ~(3UL<<(0*2));
    GPIOC->MODER   |=  (1UL<<(0*2));
    GPIOC->OTYPER  &= ~(1UL<<0);
    GPIOC->ODR     |=  (1UL<<0);             // :contentReference[oaicite:2]{index=2}&#8203;:contentReference[oaicite:3]{index=3}

    //section 5.7.1 line 133
        // Set PC8 and PC9 to output mode
    GPIOC->MODER &= ~((0b11 << (8 * 2)) | (0b11 << (9 * 2)));
    GPIOC->MODER |= (0b01 << (8 * 2)) | (0b01 << (9 * 2));

    // Set initial state: PC8 ON, PC9 OFF
    GPIOC->ODR |= (1 << 8);
    GPIOC->ODR &= ~(1 << 9);
}

/* 2) I2C2 peripheral init at 100 kHz standard-mode */
static void I2C2_Init(void) {
    /* Enable I2C2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    /* Disable peripheral to configure */
    I2C2->CR1 &= ~I2C_CR1_PE;
    /* TIMINGR for 8 MHz → 100 kHz: PRESC=1, SCLDEL=4, SDADEL=2, SCLH=15, SCLL=19 */
    I2C2->TIMINGR = 0x10420F13;
    /* Enable I2C2 */
    I2C2->CR1 |= I2C_CR1_PE;
}
int lab5_main(void) {
    I2C2_GPIO_Init();
    LED_Init55();
    I2C2_Init();
    // I2C2->TIMINGR = 0x00303D5B;
    // I2C2->CR1 |= I2C_CR1_PE;
    Gyro_Init();

    while (1) {
    int16_t gyro_x, gyro_y;
    int8_t x_l, x_h, y_l, y_h;
    
    // Set bit-7 on OUT_X_L to enable auto-increment through OUT_Y_H
    // uint8_t x_l = L3GD20_WHO_AM_I(0x28); // Read OUT_X_L register
    // uint8_t x_h = L3GD20_WHO_AM_I(0x29); // Read OUT_X_H register
    // uint8_t y_l = L3GD20_WHO_AM_I(0x2A); // Read OUT_Y_L register
    // uint8_t y_h = L3GD20_WHO_AM_I(0x2B); // Read OUT_Y_H register
  //               i2c_write(0x69, 0x28); // Read OUT_X_L register
    x_l = i2c_read(0x69,0x28); // Read OUT_X_H register
  //               i2c_write(0x69,0x2A); // Read OUT_Y_L register
    y_l = i2c_read(0x69,0x2A); // Read OUT_Y_H register
  //               i2c_write(0x69,0x29); // Read OUT_X_L register
    x_h = i2c_read(0x69,0x29); // Read OUT_X_H register
  //               i2c_write(0x69,0x2B; // Read OUT_Y_L register
    y_h = i2c_read(0x69,0x2B); // Read OUT_Y_H register
    //i2c_read(L3GD20_ADDR, data);
    gyro_x = (int16_t)((x_h<<8) | x_l); // Combine high and low bytes for X-axis
    gyro_y = (int16_t)((y_h<<8) | y_l); // Combine high and low bytes for Y-axis

    globe_x += gyro_x;
    globe_y += gyro_y;
        
    Update_LEDs(globe_x, globe_y);
    Delay_ms(10);  // Adjust delay as needed
    }
}

/* 3) Perform a write of 0x0F then a read of 1 byte from slave 0x6B */
static uint8_t L3GD20_WHO_AM_I(uint8_t wanted_address){
    uint8_t value;

    /* --- Write phase: send register address --- */
    /* Clear SADD[9:0], NBYTES[7:0], RD_WRN, AUTOEND */
    I2C2->CR2 &= ~((0x3FFUL<<0)|(0xFFUL<<16)|(1UL<<25)|(1UL<<10));
    /* Set SADD=0x6B<<1, NBYTES=1, write (RD_WRN=0) */
    I2C2->CR2 |= ((0x69<<1)<<0)|(1UL<<16);
    /* Start */
    I2C2->CR2 |= I2C_CR2_START;
    /* Wait TXIS or NACK */
    while (!(I2C2->ISR & (I2C_ISR_TXIS|I2C_ISR_NACKF)));
    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;            // clear NACK
        return 0xFF;                           // error
    }
    /* Send register address 0x0F */
    I2C2->TXDR = wanted_address;
    /* Wait Transfer Complete */
    while (!(I2C2->ISR & I2C_ISR_TC));

    /* --- Read phase: restart for 1-byte read --- */
    I2C2->CR2 &= ~((0x3FFUL<<0)|(0xFFUL<<16)|(1UL<<25)|(1UL<<10));
    /* SADD=0x6B<<1, NBYTES=1, read (RD_WRN=1 at bit-10) */
    I2C2->CR2 |= ((0x69<<1)<<0)|(1UL<<16)|(1UL<<10);
    I2C2->CR2 |= I2C_CR2_START;
    /* Wait RXNE or NACK */
    while (!(I2C2->ISR & (I2C_ISR_RXNE|I2C_ISR_NACKF)));
    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;
        I2C2->CR2 |= I2C_CR2_STOP;
        return 0xFF;
    }
    value = (uint8_t)I2C2->RXDR;               // read the byte
    /* Wait TC then STOP */
    while (!(I2C2->ISR & I2C_ISR_TC));
    I2C2->CR2 |= I2C_CR2_STOP;

    return value;                             // expected 0xD4
}


void i2c_write(uint8_t addr, uint8_t reg,uint8_t value) {
    // 1) Configure CR2 for a 2-byte write: [reg, value]
    I2C2->CR2 = (addr << 1)                // 7-bit address + write
              | (2 << I2C_CR2_NBYTES_Pos)  // two bytes to send
              | I2C_CR2_START;             // send START
    // 2) Send register address
    while (!(I2C2->ISR & I2C_ISR_TXIS)) { }
    I2C2->TXDR = reg;
    // 3) Send data
    while (!(I2C2->ISR & I2C_ISR_TXIS)) { }
    I2C2->TXDR = value;
    // 4) Wait for transfer complete, then STOP
    while (!(I2C2->ISR & I2C_ISR_TC))  { }
    I2C2->CR2 |= I2C_CR2_STOP;
    // 5) Clear STOP flag
    while (!(I2C2->ISR & I2C_ISR_STOPF)) { }
    I2C2->ICR |= I2C_ICR_STOPCF;
}

// Read OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H into data[0..3]
uint8_t i2c_read(uint8_t addr, uint8_t reg) {
    uint8_t value = 0;
        // --- write the register address ---
        I2C2->CR2 = (addr << 1)                // write mode
                  | (1 << I2C_CR2_NBYTES_Pos)  // 1 byte
                  | I2C_CR2_START;
        while (!(I2C2->ISR & I2C_ISR_TXIS)) { }
        I2C2->TXDR = reg;
        while (!(I2C2->ISR & I2C_ISR_TC))  { }
        // --- restart for read ---
        I2C2->CR2 = (addr << 1)
                  | I2C_CR2_RD_WRN            // read mode
                  | (1 << I2C_CR2_NBYTES_Pos)
                  | I2C_CR2_START;
        // get the data
        while (!(I2C2->ISR & I2C_ISR_RXNE)) { }
        value = I2C2->RXDR;
        // finish
        while (!(I2C2->ISR & I2C_ISR_TC))  { }
        I2C2->CR2 |= I2C_CR2_STOP;
        while (!(I2C2->ISR & I2C_ISR_STOPF)) { }
        I2C2->ICR |= I2C_ICR_STOPCF;

        return value;
}
// int lab5_main(void) {
//     uint8_t who = 0;
//     I2C2_GPIO_Init();
//     I2C2_Init();

//     who = L3GD20_WHO_AM_I(0x0F);
//     /* At this point, `who` should be 0xD4 if the sensor is present. */
//     if (who == 0xD3) {
//         GPIOC->ODR |= (1 << 9);  // Turn on LED to indicate success
//         GPIOC->ODR &= ~(1 << 8);  // Turn on error LED
//     } else {
//         GPIOC->ODR &= ~(1 << 9);  // Turn on error LED
//         GPIOC->ODR |= (1 << 8);  // Turn on error LED

//     }

//     while (1) {
//         /* your post-lab: maybe blink an LED if who==0xD4 */
     
//     }
// }


void Update_LEDs(int16_t x, int16_t y) {
    GPIOC->ODR &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    if (x > THRESHOLD)      GPIOC->ODR |= (1 << 9); // Orange LED = +X
    else if (x < -1*THRESHOLD) GPIOC->ODR |= (1 << 8); // Green LED = -X
    if (y > THRESHOLD)      GPIOC->ODR |= (1 << 7); // Blue LED = +Y
    else if (y < -1*THRESHOLD) GPIOC->ODR |= (1 << 6); // Red LED = -Y
}
//section 5.7.1 end line 133

// #include "stm32f0xx.h"
// #include "hal_gpio.h"   // Assume this header provides your custom HAL functions
// /* —— Pin assignments (Discovery board) —— 
//    PB11 ← I2C2_SDA (jumpered to PB15)
//    PB13 ← I2C2_SCL
//    PB14 → SDO/SA1 (set high → slave addr 0x6B)
//    PC0  → SPI/I2C select (set high) :contentReference[oaicite:0]{index=0}&#8203;:contentReference[oaicite:1]{index=1}
// */

// /* —— LED pins on PC6–PC9 —— */
// #define LED_GREEN   (1U<<6)
// #define LED_ORANGE  (1U<<7)
// #define LED_RED     (1U<<8)
// #define LED_BLUE    (1U<<9)

// /* Dead-zone threshold (raw counts) */
// #define GYRO_THRESH 200

// /* Millisecond tick counter */
// // static volatile uint32_t msTicks;

// // /* SysTick interrupt: count 1 ms ticks */
// // void SysTick_Handler(void) {
// //     msTicks++;
// // }

// // /* Block for `ms` milliseconds */
// // static void delay_ms(uint32_t ms) {
// //     uint32_t end = msTicks + ms;
// //     while (msTicks < end) { __WFI(); }
// // }

// /* —— GPIO init for LEDs —— */
// static void LED_Init(void) {
//     RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//     /* PC6–PC9 as push-pull outputs */
//     for (int p=6; p<=9; p++) {
//         GPIOC->MODER   = (GPIOC->MODER & ~(3UL<<(p*2))) |  (1UL<<(p*2));
//         GPIOC->OTYPER &= ~(1UL<<p);
//         GPIOC->OSPEEDR &= ~(3UL<<(p*2));
//         GPIOC->PUPDR   &= ~(3UL<<(p*2));
//         GPIOC->ODR     &= ~(1UL<<p);
//     }
// }

// /* —— GPIO init for I2C2 on PB11/PB13 & control pins —— */
// static void I2C2_GPIO_Init(void) {
//     RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
//     /* PB11 & PB13 to AF open-drain (AF1/AF5) :contentReference[oaicite:2]{index=2}&#8203;:contentReference[oaicite:3]{index=3} */
//     GPIOB->MODER &= ~((3UL<<22)|(3UL<<26));
//     GPIOB->MODER |=  ((2UL<<22)|(2UL<<26));
//     GPIOB->OTYPER |=  (1UL<<11)|(1UL<<13);
//     GPIOB->PUPDR   &= ~((3UL<<22)|(3UL<<26));
//     GPIOB->AFR[1] &= ~((0xFUL<<12)|(0xFUL<<20));
//     GPIOB->AFR[1] |=  ((1UL<<12)|(5UL<<20)); // PB11→I2C2_SDA, PB13→I2C2_SCL

//     /* PB14 → push-pull high (SDO high → 0x6Bh) */
//     GPIOB->MODER   = (GPIOB->MODER & ~(3UL<<(14*2))) | (1UL<<(14*2));
//     GPIOB->OTYPER &= ~(1UL<<14);
//     GPIOB->ODR     |=  (1UL<<14);

//     /* PC0 → push-pull high (I2C select) */
//     GPIOC->MODER   = (GPIOC->MODER & ~(3UL<<(0*2))) | (1UL<<(0*2));
//     GPIOC->OTYPER &= ~(1UL<<0);
//     GPIOC->ODR     |=  (1UL<<0);
// }

// /* —— I2C2 init for 100 kHz @8 MHz PCLK —— */
// static void I2C2_Init(void) {
//     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
//     I2C2->CR1 &= ~I2C_CR1_PE;
//     /* TIMINGR: PRESC=1, SCLDEL=4, SDADEL=2, SCLH=15, SCLL=19 (standard-mode) :contentReference[oaicite:4]{index=4}&#8203;:contentReference[oaicite:5]{index=5} */
//     I2C2->TIMINGR = 0x10420F13;
//     I2C2->CR1 |= I2C_CR1_PE;
// }

// /* —— Write one byte to a register —— */
// static int I2C2_WriteReg(uint8_t reg, uint8_t val) {
//     /* Clear fields, set slave addr (0x6B<<1), 2 bytes, write */
//     I2C2->CR2 &= ~((0x3FFUL<<0)|(0xFFUL<<16)|(1UL<<25)|(1UL<<10));
//     I2C2->CR2 |= ((0x6B<<1)<<0)|(2UL<<16);
//     I2C2->CR2 |= I2C_CR2_START;
//     /* Wait TXIS */
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
//     I2C2->TXDR = reg;
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
//     I2C2->TXDR = val;
//     while (!(I2C2->ISR & I2C_ISR_TC));
//     I2C2->CR2 |= I2C_CR2_STOP;
//     return 0;
// }

// /* —— Read multiple bytes starting at `reg|0x80` (auto-inc) —— */
// static int I2C2_ReadBytes(uint8_t reg, uint8_t *buf, uint8_t len) {
//     I2C2->CR2 &= ~((0x3FFUL<<0)|(0xFFUL<<16)|(1UL<<25)|(1UL<<10));
//     I2C2->CR2 |= ((0x6B<<1)<<0)|(len<<16)|(1UL<<10);
//     I2C2->CR2 |= I2C_CR2_START;
//     for (uint8_t i = 0; i < len; i++) {
//         while (!(I2C2->ISR & I2C_ISR_RXNE));
//         buf[i] = (uint8_t)I2C2->RXDR;
//     }
//     while (!(I2C2->ISR & I2C_ISR_TC));
//     I2C2->CR2 |= I2C_CR2_STOP;
//     return 0;
// }

// int lab5_main(void) {
//     uint8_t data[4];
//     int16_t gx, gy;
//     int16_t abs_gx, abs_gy;

//     /* 1 ms SysTick */
//     SysTick_Config(SystemCoreClock/1000);

//     LED_Init();
//     I2C2_GPIO_Init();
//     I2C2_Init();

//     /* Enable X & Y axes, normal mode: CTRL_REG1 (0x20) = 0x0B (PD=1, XEN=1, YEN=1) */
//     I2C2_WriteReg(0x20, 0x0B);

//     while (1) {
//         /* Read X_L/X_H, Y_L/Y_H in one transaction (auto-inc bit=1<<7) */
//         I2C2_ReadBytes(0x28 | 0x80, data, 4);

//         gx = (int16_t)((data[1]<<8) | data[0]);
//         gy = (int16_t)((data[3]<<8) | data[2]);
//         abs_gx = (gx<0 ? -gx : gx);
//         abs_gy = (gy<0 ? -gy : gy);

//         /* Choose dominant axis */
//         if (abs_gx > abs_gy && abs_gx > GYRO_THRESH) {
//             /* X-axis dominates */
//             if (gx > 0) {
//                 GPIOC->ODR = LED_ORANGE;  // positive X → orange
//             } else {
//                 GPIOC->ODR = LED_BLUE;    // negative X → blue
//             }
//         }
//         else if (abs_gy > GYRO_THRESH) {
//             /* Y-axis dominates */
//             if (gy > 0) {
//                 GPIOC->ODR = LED_GREEN;   // positive Y → green
//             } else {
//                 GPIOC->ODR = LED_RED;     // negative Y → red
//             }
//         }
//         else {
//             /* Below threshold → all off */
//             GPIOC->ODR = 0;
//         }

//        // delay_ms(100);
//       // HAL_Delay(50); // Delay of ~500 ms (between 400 and 600ms)
//     }
// }

// void GPIO_Init5(void) {
//     // Enable GPIOB and GPIOC in RCC
//     RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

//     // // Set PB11 (I2C2_SDA) to alternate function mode, open-drain output type
//     // GPIOB->MODER &= ~(0b11 << (11 * 2));
//     // GPIOB->MODER |= (0b10 << (11 * 2));
//     // GPIOB->OTYPER |= (1 << 11);
//     // GPIOB->AFR[1] |= (1 << ((11 - 8) * 4));
    
//     // // Set PB13 (I2C2_SCL) to alternate function mode, open-drain output type
//     // GPIOB->MODER &= ~(0b11 << (13 * 2));
//     // GPIOB->MODER |= (0b10 << (13 * 2));
//     // GPIOB->OTYPER |= (1 << 13);
//     // // 4. Set Alternate Function 5 (AF5) in AFR[1] for PB13 from CHIP_DATA_SHEET each pin uses 4 bits
//     // // Set alternate function to AF5 for I2C2_SCL (PB13) (from Table 16 page 46)
//     // GPIOB->AFR[1] |= (0x5 << ((13 - 8) * 4));
//       //Enable PB11 for the SDA Pin
//       GPIO_InitTypeDef initPB11 = {GPIO_PIN_11, GPIO_MODE_AF_OD, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, 1};
//       HAL_GPIO_Init(GPIOB, &initPB11);
  
//       //Enable PB13 for the SCL Pin                            
//       GPIO_InitTypeDef initPB13 = {GPIO_PIN_13, GPIO_MODE_AF_OD, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, 5};
//       HAL_GPIO_Init(GPIOB, &initPB13);
    
//     // Set PB14 to output mode, push-pull, and set high
//     GPIOB->MODER &= ~(0b11 << (14 * 2));
//     GPIOB->MODER |= (0b01 << (14 * 2));
//     GPIOB->OTYPER &= ~(1 << 14);
//     GPIOB->BSRR = (1 << 14);
    
//     // Set PC0 to output mode, push-pull, and set high
//     GPIOC->MODER &= ~(0b11 << (0 * 2));
//     GPIOC->MODER |= (0b01 << (0 * 2));
//     GPIOC->OTYPER &= ~(1 << 0);
//     GPIOC->BSRR = (1 << 0);

//     // Set PC8 and PC9 to output mode
//     GPIOC->MODER &= ~((0b11 << (8 * 2)) | (0b11 << (9 * 2)));
//     GPIOC->MODER |= (0b01 << (8 * 2)) | (0b01 << (9 * 2));

//     // Set initial state: PC8 ON, PC9 OFF
//     GPIOC->ODR |= (1 << 8);
//     GPIOC->ODR &= ~(1 << 9);
// }

// void I2C2_Init(void) {
//     // Enable I2C2 peripheral in RCC
//     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    
//     // Configure I2C2 timing for 100 kHz (TIMINGR settings depend on system clock)
//     I2C2->TIMINGR = (0x1 << 28) | (0x4 << 20) | (0x2 << 16) | (0xF << 8) | (0x13);
    
//     // Enable I2C2 peripheral
//     I2C2->CR1 |= I2C_CR1_PE;
// }

// // void I2C2_WriteRegister(uint8_t reg, uint8_t data) {

// //     /* Clear the NBYTES and SADD bit fields
// //     * The NBYTES field begins at bit 16, the SADD at bit 0
// //     */
// //     I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
// //     // Set slave address (0x6B) and write mode
// //     I2C2->CR2 = (0x6B << 1) | (0x2A << 16) | (0 << 10);
    
// //     // Start condition
// //     I2C2->CR2 |= I2C_CR2_START;
    
// //     // Wait for TXIS flag
// //     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
// //     // Send register address
// //     I2C2->TXDR = reg;
    
// //     // Wait for TXIS flag
// //     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
// //     // Send data
// //     I2C2->TXDR = data;
    
// //     // Wait for TC flag
// //     while (!(I2C2->ISR & I2C_ISR_TC));
    
// //     // Stop condition
// //     I2C2->CR2 |= I2C_CR2_STOP;
// // }

// void I2C2_Read_WHO_AM_I(void) {
//     uint8_t who_am_i = 0;

//     // Step 1: Write phase (Send register address 0x0F)
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (0 << 10) | I2C_CR2_START;

//     // Step 2: Wait for TXIS or NACKF
//     while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF))) {}
//     if (I2C2->ISR & I2C_ISR_NACKF) return;  // Handle NACK error

//     // Step 3: Write 0x0F to TXDR
//     I2C2->TXDR = 0x0F;

//     // Step 4: Wait for TC
//     while (!(I2C2->ISR & I2C_ISR_TC)) {}

//     // Step 5: Read phase (read 1 byte)
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (1 << 10) | I2C_CR2_START;

//     // Step 6: Wait for RXNE or NACKF
//     while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF))) {}
//     if (I2C2->ISR & I2C_ISR_NACKF) return;  // Handle NACK error

//     // Step 7: Read from RXDR
//     who_am_i = I2C2->RXDR;

//     // Step 8: Wait for TC
//     while (!(I2C2->ISR & I2C_ISR_TC)) {}

//     // Step 9: Stop condition
//     I2C2->CR2 |= I2C_CR2_STOP;

//     // Optional: Debug - check if WHO_AM_I == 0xD4
//     if (who_am_i == 0xD3) {
//         GPIOC->ODR |= (1 << 8);  // Turn on LED to indicate success
//     } else {
//         GPIOC->ODR |= (1 << 9);  // Turn on error LED
//     }
// }


// int lab5_main(void) {
//     GPIO_Init5();
//     I2C2_Init(); // Initialize I2C2 peripheral
//     // GPIO_PWM_LED_Init();
//     // TIM2_Init();
//     // TIM3_PWM_Init();
//    // I2C2_WriteRegister();
//     //I2C2_Read_WHO_AM_I();

//     while (1) {}
// }
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #define L3GD20_ADDR       0x69
// #define CTRL_REG1         0x20
// #define OUT_X_L           0x28
// //#define OUT_X_L           0xA8
// #define OUT_X_H           0x29
// #define OUT_Y_L           0x2A
// //#define OUT_Y_L           0xAA
// #define OUT_Y_H           0x2B
// #define THRESHOLD         50
// #define MULTI_READ     0x80
// uint8_t ctrl1 = (1<<3) | (1<<1) | (1<<0);  // 0b00001011 = 0x0B


// void I2C2_ReadMulti(uint8_t start_reg, uint8_t *buffer, uint8_t length) {
//     // Send register address with auto-increment enabled (MSB = 1)
//     I2C2->CR2 = (L3GD20_ADDR << 1) | (1 << 16);
//     I2C2->CR2 |= I2C_CR2_START;
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
//     I2C2->TXDR = start_reg | 0x80;  // auto-increment
//     while (!(I2C2->ISR & I2C_ISR_TC));

//     // Repeated start for read
//     I2C2->CR2 = (L3GD20_ADDR << 1) | (length << 16) | I2C_CR2_RD_WRN;
//     I2C2->CR2 |= I2C_CR2_START;

//     for (uint8_t i = 0; i < length; i++) {
//         while (!(I2C2->ISR & I2C_ISR_RXNE));
//         buffer[i] = I2C2->RXDR;
//     }

//     while (!(I2C2->ISR & I2C_ISR_TC));
//     I2C2->CR2 |= I2C_CR2_STOP;
// }

// int16_t ReadAxisFromBuffer(uint8_t low, uint8_t high) {
//     return (int16_t)((high << 8) | low);
// }


void Delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; ++i) __NOP(); // rough delay for 8MHz clock
}



// void I2C2_Init(void) {
//     RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
//     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

//     GPIOB->MODER &= ~((0b11 << (11 * 2)) | (0b11 << (13 * 2)));
//     GPIOB->MODER |= (0b10 << (11 * 2)) | (0b10 << (13 * 2));
//     GPIOB->OTYPER |= (1 << 11) | (1 << 13);
//     GPIOB->AFR[1] &= ~((0xF << ((11 - 8) * 4)) | (0xF << ((13 - 8) * 4)));
//     GPIOB->AFR[1] |= (0x1 << ((11 - 8) * 4)) | (0x5 << ((13 - 8) * 4));

//     I2C2->TIMINGR = 0x00303D5B;
//     I2C2->CR1 |= I2C_CR1_PE;
// }




// volatile uint16_t globe_x = 0;
// volatile uint16_t globe_y = 0;
// int lab5_main(void) {
//     I2C2_GPIO_Init();
//     LED_Init55();
//     I2C2_Init();
//     // I2C2->TIMINGR = 0x00303D5B;
//     // I2C2->CR1 |= I2C_CR1_PE;
//     Gyro_Init();

//     while (1) {
//         // int16_t x = ReadAxis(OUT_X_L, OUT_X_H);
//         // int16_t y = ReadAxis(OUT_Y_L, OUT_Y_H);
//         // Update_LEDs(x, y);
//         // Delay_ms(100);

// 	//uint8_t data[4];
//     //I2C2_ReadMulti(OUT_X_L, data, 4);  // reads X_L, X_H, Y_L, Y_H
//     // int16_t x = ReadAxisFromBuffer(data[0], data[1]);
//     // int16_t y = ReadAxisFromBuffer(data[2], data[3]);
//     int16_t gyro_x, gyro_y;
//     uint8_t data[4];
    
//     // Set bit-7 on OUT_X_L to enable auto-increment through OUT_Y_H
//      i2c_read(L3GD20_ADDR, OUT_X_L | 0x80, data, 4);
//     //i2c_read(L3GD20_ADDR, data);
//     gyro_x = (int16_t)(data[1] << 8 | data[0]);
//     gyro_y = (int16_t)(data[3] << 8 | data[2]);

//     globe_x += gyro_x;
//     globe_y += gyro_y;
    
//     Update_LEDs(globe_x, globe_y);
//     Delay_ms(10);  // Adjust delay as needed
//     }
// }



// #define GYRO_ADDR    0x69    // 7-bit I²C address of L3GD20
// // #define OUT_X_L      0x28
// // #define OUT_X_H      0x29
// // #define OUT_Y_L      0x2A
// // #define OUT_Y_H      0x2B

// // Write one byte 'value' to register 'reg' at I2C slave 'addr'




// void i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
//     // 1) Wait until bus is free
//     while (I2C2->ISR & I2C_ISR_BUSY) { }

//     // 2) Configure transaction: 2 bytes (reg + data), write mode, generate START
//     //    - Clear SADD[9:0] and NBYTES[7:0]
//     I2C2->CR2 &= ~((0x3FFU << I2C_CR2_SADD_Pos) | (0xFFU << I2C_CR2_NBYTES_Pos));
//     //    - Set slave address (shifted into bits [7:1]), NBYTES = 2
//     I2C2->CR2 |= ((dev_addr << 1) << I2C_CR2_SADD_Pos)
//               | (2UL << I2C_CR2_NBYTES_Pos);
//     //    - Ensure write (RD_WRN = 0)
//     I2C2->CR2 &= ~I2C_CR2_RD_WRN;
//     //    - Generate START
//     I2C2->CR2 |= I2C_CR2_START;

//     // 3) Send register address
//     while (!(I2C2->ISR & I2C_ISR_TXIS)) { }   // wait until TXDR is empty
//     I2C2->TXDR = reg_addr;

//     // 4) Send data byte
//     while (!(I2C2->ISR & I2C_ISR_TXIS)) { }
//     I2C2->TXDR = data;

//     // 5) Wait for transfer complete (TC) then send STOP
//     while (!(I2C2->ISR & I2C_ISR_TC)) { }
//     I2C2->CR2 |= I2C_CR2_STOP;

//     // (Optional) wait for STOP to finish before returning
//     while (I2C2->CR2 & I2C_CR2_STOP) { }
// }

// void i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len) {
//     // 1) Wait for bus to be free
//     while (I2C2->ISR & I2C_ISR_BUSY) { }

//     // 2) Send the register address (1 byte), write mode
//     i2c_write(dev_addr, reg_addr, 0); // data=0 is a dummy; only reg addr matters

//     // 3) Configure the peripheral to read 'len' bytes
//     I2C2->CR2 &= ~((0x3FFU << I2C_CR2_SADD_Pos) |    // clear slave addr
//                    (0xFFU  << I2C_CR2_NBYTES_Pos) | // clear NBYTES
//                    1U<<I2C_CR2_RD_WRN_Pos);         // clear RD_WRN
//     I2C2->CR2 |= ((dev_addr << 1) << I2C_CR2_SADD_Pos)  // set slave addr
//                | (len << I2C_CR2_NBYTES_Pos)           // set number of bytes
//                |  I2C_CR2_RD_WRN                       // set read direction
//                |  I2C_CR2_START;                       // send repeated-START

//     // 4) Read the bytes
//     for (uint8_t i = 0; i < len; i++) {
//         // wait until RXNE (Receive register not empty)
//         while (!(I2C2->ISR & I2C_ISR_RXNE)) { }
//         buf[i] = I2C2->RXDR;
//     }

//     // 5) Wait for transfer complete, send STOP
//     while (!(I2C2->ISR & I2C_ISR_TC)) { }
//     I2C2->CR2 |= I2C_CR2_STOP;

//     // 6) Optional: wait for STOP flag to clear
//     while (I2C2->CR2 & I2C_CR2_STOP) { }
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// // 

// // int main(void) {
// //     GPIO_Init();
// //     I2C2_Init();
    
// //     // Example: Writing 0x01 to register 0x20 of L3GD20 sensor
// //     I2C2_WriteRegister(0x20, 0x01);
    
// //     while (1);
// // }


// // //READING version 1

// // uint8_t I2C2_ReadRegister(uint8_t reg) {
// //     uint8_t received_data;
    
//     // Set slave address (0x6B) and write mode to send register address
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (1 << 10);
    
//     // Start condition
//     I2C2->CR2 |= I2C_CR2_START;
    
//     // Wait for TXIS flag
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
//     // Send register address
//     I2C2->TXDR = reg;
    
//     // Wait for TC flag
//     while (!(I2C2->ISR & I2C_ISR_TC));
    
//     // Set slave address (0x6B) and read mode
//     I2C2->CR2 = (0x6B << 1) | (0x2A  << 16) | (1 << 10);
    
//     // Start condition
//     I2C2->CR2 |= I2C_CR2_START;
    
//     // Wait for RXNE flag
//     while (!(I2C2->ISR & I2C_ISR_RXNE));
    
//     // Read received data
//     received_data = I2C2->RXDR;
    
//     // Stop condition
//     I2C2->CR2 |= I2C_CR2_STOP;
    
//     return received_data;
// }

// READ REGISTER VERSION 2
// uint8_t I2C2_ReadRegister(uint8_t reg) {
//     uint8_t received_data = 0;

//     // Step 1: Write phase (Send register address)
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (0 << 10) | I2C_CR2_START;

//     // Step 2: Wait for TXIS or NACKF
//     while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF))) {}
//     if (I2C2->ISR & I2C_ISR_NACKF) return 0;  // Handle NACK error

//     // Step 3: Write register address to TXDR
//     I2C2->TXDR = reg;

//     // Step 4: Wait for TC
//     while (!(I2C2->ISR & I2C_ISR_TC)) {}

//     // Step 5: Read phase (read 1 byte)
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (1 << 10) | I2C_CR2_START;

//     // Step 6: Wait for RXNE or NACKF
//     while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF))) {}
//     if (I2C2->ISR & I2C_ISR_NACKF) return 0;  // Handle NACK error

//     // Step 7: Read from RXDR
//     received_data = I2C2->RXDR;

//     // Step 8: Wait for TC
//     while (!(I2C2->ISR & I2C_ISR_TC)) {}

//     // Step 9: Stop condition
//     I2C2->CR2 |= I2C_CR2_STOP;

//     return received_data;
// }

   
void I2C2_WriteRegister(uint8_t reg, uint8_t value) {
    I2C2->CR2 = (L3GD20_ADDR << 1) | (1 << 16); // 1 byte, write
    I2C2->CR2 |= I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXIS));
    I2C2->TXDR = reg;
    while (!(I2C2->ISR & I2C_ISR_TXIS));
    I2C2->TXDR = value;
    while (!(I2C2->ISR & I2C_ISR_TC));
    I2C2->CR2 |= I2C_CR2_STOP;
}
// int lab5_main(void) {
//     GPIO_Init();
//     I2C2_Init();
    
//     // Example: Writing 0x01 to register 0x20 of L3GD20 sensor
//     I2C2_WriteRegister(0x20, 0x01);
    
//     // Example: Reading from register 0x20
//     uint8_t sensor_data = I2C2_ReadRegister(0x20);
    
//     while (1);
// }

    // GPIOB->MODER &= ~(0b11 << (11)); // Clear mode bits for PA5
    // GPIOB->MODER |=  (0b10 << (11)); // Set PA5 to output mode (01)
    // GPIOB->OTYPER &=  ~(1 << 11);
    // GPIOB->OTYPER |=  (1 << 11);
    
    // GPIOB->MODER &= ~(0b11 << (13)); // Clear mode bits for PA5
    // GPIOB->MODER |=  (0b10 << (13)); // Set PA5 to output mode (01)
    // GPIOB->OTYPER &=  ~(1 << 13);
    // GPIOB->OTYPER |=  (1 << 13);
    
    // GPIOB->MODER &= ~(0b11 << (14)); // Clear mode bits for PA5
    // GPIOB->MODER |=  (0b01 << (14)); // Set PA5 to output mode (01)
    // GPIOB->OTYPER &=  ~(1 << 14);
    // GPIOB->OTYPER |=  (0<< 14);
    // GPIOB->ODR &=  ~(1 << 14);
    // GPIOB->ODR |=  (1 << 14);

    // GPIOC->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
    // GPIOC->MODER |=  (0b01 << (0)); // Set PA5 to output mode (01)
    // GPIOC->OTYPER &=  ~(1 << 0);
    // GPIOC->OTYPER |=  (0<< 0);
    // GPIOC->ODR &=  ~(1 << 0);
    // GPIOC->ODR |=  (1 << 0);

//     RCC->AHBENR |= (1 << 18) | (1 << 19);  // Enable GPIOB and GPIOC clocks
//     RCC->APB1ENR |= (1 << 22); // Enable I2C2 Clock
//     GPIOB->AFR[1] &= ~((0xF << ((11 - 8) * 4)) | (0xF << ((13 - 8) * 4)));  // Clear AF bits
//     GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4));  // Set AF1 (I2C2)
//     I2C2->TIMINGR = (0x13 << 28) | (0xF << 20) | (0x2 << 16) | (0x4 << 8) | (0xF << 0);
//     I2C2->CR1 |= (1 << 0); // Set PE bit in CR1 to enable I2C2
//     // Set PB11 (SDA) and PB13 (SCL) to Alternate Function mode

//     GPIOB->MODER &= ~((0b11 << (11 * 2)) | (0b11 << (13 * 2)));
//     GPIOB->MODER |= (0b10 << (11 * 2)) | (0b10 << (13 * 2));

//     // Set Open-Drain Output Type
//     GPIOB->OTYPER |= (1 << 11) | (1 << 13);

//     // Select I2C2 Alternate Function (AF1)
//     GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4));

//     GPIOB->MODER &= ~(0b11 << (14 * 2));
//     GPIOB->MODER |= (0b01 << (14 * 2));
//     GPIOB->OTYPER &= ~(1 << 14); // Push-Pull Output
//     GPIOB->ODR |= (1 << 14); // Set High

//     GPIOB->MODER &= ~((0b11 << (11 * 2)) | (0b11 << (13 * 2)));  // Clear mode bits
//     GPIOB->MODER |= (0b10 << (11 * 2)) | (0b10 << (13 * 2));  // Set AF mode (10)
//     GPIOB->OTYPER |= (1 << 11) | (1 << 13);  // Set PB11 & PB13 to open-drain

//     // Set PC0 as output (Interface Selection - I2C mode)
//     GPIOC->MODER &= ~(0b11 << (0 * 2));
//     GPIOC->MODER |= (0b01 << (0 * 2));
//     GPIOC->OTYPER &= ~(1 << 0); // Push-Pull Output
//     GPIOC->ODR |= (1 << 0); // Set High

//     // Clear the NBYTES and SADD bit fields
//     I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));

//     // Set Slave Address (0x6B for L3GD20) and Number of Bytes (1)
//     I2C2->CR2 |= (1 << 16) | (0x6B << 1);  // NBYTES = 1, SADD = 0x6B

//     // Indicate a Write Operation (RD_WRN = 0)
//     I2C2->CR2 &= ~(1 << 10);

//     // Set the START Bit to Begin Transmission
//     I2C2->CR2 |= (1 << 13);
//     uint8_t config = 0x0F;  // 0b00001111 -> Enables Normal Mode, X, and Y axes
//     HAL_I2C_Mem_Write(&hI2C2, L3GD20_ADDR, 0x20, I2C_MEMADD_SIZE_8BIT, &config, 1, HAL_MAX_DELAY);
//     int16_t read_gyro_axis(uint8_t reg_addr) {
//         uint8_t data[2];
//         HAL_I2C_Mem_Read(&hI2C2, L3GD20_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
//         return (int16_t)((data[1] << 8) | data[0]);  // Combine high and low byte
//     }
    
//     void read_gyro_data() {
//         int16_t x_data = read_gyro_axis(0x28);
//         int16_t y_data = read_gyro_axis(0x2A);
//         process_gyro_data(x_data, y_data);
//     }
//     #define THRESHOLD 1000  // Ignore small fluctuations

// void process_gyro_data(int16_t x, int16_t y) {
//     // Reset all LEDs
//     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

//     // X-axis movement
//     if (x > THRESHOLD) {
//         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Orange LED for +X
//     } else if (x < -THRESHOLD) {
//         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // Red LED for -X
//     }

//     // Y-axis movement
//     if (y > THRESHOLD) {
//         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Blue LED for +Y
//     } else if (y < -THRESHOLD) {
//         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Green LED for -Y
//     }
// }
// while (1) {
//     read_gyro_data();
//     HAL_Delay(100);  // Wait 100ms before the next reading
// }