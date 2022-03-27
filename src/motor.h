#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>




#define motor_L_F_0 32
#define motor_L_F_0_PWM 0

#define motor_L_F_1 33
#define motor_L_F_1_PWM 1

#define motor_R_F_0 25
#define motor_R_F_0_PWM 2

#define motor_R_F_1 26
#define motor_R_F_1_PWM 3

#define motor_L_B_0 27
#define motor_L_B_0_PWM 4

#define motor_L_B_1 14
#define motor_L_B_1_PWM 5

#define motor_R_B_0 12
#define motor_R_B_0_PWM 6

#define motor_R_B_1 13
#define motor_R_B_1_PWM 7






class MOTOR_DERIVER
{
    public:
        uint8_t speed;
        MOTOR_DERIVER(uint8_t);

        void move_forward();
        void move_back();

        void move_left();
        void move_right();

        void charge_speed(uint8_t);

        private:
           uint8_t freq = 2000;    //设置PWM波的频率
            uint8_t channel = 0;    //设置通道,共16个通道，0~15
           uint8_t resolution = 10; //分辨率，取值0~20 duty的最大值为 2^resolution-1
            bool val = 0;
            uint8_t direction = 0;//方向，0表示停止，1表示向前，2表示向后,3表示向左，4表示向右


};
#endif