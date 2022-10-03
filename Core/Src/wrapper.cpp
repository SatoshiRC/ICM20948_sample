#include "wrapper.hpp"
#include "i2c.h"
#include "ICM20948.h"

/* Pre-Processor Begin */

/* Pre-Processor End */

/* Enum, Struct Begin */

/* Enum, Struct End */

/* Function Prototype Begin */
int whoami();
void pwrmgmt1(int data);
void pwrmgmt2(int data);
void reset();
bool changeuserbank(const uint8_t bank);
bool accelconfig(const uint8_t fssel,const bool enableDLPF,const uint8_t configDLPF);
float getaccel( int axis);
bool gyroconfig(const uint8_t fssel,const bool enableDLPF,const uint8_t configDLPF);
float getgyro(int axis);


/* Function Prototype End */

/* Variable Begin */
ICM20948 icm20948;
/* Variable End */

void init(void){
	if(icm20948.whoami() == 0xea){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	}
}

void loop(void){

}

/* Function Body Begin */
//int whoami(){
//    uint8_t adress=0;
//	HAL_I2C_Mem_Read(&hi2c1, 0xea<<1, MemAddress, 1, &adress, 1, 1000);
//	return wiringPiI2CReadReg8(ICM20948_REG_WHO_AM_I);
//}
//void pwrmgmt1(int data){
//     wiringPiI2CWriteReg8(ICM20948_REG_PWR_MGMT_1,data);
//}
//void pwrmgmt2(int data){
//     wiringPiI2CWriteReg8(ICM20948_REG_PWR_MGMT_2,data);
//}
//void reset(){
//    pwrmgmt1(ICM20948_BIT_H_RESET);
//    delay(100);
//    pwrmgmt1(ICM20948_BTT_CLK_PLL);
//    delay(100);
//    wiringPiI2CWriteReg8(ICM20948_REG_INT_PIN_CFG,ICM20948_BIT_INT_ACTL | ICM20948_BIT_INT_OPEN);
//
//
//}
//bool changeuserbank(const uint8_t bank){
//    if(bank>3 || bank<0){
//        return false;
//    }
//    printf("1\n");
//    uint8_t data=bank<<4;
//    printf("2\n");
//    int res = wiringPiI2CWriteReg8(ICM20948_REG_BANK_SEL,data);
//    printf("data=0x%02x\n",data);
//    printf("3\n");
//    printf("res=%d\n",res);
//    return true;
//}
//bool accelconfig(const uint8_t fssel,const bool enableDLPF,const uint8_t configDLPF){
//    printf("accelconfigstart\n");
//    if(fssel>3 || fssel<0){
//        return false;
//    }
//    //printf("1\n");
//    mAccelFSSel=fssel;
//    if(configDLPF>7 || configDLPF<0){
//        return false;
//    }
//    //printf("2\n");
//    uint8_t data=0;
//    data |=configDLPF<<3;
//    data |=fssel<<1;
//    data |=enableDLPF;
//    changeuserbank(2);
//    //printf("3\n");
//    data=wiringPiI2CWriteReg8(ICM20948_REG_ACCEL_CONFIG,data);
//    //printf("4\n");
//    printf("data=%d\n",data);
//    //printf("5\n");
//    changeuserbank(0);
//    return true;
//
//}
//float getaccel(int fd, int axis){
//    const uint8_t REG_ACCEL_H[3]={0x2D,0x2F,0x31};
//    const uint8_t REG_ACCEL_L[3]={0x2E,0x30,0x32};
//
//    int8_t accelH=wiringPiI2CReadReg8(REG_ACCEL_H[axis]);
//    int8_t accelL=wiringPiI2CReadReg8(REG_ACCEL_L[axis]);
//    int16_t accel=(int16_t)accelH<<8 | (int16_t)accelL;
////     printf("accel=%f\n",accel);
////     printf("ACCEL_SENSITIVITY=%f",ACCEL_SENSITIVITY[mAccelFSSel])
////     printf("accelH=%f\n",accelH);
////     printf("accelL=%f\n",accelL);
////    printf("reg accelH=%f\n",REG_ACCEL_H[axis]);
//    return (float)accel/ACCEL_SENSITIVITY[mAccelFSSel];
//}
//
//bool gyroconfig(const uint8_t fssel,const bool enableDLPF,const uint8_t configDLPF){
//    if(fssel>3 || fssel<0){
//        return false;
//    }
//    mGyroFSSel=fssel;
//    if(configDLPF>7 || configDLPF<0){
//        return false;
//    }
//    uint8_t data=0;
//    data |=configDLPF<<3;
//    data |=fssel<<1;
//    data |=enableDLPF;
//    changeuserbank(2);
//    wiringPiI2CWriteReg8(ICM20948_REG_GYRO_CONFIG,data);
//    changeuserbank(0);
//    return true;
//}
//
//float getgyro(int axis){
//    const uint8_t REG_GYRO_H[3]={0x33,0x35,0x37};
//    const uint8_t REG_GYRO_L[3]={0x34,0x36,0x38};
//
//    int8_t gyroH=wiringPiI2CReadReg8(REG_GYRO_H[axis]);
//    int8_t gyroL=wiringPiI2CReadReg8(REG_GYRO_L[axis]);
//    int16_t gyro=(int16_t)gyroH<<8 | (int16_t)gyroL;
//
//    return (float)gyro/GYRO_SENSITIVITY[mGyroFSSel];
//}
//
//

/* Function Body End */
