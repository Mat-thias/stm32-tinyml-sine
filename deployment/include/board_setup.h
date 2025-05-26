#ifndef BOARD_SETUP_H
#define BOARD_SETUP_H

#include <stdarg.h> 
#include <string.h>
#include <stdio.h>

#include <stm32l4xx_hal.h>

// Pin definition for led
#define LED_PIN                     GPIO_PIN_5
#define LED_PORT                    GPIOA

// UART definition for uart2, for serial communication
#define RX_PIN                      GPIO_PIN_3
#define RX_PORT                     GPIOA
#define TX_PIN                      GPIO_PIN_2
#define TX_PORT                     GPIOA
#define UART2_PORT                  GPIOA

#define UART_PRINT_BUFFER_SIZE 100

void init_GPIO_pins(void);
void init_UART2(void);
void init_TIM2(void);

void analogWrite(uint8_t percentage_bright);
void UART_printf(const char* format, ...);

#endif // BOARD_SETUP_H