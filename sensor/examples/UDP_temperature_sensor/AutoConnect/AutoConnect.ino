/*
 * @file 	main.c
 * @author	morten opprud
 * @date 	april 15 - 2016
 * @brief	udp communication over wifi, based on https://www.arduino.cc/en/Tutorial/UDPSendReceiveString
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "sensor.h"

/* wifi connection variables */
//TODO UPDATE AP ssid/pass
const char* ssid 		= "!insertSSIDtHere!";
const char* password 	= "deledele";
bool wifiConnected 		= false;

/* UDP variables */
WiFiUDP UDP;
unsigned int localPort 	= 8888;
bool udpConnected	 	= false;
char ReplyBuffer[] 		= "acknowledged"; 	// a string to send back
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; 	//buffer to hold incoming packet,

/* command defines */
#define	GET_DATA	2
#define	GET_INFO	3

/* sensor variables */
sensor_payload_t sensorData;
sensor_info_t	 sensorInfo;

/**
 * @brief connect to UDP
 * @return returns true if successful or false if not
 */
bool connectUDP(void)
{
	bool state = false;

	Serial.println("");
	Serial.println("Connecting to UDP");

	if (UDP.begin(localPort) == 1)
	{
		Serial.println("Connection successful");
		state = true;
	}

	else
	{
		Serial.println("Connection failed");
	}
	return state;
}

/**
 * @brief connect to wifi
 * @return returns true if successful or false if not
 */
bool connectWifi(void)
{
	bool state = true;
	int i = 0;
	WiFi.begin(ssid, password);
	Serial.println("");
	Serial.println("Connecting to WiFi");

	/* Wait for connection, try 10 times */
	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
		if (i > 10)
		{
			state = false;
			break;
		}
		i++;
	}
	/* connection ? */
	if (state)
	{
		Serial.println("");
		Serial.print("Connected to ");
		Serial.println(ssid);
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	}
	/* nope */
	else
	{
		Serial.println("");
		Serial.println("Connection failed.");
	}
	return state;
}

/**
 * @brief setup system & platform, default arduino setup function
 * @param *str poitner to recieved command string
 * @return 1 if valid
 */
int parseCmd(char *str)
{
	int cmd=0;

	if(!strcmp(str, "GET_DATA"))
		cmd = GET_DATA;
	if(!strcmp(str, "GET_INFO"))
		cmd = GET_INFO;

	return cmd;
}

/**
 * @brief setup system & platform, default arduino setup function
 */
void setup(void)
{
	/* Initialise Serial connection */
	Serial.begin(115200);

	/* Initialise wifi connection */
	wifiConnected = connectWifi();

	/* only proceed if wifi connection successful */
	if (wifiConnected)
	{
		udpConnected = connectUDP();
		if (udpConnected)
		{
			/* initialise LED pin, blue led is active LOW */
			pinMode(5, OUTPUT);
		}

		/* setup sensor sensorInfo */
		//info.sensor_name  = {"IDA IoT sensor"};
		strcpy(sensorInfo.sensor_name,"IDA IoT sensor");
		strcpy(sensorInfo.sensor_type,"Temperature");
		strcpy(sensorInfo.sensor_units,"Deg C");
		sensorInfo.sensor_resolution_bits = 10;
		sensorInfo.sensor_samplerate_us = 0;	//updated only when read
		initSensor(&sensorInfo);

		/* setup sensor data defaults*/
		sensorData.no_samples = 0;
		sensorData.size = 0;
		sensorData.timeStampUs = 0;
	}
}

/**
 * @brief loop is arduinos main-loop, executed after setup()
 * 		  should newer return
 */
void loop()
{
	int cmd;
	uint8_t* dp;
	uint8_t  len;

	/* check if the WiFi and UDP connections were successful */
	if (wifiConnected)
	{
		if (udpConnected)
		{

			/* if there?s data available, read a packet */
			int packetSize = UDP.parsePacket();
			if (packetSize)
			{
				Serial.println("");
				Serial.print("Received packet of size ");
				Serial.println(packetSize);
				Serial.print("From ");
				IPAddress remote = UDP.remoteIP();
				for (int i = 0; i < 4; i++)
				{
					Serial.print(remote[i], DEC);
					if (i < 3)
					{
						Serial.print(".");
					}
				}
				Serial.print(", port ");
				Serial.println(UDP.remotePort());

        /* read the packet into packetBufffer */
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        /* print command */
        Serial.println(packetBuffer);
        /* and parse */
        cmd = parseCmd(packetBuffer);

				switch (cmd) {
					case GET_DATA:
						//sensorData.sensor_values[0] = readTemperature();
						sensorData.sensor_values[0] = ((analogRead(A0) - 500) / 10);
						sensorData.size=1;
						sensorData.head=1;
						sensorData.timeStampUs = 100;

						dp = (uint8_t*)&sensorData;
						len = sizeof(sensorData);
						UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
						UDP.write((uint8_t*)&sensorData, len);
						UDP.endPacket();
						Serial.print("Data sent");

						break;
					case GET_INFO:
						dp = (uint8_t*)&sensorInfo;
						len = sizeof(sensorInfo);
						UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
						UDP.write((uint8_t*)&sensorInfo, len);
						UDP.endPacket();
						Serial.print("Info sent");
						break;
          default:
            Serial.print("unknown command ! \n");
            break;
				}
			}
      /* allow data to be sent...
         delay is in ms*/
      delay(100);
		}

	}
}

