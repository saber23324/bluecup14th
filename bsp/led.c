#include "led.h"

void LED_Disp(uchar dsLED)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//��������
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);//����LEDϨ��lСд��
	HAL_GPIO_WritePin(GPIOC,dsLED<<8,GPIO_PIN_RESET);//����8λ������C8-15���ţ�ֵΪ1�ĵ���
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);//��������
}
