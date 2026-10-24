// minimal Arduino.h replacement to compile abus_helper.h on the host
#pragma once
#include <cmath>
#include <cstddef>
#include <cstdint>

using std::isnan;
typedef uint8_t byte;
#define F(x) x

struct FakeSerial
{
    template <class T> void print(T) {}
    template <class T> void println(T) {}
    template <class... A> void printf(A...) {}
};
static FakeSerial Serial;
