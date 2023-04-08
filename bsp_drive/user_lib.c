#include "user_lib.h"


//���ٿ���
fp32 invSqrt(fp32 num)
{
    fp32 halfnum = 0.5f * num;
    fp32 y = num;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(fp32 *)&i;
    y = y * (1.5f - (halfnum * y * y));
    return y;
}

/**
  * @brief          б��������ʼ��
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @param[in]      ���ֵ
  * @param[in]      ��Сֵ
  * @retval         ���ؿ�
  */
void ramp_init(ramp_function_source_t *ramp_source_type, fp32 frame_period, fp32 max, fp32 min)
{
    ramp_source_type->frame_period = frame_period;
    ramp_source_type->max_value = max;
    ramp_source_type->min_value = min;
    ramp_source_type->input = 0.0f;
    ramp_source_type->out = 0.0f;
}

/**
  * @brief          б���������㣬���������ֵ���е��ӣ� ���뵥λΪ /s ��һ������������ֵ
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      ����ֵ
  * @param[in]      �˲�����
  * @retval         ���ؿ�
  */
void ramp_calc(ramp_function_source_t *ramp_source_type, fp32 input)
{
    ramp_source_type->input = input;
    ramp_source_type->out += ramp_source_type->input * ramp_source_type->frame_period;
    if (ramp_source_type->out > ramp_source_type->max_value)
    {
        ramp_source_type->out = ramp_source_type->max_value;
    }
    else if (ramp_source_type->out < ramp_source_type->min_value)
    {
        ramp_source_type->out = ramp_source_type->min_value;
    }
}
/**
  * @brief          һ�׵�ͨ�˲���ʼ��
  * @author         RM
  * @param[in]      һ�׵�ͨ�˲��ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @param[in]      �˲�����
  * @retval         ���ؿ�
  */
void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, fp32 frame_period, const fp32 num[1])
{
    first_order_filter_type->frame_period = frame_period;
    first_order_filter_type->num[0] = num[0];
    first_order_filter_type->input = 0.0f;
    first_order_filter_type->out = 0.0f;
}

/**
  * @brief          һ�׵�ͨ�˲�����
  * @author         RM
  * @param[in]      һ�׵�ͨ�˲��ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @retval         ���ؿ�
  */
void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, fp32 input)
{
    first_order_filter_type->input = input;
    first_order_filter_type->out =
        first_order_filter_type->num[0] / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * first_order_filter_type->out + first_order_filter_type->frame_period / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * first_order_filter_type->input;
}


//===�ص��ȽϺ�����ע���������ͺͻص�ƥ��===========
int compare_char(const void *a, const void *b)
{
    return (*(char *)a) - (*(char *)b);
}
 
int compare_int(const void *a, const void *b)
{
    return (*(int *)a) - (*(int *)b);
}
int compare_float(const void *a, const void *b)
{
    return (*(float *)a) - (*(float *)b);
}

#define N 15  //���ڴ�С

/**
 * @brief Move_Min_Filter ����ƽ��ֵ�˲�
 *
 * @param  input
 * @return int
 */
int Move_Min_Filter(int input)       
{
  uint8_t count;
  uint8_t i;
    int sum = 0;
    int arrary_temp[N];

    static int x[N] = {0};  /* ����Ļ����� */
    for ( i= N - 1; i > 0; i--) {  /* ����Ļ��������� */
        x[i] = x[i - 1];
    }
    x[0] = input;
    memcpy(arrary_temp, x, sizeof(int) * N);
		
		qsort (arrary_temp, N, sizeof (int),compare_int);



    for(count = 1; count < N-1; count++) {
        sum += arrary_temp[count];            //ȥ�����ֵ����Сֵ
    }
    return (int)(sum/(N-2));
}
int Move_LEFT_Min_Filter(int input)       
{
  uint8_t count;
  uint8_t i;
    int sum = 0;
    int arrary_temp[N];

    static int xi[N] = {0};  /* ����Ļ����� */
    for ( i= N - 1; i > 0; i--) {  /* ����Ļ��������� */
        xi[i] = xi[i - 1];
    }
    xi[0] = input;
    memcpy(arrary_temp, xi, sizeof(int) * N);
		
		qsort (arrary_temp, N, sizeof (int),compare_int);



    for(count = 1; count < N-1; count++) {
        sum += arrary_temp[count];            //ȥ�����ֵ����Сֵ
    }
    return (int)(sum/(N-2));
}
#define N2 9  //���ڴ�С

/**
 * @brief Move_Min_Filter ����ƽ��ֵ�˲�
 *
 * @param  input
 * @return int
 */
float K210_Min_Filter(float input)       
{
  uint8_t count;
  uint8_t i;
    float sum = 0;
    float arrary_temp[N];

    static float x[N] = {0};  /* ����Ļ����� */
    for ( i= N - 1; i > 0; i--) {  /* ����Ļ��������� */
        x[i] = x[i - 1];
    }
    x[0] = input;
    memcpy(arrary_temp, x, sizeof(float) * N);
		
		qsort (arrary_temp, N, sizeof (float),compare_int);



    for(count = 1; count < N-1; count++) {
        sum += arrary_temp[count];            //ȥ�����ֵ����Сֵ
    }
    return (float)(sum/(N-2));
}
#define Nx 9  //���ڴ�С

/**
 * @brief Move_Min_Filter ����ƽ��ֵ�˲�
 *
 * @param  input
 * @return int
 */
float K210x_Min_Filter(float input)       
{
  uint8_t count;
  uint8_t i;
    float sum = 0;
    float arrary_temp[N];

    static float x[N] = {0};  /* ����Ļ����� */
    for ( i= N - 1; i > 0; i--) {  /* ����Ļ��������� */
        x[i] = x[i - 1];
    }
    x[0] = input;
    memcpy(arrary_temp, x, sizeof(float) * N);
		
		qsort (arrary_temp, N, sizeof (float),compare_int);



    for(count = 1; count < N-1; count++) {
        sum += arrary_temp[count];            //ȥ�����ֵ����Сֵ
    }
    return (float)(sum/(N-2));
}
//��������
void abs_limit(fp32 *num, fp32 Limit)
{
    if (*num > Limit)
    {
        *num = Limit;
    }
    else if (*num < -Limit)
    {
        *num = -Limit;
    }
}

//�жϷ���λ
fp32 sign(fp32 value)
{
    if (value >= 0.0f)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

//��������
fp32 fp32_deadline(fp32 Value, fp32 minValue, fp32 maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0.0f;
    }
    return Value;
}

//int26����
int16_t int16_deadline(int16_t Value, int16_t minValue, int16_t maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0;
    }
    return Value;
}

//�޷�����
fp32 fp32_constrain(fp32 Value, fp32 minValue, fp32 maxValue)
{
    if (Value < minValue)
        return minValue;
    else if (Value > maxValue)
        return maxValue;
    else
        return Value;
}

//�޷�����
int16_t int16_constrain(int16_t Value, int16_t minValue, int16_t maxValue)
{
    if (Value < minValue)
        return minValue;
    else if (Value > maxValue)
        return maxValue;
    else
        return Value;
}

//ѭ���޷�����
fp32 loop_fp32_constrain(fp32 Input, fp32 minValue, fp32 maxValue)
{
    if (maxValue < minValue)
    {
        return Input;
    }

    if (Input > maxValue)
    {
        fp32 len = maxValue - minValue;
        while (Input > maxValue)
        {
            Input -= len;
        }
    }
    else if (Input < minValue)
    {
        fp32 len = maxValue - minValue;
        while (Input < minValue)
        {
            Input += len;
        }
    }
    return Input;
}

//���ȸ�ʽ��Ϊ-PI~PI

//�Ƕȸ�ʽ��Ϊ-180~180
fp32 theta_format(fp32 Ang)
{
    return loop_fp32_constrain(Ang, -180.0f, 180.0f);
}
