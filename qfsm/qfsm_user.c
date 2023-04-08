#include "qfsm_user.h"

#define DEBUG_PRINT_IN_THIS_FILE 0
#if DEBUG_PRINT_IN_THIS_FILE
#define PRINTF(...) PRINT(__VA_ARGS__)
#else
#define PRINTF(...) do {} while (0)
#endif

#define BUILD 1
	
//extern float movex,movey,movez;//�ƶ��ķ����Ƕ�;
//extern unsigned char head_led,back_led,left_led,right_led;
//extern PID Move_Z_pid;//����PID
//extern PID Head_Z_pid;//����PID
//extern PID Left_Z_pid;//����PID
//extern PID K210z_pid;//K210ѭ��

//extern PID K210x_pid;//K210ѭ��
//extern PID K210y_pid;//K210ѭ��
//extern K210 uart_K210;
//extern MPU_ctrl_t mpu_crtl;
//extern uint8_t uart_code[13];//��ά����ɫ
//extern uint8_t uart_color[12];//�õ�����ɫ
//extern int left_bias;
//extern MAPS map;
#define BIAS 5
#define BIAS_min 2

#define TM_BIAS 280
	
#define Min_head_bias 100
#define Mid_head_bias 150
#define Max_head_bias 200

#define Min_left_bias 100
#define Mid_left_bias 150
#define Max_left_bias 200

QState Car_initial(void *me, QEvent const *e);  /* ������ʼ��״̬���� */
	
QState QF_MID(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_FW(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_BW(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_LW(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_RW(void *me, QEvent const *e);  /* ��������״̬���� */
	
QState QF_TL(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_TR(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_TM(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Catch(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_TakeSite(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Return(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Put1(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Put2(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_GetCode(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Find_bridge(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Find_put(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Find_put2(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Get_number(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Put_bridge(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Put_end(void *me, QEvent const *e);  /* ��������״̬���� */
QState QF_Put_end2(void *me, QEvent const *e);  /* ��������״̬���� */
/**
 * @brief Menu_initial ״̬����ʼ��
 *
 * @param me
 * @param e
 * @return QState
 */
QState Car_initial(void *me, QEvent const *e)
{
    (void)e;
    return Q_TRAN(&QF_MID);  //�����ʼ״̬
}


extern QEvent car_signal;
/*
�źţ�
		MID_SIG,
		TR_SIG,
		TL_SIG,
		LW_SIG,
		BW_SIG,
		FW_SIG,
		RW_SIG,
*/
QState QF_MID(void *me, QEvent const *e)
{
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */

            PRINTF("entry QF_MID\n");
            car_signal.sig = Q_INIT_SIG;  /* ��ִֻ֤��һ�ν����¼���ִ�й������¼���ִ�������¼�(INIT) */
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            printf("in QF_MID\n");

//					if(map.turn[map.idx]==TURN_RIGHT)//���ҷ���
//					{
//						car_signal.sig = TR_SIG;
//					}
//					else if(map.turn[map.idx]==TURN_LEFT)
//					{
//						car_signal.sig = TL_SIG;
//					}
//					else if(map.turn[map.idx]==TAKE_FIRST_SITE)//��ʼ����λ��
//					{
//						car_signal.sig = Take_Site_SIG;
//					}
//					else if(map.turn[map.idx]==GETNUMBER)
//					{
//						car_signal.sig = Get_num_SIG;
//					}
//					else if(map.turn[map.idx]==GETCODECOLOR)
//					{
//						car_signal.sig = GetCode_SIG;
//					}
//					else if(map.turn[map.idx]==Find_Bridge)
//					{
//						car_signal.sig = Find_Bridge_SIG;
//					}
//					else if(map.turn[map.idx]==Find_End)
//					{
//						car_signal.sig = Find_Put_SIG;
//					}
//					else if(map.turn[map.idx]==Finish)
//					{
//						car_signal.sig = Return_SIG;
//					}
//					else 
//					{
//							if(map.set_head[map.idx]>0)//��ֱ���ٲ���
//							{
//								flag_site=1;
//								car_signal.sig = FW_SIG;
//							}
//							else if(map.set_head[map.idx]<0)
//							{
//								flag_site=1;
//								map.set_head[map.idx]=-map.set_head[map.idx];
//								car_signal.sig = BW_SIG;
//							}
//							else{
//								if(map.set_left[map.idx]>0)
//								{
//									flag_site=3;
//									car_signal.sig = LW_SIG;

//								}
//								else if(map.set_left[map.idx]<0)
//								{
//									flag_site=3;
//									map.set_left[map.idx]=-map.set_left[map.idx];
//									car_signal.sig = RW_SIG;

//								}
//								else
//								{
//									if(map.turn[map.idx]!=Finish)
//									{
//											movex=0;
//											movey=0;
//											map.idx+=1;
//									}
//								}
//						}

//					}



						return Q_HANDLED();
        }
				
				case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					
          
						return Q_HANDLED();
        }
				
//				case TR_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_TR);
//        }
//				case TL_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_TL);
//        }
//					case TM_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_TM);
//					}
//				case LW_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_LW);
//        }
//				case BW_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_BW);
//        }
//				case FW_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_FW);
//        }
//				case RW_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_RW);
//        }
//				case GetCode_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_GetCode);
//        }
//				case Take_Site_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_TakeSite);
//        }
//				case Find_Bridge_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_Find_bridge);
//        }
//				case Get_num_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_Get_number);
//        }
//				case Find_Put_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_Find_put);
//        }
//				case Return_SIG: {  /* ״̬ת�� */
//            PRINTF("train to QF_ENTER\n");
//            return Q_TRAN(&QF_Return);
//        }
				

    }
    return Q_IGNORED();
}
#if 0

QState QF_FW(void *me, QEvent const *e)
{  
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
						flag_move=1;
						map.num_head=map.set_head[map.idx];
						if(map.num_head>1)
						while(movex<=fastspeed)
						{
							movex+=25;
							HAL_Delay(25);
							if(movex>medomspeed)
							{
								movex=fastspeed;
								break;
							}
						}
						else{
							while(movex<=slowspeed)
						{
							movex+=50;
							HAL_Delay(2);
						}
						}
            car_signal.sig = Q_INIT_SIG;
					

            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
					
//					while(back_led==0x3f);//��ֹδ�ͷ�
//					while(back_led!=0x3f)
//					{HAL_Delay(5);}
//					BEEL_ON;
//					HAL_Delay(20);

					while(head_led!=0x0f)
					{HAL_Delay(5);}
					while(head_led==0x0f)//��ֹδ�ͷ�
					{HAL_Delay(5);}
					map.num_head--;
					HAL_Delay(20);
					BEEL_OFF;
					printf("num_head:%d \n",map.num_head);
					
		
					
			if(map.num_head==0){ //�жϵ������һ��ʱ
				
				movex=slowspeed;
				

			
					while(1)//�����ֵ
					{
						if(abs(left_bias)>Mid_left_bias && abs(left_bias)<Max_left_bias )
						{
							break;
						}
					}
					while(1)//�����ж�
					{
						BEEL_ON;
					//	if((left_led>=4&&left_led<=8)  || (right_led>=4&&right_led<=8) )//���ж� һ�߼���
					
							if(abs(left_bias)<Min_left_bias  )//���ж� һ�߼���
							{
								movex=0;//ͣ��
								movey=0;
								BEEL_OFF;
								break;
							}
					}
					HAL_Delay(500);
					printf("ok \n ok \n ok \n ok");
					map.set_head[map.idx]=0;//��ɱ�־
					car_signal.sig = MID_SIG;
			}
						 
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					map.idx+=1;//��λ
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}

QState QF_BW(void *me, QEvent const *e)
{
    switch(e->sig){
        case Q_ENTRY_SIG: { /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
						map.num_head=map.set_head[map.idx];
						if(map.num_head>1)
						while(movex>-fastspeed)
						{
							movex-=25;
							HAL_Delay(15);
							if(movex<-medomspeed)
							{
								movex=-fastspeed;
								break;
							}
						}
						else{
							while(movex>-slowspeed)
						{
							movex-=50;
							HAL_Delay(2);
						}
					}
            car_signal.sig = Q_INIT_SIG;
					

            return Q_HANDLED();
        }
        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
//					while(head_led==0x0f);
//					while(head_led!=0x0f)
//					{HAL_Delay(5);}
//					HAL_Delay(20);
					while(back_led!=0x3f)
					{HAL_Delay(5);}
					while(back_led==0x3f)
					{HAL_Delay(5);}
					HAL_Delay(200);
					map.num_head--;
					HAL_Delay(20);
					printf("num_head:%d \n",map.num_head);
					
		
					
			if(map.num_head<=0){//�жϵ������һ��ʱ
				
//					while(movex<-slowspeed)//����
//				{
//					movex+=50;
//					HAL_Delay(2);
//				}
				movex=-slowspeed;
				
					while(1)//�����ֵ
					{
						if(abs(left_bias)>Mid_left_bias && abs(left_bias)<Max_left_bias )
						{
							break;
						}
					}
				while(1)//�����ж�
					{
						if(abs(left_bias)<Min_left_bias )//���ж� һ�߼���
							{
								movex=0;//ͣ��
								movey=0;
								break;
							}
					}
					HAL_Delay(500);
					printf("ok \n ok \n ok \n ok");
					map.set_head[map.idx]=0;//��ɱ�־
					car_signal.sig = MID_SIG;
			}
						 
            return Q_HANDLED();
        }
				
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					map.idx+=1;//��λ
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}

QState QF_RW(void *me, QEvent const *e)
{
    switch(e->sig){
        case Q_ENTRY_SIG: { /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
					
						map.num_left=map.set_left[map.idx];
					
						if(map.num_left>1)
						{
						while(movey>-medomspeed)
						{
							movey-=25;
							HAL_Delay(20);
						
						}
					}
						else
						{
						while(movey>-slowspeed)
						{
							movey-=50;
							HAL_Delay(2);
						}
						}
            car_signal.sig = Q_INIT_SIG;
					

            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
					
					while(left_led!=0x3f)
					{HAL_Delay(5);}
					
					while(right_led!=0x3f)
					{HAL_Delay(10);}
					map.num_left--;
					HAL_Delay(20);
					printf("num_left:%d \n",map.num_left);
					
		
					
			if(map.num_left<=0){//�жϵ������һ��ʱ
				
//					while(movey<=-slowspeed)//����
//				{
//					movey+=50;
//					HAL_Delay(2);
//				}
				movey=-slowspeed;

				while(1)//��ֵ
					{
						if(abs(head_bias)>Mid_head_bias && abs(head_bias)<Max_head_bias )
						{
							break;
						}
					}
				while(1)//�����ж�
					{
						if(abs(head_bias)<Min_head_bias )//���ж� һ�߼���
							{
								movex=0;//ͣ��
								movey=0;
								break;
							}
					}
					HAL_Delay(500);
					printf("ok \n ok \n ok \n ok");
					map.set_left[map.idx]=0;
					car_signal.sig = MID_SIG;

			}
						 
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					map.idx+=1;//��λ
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
QState QF_LW(void *me, QEvent const *e)
{
    switch(e->sig){

				case Q_ENTRY_SIG: { /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
						flag_site=3;
						map.num_left=map.set_left[map.idx];
						if(map.num_left>1)
							while(movey<medomspeed)
						{
							movey+=25;
							HAL_Delay(20);
						}
						else
							movey=slowspeed;
						 printf(" LWspped ok\n");

            car_signal.sig = Q_INIT_SIG;
					

            return Q_HANDLED();
        }


        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
					
					while(right_led!=0x3f)
					{HAL_Delay(5);}
					BEEL_ON;
					while(left_led!=0x3f)
					{HAL_Delay(10);}
						BEEL_OFF;
					map.num_left--;
					HAL_Delay(20);
					printf("num_left:%d \n",map.num_left);
										
		
					
			if(map.num_left<=0){//�жϵ������һ��ʱ
				
//					while(movey>slowspeed)//����
//				{
//					movey-=50;
//					HAL_Delay(2);
//				}
					movey=slowspeed;

					while(1)//��ֵ
					{
						if(abs(head_bias)>Mid_head_bias && abs(head_bias)<Max_head_bias )
						{
							break;
						}
					}
				BEEL_ON;
				while(1)//�����ж�
					{
						if(abs(head_bias)<Min_head_bias )//���ж� һ�߼���
							{
								movex=0;//ͣ��
								movey=0;
								break;
								BEEL_OFF;
							}
					}
					HAL_Delay(500);
					printf("ok \n ok \n ok \n ok");
					map.set_left[map.idx]=0;
					car_signal.sig = MID_SIG;
			}
						 
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					map.idx+=1;//��λ
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
					
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}

QState QF_TL(void *me, QEvent const *e)
{
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
						
						Move_Z_pid.SetPoint+=90;
						Move_Z_pid.SetPoint=theta_format(Move_Z_pid.SetPoint);
					
					if(Move_Z_pid.SetPoint<=-180)
					{
						Move_Z_pid.SetPoint+=360;
					}
					
					flag_move=1;
					flag_site=0;
					
						while(1)
						{	
							HAL_Delay(20);
							
							if((fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))>(-BIAS)) && (fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))<(BIAS)) )
							{
								
								break;
							}
						}
					
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
					
						BEEL_ON;

						while(1)
						{	
							HAL_Delay(20);
							
							if((fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))<(BIAS_min)) )
							{
								flag_site=1;//ѭ��
								if(abs(head_bias)<Min_head_bias ){
								map.turn[map.idx]=0;//���ת��
								break;
								}
							}

						}
						
				BEEL_OFF;
						
					car_signal.sig = MID_SIG;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
QState QF_TR(void *me, QEvent const *e)
{
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
						movex=0;
						movey=0;
						Move_Z_pid.SetPoint-=90;
						Move_Z_pid.SetPoint=theta_format(Move_Z_pid.SetPoint);
					
					if(Move_Z_pid.SetPoint<=-180)
					{
						Move_Z_pid.SetPoint+=360;
					}
					
					flag_move=1;
					flag_site=0;
						while(1)
						{	
							HAL_Delay(20);
							
							if((fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))>(-BIAS)) && (fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))<(BIAS)) )
							{
								
								break;
							}
						}
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            PRINTF("in QF_ENTER\n");
					BEEL_ON;

						while(1)
						{	
							HAL_Delay(20);
							
							if((fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))>(-BIAS_min)) && (fabs(theta_format(mpu_crtl.yaw-Move_Z_pid.SetPoint))<(BIAS_min)) )
							{
								flag_site=1;//ѭ��
								if((abs(head_bias)<Min_head_bias) ){
								map.turn[map.idx]=0;//���ת��
								break;
								}
							}

						}
						
				BEEL_OFF;
					car_signal.sig = MID_SIG;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case TM_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TM);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
QState QF_TM(void *me, QEvent const *e)
	{
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
						movex=0;
						movey=0;

					Head_Z_pid.SetPoint=TM_BIAS;
					flag_move=1;
					flag_site=1;
					
					movex=slowspeed;
						while(1)
						{	
									if(left_led!=0x00)
									{
										
										break;
									}

						}
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
            printf("in QF_TM\n");
					flag_site=1;
					while(1)
					{
		
						if(left_led>=0x08)
						movex=-30;
					if(left_led<0x08)
						movex=30;
					if(left_led==0 || left_led==0x3f)
						movex=0;
					}
					car_signal.sig = MID_SIG;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
	QState QF_TakeSite(void *me, QEvent const *e)  /* ��������״̬���� */
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
					
					movex=0;
					movey=0;
					flag_site=0;
					flag_move=1;
					
					
					movey=slowspeed;
					movex=slowspeed;
					K210_Code_begin(GET_CODE);
					while(1){
						
					if(left_led!=0)
						{
							break;
						}
					}
					
				
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
					Servo_move_on_act;
						while(1){
					if(left_led==0x3f)//���ж� һ�߼���
					{
						BEEL_ON;
						break;
					}
				}
					
				HAL_Delay(800);
				BEEL_OFF;
				while(1){
					if(left_led==0x3f )//���ж� һ�߼���
					{
						movey=0;
						break;
					}
				}
				HAL_Delay(1000);
			
				 
					
					car_signal.sig = Get_num_SIG;
				BEEL_OFF;
            return Q_HANDLED(); 
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
						map.turn[map.idx]=0;//���ת��
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Get_num_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Get_number);
        }

    }
    return Q_IGNORED();
}

QState QF_Catch(void *me, QEvent const *e)  /* ��������״̬���� */
	{
    switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
							movex=0;
							movey=0;
							K210_Line_begin(DISABLE_UART);
							flag_site=0;//������У׼
							flag_move=0;
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
						int ix=1;
						if(uart_code[12]==1)//������
						{
							ix=7;
						}
						for(int ixx=0;ixx<3;ixx++)
						{
							switch (uart_code[ix]) {
							case 1 :
								 catch2_1();
								 break; /* ��ѡ�� */
							case 2 :
								 catch2_2();
								 break; /* ��ѡ�� */
							case 3 :
								 catch2_3();
								 break; /* ��ѡ�� */
							case 4 :
								 catch1_1();
								 break; /* ��ѡ�� */
							case 5 :
								 catch1_2();
								 break; /* ��ѡ�� */
							case 6 :
								 catch1_3();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							uart_code[ix]=ixx;//�ŵ�λ��
							
							switch (uart_code[ix]) {
							case 0 :
								 put_carsite0();
								 break; /* ��ѡ�� */
							case 1 :
								 put_carsite1();
								 break; /* ��ѡ�� */
							case 2 :
								 put_carsite2();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							ix+=2;
						}
					
					
					BEEL_ON;					

//					mpuInit();
					HAL_Delay(500);
					K210_Line_begin(DISABLE_UART);
					flag_site=0;
					flag_move=1;
					
					movey=slowspeed;
					
					while(left_led!=0x3f);
					HAL_Delay(500);
					movex=0;//ͣ��
					movey=0;
					BEEL_OFF;
					flag_site=0;//���pid
					car_signal.sig = TL_SIG;
					
//					while(1)//�����ж�
//					{
//						BEEL_ON;
//							if(abs(head_bias)<Min_head_bias  )//���ж� һ�߼���
//							{
//								movex=0;//ͣ��
//								movey=0;
//								BEEL_OFF;
//								flag_site=0;//���pid
//								car_signal.sig = TL_SIG;
//								break;
//							}
//					}

					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */

					
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}


QState QF_GetCode(void *me, QEvent const *e)  /* ��������״̬���� */
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
						movex=0;
						movey=0;
						flag_move=0;
						flag_site=2;
						K210_Line_begin(Get_6Line);

						K210y_pid.SetPoint=B_k210.GetCodeColor.ynum;
						K210x_pid.SetPoint=B_k210.GetCodeColor.xnum;
						HAL_Delay(500);
						flag_move=1;//����ͷ��λ��
						Servo_extend_act;
					
					if(uart_code[12]==0){
					K210_Code_begin(GET_COLOR);

						while(1)
					{
						if(uart_color[0]!=0)
						{
							int ix=0;
							for(ix=0;ix<3;ix++)//���ϲ�λ����Ϣ
							{
								for(int ixx=0;ixx<6;ixx+=2)
								{
									if(uart_color[ix]==uart_code[ixx])
									{
										uart_code[ixx+1]=ix+1;
									}
								}

							}
							
							for(ix=3;ix<6;ix++)//���²�λ����Ϣ
							{
								for(int ixx=6;ixx<12;ixx+=2)
								{
									if(uart_color[ix]==uart_code[ixx])
									{
										uart_code[ixx+1]=ix+1;
									}
								}

							}
							BEEL_ON;
							HAL_Delay(100);
							BEEL_OFF;
							break;
						}
						HAL_Delay(15);
					}
				}

					car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
           
						if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <10 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <10 && flag_site==2)
						{
							BEEL_ON;
							K210_Code_begin(DISABLE_UART);
							K210_Line_begin(Get_6LineL);
							if(uart_code[12]==1)//������
						{
							
							K210y_pid.SetPoint=B_k210.GetCode2.ynum;
							K210x_pid.SetPoint=B_k210.GetCode2.xnum;
						}
						else{
							K210y_pid.SetPoint=B_k210.GetCode.ynum;
							K210x_pid.SetPoint=B_k210.GetCode.xnum;
						}
						
						BEEL_OFF;
						}
						
						
						
						if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
						{
							HAL_Delay(200);
							if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
							{
								HAL_Delay(200);
							if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
							{
							BEEL_ON;
							car_signal.sig = Catch_SIG;	
							BEEL_OFF;
							}
							}
							//HAL_Delay(500);
						BEEL_OFF;
						}
						
					
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Put_bridge_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Put_bridge);
        }
				case Catch_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Catch);
        }

    }
    return Q_IGNORED();
}

QState QF_Put1(void *me, QEvent const *e)//����λ
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
					movex=0;
					movey=0;
					flag_site=4;//�Թ�����λ��
					flag_move=1;
					Left_Z_pid.SetPoint=161;
					Head_Z_pid.SetPoint=-244;

					car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
           if(fabs(Left_Z_pid.SetPoint-left_bias)<Max_left_bias && fabs(Head_Z_pid.SetPoint-head_bias)<Max_left_bias && flag_site!=2)
						{BEEL_ON;
						Left_Z_pid.SetPoint=0;
						Head_Z_pid.SetPoint=0;
							K210_Code_begin(DISABLE_UART);
							K210_Line_begin(Get_6Line);
							HAL_Delay(500);
							K210y_pid.SetPoint=206.399;
							K210x_pid.SetPoint=176.99;
							flag_site=2;//����ͷ��λ��
						
						Servo_extend_act;
						BEEL_OFF;
						}
						
						if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
						{
							HAL_Delay(100);
							if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
							{
							BEEL_ON;
							car_signal.sig = Catch_SIG;									
							BEEL_OFF;
							}
							//HAL_Delay(500);



								
						BEEL_OFF;
						}
						
					
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Take_Site_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TakeSite);
        }
				case Catch_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Catch);
        }

    }
    return Q_IGNORED();
}

QState QF_Find_bridge(void *me, QEvent const *e)  /* ��������״̬���� */
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
	
						movex=0;
						movey=0;
						flag_move=0;
						K210_Line_begin(BRIDGE);
						flag_site=5;
						K210y_pid.SetPoint=B_k210.GetBridge.ynum;
						K210x_pid.SetPoint=B_k210.GetBridge.xnum;
						K210z_pid.SetPoint=B_k210.GetBridge.degree;
					HAL_Delay(100);
						flag_move=1;//����ͷ��λ��
						HAL_Delay(500);
					car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
           
						if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1&&fabs(K210z_pid.SetPoint-uart_K210.dis_circle_y) <0.5)
						{

							HAL_Delay(200);
								if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && fabs(K210z_pid.SetPoint-uart_K210.dis_circle_y) <0.5)
								{
									HAL_Delay(200);
									if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && fabs(K210z_pid.SetPoint-uart_K210.dis_circle_y) <0.5)
								{
									
									BEEL_ON;
									movex=0;
									movey=0;
									flag_move=0;
									HAL_Delay(200);
									mpureturnzero();
									HAL_Delay(1000);
									car_signal.sig = Put_bridge_SIG;									
									BEEL_OFF;
								}

								}

//								K210_Line_begin(DISABLE_UART);
//								flag_site=0;
//								flag_move=0;
								
						BEEL_OFF;
						}
						
					
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Put_bridge_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Put_bridge);
        }


    }
    return Q_IGNORED();
}
QState QF_Put_bridge(void *me, QEvent const *e)  /* ��������״̬���� */
{
		switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
											movex=0;
						movey=0;
						flag_site=0;
						flag_move=0;
//						flag_site=5;//����ͷУ׼
//						flag_move=1;
					
            car_signal.sig = Q_INIT_SIG;;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
//					flag_site=0;
//					K210_Line_begin(DISABLE_UART);
					
					BEEL_ON;
					HAL_Delay(500);
					BEEL_OFF;

					int ix=0;//yanse
						if(uart_code[12]==1)//������
						{
							ix=6;
						}
						for(int ixx=0;ixx<3;ixx++)
						{
							switch (uart_code[ix+1]) {
							case 0 :
								 catch_carsite0();
								 break; /* ��ѡ�� */
							case 1 :
								 catch_carsite1();
								 break; /* ��ѡ�� */
							case 2 :
								 catch_carsite2();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							
							switch (uart_code[ix]) {
							case 1 ://red
								 put_brige1();
								 break; /* ��ѡ�� */
							case 2 :
								 put_brige2();
								 break; /* ��ѡ�� */
							case 3 :
								 put_brige3();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							
							ix+=2;
						}
						
						//��
						ix=0;//yanse
						if(uart_code[12]==1)//������
						{
							ix=6;
						}
						for(int ixx=0;ixx<3;ixx++)
						{
							
							switch (uart_code[ix]) {
							case 1 ://red
								 catch_brige1();
								 break; /* ��ѡ�� */
							case 2 :
								 catch_brige2();
								 break; /* ��ѡ�� */
							case 3 :
								 catch_brige3();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							
							switch (uart_code[ix+1]) {
							case 0 :
								 put_carsite0();
								 break; /* ��ѡ�� */
							case 1 :
								 put_carsite1();
								 break; /* ��ѡ�� */
							case 2 :
								 put_carsite2();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}				
							ix+=2;
						}
					if(flag_site==0)//У׼�ɹ�
					{
//						mpuInit();
					}
					flag_move=0;
					K210_Line_begin(DISABLE_UART);
					flag_site=0;
					movex=0;
					movey=slowspeed;
					HAL_Delay(500);
					flag_move=1;
					while(left_led!=0x3f);
					HAL_Delay(500);
					movex=0;//ͣ��
					movey=0;
					BEEL_OFF;
					flag_site=0;//���pid
					car_signal.sig = TL_SIG;
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
	
					map.turn[map.idx]=0;//���ת��
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
QState QF_Find_put(void *me, QEvent const *e)  /* ��������״̬���� */
{
	
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
						movex=0;
						movey=0;
						flag_move=0;
						flag_site=2;
						K210_Line_begin(Get_6Line);
						if(uart_code[12]==1)//������
						{
							
							#if BUILD
							K210y_pid.SetPoint=B_k210.Get_End.ynum;
							K210x_pid.SetPoint=B_k210.Get_End.xnum;
							#else
							K210y_pid.SetPoint=B_k210.Get_End2.ynum;
							K210x_pid.SetPoint=B_k210.Get_End2.xnum;
							#endif
						}
						else{
							K210y_pid.SetPoint=B_k210.Get_End.ynum;
							K210x_pid.SetPoint=B_k210.Get_End.xnum;
						}
						HAL_Delay(500);
						flag_move=1;//����ͷ��λ��

					car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
						
						if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1 && flag_site==2)
						{

							HAL_Delay(200);
								if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1)
								{
									HAL_Delay(200);
								if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1)//TCP�������� ��
								{
									HAL_Delay(200);
									if(fabs(K210x_pid.SetPoint-uart_K210.Rho0) <1 && fabs(K210y_pid.SetPoint-uart_K210.Rho90) <1)//TCP�������� ��
								{
									BEEL_ON;
									car_signal.sig = Put_end_SIG;									
									BEEL_OFF;
								}
									
								}
								}

//								K210_Line_begin(DISABLE_UART);
//								flag_site=0;
//								flag_move=0;
								
						BEEL_OFF;
						}
						
					
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
					map.turn[map.idx]=0;//���ת��
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Put_end_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Put_end);
        }


    }
    return Q_IGNORED();
}
QState QF_Put_end(void *me, QEvent const *e)  /* ��������״̬���� */
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					
						movex=0;
						movey=0;
						flag_site=0;
						flag_move=0;
	
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
					
					BEEL_ON;
					HAL_Delay(1000);
					BEEL_OFF;
					
						int ix=0;//yanse
						if(uart_code[12]==1)//������
						{
							ix=6;
						}
						for(int ixx=0;ixx<3;ixx++)
						{
							switch (uart_code[ix+1]) {
							case 0 :
								 catch_carsite0();
								 break; /* ��ѡ�� */
							case 1 :
								 catch_carsite1();
								 break; /* ��ѡ�� */
							case 2 :
								 catch_carsite2();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
							if(uart_code[12]==0){
							switch (uart_code[ix]) {
							case 1 ://red
								 end_put_1_1();
								 break; /* ��ѡ�� */
							case 2 :
								 end_put_1_2();
								 break; /* ��ѡ�� */
							case 3 :
								 end_put_1_3();
								 break; /* ��ѡ�� */
							default : /* ��ѡ�� */
								 BEEL_ON;
							}
						}
							else if(uart_code[12]==1)
							{
								#if BUILD
							switch (uart_code[ix]) {
								case 1 ://red
									 end_put_1_11();
									 break; /* ��ѡ�� */
								case 2 :
									 end_put_1_22();
									 break; /* ��ѡ�� */
								case 3 :
									 end_put_1_33();
									 break; /* ��ѡ�� */
								default : /* ��ѡ�� */
									 BEEL_ON;
								}
							#else
							switch (uart_code[ix]) {
								case 1 ://red
									 end_put_2_1();
									 break; /* ��ѡ�� */
								case 2 :
									 end_put_2_2();
									 break; /* ��ѡ�� */
								case 3 :
									 end_put_2_3();
									 break; /* ��ѡ�� */
								default : /* ��ѡ�� */
									 BEEL_ON;
							}
							#endif
								
								
							}
							ix+=2;
						}
					
//					if(flag_site==0)//У׼�ɹ�
//					{
////						mpuInit();
//					}
					flag_move=0;
					K210_Line_begin(DISABLE_UART);
					flag_site=0;
					movex=0;
					movey=slowspeed;
					flag_move=1;
					HAL_Delay(800);
					movex=0;//ͣ��
					movey=0;
					BEEL_OFF;
					flag_site=0;//���pid
						if(uart_code[12]==0){
							car_signal.sig = MID_SIG;
						}
						else if(uart_code[12]==1){
							car_signal.sig = MID_SIG;
					}
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
					uart_code[12]=1;
					map.turn[map.idx]=0;//���ת��
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case TR_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TR);
        }
				case TL_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_TL);
        }
				case LW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_LW);
        }
				case BW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_BW);
        }
				case FW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_FW);
        }
				case RW_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_RW);
        }
    }
    return Q_IGNORED();
}
QState QF_Get_number(void *me, QEvent const *e)
{
	
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					

						flag_site=3;//           
						flag_move=1;
						movex=0;
						movey=0;
						K210_Code_begin(GET_CODE);
						car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
					BEEL_ON;
					
					if(uart_code[0]==0)
						{
							
					movey=-slowspeed;
					HAL_Delay(500);
					movey=0;
					while(1){
					if(uart_code[0]!=0)
						{
							break;
						}
						HAL_Delay(50);
					}
					movey=slowspeed;
					HAL_Delay(1200);
					
		
					BEEL_OFF;
				}
					BEEL_OFF;
					movex=0;
					movey=0;
					K210_Code_begin(DISABLE_UART);//��������ͷ
					car_signal.sig = MID_SIG;	
					
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Put_end_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Put_end);
        }


    }
    return Q_IGNORED();
}
QState QF_Return(void *me, QEvent const *e)  /* ��������״̬���� */
{
	switch(e->sig){
        case Q_ENTRY_SIG: {  /* �����¼� */
            PRINTF("entry QF_ENTER\n");
					Servo_move_on_act;
						movex=0;
						movey=0;
						flag_site=0;
						flag_move=1;
							while(1)
						{
							movex+=25;
							HAL_Delay(15);
							if(movex>medomspeed)
							{
								break;
							}
						}
					while(1)
						{
							movey-=25;
							HAL_Delay(15);
							if(movey<=-medomspeed+70)
							{
								break;
							}
						}
					HAL_Delay(200);
					
				
            car_signal.sig = Q_INIT_SIG;
            return Q_HANDLED();
        }

        case Q_INIT_SIG: {  /* ����ִ�е�״̬ */
					while(1){
					if(left_led==0x3f )
						{
							
						break;
							BEEL_ON;
						}
					}
					HAL_Delay(200);
					BEEL_OFF;
					while(1){
					if(left_led!=0x3f )
						{
							
						break;
							
						}
					}
					while(1){
						if(head_led==0x0f )
						{
							movex=0;							
						}
							
					if(left_led==0x3f )
						{
							
							movey=0;
							BEEL_ON;
						}
					}
					
            return Q_HANDLED(); 
        }
        case Q_EXIT_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
						map.turn[map.idx]=0;//���ת��
            return Q_HANDLED();
        }
        case MID_SIG: {  /* �˳��¼� */
            PRINTF("exit QF_ENTER\n");
           return Q_TRAN(&QF_MID);
        }
				case Get_num_SIG: {  /* ״̬ת�� */
            PRINTF("train to QF_ENTER\n");
            return Q_TRAN(&QF_Get_number);
        }

    }
    return Q_IGNORED();
	}
//QState QF_Find_put(void *me, QEvent const *e);  /* ��������״̬���� */
//QState QF_Find_put2(void *me, QEvent const *e);  /* ��������״̬���� */
//QState QF_Put_end(void *me, QEvent const *e);  /* ��������״̬���� */
//QState QF_Put_end2(void *me, QEvent const *e);  /* ��������״̬���� */
#endif