/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "badc.h"
#include "i2c - hal.h"
#include "struct_typedef.h"
#include "sys.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uchar single_key_flag[4],long_key_flag[4],double_key_flag[4];
extern uint key_time[4];
extern uint  frq,pulse;//捕获

extern double pwm_cont;//占空比
extern uint8_t pwm_mode;//0低频 1高频
extern uint8_t RK_mode;//0R 1K
uint16_t pwm_time_flag;//定时
Manu_Sign menu_mode=1;
uint8_t pwmmodecont=0; 
extern int R_num;
extern int K_num;
extern double frq2v;
extern uint16_t pwm_cont_num;//占空比 num

extern double Hfrq2v;
extern double Lfrq2v;

uint8_t pwmcontrlflag=1; //1开锁
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* printf重定向 */
int fputc(int ch, FILE *f)
{

    uint8_t temp[1] = {ch};
    HAL_UART_Transmit(&huart1, temp, 1, 2);//huart1需要根据你的配置修改
    return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_RTC_Init();
  MX_TIM16_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
//	HAL_TIM_Base_Start_IT (&htim1);//tim1启动
	HAL_TIM_Base_Start_IT (&htim4);//tim4启动
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);//打开PWM
//	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);//捕获中断开启
	
	uint8_t temp[1];
	HAL_UART_Receive_IT(&huart1, temp, 1);//接收中断打开
	LED_Disp(0x00);//LED初始化
	uchar led_sta=0x00;
	
	LCD_Init();//LCD初始化
	LCD_Clear(White);
	LCD_SetBackColor(Black);
		LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line0, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line1, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line2, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line3, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line4, (uint8_t *)"                    ");
	
LCD_SetBackColor(Black);
LCD_SetTextColor(White);

	LCD_DisplayStringLine(Line5, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line6, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line8, (uint8_t *)"                    ");
	LCD_DisplayStringLine(Line9, (uint8_t *)"                    ");
	
//	uchar er_data;//eeprom操作 复位一次count增加1
//	er_data=eeprom_read(1);
//	er_data++;
//	eeprom_write(1,er_data);
//	char text[30];
//	sprintf(text,"    count:%d",er_data);
//	LCD_DisplayStringLine(Line6,(uint8_t *)text);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
extern	uint16_t arr;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		LCD_SetBackColor(Black);
		LCD_SetTextColor(White);
		char text[30];
		
		RTC_TimeTypeDef time;//RTC相关
		RTC_DateTypeDef date;
		HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);//单独获取时间不获取日期结果有误。
		HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);//RTC_FORMAT参数选BIN而不是BCD（与cubemx配置无关）否则要进行数值转换才能用
		
		if(menu_mode==recd_menu)//统计界面
		{
			LED_Disp(led_sta);
								sprintf(text,"         RECD");
			LCD_DisplayStringLine(Line2,(uint8_t *)text);
			
			sprintf(text,"      N= :%d          ",pwmmodecont);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
			
			sprintf(text,"      MH= :%.1f ",Hfrq2v);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
			
			sprintf(text,"      ML= :%.1f          ",Lfrq2v);
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
			
		}
		if(menu_mode==data_menu)//数据界面
		{
			LED_Disp(led_sta|0x01);
					sprintf(text,"         DATA");
			LCD_DisplayStringLine(Line2,(uint8_t *)text);
			
			sprintf(text,"      M= :%d          ",menu_mode);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
			
			sprintf(text,"      P= :%.0f ",pwm_cont*100);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
			
			sprintf(text,"      V= :%.1f %c",frq2v,'%');
		LCD_DisplayStringLine(Line6,(uint8_t *)text);
			
		}
		if(menu_mode==para_menu)//数据界面
		{
			LED_Disp(led_sta);
			sprintf(text,"         PARA");
			LCD_DisplayStringLine(Line2,(uint8_t *)text);
			
			sprintf(text,"      R= :%d          ",R_num);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
			
			sprintf(text,"      K= :%d ",K_num);
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
			

	}
		if(single_key_flag[0]==1)//1被单次按下 定义为“界面”按键
		{
//			led_sta=led_sta<<1;
//			LED_Disp(led_sta);
			
			
			menu_mode++;
			LCD_Clear(Black);
			if(menu_mode>3)
				menu_mode=1;
			
			RK_mode=1;
			single_key_flag[0]=0;//用完标记
		}
		if(long_key_flag[0]==1)//700ms长按键
		{
			LED_Disp(0x80);
			long_key_flag[0]=0;
			
		}
		if(double_key_flag[0]==1)//双击判断
		{
			LED_Disp(0xaa);
			double_key_flag[0]=0;
		}
		
		if(single_key_flag[1]==1)//2被单次按下
		{
			
			
			if(menu_mode==data_menu)//高低频率切换
			{
				if(arr==1000 || arr==2000){
					pwm_mode=!pwm_mode;
				pwmmodecont++;
				}
			
				}
			
			if(menu_mode==para_menu)//para
			{
				RK_mode=!RK_mode;
			}
			
			
			single_key_flag[1]=0;//用完标记
		}
		
				if(single_key_flag[2]==1)//1被单次按下 定义为“界面”按键
		{
			if(menu_mode==para_menu)//para
			{
				if(RK_mode)
				{
					R_num++;
				if(R_num>10)
					R_num=1;
				}
				else
				{
					K_num++;
				if(K_num>10)
					K_num=1;
				}					
				
				
			}
			
			single_key_flag[2]=0;//用完标记
		}
		
		
				if(single_key_flag[3]==1)//1被单次按下 定义为“界面”按键
		{

			if(menu_mode==para_menu)//para
			{
				if(RK_mode)
				{
					R_num--;
				if(R_num<1)
					R_num=10;
				}
				else
				{
					K_num--;
				if(K_num<1)
					K_num=10;
				}					
			}
			if(menu_mode==data_menu)//高低频率切换
			{
				pwmcontrlflag=1;
			}
			
			single_key_flag[3]=0;//用完标记
		}
		
		if(long_key_flag[3]==1)//700ms长按键
		{
			if(menu_mode==data_menu)//高低频率切换
			{
				pwmcontrlflag=0;
				
				}
			long_key_flag[3]=0;
		}
		if(pwmcontrlflag==0)
		{
			led_sta=led_sta|0x04;
		}
		else
			led_sta=led_sta&0xFB;
		
		
			if(arr!=1000 && arr!=2000){
				led_sta=led_sta|0x02;
			}			
			else
			led_sta=led_sta&0xFD;			
		

		
		
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
