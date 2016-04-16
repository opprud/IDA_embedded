/*
 * sensor.c
 *
 *  Created on: Apr 16, 2016
 *      Author: mortenopprudjakobsen
 */
#include "sensor.h"


sensor_info_t devices[MAX_SENSORS];

/**
 * @brief read analog input and scle to deg C
 * @return current temperature
 */
float readTemperature(void)
{

}


/**
 * @brief initialize sensor object
 * @return returns true if successful or false if not
 */
int initSensor(sensor_info_t* cfg)
{
	devices[0] = *cfg;


}

/**
 * @brief read from the sensor
 * @return returns true if successful or false if not
 */
int readSensor(sensor_info_t* dev)
{

}

