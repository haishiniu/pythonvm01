#include "object/hiString.hpp"
#include <string.h>

HiString::HiString(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);    

}

HiString::HiString(const char* x, const int length) {
    _length = length;
    _value = new char[length];

    for (int i = 0; i < length; i++) {
        _value[i] = x[i];
    }
}