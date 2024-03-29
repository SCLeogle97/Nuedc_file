#include "stdio.h"


struct _pid{
    float SetSpeed;            //定义设定值
    float ActualSpeed;        //定义实际值
    float err;                //定义偏差值
    float err_next;            //定义上一个偏差值
    float err_last;            //定义最上前的偏差值
    float Kp,Ki,Kd;            //定义比例、积分、微分系数
    float umax;
    float umin;
}pid;

void PID_init(){
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.err_next=0.0;
    pid.Kp=0.2;
    pid.Ki=0.1;
    pid.Kd=0.2;
    pid.umax=400;
    pid.umin=-200;
}

float PID_realize(float speed){
	int index;
    pid.SetSpeed=speed;
    pid.err=pid.SetSpeed-pid.ActualSpeed;
    if(pid.ActualSpeed>pid.umax)  //灰色底色表示抗积分饱和的实现
    {

       if(abs(pid.err)>200)      //积分分离
        {
            index=0;
        }else{
            index=1;
            if(pid.err<0)
            {
              pid.integral+=pid.err;
            }
        }
    }
	else if(pid.ActualSpeed<pid.umin){
        if(abs(pid.err)>200)      //积分分离过程
        {
            index=0;
        }else{
            index=1;
            if(pid.err>0)
            {
            pid.integral+=pid.err;
            }
        }
    }
	else{
        if(abs(pid.err)>200)                    //积分分离过程
        {
            index=0;
        }else{
            index=1;
            pid.integral+=pid.err;
        }
    }

    pid.voltage=pid.Kp*pid.err+index*pid.Ki*pid.integral/2+pid.Kd*(pid.err-pid.err_last);
    pid.err_last=pid.err;
    pid.ActualSpeed=pid.voltage*1.0;
    return pid.ActualSpeed;

}



int main()
{
    PID_init();
    int count=0;
    while(count<1000)
    {
        float speed=PID_realize(200.0);
        printf("%f\n",speed);
        count++;
    }
    return 0;
}
