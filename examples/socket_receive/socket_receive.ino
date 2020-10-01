/*
 This example receives a socket and triggers a callback if the socket with the same number is recevied

 */

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password

#include <abus_socket.h>
abus_socket abSock(8442, 8266);

// callback declaration for receiving a socket
void cbSocketReceived(ab_socket sock)
{
    Serial.print("socket ID=");
    Serial.print(sock.config.socket_id);
    Serial.print(",bit0=");
    Serial.print(sock.bitdata.at(0));
    Serial.print(",int0=");
    Serial.print(sock.intdata.at(0));
    Serial.print(",long0=");
    Serial.print(sock.longdata.at(0));
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
        Serial.println(SECRET_SSID);
        while (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
            Serial.print(".");
            delay(5000);
        }
        Serial.println("\nConnected.");
    }
    abSock.begin();
    abSock.setSocketCallback(3, 1, 1, 1, 1, cbSocketReceived);
}
void loop()
{
    // put your main code here, to run repeatedly:
    abSock.loop();
}