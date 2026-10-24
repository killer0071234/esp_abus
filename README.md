# esp_abus

[![Arduino Library Manager: esp_abus](https://www.ardu-badge.com/badge/esp_abus.svg)](https://www.ardu-badge.com/esp_abus)
[![PlatformIO Library Manager: esp_abus](https://img.shields.io/badge/Library%20Manager-esp_abus%20v0.1.0-success?logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB3aWR0aD0iMjUwMCIgaGVpZ2h0PSIyNTAwIiB2aWV3Qm94PSIwIDAgMjU2IDI1NiIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiBwcmVzZXJ2ZUFzcGVjdFJhdGlvPSJ4TWlkWU1pZCI+PHBhdGggZD0iTTEyOCAwQzkzLjgxIDAgNjEuNjY2IDEzLjMxNCAzNy40OSAzNy40OSAxMy4zMTQgNjEuNjY2IDAgOTMuODEgMCAxMjhjMCAzNC4xOSAxMy4zMTQgNjYuMzM0IDM3LjQ5IDkwLjUxQzYxLjY2NiAyNDIuNjg2IDkzLjgxIDI1NiAxMjggMjU2YzM0LjE5IDAgNjYuMzM0LTEzLjMxNCA5MC41MS0zNy40OUMyNDIuNjg2IDE5NC4zMzQgMjU2IDE2Mi4xOSAyNTYgMTI4YzAtMzQuMTktMTMuMzE0LTY2LjMzNC0zNy40OS05MC41MUMxOTQuMzM0IDEzLjMxNCAxNjIuMTkgMCAxMjggMCIgZmlsbD0iI0ZGN0YwMCIvPjxwYXRoIGQ9Ik0yNDkuMzg2IDEyOGMwIDY3LjA0LTU0LjM0NyAxMjEuMzg2LTEyMS4zODYgMTIxLjM4NkM2MC45NiAyNDkuMzg2IDYuNjEzIDE5NS4wNCA2LjYxMyAxMjggNi42MTMgNjAuOTYgNjAuOTYgNi42MTQgMTI4IDYuNjE0YzY3LjA0IDAgMTIxLjM4NiA1NC4zNDYgMTIxLjM4NiAxMjEuMzg2IiBmaWxsPSIjRkZGIi8+PHBhdGggZD0iTTE2MC44NjkgNzQuMDYybDUuMTQ1LTE4LjUzN2M1LjI2NC0uNDcgOS4zOTItNC44ODYgOS4zOTItMTAuMjczIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzJzLTEwLjMyIDQuNjItMTAuMzIgMTAuMzJjMCAzLjc1NSAyLjAxMyA3LjAzIDUuMDEgOC44MzdsLTUuMDUgMTguMTk1Yy0xNC40MzctMy42Ny0yNi42MjUtMy4zOS0yNi42MjUtMy4zOWwtMi4yNTggMS4wMXYxNDAuODcybDIuMjU4Ljc1M2MxMy42MTQgMCA3My4xNzctNDEuMTMzIDczLjMyMy04NS4yNyAwLTMxLjYyNC0yMS4wMjMtNDUuODI1LTQwLjU1NS01Mi4xOTd6TTE0Ni41MyAxNjQuOGMtMTEuNjE3LTE4LjU1Ny02LjcwNi02MS43NTEgMjMuNjQzLTY3LjkyNSA4LjMyLTEuMzMzIDE4LjUwOSA0LjEzNCAyMS41MSAxNi4yNzkgNy41ODIgMjUuNzY2LTM3LjAxNSA2MS44NDUtNDUuMTUzIDUxLjY0NnptMTguMjE2LTM5Ljc1MmE5LjM5OSA5LjM5OSAwIDAgMC05LjM5OSA5LjM5OSA5LjM5OSA5LjM5OSAwIDAgMCA5LjQgOS4zOTkgOS4zOTkgOS4zOTkgMCAwIDAgOS4zOTgtOS40IDkuMzk5IDkuMzk5IDAgMCAwLTkuMzk5LTkuMzk4em0yLjgxIDguNjcyYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDkgMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OXoiIGZpbGw9IiNFNTcyMDAiLz48cGF0aCBkPSJNMTAxLjM3MSA3Mi43MDlsLTUuMDIzLTE4LjkwMWMyLjg3NC0xLjgzMiA0Ljc4Ni01LjA0IDQuNzg2LTguNzAxIDAtNS43LTQuNjItMTAuMzItMTAuMzItMTAuMzItNS42OTkgMC0xMC4zMTkgNC42Mi0xMC4zMTkgMTAuMzIgMCA1LjY4MiA0LjU5MiAxMC4yODkgMTAuMjY3IDEwLjMxN0w5NS44IDc0LjM3OGMtMTkuNjA5IDYuNTEtNDAuODg1IDIwLjc0Mi00MC44ODUgNTEuODguNDM2IDQ1LjAxIDU5LjU3MiA4NS4yNjcgNzMuMTg2IDg1LjI2N1Y2OC44OTJzLTEyLjI1Mi0uMDYyLTI2LjcyOSAzLjgxN3ptMTAuMzk1IDkyLjA5Yy04LjEzOCAxMC4yLTUyLjczNS0yNS44OC00NS4xNTQtNTEuNjQ1IDMuMDAyLTEyLjE0NSAxMy4xOS0xNy42MTIgMjEuNTExLTE2LjI4IDMwLjM1IDYuMTc1IDM1LjI2IDQ5LjM2OSAyMy42NDMgNjcuOTI2em0tMTguODItMzkuNDZhOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTkgOS4zOTggOS4zOTkgOS4zOTkgMCAwIDAgOS40IDkuNCA5LjM5OSA5LjM5OSAwIDAgMCA5LjM5OC05LjQgOS4zOTkgOS4zOTkgMCAwIDAtOS4zOTktOS4zOTl6bS0yLjgxIDguNjcxYTIuMzc0IDIuMzc0IDAgMSAxIDAtNC43NDggMi4zNzQgMi4zNzQgMCAwIDEgMCA0Ljc0OHoiIGZpbGw9IiNGRjdGMDAiLz48L3N2Zz4=)](https://platformio.org/lib/show/11240/esp_abus)
[![Build Status](https://github.com/killer0071234/esp_abus/workflows/LibraryBuild/badge.svg)](https://github.com/killer0071234/esp_abus/actions)

ABUS socket communication between ESP32 / ESP8266 and Cybro-2 / Cybro-3 PLCs from Cybrotech over WiFi.

## Description

The PLC and the ESP exchange data with so called ABUS sockets: UDP broadcast packets (port 8442) with a fixed set of tags (variables). The ESP can send sockets to the PLC and receive sockets from it.

For the examples, add a socket in the CyPro environment with this configuration: socket id 3, one bit tag, one int tag, one long tag and one real tag.

Supported boards:

- ESP8266
- ESP32, including newer chips like the ESP32-C6. On PlatformIO, these need arduino-esp32 3.x, e.g. from the [pioarduino](https://github.com/pioarduino/platform-espressif32) platform.

## Installation

- **Arduino IDE:** install `esp_abus` with the Library Manager.
- **PlatformIO:** add the library to `platformio.ini`:

  ```ini
  lib_deps = killer0071234/esp_abus
  ```

## Quick start

```cpp
#include <abus_socket.h>

abus_socket abSock;

// called for every received socket with id 3 and 1 bit, 1 int, 1 long and 1 real tag
void onSocket(const ab_socket &sock)
{
    Serial.println(sock.realdata[0]);
}

void setup()
{
    Serial.begin(115200);
    // ... connect to WiFi first ...
    abSock.begin();
    abSock.setSocketCallback(3, 1, 1, 1, 1, onSocket);
}

void loop()
{
    abSock.loop();  // handles received packets, call it as often as possible

    // send a socket with id 3 to the PLC
    ab_socket sock;
    sock.config.socket_id = 3;
    ab_addBitTag(&sock, true);
    ab_addIntTag(&sock, 123);
    ab_addLongTag(&sock, 123456);
    ab_addRealTag(&sock, 12.3);
    abSock.sendSocket(sock);
    delay(10000);
}
```

The complete sketches are in the [examples](examples) folder.

## API

### `abus_socket`

| Function | Description |
|---|---|
| `abus_socket()` | Port 8442, NAD derived from the MAC address (see below). |
| `abus_socket(uint32_t NAD)` | Port 8442 with a fixed NAD. |
| `abus_socket(uint16_t localUdpPort, uint32_t NAD)` | Different port and fixed NAD. `0` as NAD means "derive from the MAC". |
| `void begin()` | Starts the UDP socket. Call it after WiFi is connected. |
| `void begin(uint16_t localUdpPort)` | Same, with a different port. |
| `void begin(IPAddress bCastIP)` | Same, with a fixed broadcast address for sending. Without it, the broadcast address is calculated from the current WiFi IP for every packet. |
| `void begin(IPAddress bCastIP, uint16_t localUdpPort)` | Both of the above. |
| `void loop()` | Reads one received packet and calls the matching callback. Call it from `loop()`. |
| `void sendSocket(const ab_socket &socket)` | Sends a socket as broadcast. Socket id and at least one tag are required. If `socket.sender` is 0, the own NAD is used. |
| `void sendRaw(char *data, size_t datalen)` | Sends an already built packet. |
| `uint8_t setSocketCallback(uint8_t sock_id, uint8_t bitcount, uint8_t intcount, uint8_t longcount, uint8_t realcount, callback)` | Calls `callback` for every received socket with this id and exactly these tag counts. Returns a handle (1–32), or 0 if all slots are in use. |
| `uint8_t setSocketCallback(ab_socket_config config, callback)` | Same, with the configuration in a struct. |
| `bool removeSocketCallback(uint8_t handle)` | Removes a callback. Returns `false` if the handle does not exist. |

A callback can be a function `void cb(const ab_socket &sock)`, a function taking `ab_socket` by value (works, but copies the socket), or a lambda, e.g. `[&](const ab_socket &sock) { ... }`. If several callbacks match a received socket, only the first one is called.

### `ab_socket`

| Member | Description |
|---|---|
| `config.socket_id` | Socket id (1–255). |
| `config.bitcount`, `intcount`, `longcount`, `realcount` | Number of tags per type. |
| `sender` | NAD of the sender. |
| `socket_valid` | `true` for a correctly received socket. |
| `bitdata` | `std::vector<uint8_t>`, bit tags (0 / 1). |
| `intdata` | `std::vector<int16_t>`, int tags. |
| `longdata` | `std::vector<int32_t>`, long tags. |
| `realdata` | `std::vector<float_t>`, real tags. |

To build a socket for sending, use `ab_addBitTag(&sock, bool)`, `ab_addIntTag(&sock, int16_t)`, `ab_addLongTag(&sock, int32_t)` and `ab_addRealTag(&sock, float)`; they also update the tag counts. A NaN real is sent as 0.

The tags are always transferred in the order bit, int, long, real, so the socket in the PLC needs the same number of tags per type.

### NAD (sender address)

Without a fixed NAD, the NAD is built from the last 4 bytes of the MAC address, in reading order: MAC `…:AB:12:34:56` → NAD `0xAB123456`. On the PLC the NAD only shows who sent the socket.

## Limits

- Up to 32 socket callbacks (change with `ABSOCK_MAX_SOCKETS`).
- A packet is at most 255 bytes, so a socket can carry at most 237 bytes of tag data: bit = 1 byte, int = 2 bytes, long = 4 bytes, real = 4 bytes. Larger sockets are not sent, and an error is printed.

## Configuration

These options can be set with `build_flags` in PlatformIO, or with a `#define` in the sketch **before** `#include <abus_socket.h>`:

| Option | Default | Description |
|---|---|---|
| `ABSOCK_DEBUG` | off | Debug output of `abus_socket` (every sent and received packet). |
| `ABUS_DEBUG` | off | Debug output of the packet helper functions (every tag). |
| `ABSOCK_NO_ERROR`, `ABUS_NO_ERROR` | errors are printed | Turn off the error output. |
| `ABSOCK_DBG_PRINTER`, `ABUS_DBG_PRINTER` | `Serial` | Where the output is printed. |
| `ABSOCK_MAX_SOCKETS` | 32 | Number of callback slots. |

Example for PlatformIO:

```ini
build_flags = -DABSOCK_DEBUG
```

## Upgrading to 0.1.0

- Debug output is now off by default. Turn it on with `ABSOCK_DEBUG` (see above).
- The global helper variables `ab_real`, `ab_int`, `ab_long` and `ab_ulong` are removed. The union types still exist.
- Existing callbacks with `ab_socket` by value keep working. `const ab_socket &` avoids a copy.

## Development

Unit tests for the packet encoding run on the PC:

```
pio test -e native
```

## License

MIT, see [LICENSE](LICENSE).
