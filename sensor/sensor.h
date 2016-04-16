/*
 * sensor.h
 *
 *  Created on: Apr 16, 2016
 *      Author: mortenopprudjakobsen
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define MAX_SENSORS	4	/**< How many sensors can we handle */

/**
 * @brief	generic sensor information
 *
 * */
typedef struct
{
	char sensor_name[40];					/**< Name of sensor, eg "LM35", "DS18B20", etc,  */
	char sensor_type[40];					/**< Type of sensor, eg "Temperature", "Humidity", etc,  */
	char sensor_units[20];					/**< Name of Si unit, eg "Degree", "mBar", "Lux"  */
	unsigned char sensor_resolution_bits; 	/**< digital resolution of sensor, or analog input for sampling  */
	unsigned int sensor_samplerate_us;	  	/**< how often is the sensor read  */
	unsigned int sensor_update_interval_ms; /**< how often is the sensor data sent to the reciever  */
}sensor_info_t;

typedef struct
{
	float sensor_values[10];				/**< Scaled calculated sensor values  */
	unsigned char head;						/**< Data pointer head  */
	unsigned char tail;						/**< Data pointer tail  */
	unsigned char size;						/**< Data no of data  */
	unsigned int timeStampUs;				/**< Last sample - timestamp in microseconds  */
	unsigned char no_samples;				/**< How many new samples ?  */
}sensor_payload_t;

float readTemperature(void);
int initSensor(sensor_info_t* cfg);
int readSensor(sensor_info_t* dev);

#endif /* SENSOR_H_ */
