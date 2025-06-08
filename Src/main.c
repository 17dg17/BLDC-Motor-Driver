// main.c
#include "main.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include <stdint.h>
#include <stdio.h>

/* PWM Duty Cycle */
#define MAX_DUTY_CYCLE 100
#define MIN_DUTY_CYCLE 0
#define STEP_DELAY_MS 1000  // Her komütasyon adımı arasında 5 saniye bekleme
#define DEAD_TIME_US 10
#define COMMUTATION_STEPS 6

/* GPIO Tanımları */
#define PHASE_A_HIGH GPIOE, GPIO_PIN_14 // TIM1_CH4
#define PHASE_B_HIGH GPIOB, GPIO_PIN_3  // TIM2_CH2
#define PHASE_C_HIGH GPIOB, GPIO_PIN_7  // TIM4_CH2
#define PHASE_A_LOW GPIOB, GPIO_PIN_9
#define PHASE_B_LOW GPIOE, GPIO_PIN_5
#define PHASE_C_LOW GPIOE, GPIO_PIN_1

/* External Timer Handle Declarations */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

/* Function Declarations */
static void SystemClock_Config(void);
void Error_Handler(void);

/* Ölü Zaman Fonksiyonu */
void Apply_Dead_Time(void) {
    uint32_t us = DEAD_TIME_US;
    while(us--) {
        __NOP(); __NOP(); __NOP(); __NOP();
    }
}

/* PWM Duty Cycle Ayarı */
void Set_PWM_Duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t DutyCycle) {
    if (DutyCycle > MAX_DUTY_CYCLE) DutyCycle = MAX_DUTY_CYCLE;
    uint32_t pulse = (htim->Init.Period + 1) * DutyCycle / 100;
    __HAL_TIM_SET_COMPARE(htim, Channel, pulse);
}

/* Tüm Fazları Kapalı Duruma Getirme */
void All_Phases_Off(void) {
    Set_PWM_Duty(&htim1, TIM_CHANNEL_4, 0);
    Set_PWM_Duty(&htim2, TIM_CHANNEL_2, 0);
    Set_PWM_Duty(&htim4, TIM_CHANNEL_2, 0);

    HAL_GPIO_WritePin(PHASE_A_LOW, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PHASE_B_LOW, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PHASE_C_LOW, GPIO_PIN_RESET);

    Apply_Dead_Time();
}

/* Komütasyon Adımlarını Çalıştırma */
void Execute_Step(uint8_t step) {
    if (step >= COMMUTATION_STEPS) return;

    All_Phases_Off();

    switch (step) {
    // Faz kontrolü güncellenmiş açıklamalarla

        case 0: // A-B
            HAL_GPIO_WritePin(PHASE_B_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim1, TIM_CHANNEL_4, MAX_DUTY_CYCLE);
            break;
        case 1: // A-C
            HAL_GPIO_WritePin(PHASE_C_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim1, TIM_CHANNEL_4, MAX_DUTY_CYCLE);
            break;
        case 2: // B-C
            HAL_GPIO_WritePin(PHASE_C_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim2, TIM_CHANNEL_2, MAX_DUTY_CYCLE);
            break;
        case 3: // B-A
            HAL_GPIO_WritePin(PHASE_A_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim2, TIM_CHANNEL_2, MAX_DUTY_CYCLE);
            break;
        case 4: // C-A
            HAL_GPIO_WritePin(PHASE_A_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim4, TIM_CHANNEL_2, MAX_DUTY_CYCLE);
            break;
        case 5: // C-B
            HAL_GPIO_WritePin(PHASE_B_LOW, GPIO_PIN_SET);
            Apply_Dead_Time();
            Set_PWM_Duty(&htim4, TIM_CHANNEL_2, MAX_DUTY_CYCLE);
            break;
    }
}

/* System Clock Configuration */
static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();

    All_Phases_Off();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

// PWM Test Görev Döngüsü
#define TEST_DUTY_CYCLE 50  // %50 görev döngüsü

// Test Görev Döngüsü Ayarı
Set_PWM_Duty(&htim1, TIM_CHANNEL_4, TEST_DUTY_CYCLE);
Set_PWM_Duty(&htim2, TIM_CHANNEL_2, TEST_DUTY_CYCLE);
Set_PWM_Duty(&htim4, TIM_CHANNEL_2, TEST_DUTY_CYCLE);

// Test için bekleme
HAL_Delay(5000);

    while (1) {
        for (uint8_t step = 0; step < COMMUTATION_STEPS; step++) {
            Execute_Step(step);
            HAL_Delay(STEP_DELAY_MS);  // 5 saniye bekleme eklendi
        }
    }
}

void Error_Handler(void) {
    while (1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(500);
    }
}
