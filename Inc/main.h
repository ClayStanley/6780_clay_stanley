#pragma once
#include <stdint.h>

void SystemClock_Config(void);
int lab1_main(void);
int lab2_main(void);
int lab3_main(void);
int lab4_main(void);
int lab5_main(void);
int lab6_main(void);
int lab7_main(void);

#ifndef MAIN_H
#define MAIN_H

// Prototype for the custom function to configure EXTI line 0 for PA0
void configure_EXTI0_for_PA0(void);

#endif // MAIN_H
