/*!
 * @file Adafruit_PM25AQI.cpp
 *
 * @mainpage Adafruit PM2.5 air quality sensor driver
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's PM2.5 AQI driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit PM2.5 Air quality sensors: http://www.adafruit.com/products/4632
 *
 * These sensors use I2C or UART to communicate.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 *
 * @section author Author
 * Written by Ladyada for Adafruit Industries.
 *
 * @section license License
 * BSD license, all text here must be included in any redistribution.
 *
 */
#include "Adafruit_PM25AQI.h"

/*!
 *  @brief  Setups the hardware and detects a valid PMSA003I. Initializes I2C.
 *  @param  theWire
 *          Optional pointer to I2C interface, otherwise use Wire
 *  @return True if PMSA003I found on I2C, False if something went wrong!
 */
bool Adafruit_PM25AQI_init_I2C(Adafruit_PM25AQI* pm25aqi, I2C_HandleTypeDef *hi2c) {
  if (!pm25aqi) {
    return false;
  }
  if (!hi2c) {
    return false;
  }

  pm25aqi->hi2c = hi2c;

  return true;
}

/*!
 *  @brief  Setups the hardware and detects a valid UART PM2.5
 *  @param  theSerial
 *          Pointer to Stream (HardwareSerial/SoftwareSerial) interface
 *  @return True
 */
bool Adafruit_PM25AQI_init_UART(Adafruit_PM25AQI* pm25aqi, UART_HandleTypeDef *huart) {
  if (!pm25aqi) {
    return false;
  }
  if (!huart) {
    return false;
  }

  pm25aqi->huart = huart;

  return true;
}

/*!
 *  @brief  Setups the hardware and detects a valid UART PM2.5
 *  @param  data
 *          Pointer to PM25_AQI_Data that will be filled by read()ing
 *  @return True on successful read, false if timed out or bad data
 */
bool Adafruit_PM25AQI_read(Adafruit_PM25AQI *pm25aqi, PM25_AQI_Data *data) {
  uint8_t buffer[32];
  uint16_t sum = 0;

  if (!data) {
    return false;
  }

  if (pm25aqi->hi2c) { // ok using i2c?
    if (HAL_OK != HAL_I2C_Mem_Read(pm25aqi->hi2c, PMSA003I_I2CADDR_DEFAULT, 0, 32, buffer, 32, 50)) {
      return false;
    }
  } else if (pm25aqi->huart) { // ok using uart
    //if (!serial_dev->available()) {
    //  return false;
    //}
    //int skipped = 0;
    //while ((skipped < 32) && (serial_dev->peek() != 0x42)) {
    //  serial_dev->read();
    //  skipped++;
    //  if (!serial_dev->available()) {
    //    return false;
    //  }
    //}
    //if (serial_dev->peek() != 0x42) {
    //  serial_dev->read();
    //  return false;
    //}
    //// Now read all 32 bytes
    //if (serial_dev->available() < 32) {
    //  return false;
    //}
    //serial_dev->readBytes(buffer, 32);
  } else {
    return false;
  }

  // Check that start byte is correct!
  if (buffer[0] != 0x42) {
    return false;
  }

  // get checksum ready
  for (uint8_t i = 0; i < 30; i++) {
    sum += buffer[i];
  }

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)data, (void *)buffer_u16, 30);

  if (sum != data->checksum) {
    return false;
  }

  // success!
  return true;
}
