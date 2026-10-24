/**
    abus_socket.h
    Purpose: Send and Receive Abus Socket messages between Cybro PLC and ESP8266 / ESP32

    @author Daniel Gangl
    @version 0.0.7 24.10.2026 
*/
#ifndef _ABUS_SOCKET_H_
#define _ABUS_SOCKET_H_

#define MAX_DATA_LEN 255

// Maximum amount of socket callbacks, can be changed with build_flags = -DABSOCK_MAX_SOCKETS=<n>
#ifndef ABSOCK_MAX_SOCKETS
#define ABSOCK_MAX_SOCKETS 32
#endif

// The output can be configured with build_flags (PlatformIO) or with a #define before
// including abus_socket.h:
//   ABSOCK_DEBUG        turn on debug output of this file (off by default)
//   ABSOCK_NO_ERROR     turn off error output of this file (on by default)
//   ABSOCK_DBG_PRINTER  where the output is printed (Serial by default)
// The same options exist for abus_helper.h with the prefix ABUS_ (ABUS_DEBUG, ABUS_NO_ERROR, ABUS_DBG_PRINTER).
#if !defined(ABSOCK_NO_ERROR) && !defined(ABSOCK_ERROR)
#define ABSOCK_ERROR
#endif

//#define ABSOCK_PARSE_NON_SOCKET

#ifndef ABSOCK_DBG_PRINTER
#define ABSOCK_DBG_PRINTER Serial
#endif

// Define actual debug output functions when necessary.
#ifdef ABSOCK_DEBUG
#define ABSOCK_DBG_PRINT(...) do { ABSOCK_DBG_PRINTER.print(__VA_ARGS__); } while (0)
#define ABSOCK_DBG_PRINTF(...) do { ABSOCK_DBG_PRINTER.printf(__VA_ARGS__); } while (0)
#define ABSOCK_DBG_PRINTLN(...) do { ABSOCK_DBG_PRINTER.println(__VA_ARGS__); } while (0)
#else
#define ABSOCK_DBG_PRINT(...) do {} while (0)
#define ABSOCK_DBG_PRINTF(...) do {} while (0)
#define ABSOCK_DBG_PRINTLN(...) do {} while (0)
#endif

#ifdef ABSOCK_ERROR
#define ABSOCK_ERR_PRINT(...) do { ABSOCK_DBG_PRINTER.print(__VA_ARGS__); } while (0)
#define ABSOCK_ERR_PRINTF(...) do { ABSOCK_DBG_PRINTER.printf(__VA_ARGS__); } while (0)
#define ABSOCK_ERR_PRINTLN(...) do { ABSOCK_DBG_PRINTER.println(__VA_ARGS__); } while (0)
#else
#define ABSOCK_ERR_PRINT(...) do {} while (0)
#define ABSOCK_ERR_PRINTF(...) do {} while (0)
#define ABSOCK_ERR_PRINTLN(...) do {} while (0)
#endif

#include "abus_helper.h"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <functional>

// Callback for a received socket. Functions taking the socket by value (void cb(ab_socket sock))
// still work, but void cb(const ab_socket &sock) avoids a copy. Lambdas with captures are possible as well.
typedef std::function<void(const ab_socket &)> SubscribeCallbackAbSocket;

class abus_socket
{
private:
    /* data */
    WiFiUDP Udp;                                            // wifi udp driver
    uint16_t m_localUdpPort = 8442;                         // local udp abus port
    IPAddress m_BroadCastIp;                                // broadcast ip address
    uint32_t m_ownNad = 0;                                  // own communication NAD
    ab_socket_config cb_socketInfo[ABSOCK_MAX_SOCKETS];     // callback socket config data
    uint8_t cb_id[ABSOCK_MAX_SOCKETS] = {};                 // callback ids
    SubscribeCallbackAbSocket cb_fct[ABSOCK_MAX_SOCKETS];   // callback functions
public:
    /**
     * * abus_socket 
     * 
     */
    abus_socket();
    /**
     * * abus_socket 
     * constructor with fixed NAD
     * @param  {uint32_t} NAD : communication NAD (network address)
     */
    abus_socket(uint32_t NAD);
    /**
     * * abus_socket 
     * constructor with fixed NAd and different abus port
     * @param  {uint16_t} localUdpPort : local abus udp port to listen on
     * @param  {uint32_t} NAD          : communication NAD (network address)
     */
    abus_socket(uint16_t localUdpPort, uint32_t NAD);
    /**
     * * ~abus_socket 
     * deconstructor
     */
    ~abus_socket();
    /**
        initialization after connect to wifi
    */
    void begin();    
    /**
        initialization after connect to wifi
        @param localUdpPort overwrites the default UDP receive port of 8442
    */
    void begin(uint16_t localUdpPort);
    /**
        initialization after connect to wifi
        @param bCastIP overwrite / set the broadcast ip
    */
    void begin(IPAddress bCastIP);
    /**
        initialization after connect to wifi
        @param bCastIP overwrite / set the broadcast ip
        @param localUdpPort overwrites the default UDP receive port of 8442
    */
    void begin(IPAddress bCastIP, uint16_t localUdpPort);
    /**
        cyclic loop for receive handling of Abus messages
        this function should be called in the main cycle to handle all incoming messages
    */
    void loop();
    /**
     * send out a abus socket message
     * @param ab_socket the socket to send out
    */
    void sendSocket(const ab_socket &socket);
    /**
     * send a raw message on the network
     * @param data pointer to send data buffer
     * @param datalen datalength to seond out
    */
    void sendRaw(char *data, size_t datalen);
    /**
     * set a callback for a specific socket with the given configuration
     * @param config socket configuration informations (id, amount of bit, int, long and real tags)
     * @param cbFunction callback function name which is triggered after the socket is received
     * @return return the handle number of the socket (0 = error, >0 = handler)
    */
    uint8_t setSocketCallback(ab_socket_config config, SubscribeCallbackAbSocket cbFunction);
    /**
     * set a callback for a specific socket with the given configuration
     * @param sock_id the socket id to listen on
     * @param bitcount the amount of bit values in the socket
     * @param intcount the amount of integer values in the socket
     * @param longcount the amount of long values in the socket
     * @param realcount the amount of real values in the socket
     * @param cbFunction callback function name which is triggered after the socket is received
     * @return return the handle number of the socket (0 = error, >0 = handler)
     */
    uint8_t setSocketCallback(uint8_t sock_id, uint8_t bitcount, uint8_t intcount, uint8_t longcount, uint8_t realcount, SubscribeCallbackAbSocket cbFunction);
    /**
     * remove / delete a socket callback function
     * @param handler the handler of the socket callback which should be deleted
     * @return true = succesful, false = error / no callback found
    */
    bool removeSocketCallback(uint8_t handle);
};

// code implementations

inline abus_socket::abus_socket()
{
}
inline abus_socket::abus_socket(uint32_t NAD)
{
    m_ownNad = NAD;
}
inline abus_socket::abus_socket(uint16_t localUdpPort, uint32_t NAD)
{
    m_localUdpPort = localUdpPort;
    m_ownNad = NAD;
}
inline abus_socket::~abus_socket()
{
    Udp.stop();
}
inline void abus_socket::begin()
{
#if defined(ESP8266)
    if (!m_BroadCastIp.isSet())
    {
        //ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
        m_BroadCastIp = WiFi.localIP().v4() | ~WiFi.subnetMask().v4();
    }
#elif defined(ESP32)
    if (m_BroadCastIp == IPAddress(0, 0, 0, 0) ||
        m_BroadCastIp == IPAddress(INADDR_NONE))
    {
        //ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
        m_BroadCastIp = WiFi.localIP() | ~WiFi.subnetMask();
    }
#endif
    if (!m_ownNad)
    {
        // use the last 4 bytes of the mac address (the first 3 bytes are the vendor id)
        // e.g. mac ..:AB:12:34:56 -> NAD 0xAB123456
        uint8_t mac[6];
        WiFi.macAddress(mac);
        m_ownNad = (uint32_t)mac[2] << 24 | (uint32_t)mac[3] << 16 | (uint32_t)mac[4] << 8 | mac[5];
    }
    Udp.begin(m_localUdpPort);
    ABSOCK_DBG_PRINTF("*AB: begin()->bCastIP=%s, port=%d, nad=%lu\n", m_BroadCastIp.toString().c_str(), m_localUdpPort, (long uint16_t)m_ownNad);

}
inline void abus_socket::begin(uint16_t localUdpPort)
{
    m_localUdpPort = localUdpPort;
    begin();
}
inline void abus_socket::begin(IPAddress bCastIP)
{
    m_BroadCastIp = bCastIP;
    begin();
}
inline void abus_socket::begin(IPAddress bCastIP, uint16_t localUdpPort)
{
    m_localUdpPort = localUdpPort;
    m_BroadCastIp = bCastIP;
    begin();
}
inline void abus_socket::loop()
{
    int len = Udp.parsePacket();
    if (len > MAX_DATA_LEN)
    {
        ABSOCK_ERR_PRINTF("*AB: loop()->packet too long (%d)\n", len);
        return;
    }
    if (len > 0)
    {
        //ABSOCK_DBG_PRINTF("*AB: rec-len=%d, ", len);

        char recbuf[MAX_DATA_LEN];
        Udp.read(recbuf, sizeof(recbuf));

        if (ab_checkValidPacket(recbuf, len))
        {
            ab_header header = ab_getHeader(recbuf, len);
            // we got a socket message
            if (header.dir == 1u && header.typ > 0u)
            {
                ABSOCK_DBG_PRINTF("*AB: rec-len=%d, ", len);
                ABSOCK_DBG_PRINTF("<SOCK:  ID: %3d: ", header.typ);
                uint8_t cbPos = 0;
                // loop through all socket callbacks and forward the socket data for it
                // the function will only raise the callback if the following criteria are fulfilled:
                // socket ID is correct
                // total amount of data is correct (1 bit 2 int and 3 real, socket has 17 byte of data)
                while (cbPos < ABSOCK_MAX_SOCKETS)
                {
                    if(cb_id[cbPos] > 0 && cb_socketInfo[cbPos].socket_id == header.typ)
                    {
                        ab_socket newSock = ab_getSocket(recbuf, len, header, cb_socketInfo[cbPos]);
                        if (newSock.socket_valid && cb_fct[cbPos])
                        {
                            ABSOCK_DBG_PRINTF(" --> cb(%u) ", cbPos);
                            cb_fct[cbPos](newSock);
                            break; // stop cycling throught all socket callbacks if we found one
                        }
                    }
                    cbPos++;
                }
                /*
                else
                {
                    ABSOCK_ERR_PRINTLN(F("*AB: *** no socket implemented ***"));
                } */
                ABSOCK_DBG_PRINTLN("");
            }
            #ifdef ABSOCK_PARSE_NON_SOCKET
            else
            {
                ABSOCK_DBG_PRINTF("*AB: rec-len=%d, ", len);
                ABSOCK_DBG_PRINTF("<  AB: D%3d, T%1d: ", header.dir, header.typ);
                ABSOCK_DBG_PRINTF("l=%d, %lu --> %lu, ts=%04X", header.len, header.from, header.to, header.ts_id);
                ABSOCK_DBG_PRINTF(", Data");
                int pos = 14;
                while (pos < len - 4)
                {
                    ABSOCK_DBG_PRINTF(":%02X", recbuf[pos]);
                    pos++;
                }
                ABSOCK_DBG_PRINTLN("");

            }
            #endif       
        }
    }
}
inline void abus_socket::sendSocket(const ab_socket &socket)
{
    // check for valid socket
    if (socket.config.socket_id == 0)
    {
        ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->ID missing!"));
        return;
    }
    uint32_t sender = socket.sender;
    if (sender == 0)
    {
        if (m_ownNad == 0)
        {
            ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->sender missing!"));
            return;
        }
        sender = m_ownNad;
    }
    if (socket.bitdata.size() == 0 && socket.intdata.size() == 0 && socket.longdata.size() == 0 && socket.realdata.size() == 0)
    {
        ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->socketdata empty!"));
        return;
    }
    // generate a the header
    ab_header header;
    header.dir = 1;
    header.typ = socket.config.socket_id;
    header.from = sender;
    header.to = 0;
    size_t datalen = socket.bitdata.size() + socket.intdata.size() * 2 + socket.longdata.size() * 4 + socket.realdata.size() * 4 + 4;
    if (datalen + 14 > MAX_DATA_LEN)
    {
        ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->socketdata too long!"));
        return;
    }
    header.len = datalen;

    // generate dataarray
    char sendbuf[MAX_DATA_LEN];
    ab_setHeader(sendbuf, sizeof(sendbuf), header);
    // set socket data into it
    ab_setSocket(sendbuf, sizeof(sendbuf), socket);

    ab_setUIntVal(sendbuf, sizeof(sendbuf), header.len + 12, ab_calcCRC(sendbuf, header.len + 12));

    sendRaw(sendbuf, header.len + 14);
}
inline void abus_socket::sendRaw(char *data, size_t datalen)
{
    ABSOCK_DBG_PRINTF(">  AB:    L%3d: ", datalen);
    size_t pos = 0;
    while (pos < datalen)
    {
        ABSOCK_DBG_PRINTF(":%02X", data[pos]);
        pos++;
    }
    Udp.beginPacket(m_BroadCastIp, m_localUdpPort);
#if defined(ESP8266)
    Udp.write(data, datalen);
#elif defined(ESP32)
    size_t i = 0;
    while (i < datalen)
        Udp.write((uint8_t)data[i++]);
#endif
    if (Udp.endPacket())
    {
        ABSOCK_DBG_PRINTLN(F(" sndOK"));
    }
    else
    {
        ABSOCK_ERR_PRINTLN(F("*AB: send failed!"));
    }
}
inline uint8_t abus_socket::setSocketCallback(ab_socket_config config, SubscribeCallbackAbSocket cbFunction)
{
    uint8_t pos = 1;
    while (pos <= ABSOCK_MAX_SOCKETS)
    {
        if(cb_id[pos - 1] == 0)
        {
            cb_id[pos - 1] = pos;
            cb_socketInfo[pos - 1] = config;
            cb_fct[pos - 1] = std::move(cbFunction);
            ABSOCK_DBG_PRINTF("*AB: subscribeSocket: pos=%d, id=%d, bits=%d, ints=%d, longs=%d, reals=%d\n", pos - 1, config.socket_id, config.bitcount, config.intcount, config.longcount, config.realcount);
            return pos;
        }
        pos++;
    }
    return 0;
}
inline uint8_t abus_socket::setSocketCallback(uint8_t sock_id, uint8_t bitcount, uint8_t intcount, uint8_t longcount, uint8_t realcount, SubscribeCallbackAbSocket cbFunction)
{
    ab_socket_config config;
    config.socket_id = sock_id;
    config.bitcount = bitcount;
    config.intcount = intcount;
    config.longcount = longcount;
    config.realcount = realcount;
    return setSocketCallback(config, cbFunction);
}
inline bool abus_socket::removeSocketCallback(uint8_t handle)
{
    if (handle == 0 || handle > ABSOCK_MAX_SOCKETS)
        return false;
    uint8_t pos = 1;
    while (pos <= ABSOCK_MAX_SOCKETS)
    {
        if(cb_id[pos - 1] == handle)
        {
            cb_id[pos - 1] = 0;
            cb_fct[pos - 1] = nullptr;
            ABSOCK_DBG_PRINTF("*AB: unsubscribeSocket: handle=%1d\n", handle);
            return true;
        }
        pos++;
    }
    return false;
}

#endif