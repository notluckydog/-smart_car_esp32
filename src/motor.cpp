#include "motor.h"


//初始化，设置初始速度为50%
MOTOR_DERIVER::MOTOR_DERIVER(uint8_t speed){

    speed=50;
    ledcSetup(motor_L_F_0_PWM,speed,resolution);   //设置通道0
    ledcAttachPin(motor_L_F_0,motor_L_F_0_PWM); //将通道0和gpio_pin连接起来
   
    ledcSetup(motor_L_F_1_PWM,speed,resolution);   //设置通道1
    ledcAttachPin(motor_L_F_1,motor_L_F_1_PWM); //将通道0和gpio_pin连接起来
   
    ledcSetup(motor_R_F_0_PWM,speed,resolution);   //设置通道2
    ledcAttachPin(motor_R_F_0,motor_R_F_0_PWM); //将通道0和gpio_pin连接起来
   
   ledcSetup(motor_R_F_1_PWM,speed,resolution);   //设置通道3
    ledcAttachPin(motor_R_F_1,motor_R_F_1_PWM); //将通道0和gpio_pin连接起来
   
   ledcSetup(motor_L_B_0_PWM,speed,resolution);   //设置通道4
    ledcAttachPin(motor_L_B_0,motor_L_B_0_PWM); //将通道0和gpio_pin连接起来
   
   ledcSetup(motor_L_B_1_PWM,speed,resolution);   //设置通道0
    ledcAttachPin(motor_L_B_1,motor_L_B_1_PWM); //将通道0和gpio_pin连接起来
   
   ledcSetup(motor_R_B_0_PWM,speed,resolution);   //设置通道0
    ledcAttachPin(motor_R_B_0,motor_R_B_0_PWM); //将通道0和gpio_pin连接起来
   
   ledcSetup(motor_R_B_1_PWM,speed,resolution);   //设置通道0
    ledcAttachPin(motor_R_B_1,motor_R_B_1_PWM); //将通道0和gpio_pin连接起来
   
}


//打印PWM信息

//前进
void MOTOR_DERIVER::move_forward(){
    ledcWrite(motor_L_F_0_PWM,1000);
    ledcWrite(motor_L_F_1_PWM,0);
    ledcWrite(motor_R_F_0_PWM,1000);
    ledcWrite(motor_R_F_1_PWM,0);
    /*ledcWrite(motor_L_B_0_PWM,speed);
    ledcWrite(motor_L_B_1_PWM,0);
    ledcWrite(motor_R_B_1_PWM,speed);
    ledcWrite(motor_R_B_0_PWM,0);
    direction=1;*/

    Serial.println("movforward");
    
}


//后退
void MOTOR_DERIVER::move_back(){
    ledcWrite(motor_L_F_0_PWM,speed);
    ledcWrite(motor_L_F_1_PWM,0);
    ledcWrite(motor_R_F_0_PWM,speed);
    ledcWrite(motor_R_F_1_PWM,0);
    /*ledcWrite(motor_L_B_0_PWM,0);
    ledcWrite(motor_L_B_1_PWM,speed);
    ledcWrite(motor_R_B_1_PWM,0);
    ledcWrite(motor_R_B_0_PWM,speed);
    direction=2;*/

    Serial.println("moveback");
    
}


//左转   微测试
void MOTOR_DERIVER::move_left(){
    ledcWrite(motor_L_F_0_PWM,speed);
    ledcWrite(motor_L_F_1_PWM,0);
    ledcWrite(motor_R_F_0_PWM,speed);
    ledcWrite(motor_R_F_1_PWM,0);
    ledcWrite(motor_L_B_0_PWM,0);
    ledcWrite(motor_L_B_1_PWM,speed);
    ledcWrite(motor_R_B_1_PWM,0);
    ledcWrite(motor_R_B_0_PWM,speed);
    direction=3;
    
}

//右转   微测试
void MOTOR_DERIVER::move_right(){
    ledcWrite(motor_L_F_0_PWM,speed);
    ledcWrite(motor_L_F_1_PWM,0);
    ledcWrite(motor_R_F_0_PWM,speed);
    ledcWrite(motor_R_F_1_PWM,0);
    ledcWrite(motor_L_B_0_PWM,0);
    ledcWrite(motor_L_B_1_PWM,speed);
    ledcWrite(motor_R_B_1_PWM,0);
    ledcWrite(motor_R_B_0_PWM,speed);
    direction=4;
    
}

//改变速度 未测试
/*void charge_speed(uint8_t motor_speed){
    speed=motor_speed;

    if(direction=1) move_forward();
    if(direction=2) move_back();
    if(direction=3) move_left();
    if(direction=4) move_right();
} */

