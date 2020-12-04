/*
 * spi.h
 *
 *  Created on: 28 ago. 2019
 *      Author: Larraitz
 */

/*!
 * SPI peripheral ID
 */


void SPI_Init();
void SPI_DeInit(SPI_HandleTypeDef* hspi);
void SPI1_Init(void);
void SPI2_Init(void);
void SPI4_Init(void);
