#ifndef _HI_INTEGER_HPP
#define _HI_INTEGER_HPP
#include "object/hiObject.hpp"

class HiInteger : public HiObject {
private:
    int _value;

public:
    HiInteger(int x);
    int value() { return _value; } // 通过 value 方法 来获取实例私有变量的值

    virtual void print();
    virtual HiObject* add(HiObject* x);

    virtual HiObject* greater  (HiObject* x);
    virtual HiObject* less     (HiObject* x);
    virtual HiObject* equal    (HiObject* x);
    virtual HiObject* not_equal(HiObject* x);
    virtual HiObject* ge       (HiObject* x);
    virtual HiObject* le       (HiObject* x);
};

#endif