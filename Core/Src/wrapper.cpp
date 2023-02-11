#include "ICM20948/ICM20948_USER.h"
#include "wrapper.hpp"
#include "i2c.h"
#include "usart.h"
#include <string>
#include "tim.h"
#include "TIM_stopwatch/TIMStopWatch.h"
#include <array>
#include <cmath>

/* Pre-Processor Begin */

/* Pre-Processor End */

/* Enum, Struct Begin */
//typedef std::array<float,4> quaternion;
typedef float quaternion[4];
/* Enum, Struct End */

/* Function Prototype Begin */



/* Function Prototype End */

/* Variable Begin */
ICM20948_USER icm20948(&hi2c1,ICM20948::Address::LOW);
float anglez=0;
int timer=0;
float preTime=0;
float time=0;
float stepTime = 0;
TIM_StopWatch stopwatch(&htim1);

std::array<float,3> accelValue;
std::array<float,3> gyroValue;
quaternion preAttitude={1};
quaternion attitude={1};

float roll;
float pitch;
float yaw;

quaternion qDotOmega={};
std::array<float,3> f;
std::array<std::array<float,3>,4> j;
quaternion qDotEpsilon={};
float fSize=0;
quaternion qDot;
const float beta=std::sqrt(3/4.0)*M_PI*(5.0/180.0);

float gyroX;
float gyroY;
float gyroZ;

/* Variable End */

void init(void){
   stopwatch.start();
   icm20948.init();
}

void loop(void){
stopwatch.update();
}

float gyro;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	if (GPIO_Pin == GPIO_PIN_11){

		//following variables are used for madgwick filter
//		quaternion qDotOmega={};
//		std::array<float,3> f;
//		std::array<std::array<float,3>,4> j;
//		quaternion qDotEpsilon={};
//		float fSize=0;
//		quaternion qDot;
//		const float beta=std::sqrt(3/4.0)*M_PI*(5.0/180.0);


		preTime=time;
		stopwatch.update();
		time=stopwatch.getTimeMS();
		stepTime=time-preTime;

		icm20948.get6ValueBurst(accelValue, gyroValue);
		float accelSize=std::sqrt(std::pow(accelValue[0],2)+std::pow(accelValue[1],2)+std::pow(accelValue[2],2));
		if(accelSize!=0){
		accelValue[0]/=accelSize;
		accelValue[1]/=accelSize;
		accelValue[2]/=accelSize;
		}

		for(auto& it:gyroValue){
			it *= M_PI/180.0;
		}
		gyroX=gyroValue[0];
		gyroY=gyroValue[1];
		gyroZ=gyroValue[2];

		qDotOmega[0]=1/2.0*(-preAttitude[1]*gyroValue[0]-preAttitude[2]*gyroValue[1]-preAttitude[3]*gyroValue[2]);
		qDotOmega[1]=1/2.0*(preAttitude[0]*gyroValue[0]+preAttitude[2]*gyroValue[2]-preAttitude[3]*gyroValue[1]);
		qDotOmega[2]=1/2.0*(preAttitude[0]*gyroValue[1]-preAttitude[1]*gyroValue[2]+preAttitude[3]*gyroValue[0]);
		qDotOmega[3]=1/2.0*(preAttitude[0]*gyroValue[2]+preAttitude[1]*gyroValue[1]-preAttitude[2]*gyroValue[0]);

		f[0]=2*(preAttitude[1]*preAttitude[3]-preAttitude[0]*preAttitude[2])-accelValue[0];
		f[1]=2*(preAttitude[0]*preAttitude[1]+preAttitude[2]*preAttitude[3]-accelValue[1]);
		f[2]=2*(1/2.0-std::pow(preAttitude[1],2)-std::pow(preAttitude[2],2))-accelValue[2];

		j[0]=  {-2*preAttitude[2], 2*preAttitude[1],0};
		j[1]=  {2*preAttitude[3],2*preAttitude[0],-4*preAttitude[1]};
		j[2]=  {-2*preAttitude[0],2*preAttitude[3],-4*preAttitude[2]};
		j[3]=  {2*preAttitude[1],2*preAttitude[2],0};

		 for(uint8_t n=0;n<4;n++){
			 qDotEpsilon[n]=0;
			 for(uint8_t m=0;m<3;m++){
				 qDotEpsilon[n]+= j[n][m]*f[m];
			 }
		 }

		 fSize=std::sqrt(std::pow(qDotEpsilon[0],2)+std::pow(qDotEpsilon[1],2)+std::pow(qDotEpsilon[2],2)+std::pow(qDotEpsilon[3],2));

		 for(uint8_t n=0;n<4;n++){
			 if(fSize==0){
				 qDot[n]=qDotOmega[n];
			 }else{
				 qDot[n]=qDotOmega[n]-beta*qDotEpsilon[n]/fSize;
			 }

			 attitude[n]=preAttitude[n]+qDot[n]*stepTime/1000.0;
			 preAttitude[n]=attitude[n];
		 }


		 float attitudeSize=std::sqrt(std::pow(attitude[0],2)+std::pow(attitude[1],2)+std::pow(attitude[2],2)+std::pow(attitude[3],2));
		 attitude[0]/=attitudeSize;
		 attitude[1]/=attitudeSize;
		 attitude[2]/=attitudeSize;
		 attitude[3]/=attitudeSize;

		 roll=std::atan2(2*(attitude[0]*attitude[1]+attitude[2]*attitude[3])
			 ,std::pow(attitude[0],2)-std::pow(attitude[1],2)-std::pow(attitude[2],2)+std::pow(attitude[3],2));

		 pitch=std::asin(2*(attitude[0]*attitude[2]-attitude[1]*attitude[3]));

		 yaw=std::atan2(2*(attitude[0]*attitude[3]+attitude[1]*attitude[2])
			 ,std::pow(attitude[0],2)+std::pow(attitude[1],2)-std::pow(attitude[2],2)-std::pow(attitude[3],2));

		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		uint8_t n=0;
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x1a, 1, &n, 1, 100);
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x11, 1, &n, 1, 100);
	}
}


/* Function Body End */
