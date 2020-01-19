#include <Arduino.h>

#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

IPAddress localIPaddr;

#include <abus_socket.h>
abus_socket abSock(8442, 8266);

#define USE_DEBUG 1
#define DEBUG_ESP_PORT Serial

// für debug ausgaben
#if USE_DEBUG
#ifdef DEBUG_ESP_PORT
#define DEBUG_ESP_SERIAL(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#endif
#endif

#ifndef DEBUG_ESP_SERIAL
#define DEBUG_ESP_SERIAL(...)
#endif

void cbSockAnfWhg(ab_socket sock)
{
	DEBUG_ESP_SERIAL("wd=%d, og-anf=%d, og-anf-zirk=%d",
                                          sock.bitdata.at(0), sock.bitdata.at(1), sock.bitdata.at(2));
	return;
}
void cbSockToWhg(ab_socket sock)
{
	DEBUG_ESP_SERIAL("wd=%d, winter=%d, k-ein=%d, uwp-wwb=%d, strg=%d, strg-inf=%d, a-te=%.1f, wwb-te=%.1f, k-vorl-te=%.1f, s-vorl-te=%.1f, i-vorl-te=%.1f, vent-st=%.0f",
                                          sock.bitdata[0], sock.bitdata[1], sock.bitdata[2], sock.bitdata[3], sock.bitdata[4],
                                          sock.intdata[0], sock.realdata[0], sock.realdata[1], sock.realdata[2], sock.realdata[3],
                                          sock.realdata[4], sock.realdata[5]);
	return;
}
void cbSockPool(ab_socket sock)
{
	DEBUG_ESP_SERIAL("t1=%.1f, t2=%.1f\n",
                                          sock.realdata[0], sock.realdata[1]);
	return;
}
void cbSockRegler(ab_socket sock)
{
	DEBUG_ESP_SERIAL("C=%.3f, H=%.3f, max=%.1f, min=%.1f, innensoll=%.1f, Kp=%.3f, Td=%.1f, Ti=%.1f, og-raum-te=%.1f",
                                          sock.realdata[0], sock.realdata[1], sock.realdata[2], sock.realdata[3], sock.realdata[4],
                                          sock.realdata[5], sock.realdata[6], sock.realdata[7], sock.realdata[8]);
	return;
}
void cbSockToKessel(ab_socket sock)
{
	DEBUG_ESP_SERIAL("wd=%d, anf=%d, bereitzumh=%d, soll-vorl=%d",
                                          sock.bitdata[0], sock.bitdata[1], sock.bitdata[2], sock.intdata[0]);
	return;
}
void cbSockWeatherRec(ab_socket sock)
{
	DEBUG_ESP_SERIAL("temp=%d, hum=%d, dew=%d, bri=%d, rain=%d, sun=%d, wind=%d, wind-dir=%d, wind-dir-ran=%d, rain1h=%d, rain3h=%d, rain24h=%d, pre=%d, pre-red=%d",
						sock.intdata[0], sock.intdata[1], sock.intdata[2], sock.intdata[3],
						sock.intdata[4], sock.intdata[5], sock.intdata[6], sock.intdata[7],
						sock.intdata[8], sock.intdata[9], sock.intdata[10], sock.intdata[11],
						sock.longdata[0], sock.longdata[1]);
	return;
}
void cbSockToVorlauf(ab_socket sock)
{
	DEBUG_ESP_SERIAL("wd=%d, vorl-force=%d, vorl-kessel=%d, aussentemp=%d, vorl-holzk=%d",
                                          sock.bitdata[0], sock.bitdata[1],
                                          sock.intdata[1], sock.intdata[2], sock.intdata[3]);
	return;
}

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

	//WiFiManager
	//Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;

	// Disable Debug Out if globally disabled
	wifiManager.setDebugOutput(USE_DEBUG);

	// id/name, placeholder/prompt, default, length
	/* WiFiManagerParameter custom_mqtt_server("mqttserver", "mqtt server",
											mqtt_server, 40);
	wifiManager.addParameter(&custom_mqtt_server);
	char port[5];
	itoa(mqtt_port, port, 10);
	WiFiManagerParameter custom_mqtt_server_port("mqttport", "mqtt server port",
												 port, 5);
	wifiManager.addParameter(&custom_mqtt_server_port);
  */
	//exit after config instead of connecting
	wifiManager.setBreakAfterConfig(true);

	//reset settings - for testing
	//wifiManager.resetSettings();

	//sets timeout until configuration portal gets turned off
	//useful to make it all retry or go to sleep
	//in seconds
	wifiManager.setTimeout(300);

	//tries to connect to last known settings
	//if it does not connect it starts an access point with the specified name
	//here  "AutoConnectAP" with password "password"
	//and goes into a blocking loop awaiting configuration
	if (!wifiManager.autoConnect("test_esp", "test_esp"))
	{
#if HAS_OLED
		printInfo("FAIL\n");
#endif
		DEBUG_ESP_SERIAL("failed to connect, we should reset as see if it connects\n");
		delay(3000);
		ESP.reset();
		delay(5000);
	}

	localIPaddr = WiFi.localIP();


	abSock.begin();
	abSock.setSocketCallback(3, 3, 0, 0, 0, cbSockAnfWhg);
	abSock.setSocketCallback(4, 5, 1, 0, 6, cbSockToWhg);
	abSock.setSocketCallback(5, 0, 0, 0, 2, cbSockPool);
	abSock.setSocketCallback(11, 0, 0, 0, 13, cbSockRegler);
	abSock.setSocketCallback(20, 3, 1, 0, 0, cbSockToKessel);
	abSock.setSocketCallback(22, 0, 12, 2, 0, cbSockWeatherRec);
	abSock.setSocketCallback(30, 2, 3, 0, 0, cbSockToVorlauf);
}
uint32_t millis_next = 10000;
void loop()
{
	// put your main code here, to run repeatedly:
	abSock.loop();

	if (millis() >= millis_next)
	{
		ab_socket sock;
		ab_addBitTag(&sock, true);
		sock.config.socket_id = 5;

		//abSock.sendSocket(sock);
		millis_next = millis() + 10000;
	}
}