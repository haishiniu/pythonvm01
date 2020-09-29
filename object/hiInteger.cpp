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