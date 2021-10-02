/**
 * @Author: Daniel Gangl
 * @Date:   2021-01-09 16:56:12
 * @Last Modified by:   Daniel Gangl
 * @Last Modified time: 2021-09-19 16:52:53
 */
/**
    abus_socket.h
    Purpose: Send and Receive Abus Socket messages between Cybro PLC and ESP8266 / ESP32

    @author Daniel Gangl
    @version 0.0.6 08.03.2021 
*/
#ifndef _ABUS_SOCKET_H_
#define _ABUS_SOCKET_H_

#define MAX_DATA_LEN 255

#define ABSOCK_MAX_SOCKETS 32

// Uncomment/comment to turn on/off debug output messages.
#define ABSOCK_DEBUG
// Uncomment/comment to turn on/off error output messages.
#define ABSOCK_ERROR

//#define ABSOCK_PARSE_NON_SOCKET

// Set where debug messages will be printed.
#define ABSOCK_DBG_PRINTER Serial
// If using something like Zero or Due, change the above to SerialUSB

// Define actual debug output functions when necessary.
#ifdef ABSOCK_DEBUG
#define ABSOCK_DBG_PRINT(...)                  \
    {                                          \
        ABSOCK_DBG_PRINTER.print(__VA_ARGS__); \
    }
#define ABSOCK_DBG_PRINTF(...)                  \
    {                                           \
        ABSOCK_DBG_PRINTER.printf(__VA_ARGS__); \
    }
#define ABSOCK_DBG_PRINTLN(...)                  \
    {                                            \
        ABSOCK_DBG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define ABSOCK_DBG_PRINT(...) \
    {                         \
    }
#define ABSOCK_DBG_PRINTF(...) \
    {                          \
    }
#define ABSOCK_DBG_PRINTLN(...) \
    {                           \
    }
#endif

#ifdef ABSOCK_ERROR
#define ABSOCK_ERR_PRINT(...)                  \
    {                                          \
        ABSOCK_DBG_PRINTER.print(__VA_ARGS__); \
    }
#define ABSOCK_ERR_PRINTF(...)                  \
    {                                           \
        ABSOCK_DBG_PRINTER.printf(__VA_ARGS__); \
    }
#define ABSOCK_ERR_PRINTLN(...)                  \
    {                                            \
        ABSOCK_DBG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define ABSOCK_ERR_PRINT(...) \
    {                         \
    }
#define ABSOCK_ERR_PRINTF(...) \
    {                          \
    }
#define ABSOCK_ERR_PRINTLN(...) \
    {                           \
    }
#endif

#include <abus_helper.h>
#include <WiFiUdp.h>

//Function pointer that returns a received socket
typedef void (*SubscribeCallbackAbSocket)(ab_socket);

class abus_socket
{
private:
    /* data */
    WiFiUDP Udp;                                            // wifi udp driver
    uint16_t m_localUdpPort = 8442;                         // local udp abus port
    IPAddress m_BroadCastIp;                                // broadcast ip address
    uint32_t m_ownNad = 0;                                  // own communication NAD
    ab_socket_config cb_socketInfo[ABSOCK_MAX_SOCKETS];     // callback socket config data
    uint8_t cb_id[ABSOCK_MAX_SOCKETS];                      // callback ids
    SubscribeCallbackAbSocket cb_fct[ABSOCK_MAX_SOCKETS];   // callback function pointers
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
    void sendSocket(ab_socket);
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

abus_socket::abus_socket()
{
}
abus_socket::abus_socket(uint32_t NAD)
{
    m_ownNad = NAD;
}
abus_socket::abus_socket(uint16_t localUdpPort, uint32_t NAD)
{
    m_localUdpPort = localUdpPort;
    m_ownNad = NAD;
}
abus_socket::~abus_socket()
{
    Udp.stop();
}
void abus_socket::begin()
{
#if defined(ESP8266)
    if (!m_BroadCastIp.isSet())
    {
        //ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
        m_BroadCastIp = WiFi.localIP().v4() | ~WiFi.subnetMask().v4();
    }
#elif defined(ESP32)
    if (m_BroadCastIp == IPAddress(INADDR_ANY) ||
        m_BroadCastIp == IPAddress(INADDR_NONE))
    {
        //ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
        m_BroadCastIp = WiFi.localIP() | ~WiFi.subnetMask();
    }
#endif
    if (!m_ownNad)
    {
        char mac[12];
        WiFi.macAddress().toCharArray(mac, sizeof(mac), 0);
        m_ownNad = mac[0] | mac[1] << 8L | mac[2] << 16L | mac[3] << 24L;
    }
    Udp.begin(m_localUdpPort);
    ABSOCK_DBG_PRINTF("*AB: begin()->bCastIP=%s, port=%d, nad=%lu\n", m_BroadCastIp.toString().c_str(), m_localUdpPort, (long uint16_t)m_ownNad);

}
void abus_socket::begin(uint16_t localUdpPort)
{
    m_localUdpPort = localUdpPort;
    begin();
}
void abus_socket::begin(IPAddress bCastIP)
{
    m_BroadCastIp = bCastIP;
    begin();
}
void abus_socket::begin(IPAddress bCastIP, uint16_t localUdpPort)
{
    m_localUdpPort = localUdpPort;
    m_BroadCastIp = bCastIP;
    begin();
}
void abus_socket::loop()
{
    int len = Udp.parsePacket();
    if (len > 0)
    {
        //ABSOCK_DBG_PRINTF("*AB: rec-len=%d, ", len);

        char recbuf[MAX_DATA_LEN];
        Udp.read(recbuf, sizeof(recbuf));

        if (ab_checkValidPacket(recbuf, min(len, (int)sizeof(recbuf))))
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
                        if (newSock.socket_valid && cb_fct[cbPos] != NULL)
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
                int posmax = len;
                int pos = 14;
                if (len > (int)sizeof(recbuf))
                    posmax = sizeof(recbuf);
                while (pos < posmax - 4)
                {
                    ABSOCK_DBG_PRINTF(":%02X", recbuf[pos]);
                    pos++;
                }
                ABSOCK_DBG_PRINTLN("");

            }
            #endif       
        }
		else
		{
			// if there was an error restart the UDP protocol driver
			begin();
		}
    }
}
void abus_socket::sendSocket(ab_socket socket)
{
    // check for valid socket
    if (socket.config.socket_id == 0)
    {
        ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->ID missing!"));
        return;
    }
    if (socket.sender == 0)
    {
        if (m_ownNad == 0)
        {
            ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->sender missing!"));
            return;
        }
        socket.sender = m_ownNad;
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
    header.from = socket.sender;
    header.to = 0;
    header.len = socket.bitdata.size() + socket.intdata.size() * 2 + socket.longdata.size() * 4 + socket.realdata.size() * 4 + 4;

    // generate dataarray
    char sendbuf[MAX_DATA_LEN];
    ab_setHeader(sendbuf, sizeof(sendbuf), header);
    // set socket data into it
    ab_setSocket(sendbuf, sizeof(sendbuf), socket);

    ab_setUIntVal(sendbuf, sizeof(sendbuf), header.len + 12, ab_calcCRC(sendbuf, header.len + 12));

    sendRaw(sendbuf, header.len + 14);
}
void abus_socket::sendRaw(char *data, size_t datalen)
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
uint8_t abus_socket::setSocketCallback(ab_socket_config config, SubscribeCallbackAbSocket cbFunction)
{
    uint8_t pos = 1;
    while (pos <= ABSOCK_MAX_SOCKETS)
    {
        if(cb_id[pos - 1] == 0)
        {
            cb_id[pos - 1] = pos;
            cb_socketInfo[pos - 1] = config;
            cb_fct[pos - 1] = cbFunction;
            ABSOCK_DBG_PRINTF("*AB: subscribeSocket: pos=%d, id=%d, bits=%d, ints=%d, longs=%d, reals=%d\n", pos - 1, config.socket_id, config.bitcount, config.intcount, config.longcount, config.realcount);
            return pos;
        }
        pos++;
    }
    return 0;
}
uint8_t abus_socket::setSocketCallback(uint8_t sock_id, uint8_t bitcount, uint8_t intcount, uint8_t longcount, uint8_t realcount, SubscribeCallbackAbSocket cbFunction)
{
    ab_socket_config config;
    config.socket_id = sock_id;
    config.bitcount = bitcount;
    config.intcount = intcount;
    config.longcount = longcount;
    config.realcount = realcount;
    return setSocketCallback(config, cbFunction);
}
bool abus_socket::removeSocketCallback(uint8_t handle)
{
    if (handle == 0 || handle > ABSOCK_MAX_SOCKETS)
        return false;
    uint8_t pos = 1;
    while (pos <= ABSOCK_MAX_SOCKETS)
    {
        if(cb_id[pos - 1] == handle)
        {
            cb_id[pos - 1] = 0;
            cb_fct[pos - 1] = NULL;
            ABSOCK_DBG_PRINTF("*AB: unsubscribeSocket: handle=%1d\n", handle);
            return true;
        }
    }
    return false;
}

#endif