#include "pid.h"
#include "timer.h"
#include "usart.h"



	
//void PID_Init(int SETtemp)  //启动时，PID的参数值应该从保存上次值的存储器空间读取出来。在此直接设定
//{
//    Pid->Set_temperature=SETtemp;    //用户设定值
//	Pid->proportion=20;
//	Pid->integral=5000;   //积分系数
//	Pid->differential=1200;   //微分系数
//	Pid->T=1000;    //Pid计算周期（采样周期)
//    Pid->error_current=0.0;
//	Pid->error_last=0;
//	Pid->error_sum=0;
//	Pid->pid_proportion_out=0;
//	Pid->pid_integral_out=0;
//	Pid->pid_differential_out=0;
//	Pid->pid_out=0;
//}	

	//struct PID *PP,
float pid_control( PID *PP,float current_temp){
static float PID_ZL=0.0;
	float result=0.0;
	float A0,A1,A2;
	PP->error_current = PP->Set_temperature - current_temp;
//printf("the PP->error_current is:%.2f\n",PP->error_current);
//printf("\n");
    if(PP->error_current>10)PWM_Out(0);
    else if(PP->error_current<-10)PWM_Out(5);
    else if(PP->error_current>3 && PP->error_current<10)PWM_Out(2);
    else if(PP->error_current>-10 && PP->error_current<-3)PWM_Out(1);
  //  else if(PP->error_current>-20 && PP->error_current<-10)PWM_Out(3);
	/*==============
	增量式PID算法 增量式PID需每次叠加
	================*/
    else if(PP->error_current<3 && PP->error_current>-3){
         A0=PP->proportion * (1+ PP->T/PP->integral +PP->differential/PP->T);
		 A1=-PP->proportion * (1+ 2*PP->differential/PP->T);
		 A2=PP->proportion * (PP->differential/PP->T);
		result =A0 * PP->error_current + A1 * PP->error_last +A2 * PP->error_sum ;
		result += PID_ZL;
		if (result>900){
		       if(PP->error_current>0)PWM_Out(2);
			   else PWM_Out(3);
		}
		else if (result<-5){
		       if(PP->error_current>0)PWM_Out(2);
			   else PWM_Out(3);
		}
		else if (-5<result&&result<5){
		        PWM_Out(4);
		}	
		else if (result>5&&result<900){
			if(PP->error_current>0)PWM_CONTROL(900-result);
			else PWM_CONTROL(-900+result);
		}
		
	}
	
	
	PID_ZL=result;
printf("the PID_ZL is:%.2f\n",PID_ZL);
printf("\n");	
	PP->error_sum=PP->error_last;
    PP->error_last=PP->error_current;
	return result;
}


/*==============
3度以内  PID控制	判断result范围，确定占空比	
=============*/
void PWM_CONTROL(float SUM){
int a=0;
	a=SUM;
//printf("the ccrx<ten is:%d\n",a);
//printf("\n");
if (a>0)TIM_SetCompare3(TIM3,a);
else TIM_SetCompare1(TIM3,-a);
	
}




void PWM_Out(u8 m){

switch(m)
	{
/*=========================================
		tim_ch3--加热    tim_ch1--冷却
		tim_ch2--加热    tim_ch4--冷却
20度差值： 全功：0：加热不冷却  1：冷却不加热
10-20度之间   半功热	
===========================================*/
	 	case 0 :	
		        TIM_SetCompare1(TIM3,900);
		        TIM_SetCompare3(TIM3,0);
		        break;	
		case 1 : 
				
		        TIM_SetCompare1(TIM3,449);
		        TIM_SetCompare3(TIM3,900);
				break;	
		case 2 : 
				TIM_SetCompare1(TIM3,900);
		        TIM_SetCompare3(TIM3,449);
				break;	
		case 3 : 
				TIM_SetCompare1(TIM3,449);
		        TIM_SetCompare3(TIM3,600);
				break;	
        case 4 : //
				TIM_SetCompare1(TIM3,600);
		        TIM_SetCompare3(TIM3,449);
				break;	
        case 5 : 
				TIM_SetCompare1(TIM3,0);
		        TIM_SetCompare3(TIM3,900);
				break;			
		/*
		
		*/
		case 6 : //制热
				TIM_SetCompare2(TIM3,0);
		        TIM_SetCompare4(TIM3,900);
				break;	
		case 7 : //半工制热
				TIM_SetCompare2(TIM3,449);
		        TIM_SetCompare4(TIM3,900);
				break;
		case 8 : //半工热、半工冷
				TIM_SetCompare2(TIM3,449);
		        TIM_SetCompare4(TIM3,449);
				break;	
		
	}	

}


