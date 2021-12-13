#include <wiringPi.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include "mqtt.h"

#define CLIENT_ID "esp8266_checker"
#define BROKER_ADDRESS "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC1 "TEST_Pub"
#define MQTT_TOPIC2 "TEST_reliability"

std::string longtime();

int main() {

	// MQTT
	class mqtt_client* iot_client;
	char client_id[] = CLIENT_ID;
	int sadf = 13245;
	int* clint_id = &sadf;
	char host[] = BROKER_ADDRESS;
	int port = MQTT_PORT;
	// MQTT

	std::string outFilePath = "/home/pi/Desktop/esp8226_test/esp8266_log.txt";
	int min;

	std::ofstream oF;



	mosqpp::lib_init();

	iot_client = new mqtt_client(client_id, host, port, outFilePath);

	std::cout << "clientid: " << client_id << std::endl;
	std::cout << "host: " << host << std::endl << "port: " << port << std::endl;

	std::cout << "Subsribing to " << MQTT_TOPIC1 << ": ";
	if (iot_client->subscribe(clint_id, MQTT_TOPIC1, 2) == 0) std::cout << "OK" << std::endl;
	std::cout << "Subsribing to " << MQTT_TOPIC2 << ": ";
	if (iot_client->subscribe(clint_id, MQTT_TOPIC2, 2) == 0) std::cout << "OK" << std::endl;

	iot_client->loop_start();

	while (iot_client->getY() != 1) {

		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		min = tm.tm_min;
		if (min == 15 || min == 30 || min == 45 || min == 0) {

			iot_client->send(MQTT_TOPIC2, "x");
			oF.open(outFilePath.c_str(), std::ios_base::app);
			oF << longtime() << " -- Request has been sent!\n";
			oF.close();

		}
		
		delay(50000);

	};

	mosqpp::lib_cleanup();

	return 0;
}

std::string longtime() {

	std::time_t result = std::time(nullptr);
	std::string ltime = std::asctime(std::localtime(&result));
	return ltime.substr(0, ltime.length() - 1);

}