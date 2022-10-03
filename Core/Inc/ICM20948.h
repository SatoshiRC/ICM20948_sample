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
	ICM20948();
	virtual ~ICM20948();

	const struct{
		uint8_t WHO_AM_I = 0x00;
		uint8_t PWR_MGMT_1 = 0x06;
		uint8_t PWR_MGMT_2 = 0x07;
		uint8_t INT_PIN_CFG = 0x0F;
		uint8_t BANK_SEL = 0x7F;
		uint8_t ACCEL_CONFIG = 0x14;
		uint8_t GYRO_CONFIG = 0x01;
	}REGISTER;

	uint8_t whoami(){
	    uint8_t adress=0;
		HAL_I2C_Mem_Read(&hi2c1, 0x68<<1, REGISTER.WHO_AM_I, 1, &adress, 1, 1000);
		return adress;
	}
};

#endif /* INC_ICM20948_H_ */
