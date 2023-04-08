#include "interrupt.h"
#include "usart.h"
#include "lcd.h"
#include "tim.h"
#include "badc.h"

uchar judge_state[4]={0},double_click_time[4]={0},key_state[4]={0},double_click_timerEN[4]={0},
							single_key_flag[4]={100},double_key_flag[4]={0},long_key_flag[4]={0};
uint key_time[4]={0};
uint16_t tim_cnt_20 = 0;
uint16_t tim_cnt_15 = 0;
uint16_t tim_cnt_10 = 0;
uint16_t tim_cnt_50 = 0;
uint16_t text = 0;
double get_adc_value;
double pwm_cont=0;//占空比 %
uint16_t pwm_cont_num=0;//占空比 num
uint8_t pwm_mode=0;//0低频 1高频
uint8_t RK_mode=0;//0R 1K
uint16_t arr=2000;
extern uint8_t pwmcontrlflag; //1开锁
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//回调函数
{
	    if(htim->Instance == htim4.Instance)
    {
        tim_cnt_20++;
        tim_cnt_15++;
        tim_cnt_10++;
				tim_cnt_50++;
        if(tim_cnt_15 >= 15)
        {
            tim_cnt_15 = 0;
			get_adc_value = getADC(&hadc2);//获取电压值
		if(get_adc_value>1 && get_adc_value <3 && pwmcontrlflag==1 )
		{
			pwm_cont =(get_adc_value*37.5-27.5)*0.01;//换算
		}

					
				}

       

        if(tim_cnt_10 >= 10)
        {
            tim_cnt_10 = 0;

					

		key_state[0]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key_state[1]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key_state[2]=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key_state[3]=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
		for(int i=0;i<4;i++)
		{
			switch (judge_state[i])
			{
				case 0:
				{
					if(key_state[i]==0)//按键按下
					{
						judge_state[i]=1;
						key_time[i]=0;
					}
					break;
				}
				case 1://消抖过程
				{
					if(key_state[i]==0)
					{
						judge_state[i]=2;
					}
					else judge_state[i]=0;//未按下
					break;
				}
				case 2:
				{
					if((key_state[i]==1)&&key_time[i]<70)//等待松开过程,且非长按键
					{	
						if(double_click_timerEN[i]==0) //可能双击按键的第一次，进入计时
						{
							double_click_timerEN[i]=1;
							double_click_time[i]=0;
						}
						else //在计时范围内又按了一次
						{
							double_key_flag[i]=1;//双击情况
							double_click_timerEN[i]=0;
						}
						judge_state[i]=0;
					}
					else if(key_state[i]==1&&key_time[i]>=200) judge_state[i]=0;//松开且是长按键
					else  
					{
						if (key_time[i]>=70)long_key_flag[i]=1;//长按键
						key_time[i]++;//长按键计时 还没松开
					}
					break;
				}
			}
			if(double_click_timerEN[i]==1)//延时确认是否双击
			{
				double_click_time[i]++;
				if(double_click_time[i]>=35) 
				{
					single_key_flag[i]=1;//按键1单次按下
					double_click_timerEN[i]=0;
				}
			}
		}
					
					
        }
				if(tim_cnt_20 >= 20)
        {
				
					
					
							tim_cnt_20 = 0;

        }
				if(tim_cnt_50 >= 50)
				{
					if(pwm_mode==1 && arr>1000)
					{
						arr-=10;

					}
					
					if(pwm_mode==0 && arr<2000)
					{
						arr+=10;
						
					}
					if(arr > 2000){
							arr=2000;

						}
						else if(arr < 1000){
							arr=1000;
						}
					
					TIM2->ARR=arr;
					
					pwm_cont_num= (unsigned  int)(pwm_cont*arr);
					__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,pwm_cont_num);
					tim_cnt_50=0;
				}
				
    }
		
	if(htim->Instance == htim1.Instance)//定时器1的事件
	{
	}
	
		
}

uint8_t rx[100];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(&huart1, rx, 1);
	LCD_DisplayStringLine(Line9,(uint8_t *)rx);//显示接收到的一个字符
}


double  tim_val1 = 0,tim_val2=0;  									// TIMx_CCR1 的值 
uint  frq = 0,pulse=0;
double frq2v=0;
double Hfrq2v=0;
double Lfrq2v=0;
uint16_t tim_val=0;
int R_num=1;
int K_num=1;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获计数器 频率测量
{
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)//中断消息来源 选择直接输入的通道
	{
			tim_val = __HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SetCounter(&htim3,0);
	frq = 1000000/tim_val;
	frq2v=(frq*2*3.14*R_num)/(100*K_num);
		if(pwm_mode==1)
		{
			if(frq2v>Hfrq2v)
				Hfrq2v=frq2v;
		}
		if(pwm_mode==0)
		{
			if(frq2v>Lfrq2v)
				Lfrq2v=frq2v;
		}
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
		
		
//		tim_val1= HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//获取计数器1的值
//		tim_val2= HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);//获取计数器2的值
//		__HAL_TIM_SetCounter(htim,0);//计数器归零
//		frq = 1000000/tim_val1; //frq=时钟（80m）/prescaler（80）/tim_val1
//		pulse=(tim_val2/tim_val1)*100;
//		HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_1);
//		HAL_TIM_IC_Start_IT(htim, TIM_CHANNEL_2);
	}
}
