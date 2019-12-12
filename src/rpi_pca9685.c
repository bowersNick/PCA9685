#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <sys/time.h>
#include <time.h>
#include "rpi_pca9685.h"

#define MAX_CHANNELS 16

// Register Define
#define MODE1           (uint8_T)0
#define MODE2           (uint8_T)1
#define ALLCALLADR      (uint8_T)5
#define LED0_ON_L       (uint8_T)6
#define LED0_ON_H       (uint8_T)7
#define LED0_OFF_L      (uint8_T)8
#define LED0_OFF_H      (uint8_T)9
#define ALL_LED_ON_L    (uint8_T)250
#define ALL_LED_ON_H    (uint8_T)251
#define ALL_LED_OFF_L   (uint8_T)252
#define ALL_LED_OFF_H   (uint8_T)253
#define PRE_SCALE       (uint8_T)254

// MODE1 bits
#define ALLCALL     0x01
#define SUB3        0x02
#define SUB2        0x04
#define SUB1        0x08
#define SLEEP       0x10
#define AI          0x20
#define EXTCLK      0x40
#define RESTART     0x80

// MODE2 bits
#define OUTNE       0x03
#define OUTDRV      0x04
#define OCH         0x08
#define INVRT       0x10

#define MAX_RETRIES     5

boolean_T initialized_pca = false;
static int pca_fd = -1;

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void pca9685IOSetup(uint8_T address)
{
    uint8_T old_mode;
    uint8_T count = 0;
    // Perform one-time wiringPi initialization
    if (!initialized_pca)
    {
        do
        {
            sleep_ms(50);
            pca_fd = wiringPiI2CSetup(address);
            if (pca_fd != -1)
            {
                pca9685IOSetAllPositions(0,0);
                wiringPiI2CWriteReg8(pca_fd, MODE2, OUTDRV);
                wiringPiI2CWriteReg8(pca_fd, MODE1, ALLCALL);
                sleep_ms(5);
                old_mode = wiringPiI2CReadReg8(pca_fd, MODE1);
                old_mode = old_mode & ~SLEEP;
                wiringPiI2CWriteReg8(pca_fd, MODE1, old_mode);
                sleep_ms(5);
                initialized_pca = true;
            } 
            else
                count++;
        }
        while (pca_fd < 0 && count < MAX_RETRIES);        
    } 

}

void pca9685IOSetFrequency(uint16_T frequency)
{
    uint8_T old_mode, new_mode;
    float prescaleval;
    int prescale;
    
    prescaleval = 25000000.0;    //# 25MHz
    prescaleval /= 4096.0;       //# 12-bit
    prescaleval /= (float)frequency;
    prescaleval -= 1.0;
    prescale = (int)(floor(prescaleval + 0.5));
    old_mode = wiringPiI2CReadReg8(pca_fd, MODE1);
    new_mode = ((old_mode & 0x7F) | SLEEP);
    wiringPiI2CWriteReg8(pca_fd, MODE1, new_mode);

    wiringPiI2CWriteReg8(pca_fd, PRE_SCALE, prescale);
    wiringPiI2CWriteReg8(pca_fd, MODE1, old_mode);
    sleep_ms(5);
    wiringPiI2CWriteReg8(pca_fd, MODE1, old_mode | RESTART);

}

void pca9685IOSetPosition(uint8_T channel, uint16_T A, uint16_T B)
{
    wiringPiI2CWriteReg8(pca_fd, (LED0_ON_L + (channel * 4)), (uint8_T)(A & 0xFF));
    wiringPiI2CWriteReg8(pca_fd, (LED0_ON_H + (channel * 4)), (uint8_T)(A >> 8));
    wiringPiI2CWriteReg8(pca_fd, (LED0_OFF_L + (channel * 4)), (uint8_T)(B & 0xFF));
    wiringPiI2CWriteReg8(pca_fd, (LED0_OFF_H + (channel * 4)), (uint8_T)(B >> 8));
}

void pca9685IOSetAllPositions(uint16_T A, uint16_T B)
{
    wiringPiI2CWriteReg8(pca_fd, ALL_LED_ON_L, (uint8_T)(A & 0xFF));
    wiringPiI2CWriteReg8(pca_fd, ALL_LED_ON_H, (uint8_T)(A >> 8));
    wiringPiI2CWriteReg8(pca_fd, ALL_LED_OFF_L, (uint8_T)(B & 0xFF));
    wiringPiI2CWriteReg8(pca_fd, ALL_LED_OFF_H, (uint8_T)(B >> 8));
}



        
	

       
	

