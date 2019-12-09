#include "LogStream.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <limits>

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

template<typename T>
size_t convert(char buff[], T value) {
    T i = value;
    char* p = buf;

    do {
      int lsd = static_cast<int>(i % 10);
      i /= 10;
      *p++ = zero[lsd];
    } while (i != 0);
    
}