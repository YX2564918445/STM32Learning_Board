#ifndef __QMC5883_H
#define __QMC5883_H
 
#include "I2C.h"
#include "math.h"
 
#define QMC5883L_ADDRESS  		0x1A
 
 
#define QMC5883L_DATA_READ_X_LSB	0x00
#define QMC5883L_DATA_READ_X_MSB	0x01
#define QMC5883L_DATA_READ_Y_LSB	0x02
#define QMC5883L_DATA_READ_Y_MSB	0x03
#define QMC5883L_DATA_READ_Z_LSB	0x04
#define QMC5883L_DATA_READ_Z_MSB	0x05
#define QMC5883L_TEMP_READ_LSB		0x07
#define QMC5883L_TEMP_READ_MSB		0x08 
#define QMC5883L_STATUS		        0x06 // DOR | OVL | DRDY
#define QMC5883L_CONFIG_1		0x09 // OSR | RNG | ODR | MODE
#define QMC5883L_CONFIG_2		0x0A // SOFT_RST | ROL_PNT | INT_ENB
#define QMC5883L_CONFIG_3		0x0B // SET/RESET Period FBR [7:0]
#define QMC5883L_ID			0x0D
 
#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288f 
#endif 
 
#define QMC5883L_SCALE_FACTOR 		0.732421875f
#define QMC5883L_CONVERT_GAUSS_2G 	12000.0f
#define QMC5883L_CONVERT_GAUSS_8G 	3000.0f
#define QMC5883L_CONVERT_MICROTESLA 	100
#define QMC5883L_DECLINATION_ANGLE	93.67/1000  // radian, Tekirdag/Turkey
 
extern unsigned short QMC_X,QMC_Y,QMC_Z;
extern float QMC_Angel;
void QMC5883_Init(void);
 
void QMC5883_Get_Val(void);
 
 
#endif /*_QMC5883L_H_*/
 
#ifndef QMC5883_H
#define QMC5883L_H
 
#include "I2C.h"
#include "math.h"
 
#define QMC5883L_ADDRESS  		0x1A
 
 
#define QMC5883L_DATA_READ_X_LSB	0x00
#define QMC5883L_DATA_READ_X_MSB	0x01
#define QMC5883L_DATA_READ_Y_LSB	0x02
#define QMC5883L_DATA_READ_Y_MSB	0x03
#define QMC5883L_DATA_READ_Z_LSB	0x04
#define QMC5883L_DATA_READ_Z_MSB	0x05
#define QMC5883L_TEMP_READ_LSB		0x07
#define QMC5883L_TEMP_READ_MSB		0x08 
#define QMC5883L_STATUS		        0x06 // DOR | OVL | DRDY
#define QMC5883L_CONFIG_1		0x09 // OSR | RNG | ODR | MODE
#define QMC5883L_CONFIG_2		0x0A // SOFT_RST | ROL_PNT | INT_ENB
#define QMC5883L_CONFIG_3		0x0B // SET/RESET Period FBR [7:0]
#define QMC5883L_ID			0x0D
 
#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288f 
#endif 
 
#define QMC5883L_SCALE_FACTOR 		0.732421875f
#define QMC5883L_CONVERT_GAUSS_2G 	12000.0f
#define QMC5883L_CONVERT_GAUSS_8G 	3000.0f
#define QMC5883L_CONVERT_MICROTESLA 	100
#define QMC5883L_DECLINATION_ANGLE	93.67/1000  // radian, Tekirdag/Turkey
 
extern unsigned short QMC_X,QMC_Y,QMC_Z;
extern float QMC_Angel;
void QMC5883_Init(void);
 
void QMC5883_Get_Val(void);
 
 
#endif /*_QMC5883L_H_*/
 