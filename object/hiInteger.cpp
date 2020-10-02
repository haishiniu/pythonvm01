#include "object/hiInteger.hpp"
#include <stdio.h>
#include <stdio.h>

// 初始化 HiInteger
HiInteger::HiInteger(int x) {
    _value = x;
}

void HiInteger::print() {
    printf("%d", _value);
}

HiObject* HiInteger::add(HiObject* x) {
    // ((HiInteger*)x) ？ 类型强制转化？
    return new HiInteger(_value + ((HiInteger*)x)->_value);
}

HiObject* HiInteger::greater(HiObject* x) {
    if (_value > ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}

HiObject* HiInteger::less(HiObject* x) {
    if (_value < ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}

HiObject* HiInteger::le(HiObject* x) {
    if (_value <= ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}

HiObject* HiInteger::ge(HiObject* x) {
    if (_value >= ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}

HiObject* HiInteger::equal(HiObject* x) {
    if (_value == ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}

HiObject* HiInteger::not_equal(HiObject* x) {
    if (_value != ((HiInteger*)x)->_value)
        return new HiInteger(1);
    else
        return new HiInteger(0);
}
