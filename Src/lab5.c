// #include "stm32f0xx.h"

// void GPIO_Init(void) {
//     // Enable GPIOB and GPIOC in RCC
//     RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

//     // Set PB11 (I2C2_SDA) to alternate function mode, open-drain output type
//     GPIOB->MODER &= ~(0b11 << (11 * 2));
//     GPIOB->MODER |= (0b10 << (11 * 2));
//     GPIOB->OTYPER |= (1 << 11);
//     GPIOB->AFR[1] |= (1 << ((11 - 8) * 4));
    
//     // Set PB13 (I2C2_SCL) to alternate function mode, open-drain output type
//     GPIOB->MODER &= ~(0b11 << (13 * 2));
//     GPIOB->MODER |= (0b10 << (13 * 2));
//     GPIOB->OTYPER |= (1 << 13);
//     GPIOB->AFR[1] |= (1 << ((13 - 8) * 4));
    
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
// }

// void I2C2_Init(void) {
//     // Enable I2C2 peripheral in RCC
//     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    
//     // Configure I2C2 timing for 100 kHz (TIMINGR settings depend on system clock)
//     I2C2->TIMINGR = (0x1 << 28) | (0x4 << 20) | (0x2 << 16) | (0xF << 8) | (0x13);
    
//     // Enable I2C2 peripheral
//     I2C2->CR1 |= I2C_CR1_PE;
// }

// void I2C2_WriteRegister(uint8_t reg, uint8_t data) {
//     // Set slave address (0x6B) and write mode
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (0 << 10);
    
//     // Start condition
//     I2C2->CR2 |= I2C_CR2_START;
    
//     // Wait for TXIS flag
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
//     // Send register address
//     I2C2->TXDR = reg;
    
//     // Wait for TXIS flag
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
//     // Send data
//     I2C2->TXDR = data;
    
//     // Wait for TC flag
//     while (!(I2C2->ISR & I2C_ISR_TC));
    
//     // Stop condition
//     I2C2->CR2 |= I2C_CR2_STOP;
// }

// int main(void) {
//     GPIO_Init();
//     I2C2_Init();
    
//     // Example: Writing 0x01 to register 0x20 of L3GD20 sensor
//     I2C2_WriteRegister(0x20, 0x01);
    
//     while (1);
// }


// //READING

// uint8_t I2C2_ReadRegister(uint8_t reg) {
//     uint8_t received_data;
    
//     // Set slave address (0x6B) and write mode to send register address
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (0 << 10);
    
//     // Start condition
//     I2C2->CR2 |= I2C_CR2_START;
    
//     // Wait for TXIS flag
//     while (!(I2C2->ISR & I2C_ISR_TXIS));
    
//     // Send register address
//     I2C2->TXDR = reg;
    
//     // Wait for TC flag
//     while (!(I2C2->ISR & I2C_ISR_TC));
    
//     // Set slave address (0x6B) and read mode
//     I2C2->CR2 = (0x6B << 1) | (1 << 16) | (1 << 10);
    
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

// int main(void) {
//     GPIO_Init();
//     I2C2_Init();
    
//     // Example: Writing 0x01 to register 0x20 of L3GD20 sensor
//     I2C2_WriteRegister(0x20, 0x01);
    
//     // Example: Reading from register 0x20
//     uint8_t sensor_data = I2C2_ReadRegister(0x20);
    
//     while (1);
// }

//     // GPIOB->MODER &= ~(0b11 << (11)); // Clear mode bits for PA5
//     // GPIOB->MODER |=  (0b10 << (11)); // Set PA5 to output mode (01)
//     // GPIOB->OTYPER &=  ~(1 << 11);
//     // GPIOB->OTYPER |=  (1 << 11);
    
//     // GPIOB->MODER &= ~(0b11 << (13)); // Clear mode bits for PA5
//     // GPIOB->MODER |=  (0b10 << (13)); // Set PA5 to output mode (01)
//     // GPIOB->OTYPER &=  ~(1 << 13);
//     // GPIOB->OTYPER |=  (1 << 13);
    
//     // GPIOB->MODER &= ~(0b11 << (14)); // Clear mode bits for PA5
//     // GPIOB->MODER |=  (0b01 << (14)); // Set PA5 to output mode (01)
//     // GPIOB->OTYPER &=  ~(1 << 14);
//     // GPIOB->OTYPER |=  (0<< 14);
//     // GPIOB->ODR &=  ~(1 << 14);
//     // GPIOB->ODR |=  (1 << 14);

//     // GPIOC->MODER &= ~(0b11 << (0)); // Clear mode bits for PA5
//     // GPIOC->MODER |=  (0b01 << (0)); // Set PA5 to output mode (01)
//     // GPIOC->OTYPER &=  ~(1 << 0);
//     // GPIOC->OTYPER |=  (0<< 0);
//     // GPIOC->ODR &=  ~(1 << 0);
//     // GPIOC->ODR |=  (1 << 0);

// //     RCC->AHBENR |= (1 << 18) | (1 << 19);  // Enable GPIOB and GPIOC clocks
// //     RCC->APB1ENR |= (1 << 22); // Enable I2C2 Clock
// //     GPIOB->AFR[1] &= ~((0xF << ((11 - 8) * 4)) | (0xF << ((13 - 8) * 4)));  // Clear AF bits
// //     GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4));  // Set AF1 (I2C2)
// //     I2C2->TIMINGR = (0x13 << 28) | (0xF << 20) | (0x2 << 16) | (0x4 << 8) | (0xF << 0);
// //     I2C2->CR1 |= (1 << 0); // Set PE bit in CR1 to enable I2C2
// //     // Set PB11 (SDA) and PB13 (SCL) to Alternate Function mode

// //     GPIOB->MODER &= ~((0b11 << (11 * 2)) | (0b11 << (13 * 2)));
// //     GPIOB->MODER |= (0b10 << (11 * 2)) | (0b10 << (13 * 2));

// //     // Set Open-Drain Output Type
// //     GPIOB->OTYPER |= (1 << 11) | (1 << 13);

// //     // Select I2C2 Alternate Function (AF1)
// //     GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4));

// //     GPIOB->MODER &= ~(0b11 << (14 * 2));
// //     GPIOB->MODER |= (0b01 << (14 * 2));
// //     GPIOB->OTYPER &= ~(1 << 14); // Push-Pull Output
// //     GPIOB->ODR |= (1 << 14); // Set High

// //     GPIOB->MODER &= ~((0b11 << (11 * 2)) | (0b11 << (13 * 2)));  // Clear mode bits
// //     GPIOB->MODER |= (0b10 << (11 * 2)) | (0b10 << (13 * 2));  // Set AF mode (10)
// //     GPIOB->OTYPER |= (1 << 11) | (1 << 13);  // Set PB11 & PB13 to open-drain

// //     // Set PC0 as output (Interface Selection - I2C mode)
// //     GPIOC->MODER &= ~(0b11 << (0 * 2));
// //     GPIOC->MODER |= (0b01 << (0 * 2));
// //     GPIOC->OTYPER &= ~(1 << 0); // Push-Pull Output
// //     GPIOC->ODR |= (1 << 0); // Set High

// //     // Clear the NBYTES and SADD bit fields
// //     I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));

// //     // Set Slave Address (0x6B for L3GD20) and Number of Bytes (1)
// //     I2C2->CR2 |= (1 << 16) | (0x6B << 1);  // NBYTES = 1, SADD = 0x6B

// //     // Indicate a Write Operation (RD_WRN = 0)
// //     I2C2->CR2 &= ~(1 << 10);

// //     // Set the START Bit to Begin Transmission
// //     I2C2->CR2 |= (1 << 13);
// //     uint8_t config = 0x0F;  // 0b00001111 -> Enables Normal Mode, X, and Y axes
// //     HAL_I2C_Mem_Write(&hi2c1, L3GD20_ADDR, 0x20, I2C_MEMADD_SIZE_8BIT, &config, 1, HAL_MAX_DELAY);
// //     int16_t read_gyro_axis(uint8_t reg_addr) {
// //         uint8_t data[2];
// //         HAL_I2C_Mem_Read(&hi2c1, L3GD20_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
// //         return (int16_t)((data[1] << 8) | data[0]);  // Combine high and low byte
// //     }
    
// //     void read_gyro_data() {
// //         int16_t x_data = read_gyro_axis(0x28);
// //         int16_t y_data = read_gyro_axis(0x2A);
// //         process_gyro_data(x_data, y_data);
// //     }
// //     #define THRESHOLD 1000  // Ignore small fluctuations

// // void process_gyro_data(int16_t x, int16_t y) {
// //     // Reset all LEDs
// //     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

// //     // X-axis movement
// //     if (x > THRESHOLD) {
// //         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Orange LED for +X
// //     } else if (x < -THRESHOLD) {
// //         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // Red LED for -X
// //     }

// //     // Y-axis movement
// //     if (y > THRESHOLD) {
// //         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Blue LED for +Y
// //     } else if (y < -THRESHOLD) {
// //         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // Green LED for -Y
// //     }
// // }
// // while (1) {
// //     read_gyro_data();
// //     HAL_Delay(100);  // Wait 100ms before the next reading
// // }