#include "board_setup.h"

// Global UART and Timer handles
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;

char uart_print_buffer[UART_PRINT_BUFFER_SIZE];

// SysTick interrupt handler: increments system tick for HAL timekeeping
extern "C" void SysTick_Handler(void) {
    HAL_IncTick();
}

// Initializes GPIO pins for LED (PWM output) for analog output 
// and UART2 TX/RX for serial communcation for printing.
void init_GPIO_pins(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure LED pin for alternate function (TIM2 PWM output)
    GPIO_InitStruct.Pin     = LED_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    // Configure UART2 TX and RX pins for alternate function
    GPIO_InitStruct.Pin     = RX_PIN | TX_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART2_PORT, &GPIO_InitStruct);
}

// Initializes UART2 with standard 115200 baud 8N1 configuration
void init_UART2(void) {
    __HAL_RCC_USART2_CLK_ENABLE();

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}


// Initializes TIM2 in PWM mode on channel 1 (for LED brightness control)
void init_TIM2(void) {
    __HAL_RCC_TIM2_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 79;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim2);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 499;  // 50% duty
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

// Writes a brightness percentage (0–100%) to the PWM output (LED)
void analogWrite(uint8_t percentage_bright) {
    if (percentage_bright > 100) percentage_bright = 100;

    uint32_t pulse = (htim2.Init.Period * percentage_bright) / 100;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
}

// Allows for print over the UART using the printf format
void UART_printf(const char* format, ...) {

    va_list args_list;
    va_start(args_list, format);

    vsnprintf(uart_print_buffer, UART_PRINT_BUFFER_SIZE, format, args_list);
    va_end(args_list);

    HAL_UART_Transmit(&huart2, (uint8_t *)uart_print_buffer, strlen(uart_print_buffer), HAL_MAX_DELAY);
}