#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct {
	int Set_temperature;		//期望值
	int Current_temperature;    //当前值
	
	float proportion; 			//比例系数P
	float integral; 			//积分系数I
	float differential; 		//微分系数D
	int T; //采样周期
	
	float error_current; 	        //当前误差:浮点数
	int error_last; 		    //上一步误差
    int error_sum;              //误差累计
	
	float pid_proportion_out;		   //比例项
	float pid_integral_out;		       //积分项
	float pid_differential_out;		   //微分项
	float pid_out; 	                  //PID控制输出
	
}PID;

//PID  *Pid;//存放PID算法需要的数据
//void PID_Init(int SETtemp); 
float pid_control(PID *PP,float current_temp); 
void PWM_CONTROL(float SUM); //占空比计算的
void PWM_Out(u8 m); //按功率输出的

 //struct PID  *PP;
 




#endif
