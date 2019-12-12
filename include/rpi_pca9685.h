/*
** EPITECH PROJECT, 2019
** UltrasonicReader
** File description:
** ultrasonicio_raspi
*/

#ifndef _RPI_PCA9685_H_
#define _RPI_PCA9685_H_

#include "rtwtypes.h"


void pca9685IOSetup(uint8_T address);
void pca9685IOSetFrequency(uint16_T frequency);
void pca9685IOSetPosition(uint8_T channel, uint16_T A, uint16_T B);
void pca9685IOSetAllPositions(uint16_T A, uint16_T B);


#endif /* !_RPI_PCA9685_H_ */
