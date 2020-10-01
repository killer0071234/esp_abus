/**
    abus_socket.h
    Purpose: Send and Receive Abus Socket messages between Cybro PLC and ESP8266 / ESP32

    @author Daniel Gangl
    @version 1.0 02.01.2020 
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
    WiFiUDP Udp;
    unsigned int localUdpPort = 8442;
    IPAddress IP_Remote;
    uint32_t ownNad = 0;
    ab_socket_config cb_socketInfo[ABSOCK_MAX_SOCKETS];
    uint8_t cb_id[ABSOCK_MAX_SOCKETS];
    SubscribeCallbackAbSocket cb_fct[ABSOCK_MAX_SOCKETS];
public:
    abus_socket();
    abus_socket(uint32_t NAD);
    abus_socket(unsigned int localUdpPort, uint32_t NAD);
    ~abus_socket();
    /**
        initialization after connect to wifi
    */
    void begin();    
    /**
        initialization after connect to wifi
        @param localUdpPort overwrites the default UDP receive port of 8442
    */
    void begin(unsigned int localUdpPort);
    /**
        initialization after connect to wifi
        @param remoteIP overwrites the default UDP receive port of 8442
    */
    void begin(IPAddress remoteIP);
    /**
        initialization after connect to wifi
    */
    void begin(IPAddress remoteIP, unsigned int localUdpPort);
    /**
        cyclic loop for receive handling of Abus messages
        this function should be called in the main cycle to handle all incoming messages
    */
    void loop();
    /**
     * send out a abus socket message
    */
    void sendSocket(ab_socket);
    /**
     * send a raw message on the network
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
     * @param sock_id socket id (id, amount of bit, int, long and real tags)
     * @param bitcount number of used bit tags
     * @param intcount number of used int tags
     * @param longcount number of used long tags
     * @param realcount number of used real tags
     * @param cbFunction callback function name which is triggered after the socket is received
     * @return return the handle number of the socket (0 = error, >0 = handler)
    */
    uint8_t setSocketCallback(uint8_t sock_id, uint8_t bitcount, uint8_t intcount, uint8_t longcount, uint8_t realcount, SubscribeCallbackAbSocket cbFunction);
    /**
     * remove / delete a socket callback function
     * @param handler the handler of the socket callback which should be deleted
    */
    bool removeSocketCallback(uint8_t handle);
};

abus_socket::abus_socket()
{
}
abus_socket::abus_socket(uint32_t NAD)
{
    ownNad = NAD;
}
abus_socket::abus_socket(unsigned int localUdpPort, uint32_t NAD)
{
    localUdpPort = localUdpPort;
    ownNad = NAD;
}
abus_socket::~abus_socket()
{
    Udp.stop();
}

void abus_socket::begin()
{
#if defined(ESP8266)
    if (!IP_Remote.isSet())
    {
        IP_Remote = WiFi.localIP().v4() | ~WiFi.subnetMask().v4();
        //ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
    }
#elif defined(ESP32)
    if (IP_Remote == INADDR_NONE)
    {
        ABSOCK_DBG_PRINTLN(F("*AB: calc broadcast IP from WiFi IP"));
        IP_Remote = WiFi.localIP() | ~WiFi.subnetMask();
    }
#endif
    if (!ownNad)
    {
        char mac[12];
        WiFi.macAddress().toCharArray(mac, sizeof(mac), 0);
        ownNad = mac[0] | mac[1] << 8L | mac[2] << 16L | mac[3] << 24L;
    }
    Udp.begin(localUdpPort);
    ABSOCK_DBG_PRINTF("*AB: begin()->remoteIP=%s, port=%d, nad=%lu\n", IP_Remote.toString().c_str(), localUdpPort, ownNad);
}
void abus_socket::begin(unsigned int localUdpPort)
{
    localUdpPort = localUdpPort;
    begin();
}
void abus_socket::begin(IPAddress remoteIP)
{
    IP_Remote = remoteIP;
    begin();
}
void abus_socket::begin(IPAddress remoteIP, unsigned int localUdpPort)
{
    localUdpPort = localUdpPort;
    IP_Remote = remoteIP;
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
                            ABSOCK_DBG_PRINTF(" --> cb() ");
                            cb_fct[cbPos](newSock);
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
        if (ownNad == 0)
        {
            ABSOCK_ERR_PRINTLN(F("*AB: sendSocket()->sender missing!"));
            return;
        }
        socket.sender = ownNad;
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
    Udp.beginPacket(IP_Remote, localUdpPort);
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