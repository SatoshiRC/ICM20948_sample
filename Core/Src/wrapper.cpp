#include "wrapper.hpp"
#include "i2c.h"
#include "ICM20948.h"
#include "usart.h"
#include <string>
#include "tim.h"

/* Pre-Processor Begin */

/* Pre-Processor End */

/* Enum, Struct Begin */

/* Enum, Struct End */

/* Function Prototype Begin */



/* Function Prototype End */

/* Variable Begin */
ICM20948 icm20948(&hi2c1,ICM20948::Address::LOW);
float anglez=0;
uint64_t timer=0;
uint16_t preCounter=0;
uint16_t counter=0;
uint16_t stepTime = 0;

/* Variable End */

void init(void){
    HAL_TIM_Base_Start(&htim1);
	if(icm20948.whoami() == 0xea){
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	}
	int result = icm20948.whoami();
//		    result=whoami();
		    if(result==0xea){
//		        printf("ICM20948 confirm\n");
		    	icm20948.reset();
		    	icm20948.pwrmgmt2(ICM20948_DISABLE_SENSORS);
		    	icm20948.accelConfig(ICM20948::AccelSensitivity::SENS_2G,false,0);
		    	icm20948.pwrmgmt2(ICM20948_ENABLE_SENSORS);
		    	icm20948.intPinConfig(0b01110000);
		    	icm20948.intenable();

		        HAL_Delay(100);
//		        printf("initialized\n");

		        icm20948.changeUserBank(2);
		        uint8_t buffer2=0;
		        //HAL_I2C_Mem_Read(&hi2c1, 0x68<<1,ICM20948::REGISTER.ACCEL_CONFIG,1,&buffer2,1,1000);
		        icm20948.changeUserBank(0);
		    }


}

void loop(void){
//	HAL_Delay(50);
//	{
//	float accelx = icm20948.getaccel(0);
//	int16_t num = ((int16_t)(accelx*10))/10;
//	std::string str;
//	str = std::to_string(num);
//	num = (uint16_t)(accelx*1000)%1000;
//	str += "."+std::to_string(num)+"\r\n";
//	HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(), str.size(), 100);
//	}
//	float gyrox = icm20948.getgyro(0);
//	int16_t num = ((int16_t)(gyrox*10))/10;
//	std::string str;
//	str = std::to_string(num);
//	num = (uint16_t)(gyrox*1000)%1000;
//	str += "."+std::to_string(num)+"\r\n";
//	HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(), str.size(), 100);

//	           printf("0x%x\n",whoami());
//	           printf("accel x=%f\t",getaccel(0));
//	           //printf("I am %x\n",whoami(fd,ICM20948_REG_WHO_AM_I));
//	           printf("accel y=%f\t",getaccel(1));
//	           printf("accel z=%f\n",getaccel(2));
//	           delay(500);

//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11));
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);

}

float gyroz;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	if (GPIO_Pin == GPIO_PIN_11){
		stepTime = 0;
		{uint8_t flag=__HAL_TIM_GET_FLAG(&htim1,TIM_FLAG_UPDATE);
		  counter=__HAL_TIM_GET_COUNTER(&htim1);
		  if(flag==0){
			  stepTime=counter-preCounter;
		  }else{
			  stepTime=counter+__HAL_TIM_GET_AUTORELOAD(&htim1)-preCounter;
			  __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_UPDATE);
		  }
		  preCounter=counter;
		  timer+=stepTime;
		}
		{
//			float accelx = icm20948.getaccel(0);
//			int16_t num = ((int16_t)(accelx*10))/10;
//			std::string str;
//			str = std::to_string(num);
//			num = (uint16_t)(accelx*1000)%1000;
//			str += "."+std::to_string(num)+"\r\n";
//			HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(), str.size(), 100);
		}

//		float gyroz = icm20948.getgyro(0);
		gyroz = icm20948.getGyro(0);

		anglez+=gyroz*stepTime/1000000.0;

//		int16_t num = ((int16_t)(anglez*10))/10;
		int16_t num = stepTime;
		std::string str;
		str = std::to_string(num);
		num = (uint16_t)(anglez*1000)%1000;
		str += "."+std::to_string(num)+"\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(), str.size(), 100);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		uint8_t n=0;
		n=0;
//		HAL_I2C_Mem_Write(&hi2c1, 0x68<<1, 0x1A, 1, &n, 1, 100);
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x1a, 1, &n, 1, 100);
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, 0x11, 1, &n, 1, 100);
				str = std::to_string(n);
				str += "\r\n";
				HAL_UART_Transmit(&huart2, (uint8_t *)str.c_str(), str.size(), 100);
	}
}


/* Function Body End */
