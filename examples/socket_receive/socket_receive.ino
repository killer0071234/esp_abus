/*
 This example receives a socket and triggers a callback if the socket with the same number is recevied

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
    // print the received boolean / bit value
    Serial.print(",bit0=");
    Serial.print(sock.bitdata.at(0));
    // print the received integer value
    Serial.print(",int0=");
    Serial.print(sock.intdata.at(0));
    // print the received long value
    Serial.print(",long0=");
    Serial.print(sock.longdata.at(0));
    // print the received real value
    Serial.print(",real0=");
    Serial.print(sock.realdata.at(0));
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
void loop()
{
    // put your main code here, to run repeatedly:
    abSock.loop();
}