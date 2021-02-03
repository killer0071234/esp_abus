/*
 * This example sends and receives a socket
 * if a socket is received, the callback is triggered
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

// callback declaration for receiving a socket
void cbSocketReceived(ab_socket sock)
{
    // print the received socket id
    Serial.print("socket ID=");
    Serial.print(sock.config.socket_id);
    // print all received boolean / bit value
    for (uint8_t i = 0; i < sock.bitdata.size(); i++)
    {
        Serial.printf(",bit%d=", i);
        Serial.print(sock.bitdata.at(i));
    }
    // print the received integer value
    for (uint8_t i = 0; i < sock.intdata.size(); i++)
    {
        Serial.printf(",int%d=", i);
        Serial.print(sock.intdata.at(i));
    }
    // print the received long value
    for (uint8_t i = 0; i < sock.longdata.size(); i++)
    {
        Serial.printf(",long%d=", i);
        Serial.print(sock.longdata.at(i));
    }
    // print the received real value
    for (uint8_t i = 0; i < sock.realdata.size(); i++)
    {
        Serial.printf(",real%d=", i);
        Serial.print(sock.realdata.at(i));
    }
    return;
}
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
    // add a callback to receive a socket with id: 3 and 1 bit, 1 int, 1 long, 1 real tag / variable
    abSock.setSocketCallback(3, 1, 1, 1, 1, cbSocketReceived);
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