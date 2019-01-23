#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct {
	int Set_temperature;		//����ֵ
	int Current_temperature;    //��ǰֵ
	
	float proportion; 			//����ϵ��P
	float integral; 			//����ϵ��I
	float differential; 		//΢��ϵ��D
	int T; //��������
	
	float error_current; 	        //��ǰ���:������
	int error_last; 		    //��һ�����
    int error_sum;              //����ۼ�
	
	float pid_proportion_out;		   //������
	float pid_integral_out;		       //������
	float pid_differential_out;		   //΢����
	float pid_out; 	                  //PID�������
	
}PID;

//PID  *Pid;//���PID�㷨��Ҫ������
//void PID_Init(int SETtemp); 
float pid_control(PID *PP,float current_temp); 
void PWM_CONTROL(float SUM); //ռ�ձȼ����
void PWM_Out(u8 m); //�����������

 //struct PID  *PP;
 




#endif
