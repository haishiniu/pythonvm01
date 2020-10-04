#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"
#include "object/hiString.hpp"


// __init__ 
HiObject* Universe::HiTrue   = NULL;
HiObject* Universe::HiFalse  = NULL;
HiObject*  Universe::HiNone   = NULL;


void Universe::genesis() {
    HiTrue       = new HiInteger(1);
    HiFalse      = new HiInteger(0);
    HiNone       = new HiObject();

    HiDict* klass_dict = new HiDict();  // because of --> typedef Map<HiObject*, HiObject*> HiDict;
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    StringKlass::get_instance()->set_name(new HiString("str"));
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));
}

void Universe::destroy() {
}