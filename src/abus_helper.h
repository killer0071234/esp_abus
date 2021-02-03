/**
 * abus_helper.h
 * This is a small collection of helper functions to communicate with Cybro PLCs (Cybro-2 / Cybro-3) from Cybrotech Ltd.
 * @author Daniel Gangl <killer007@gmx.at>
 * @version 1.1 03.02.2020 
 */
#ifndef _ABUS_HELPER_H_
#define _ABUS_HELPER_H_

// Uncomment/comment to turn on/off debug output messages.
//#define ABUS_DEBUG
// Uncomment/comment to turn on/off error output messages.
#define ABUS_ERROR

// Set where debug messages will be printed.
#define ABUS_DBG_PRINTER Serial
// If using something like Zero or Due, change the above to SerialUSB

// Define actual debug output functions when necessary.
#ifdef ABUS_DEBUG
#define ABUS_DBG_PRINT(...)                  \
    {                                        \
        ABUS_DBG_PRINTER.print(__VA_ARGS__); \
    }
#define ABUS_DBG_PRINTF(...)                  \
    {                                         \
        ABUS_DBG_PRINTER.printf(__VA_ARGS__); \
    }
#define ABUS_DBG_PRINTLN(...)                  \
    {                                          \
        ABUS_DBG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define ABUS_DBG_PRINT(...) \
    {                       \
    }
#define ABUS_DBG_PRINTF(...) \
    {                        \
    }
#define ABUS_DBG_PRINTLN(...) \
    {                         \
    }
#endif

#ifdef ABUS_ERROR
#define ABUS_ERR_PRINT(...)                  \
    {                                        \
        ABUS_DBG_PRINTER.print(__VA_ARGS__); \
    }
#define ABUS_ERR_PRINTF(...)                  \
    {                                         \
        ABUS_DBG_PRINTER.printf(__VA_ARGS__); \
    }
#define ABUS_ERR_PRINTLN(...)                  \
    {                                          \
        ABUS_DBG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define ABUS_ERR_PRINT(...) \
    {                       \
    }
#define ABUS_ERR_PRINTF(...) \
    {                        \
    }
#define ABUS_ERR_PRINTLN(...) \
    {                         \
    }
#endif

#include <vector>
#include "Arduino.h"
// Primtable for CRC calculation
const uint16_t ab_PrimTable[16] = {0x049D, 0x0C07, 0x1591, 0x1ACF, 0x1D4B, 0x202D, 0x2507, 0x2B4B,
                                   0x34A5, 0x38C5, 0x3D3F, 0x4445, 0x4D0F, 0x538F, 0x5FB3, 0x6BBF};
// abus header structure
struct ab_header
{
    uint16_t len = 0;
    uint16_t ts_id = 0;
    uint32_t from = 0;
    uint32_t to = 0;
    uint8_t dir = 0;
    uint8_t typ = 0;
};
// abus socket configuration
struct ab_socket_config
{
    uint8_t socket_id = 0;
    uint8_t bitcount = 0;
    uint8_t intcount = 0;
    uint8_t longcount = 0;
    uint8_t realcount = 0;
};
// abus socket structure which holds the data of a single socket
struct ab_socket
{
    ab_socket_config config;
    uint32_t sender = 0;
    bool socket_valid = false;
    std::vector<uint8_t> bitdata; // representation of a boolean value (must be byte because bool(ean) has unpredictable behaviour, and in cybro plc it is actually stored in one byte)
    std::vector<int16_t> intdata;
    std::vector<int32_t> longdata;
    std::vector<float_t> realdata;
};

// helper union for converting 4 bytes into one real value and vice versa
union ab_real {
    float_t real;
    char bytes[sizeof(float_t)];
} ab_real;
// helper union for converting 2 bytes into one integer value and vice versa
union ab_int {
    byte b[sizeof(int16_t)];
    int16_t i;
} ab_int;
// helper union for converting 4 bytes into one long value and vice versa
union ab_long {
    byte b[sizeof(int32_t)];
    int32_t l;
} ab_long;
// helper union for converting 4 bytes into one unsigned integer value and vice versa (not used in socket)
union ab_ulong {
    byte b[sizeof(uint32_t)];
    uint32_t ul;
} ab_ulong;

/**
 * Calculates the Checksum of the packet
 * @param data pointer do data buffer
 * @param datalen data length on which the crc shall be calculated
 * @return calculated crc value 
 */
uint16_t ab_calcCRC(char *data, size_t datalen)
{
    uint16_t crc = 0;
    size_t index = 0;
    while (index < datalen)
    {
        crc += static_cast<uint16_t>(((data[index] ^ 0x5A) * ab_PrimTable[index & 0x0F]) & 0xFFFF);
        index++;
    }
    return crc;
}

/**
 * extract a single boolean value out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos position of bool(ean) data in data buffer
 * @return bool(ean) data value
 */
bool ab_getBoolVal(char *data, size_t len, uint16_t pos)
{
    if (len >= pos)
    {
        return data[pos] > 0;
    }
    return false;
}

/**
 * extract 2 byte singed value (integer16) out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of integer data in data buffer
 * @return integer data value
 */
int16_t ab_getIntVal(char *data, size_t len, uint16_t pos)
{
    int16_t retval = 0;
    if (len >= pos + 1u)
    {
        retval = data[pos];
        retval += data[pos + 1] << 8;
    }
    return retval;
}

/**
 * extract 2 byte unsinged value (unsigned integer 16) out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of unsigned integer data in data buffer
 * @return unsigned integer data value
 */
uint16_t ab_getUIntVal(char *data, size_t len, uint16_t pos)
{
    uint16_t retval = 0;
    if (len >= pos + 1u)
    {
        retval = data[pos];
        retval += data[pos + 1] << 8;
    }
    return retval;
}

/**
 * extract 4 byte singed long value (integer32) out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of long data in data buffer
 * @return long data value
 */
int32_t ab_getLongVal(char *data, size_t len, uint16_t pos)
{
    int32_t retval = 0;
    if (len >= pos + 3u)
    {
        retval = data[pos];
        retval += data[pos + 1] << 8;
        retval += data[pos + 2] << 16;
        retval += data[pos + 3] << 24;
    }
    return retval;
}

/**
 * extract 4 byte unsigned long value (unsigned integer32) out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of unsigned long data in data buffer
 * @return unsigned long data value
 */
uint32_t ab_getULongVal(char *data, size_t len, uint16_t pos)
{
    uint32_t retval = 0;
    if (len >= pos + 3u)
    {
        retval = data[pos];
        retval += data[pos + 1] << 8;
        retval += data[pos + 2] << 16;
        retval += data[pos + 3] << 24;
    }
    return retval;
}

/**
 * extract 4 byte real / float value (float32) out of the packet
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @return real / float data value
 */
float_t ab_getRealVal(char *data, size_t len, uint16_t pos)
{
    ab_real.real = 0.0;
    if (len >= pos + 3u)
    {
        ab_real.bytes[0] = data[pos];
        ab_real.bytes[1] = data[pos + 1];
        ab_real.bytes[2] = data[pos + 2];
        ab_real.bytes[3] = data[pos + 3];
    }
    return ab_real.real;
}

/**
 * write 4 byte real / float value (float32) into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val real / float data value
 */
void ab_setRealVal(char *data, size_t len, uint16_t pos, float_t val)
{
    ab_real.real = val;
    if (len >= pos + 3u)
    {
        data[pos] = ab_real.bytes[0];
        data[pos + 1] = ab_real.bytes[1];
        data[pos + 2] = ab_real.bytes[2];
        data[pos + 3] = ab_real.bytes[3];
    }
}

/**
 * write bool(ean) value into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val bool(ean) data value
 */
void ab_setBoolVal(char *data, size_t len, uint16_t pos, bool val)
{
    if (len >= pos)
        data[pos] = (char)val;
}

/**
 * write integer value into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val integer data value
 */
void ab_setIntVal(char *data, size_t len, uint16_t pos, int16_t val)
{
    if (len >= pos + 1u)
    {
        data[pos] = (char)(val & 0xFF);
        data[pos + 1] = (char)(val >> 8);
    }
}

/**
 * write unsigned integer value into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val unsigned integer data value
 */
void ab_setUIntVal(char *data, size_t len, uint16_t pos, uint16_t val)
{
    if (len >= pos + 1u)
    {
        data[pos] = (char)(val & 0xFF);
        data[pos + 1] = (char)(val >> 8);
    }
}

/**
 * write long value into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val long data value
 */
void ab_setLongVal(char *data, size_t len, uint16_t pos, int32_t val)
{
    if (len >= pos + 3u)
    {
        ab_long.l = val;
        data[pos] = ab_long.b[0];
        data[pos + 1] = ab_long.b[1];
        data[pos + 2] = ab_long.b[2];
        data[pos + 3] = ab_long.b[3];
    }
}

/**
 * write unsigned long value into the packet buffer
 * @param data pointer to data buffer
 * @param len maximum length of data buffer
 * @param pos start position of real / float data in data buffer
 * @param val unsigned long data value
 */
void ab_setULongVal(char *data, size_t len, uint16_t pos, uint32_t val)
{
    if (len >= pos + 3u)
    {
        ab_ulong.ul = val;
        data[pos] = ab_ulong.b[0];
        data[pos + 1] = ab_ulong.b[1];
        data[pos + 2] = ab_ulong.b[2];
        data[pos + 3] = ab_ulong.b[3];
    }
}

/**
 * checks the received packet if it is valid
 * @param data pointer to data buffer
 * @param datalen length of (received) data packet
 */
bool ab_checkValidPacket(char *data, size_t datalen)
{
    if (datalen > 12 + 2)
    {
        // first check for valid header
        if (data[0] != 0xAA || data[1] != 0x55)
        {
            ABUS_ERR_PRINTLN(F("*AB: checkValidPacket()-> invalid header!"));
            return false;
        }
        // check for valid datalength
        if (datalen != (ab_getUIntVal(data, datalen, 2) + 14u))
        {
            ABUS_ERR_PRINTLN(F("*AB: checkValidPacket()-> invalid packet length!"));
            return false;
        }
        // check for falid checksum
        if (ab_calcCRC(data, datalen - 2) != ab_getUIntVal(data, datalen, datalen - 2))
        {
            ABUS_ERR_PRINTLN(F("*AB: checkValidPacket()-> invalid crc!"));
            return false;
        }
        return true;
    }
    ABUS_ERR_PRINTLN(F("*AB: checkValidPacket()-> length too short!"));
    return false;
}

/**
 * extract the header out of the received data
 * @param data pointer to data buffer
 * @param datalen maximum data length of data buffer
 */
ab_header ab_getHeader(char *data, size_t datalen)
{
    ab_header retval;
    retval.len = ab_getUIntVal(data, datalen, 2);
    retval.from = ab_getULongVal(data, datalen, 4);
    retval.to = ab_getULongVal(data, datalen, 8);
    if (retval.len > 2u)
        retval.dir = data[12];
    if (retval.len > 3u)
        retval.typ = data[13];
    if (retval.len > 4u)
        retval.ts_id = ab_getUIntVal(data, datalen, datalen - 4);
    return retval;
}

/**
 * generate header into char array
 * @param data pointer to data buffer
 * @param len maximum data length of data buffer
 * @param header abus header structure which sall be added to data buffer
 */
void ab_setHeader(char *data, size_t len, ab_header header)
{
    if (len > header.len + 14u)
    {
        ABUS_DBG_PRINTF("*AB: setHeader()->len=%d, from=%u, to=%u\n", header.len, header.from, header.to);
        data[0] = 0xAA;
        data[1] = 0x55;
        ab_setUIntVal(data, len, 2, header.len);
        ab_setULongVal(data, len, 4, header.from);
        ab_setULongVal(data, len, 8, header.to);
        data[12] = header.dir;
        data[13] = header.typ;
        ab_setUIntVal(data, len, header.len + 10u, header.ts_id);
    }
}

/**
 * with this function it is possible to parse a received package for valid socket data
 * @param data pointer to data buffer
 * @param datalen maximum data length of data buffer
 * @param header pointer to abus socket header
 * @param sock_id socket id no
 * @param bitcount amount of bool(ean) tags in socket
 * @param intcount amount of int tags in socket
 * @param longcount amount of long tags in socket
 * @param realcount amount of real tags in socket
 * @return parsed abus socket (empty / null if no socket)
 */
ab_socket ab_getSocket(char *data, size_t datalen, ab_header &header, uint8_t sock_id = 0, uint8_t bitcount = 0, uint8_t intcount = 0, uint8_t longcount = 0, uint8_t realcount = 0)
{
    ab_socket retval;
    // check if we had a socket message
    if (((header.typ > 0 && sock_id == 0) || (header.typ == sock_id)) && header.dir == 1)
    {
        //read the socket id
        retval.config.socket_id = header.typ;
        retval.sender = header.from;
        ABUS_DBG_PRINTF("*AB: getSocket()->id=%d, sender=%u, header.len=%d", retval.config.socket_id, retval.sender, header.len);
        uint16_t pos = 14;
        // check for valid length of data block
        if (header.len != (bitcount + intcount * 2 + longcount * 4 + realcount * 4 + 4))
        {
            ABUS_ERR_PRINTLN(F("*AB: getSocket()->amount of variables wrong!"));
            return retval;
        }
        uint8_t slotpos = 0;
        //retval.bitdata.resize(bitcount, false);
        retval.bitdata.resize(bitcount, false);
        while (bitcount > slotpos)
        {
            retval.bitdata[slotpos] = data[pos] > 0;
            ABUS_DBG_PRINTF(", b%d=%d", slotpos, retval.bitdata[slotpos]);
            pos++;
            slotpos++;
        }
        slotpos = 0;
        retval.intdata.resize(intcount);
        while (intcount > slotpos)
        {
            //retval.intdata.push_back(ab_getIntVal(data, datalen, pos));
            retval.intdata[slotpos] = ab_getIntVal(data, datalen, pos);
            ABUS_DBG_PRINTF(", i%d=%d", slotpos, retval.intdata[slotpos]);
            pos += 2;
            slotpos++;
        }
        slotpos = 0;
        retval.longdata.resize(longcount);
        while (longcount > slotpos)
        {
            //retval.longdata.push_back(ab_getLongVal(data, datalen, pos));
            retval.longdata[slotpos] = ab_getLongVal(data, datalen, pos);
            ABUS_DBG_PRINTF(", i%d=%d", slotpos, retval.longdata[slotpos]);
            pos += 4;
            slotpos++;
        }
        slotpos = 0;
        retval.realdata.resize(realcount);
        while (realcount > slotpos)
        {
            //retval.realdata.push_back(ab_getRealVal(data, datalen, pos));
            retval.realdata[slotpos] = ab_getRealVal(data, datalen, pos);
            ABUS_DBG_PRINTF(", r%d=%.1f", slotpos, retval.realdata[slotpos]);
            pos += 4;
            slotpos++;
        }
        retval.config.bitcount  = bitcount;
        retval.config.intcount = intcount;
        retval.config.longcount = longcount;
        retval.config.realcount = realcount;
        retval.config.socket_id = sock_id;
        retval.socket_valid = true;
        ABUS_DBG_PRINTLN("");
    }
    return retval;
}

/**
 * with this function it is possible to parse a received package for valid socket data
 * @param data pointer to data buffer
 * @param datalen maximum data length of data buffer
 * @param header pointer to abus socket header
 * @param sock_conf structure with socket configuraton
 * @return parsed abus socket (empty / null if no socket)
 */
ab_socket ab_getSocket(char *data, size_t datalen, ab_header &header, ab_socket_config sock_conf)
{
    return ab_getSocket(data, datalen, header, sock_conf.socket_id, sock_conf.bitcount, sock_conf.intcount, sock_conf.longcount, sock_conf.realcount);
}

/**
 * add all socket values into the data buffer
 * @param data pointer to data buffer
 * @param datalen maximum data buffer length
 * @param socket socket with tag data
 */
void ab_setSocket(char *data, size_t datalen, ab_socket socket)
{
    ABUS_DBG_PRINTF("*AB: setSocket()->id=%d, sender=%d", socket.config.socket_id, socket.sender);
    uint16_t pos = 14;
    uint8_t slotpos = 0;
    while (socket.bitdata.size() > slotpos)
    {
        ABUS_DBG_PRINTF(", b%d=%d", slotpos, socket.bitdata[slotpos]);
        data[pos] = socket.bitdata[slotpos];
        pos++;
        slotpos++;
    }
    slotpos = 0;
    while (socket.intdata.size() > slotpos)
    {
        ABUS_DBG_PRINTF(", i%d=%d", slotpos, socket.intdata[slotpos]);
        ab_setIntVal(data, datalen, pos, socket.intdata[slotpos]);
        pos += 2;
        slotpos++;
    }
    slotpos = 0;
    while (socket.longdata.size() > slotpos)
    {
        ABUS_DBG_PRINTF(", l%d=%d", slotpos, socket.longdata[slotpos]);
        ab_setLongVal(data, datalen, pos, socket.longdata[slotpos]);
        pos += 4;
        slotpos++;
    }
    slotpos = 0;
    while (socket.realdata.size() > slotpos)
    {
        ABUS_DBG_PRINTF(", f%d=%f", slotpos, socket.realdata[slotpos]);
        ab_setRealVal(data, datalen, pos, socket.realdata[slotpos]);
        pos += 4;
        slotpos++;
    }
    ABUS_DBG_PRINTLN("");
}

/**
 * add a bool(ean) value to a socket
 * @param socket pointer to socket structure
 * @param value value which shall be added
 */
void ab_addBitTag(ab_socket *socket, bool value)
{
    size_t pos = socket->bitdata.size() + 1;
    socket->bitdata.resize(pos, value);
    socket->config.bitcount = pos;
}

/**
 * add a integer value to a socket
 * @param socket pointer to socket structure
 * @param value value which shall be added
 */
void ab_addIntTag(ab_socket *socket, int16_t value)
{
    size_t pos = socket->intdata.size() + 1;
    socket->intdata.resize(pos, value);
    socket->config.intcount = pos;
}

/**
 * add a long value to a socket
 * @param socket pointer to socket structure
 * @param value value which shall be added
 */
void ab_addLongTag(ab_socket *socket, int32_t value)
{
    size_t pos = socket->longdata.size() + 1;
    socket->longdata.resize(pos, value);
    socket->config.longcount = pos;
}

/**
 * add a real value to a socket
 * @param socket pointer to socket structure
 * @param value value which shall be added
 */
void ab_addRealTag(ab_socket *socket, float_t value)
{
    if(isnan(value))
        value = 0.0;
    size_t pos = socket->realdata.size() + 1;
    socket->realdata.resize(pos, value);
    socket->config.realcount = pos;
}

#endif