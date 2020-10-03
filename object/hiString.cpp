#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"
#include <string.h>


StringKlass* StringKlass::instance = NULL;

StringKlass* StringKlass::get_instance() {
    if (instance == NULL)
        instance = new StringKlass();

    return instance;
}

StringKlass::StringKlass() {
}

HiObject* StringKlass::equal(HiObject* x, HiObject* y) {
    if (x->klass() != y->klass())
        return Universe::HiFalse;

    HiString* sx = (HiString*) x;
    HiString* sy = (HiString*) y;

    assert(sx && sx->klass() == (Klass*)this);
    assert(sy && sy->klass() == (Klass*)this);

    if (sx->length() != sy->length())
        return Universe::HiFalse;

    for (int i = 0; i < sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i])
            return Universe::HiFalse;
    }

    return Universe::HiTrue;
}

void StringKlass::print(HiObject* obj) {
    HiString* str_obj = (HiString*) obj;
    assert(str_obj && str_obj->klass() == (Klass*)this);

    for (int i = 0; i < str_obj->length(); i++) {
        printf("%c", str_obj->value()[i]);
    }
}

HiObject* StringKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == (Klass*) this);
    assert(y && y->klass() == (Klass*) IntegerKlass::get_instance());

    HiString * sx = (HiString*)x;
    HiInteger* iy = (HiInteger*)y;
    
    return new HiString(&(sx->value()[iy->value()]), 1);
}

HiString::HiString(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);  
    set_klass(StringKlass::get_instance());  

}

HiString::HiString(const char* x, const int length) {
    _length = length;
    _value = new char[length];

    for (int i = 0; i < length; i++) {
        _value[i] = x[i];
    }
    set_klass(StringKlass::get_instance());
}

