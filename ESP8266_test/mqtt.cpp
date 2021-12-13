#include "mqtt.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <ctime>

#define PUBLISH_TOPIC "EXAMPLE_TOPIC"

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

mqtt_client::mqtt_client(const char *id, const char *host, int port, string filePath) : mosquittopp(id)
{
	int ret = connect(host, port, DEFAULT_KEEP_ALIVE);
	if (ret == 0) {
		cout << longtime() << endl;
		cout << "Connected" << endl;
	}
	else {
		cout << "Connection fail: " << to_string(ret) << endl;
	}

	outFilePath = filePath;

	y = 0;

}

mqtt_client::~mqtt_client()
{
}

void mqtt_client::on_connect(int rc)
{
	Connected = true;
	//cout << "On_connect" << endl;
	if (!rc)
	{
#ifdef DEBUG
		std::cout << "Connected - code " << rc << std::endl;
#endif
	}
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{

	//cout << "on_subscribe" << endl;

#ifdef DEBUG
	std::cout << "Subscription succeeded." << std::endl;
#endif
}

void mqtt_client::on_disconnect(int rc)
{
	Connected = false;
	cout << longtime() << "Disconnected" << endl;
	this->reconnect();
	
}

int mqtt_client::szam(string szo)
{
	stringstream s;
	int x;

	s << szo;
	s >> x;

	return x;
}

int mqtt_client::cmd(string mes)
{
	static const size_t npos = -1;

	if (mes.find("x")!= npos) {
		setX(szam(mes.substr(mes.find("=")+1)));
		cout << "cmd(), X= " << getX() << endl;
		return 0;
	//}
	//else if (mes.find("y") != npos) {
	//	setY(szam(mes.substr(mes.find("=") + 1)));
	//	cout << "cmd(), Y= " << getY() << endl;
	//	return 0;
	}else if (mes.find("z") != npos) {
		setZ(szam(mes.substr(mes.find("=") + 1)));
		cout << "cmd(), Z= " << getZ() << endl;
		return 0;
	}
	else if (mes.find("recon") != npos) {
		this->reconnect();
	}
	else if (mes.find("exit") != npos) {
		O = false;
	}
	return 1;
}

int mqtt_client::getX()
{
	return x;
}

int mqtt_client::getY()
{
	return y;
}

int mqtt_client::getZ()
{
	return z;
}

void mqtt_client::setX(int newx)
{
	x = newx;
}

void mqtt_client::setY(int newy)
{
	y = newy;
}

void mqtt_client::setZ(int newz)
{
	z = newz;
}

int mqtt_client::send(string topic, string newPl)
{
	int result;
	char *buf;
	buf = new char[newPl.length() + 1];
	strcpy(buf, newPl.c_str());

	result = this->publish(NULL, "TEST_reliability", 1, buf, 2, false);

	return result;
}

void mqtt_client::on_message(const struct mosquitto_message* message)
{	
	string pyl = (char*)message->payload;

	if (pyl == "1") y = 1;

	ofstream of;

	of.open(outFilePath.c_str(), ios_base::app);
	of << longtime() << " -- " << message->topic << " - " << pyl << endl;

	of.close();

}

void mqtt_client::reconnect() {

	this->reinitialise(CLIENT_ID, true);
	
	int x = this->connect(BROKER_ADDRESS, MQTT_PORT, DEFAULT_KEEP_ALIVE);
	int y = this->subscribe(i, MQTT_TOPIC, 0);

	if (x == 0 && y == 0) cout << longtime() << "Reconnected" << endl;
	else {
		//cout << "x: " << to_string(x) << ", y: " << to_string(y) << endl;
	}
}

std::string mqtt_client::longtime() {

	std::time_t result = std::time(nullptr);
	string ltime = std::asctime(std::localtime(&result));
	return ltime.substr(0, ltime.length() - 1);

}