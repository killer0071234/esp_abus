/*
 This example sends a socket every 10 seconds
 the socket can be received with the socket_receive.ino function
 */

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
char ssid[] = "SECRET_SSID"; // your network SSID (name)
char pass[] = "SECRET_PASS"; // your network password

#include <abus_socket.h>
abus_socket abSock(8442, 8266);

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        while (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
            Serial.print(".");
            delay(5000);
        }
        Serial.println("\nConnected.");
    }
    // initialize the socket function
    abSock.begin();
}

uint32_t millis_next = 10000;
void loop()
{
	// put your main code here, to run repeatedly:
	abSock.loop();

	if (millis() >= millis_next)
	{
        // define a new socket object
		ab_socket sock;
        // add a boolen / bit tag / variable to the socket element
		ab_addBitTag(&sock, true);
        // add a integer tag / variable to the socket element
		ab_addIntTag(&sock, 123);
        // add a integer tag / variable to the socket element
		ab_addLongTag(&sock, 123456);
        // add a real / float tag / variable to the socket element
		ab_addRealTag(&sock, 12.3);
        // set the socket id for sending it
		sock.config.socket_id = 3;

        // send the socket over wifi to the PLC
		abSock.sendSocket(sock);
		millis_next = millis() + 10000;
	}
}