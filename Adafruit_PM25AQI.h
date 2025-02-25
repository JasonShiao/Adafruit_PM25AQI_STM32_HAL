/*!
 * @file Adafruit_PM25AQI.h
 *
 * This is the documentation for Adafruit's PM25 AQI driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit PM25 air quality sensors: http://www.adafruit.com/products/4632
 *
 * These sensors use I2C or UART to communicate.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Ladyada for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_PM25AQI_H
#define ADAFRUIT_PM25AQI_H

#include "stm32f4xx_hal.h"

// the i2c address (left-shifted by 1 bit for STM32 HAL api)
#define PMSA003I_I2CADDR_DEFAULT (0x12 << 1) ///< PMSA003I has only one I2C address

/**! Structure holding Plantower's standard packet **/
typedef struct PMSAQIdata {
  uint16_t framelen;       ///< How long this data chunk is
  uint16_t pm10_standard,  ///< Standard PM1.0
      pm25_standard,       ///< Standard PM2.5
      pm100_standard;      ///< Standard PM10.0
  uint16_t pm10_env,       ///< Environmental PM1.0
      pm25_env,            ///< Environmental PM2.5
      pm100_env;           ///< Environmental PM10.0
  uint16_t particles_03um, ///< 0.3um Particle Count
      particles_05um,      ///< 0.5um Particle Count
      particles_10um,      ///< 1.0um Particle Count
      particles_25um,      ///< 2.5um Particle Count
      particles_50um,      ///< 5.0um Particle Count
      particles_100um;     ///< 10.0um Particle Count
  uint16_t unused;         ///< Unused
  uint16_t checksum;       ///< Packet checksum
} PM25_AQI_Data;

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          PM2.5 Air Quality Sensor
 */

bool Adafruit_PM25AQI_init_I2C(Adafruit_PM25AQI* pm25aqi, I2C_HandleTypeDef *hi2c);
bool Adafruit_PM25AQI_init_UART(Adafruit_PM25AQI* pm25aqi, UART_HandleTypeDef *huart);
bool Adafruit_PM25AQI_read(Adafruit_PM25AQI *pm25aqi, PM25_AQI_Data *data);

struct Adafruit_PM25AQI {
  I2C_HandleTypeDef *hi2c = NULL;
  UART_HandleTypeDef *huart = NULL;
  uint8_t _readbuffer[32];
};

#endif
