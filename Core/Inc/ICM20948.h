/*
 * ICM20948.h
 *
 *  Created on: Oct 3, 2022
 *      Author: zeroi
 */

#ifndef INC_ICM20948_H_
#define INC_ICM20948_H_

#include"i2c.h"

#define ICM20948_DISABLE_SENSORS 0x3F
#define ICM20948_ENABLE_SENSORS 0x00
#define ICM20948_BIT_H_RESET 0x80
#define ICM20948_BTT_CLK_PLL 0x01
#define ICM20948_BIT_INT_ACTL 0x80
#define ICM20948_BIT_INT_OPEN 0x40

class ICM20948 {
public:
	ICM20948(){};
	virtual ~ICM20948(){};

	enum class AccelSensitivity: uint8_t{
		SENS_2G,
		SENS_4G,
		SENS_8G,
		SENS_16G,
	};

	enum class GyroSensitivity: uint8_t{
		SENS_2G,
		SENS_4G,
		SENS_8G,
		SENS_16G,
	};

	const struct{
		uint8_t WHO_AM_I = 0x00;
		uint8_t PWR_MGMT_1 = 0x06;
		uint8_t PWR_MGMT_2 = 0x07;
		uint8_t INT_PIN_CFG = 0x0F;
		uint8_t BANK_SEL = 0x7F;
		uint8_t ACCEL_CONFIG = 0x14;
		uint8_t GYRO_CONFIG = 0x01;
		uint8_t INT_ENABLE = 0x11;

	}REGISTER;

	uint8_t whoami(){
	    uint8_t adress=0;
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, REGISTER.WHO_AM_I, 1, &adress, 1, 1000);
		return adress;

	}

	uint8_t pwrmgmt1(uint8_t data){
	     HAL_I2C_Mem_Write(&hi2c1, 0x68<<1,REGISTER.PWR_MGMT_1,1,&data,1,1000);
	     return 0;
	}
	uint8_t pwrmgmt2(uint8_t data){
	     HAL_I2C_Mem_Write(&hi2c1, 0x68<<1,REGISTER.PWR_MGMT_2,1,&data,1,1000);
	     return 0;
	}
	void reset(){
	    pwrmgmt1(ICM20948_BIT_H_RESET);
	    HAL_Delay(100);
	    pwrmgmt1(ICM20948_BTT_CLK_PLL);
	    HAL_Delay(100);
	    uint8_t buffer=ICM20948_BIT_INT_ACTL | ICM20948_BIT_INT_OPEN;
	    HAL_I2C_Mem_Write(&hi2c1, 0x68<<1,REGISTER.INT_PIN_CFG,1,&buffer,1,1000);
		}
	bool changeuserbank(const uint8_t bank){
	    if(bank>3 || bank<0){
	        return false;
	    }
	    uint8_t data=bank<<4;
	    uint8_t res = HAL_I2C_Mem_Write(&hi2c1, 0x68<<1,REGISTER.BANK_SEL,1,&data,1,1000);
	    //printf("data=0x%02x\n",data);
	    //printf("res=%d\n",res);
	    return true;
	}

	bool accelconfig(const AccelSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF){

	    _accelsensitivity=fssel;
	    if(configDLPF>7 || configDLPF<0){
	        return false;
	    }
	    uint8_t data=0;
	    data |=configDLPF<<3;
	    data |=(uint8_t)fssel<<1;
	    data |=enableDLPF;
	    changeuserbank(2);

	    data=HAL_I2C_Mem_Write(&hi2c1, 0x68<<1,REGISTER.ACCEL_CONFIG,1,&data,1,1000);
	    //printf("data=%d\n",data);
	    changeuserbank(0);
	    return true;

	}
	bool gyroconfig(const GyroSensitivity fssel,const bool enableDLPF,const uint8_t configDLPF){

		_gyrosensitivity=fssel;
		if(configDLPF>7||configDLPF<0){
			return false;
		}
		uint8_t data=0;
		data |=configDLPF<<3;
		data |=(uint8_t)fssel<<1;
		data |=enableDLPF;
		changeuserbank(2);

		data=HAL_I2C_Mem_Write(&hi2c1,0x68<<1,REGISTER.GYRO_CONFIG,1,&data,1,1000);
		changeuserbank(0);
		return true;

	}
	float getaccel(uint8_t axis){
	    const uint8_t REG_ACCEL_H[3]={0x2D,0x2F,0x31};
	    const uint8_t REG_ACCEL_L[3]={0x2E,0x30,0x32};

	    int8_t accelH;
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1,REG_ACCEL_H[axis],1,(uint8_t*)&accelH,1,1000);
	    int8_t accelL;
	    HAL_I2C_Mem_Read(&hi2c1, 0x68<<1,REG_ACCEL_L[axis],1,(uint8_t*)&accelL,1,1000);
	    int16_t accel=(int16_t)accelH<<8 | (int16_t)accelL;

	    return (float)accel/ACCEL_SENSITIVITY[(uint8_t)_accelsensitivity];
	}

	float getgyro(uint8_t axis){
		const uint8_t REG_GYRO_H[3]={0x33,0x35,0x37};
		const uint8_t REG_GYRO_L[3]={0x34,0x36,0x38};

		int8_t gyroH;
		HAL_I2C_Mem_Read(&hi2c1,0x68<<1,REG_GYRO_H[axis],1,(uint8_t*)&gyroH,1,1000);
		int8_t gyroL;
		HAL_I2C_Mem_Read(&hi2c1,0x68<<1,REG_GYRO_L[axis],1,(uint8_t*)&gyroL,1,1000);
		int16_t gyro=(int16_t)gyroH<<8 | (int16_t)gyroL;

		return (float)gyro/GYRO_SENSITIVITY[(uint8_t)_gyrosensitivity];
	}

	void pinconfig(uint8_t value){
		HAL_I2C_Mem_Write(&hi2c1,0x68<<1,REGISTER.INT_PIN_CFG,1,&value,1,1000);

	}

	void intenable(uint8_t value=1){
		HAL_I2C_Mem_Write(&hi2c1,0x68<<1,REGISTER.INT_ENABLE,1,&value,1,1000);
	}


private:
	AccelSensitivity _accelsensitivity;
	GyroSensitivity _gyrosensitivity;
	const float ACCEL_SENSITIVITY[4]={16384.0,8192.0,4096.0,2048.0};
	const float GYRO_SENSITIVITY[4]={131,65.5,32.8,16.4};


};
#endif /* INC_ICM20948_H_ */
