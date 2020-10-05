#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/interpreter.hpp"

#define ST(x) StringTable::get_instance()->STR(x)
#define STR(x) x##_str


int Klass::compare_klass(Klass* x, Klass* y) {
    if (x == y)
        return 0;

    if (x == IntegerKlass::get_instance())
        return -1;
    else if (y == IntegerKlass::get_instance())
        return 1;

    if (x->name()->less(y->name()) == (HiObject*)Universe::HiTrue)
        return -1;
    else
        return 1;
}

// void Klass::print(HiObject* x) {
//     printf("<object of ");
//     x->klass()->name()->print();
//     printf(", at %p>", x);
// }

HiObject* Klass::create_klass(HiObject* x, HiObject* supers, HiObject* name) {
    assert(x->klass()      == (Klass*)DictKlass::get_instance());
    assert(supers->klass() == (Klass*)ListKlass::get_instance());
    assert(name->klass()   == (Klass*)StringKlass::get_instance());

    Klass* new_klass   = new Klass();
    HiDict* klass_dict = (HiDict*) x;
    HiList* supers_list = (HiList*) supers;
    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name((HiString*)name);
    if (supers_list->inner_list()->length() > 0) {
        HiTypeObject* super = (HiTypeObject*)supers_list->inner_list()->get(0);
        new_klass->set_super(super->own_klass());
    }

    HiTypeObject* type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);
    
    return type_obj;
}

HiObject* Klass::allocate_instance(HiObject* callable, ArrayList<HiObject*>* args) {
    HiObject* inst = new HiObject();
    inst->set_klass(((HiTypeObject*)callable)->own_klass());
    HiObject* constructor = inst->getattr(StringTable::get_instance()->init_str);
    if (constructor != Universe::HiNone) {
        Interpreter::get_instance()->call_virtual(constructor, args);
    }

    return inst;
}

HiObject* Klass::add(HiObject* lhs, HiObject* rhs) {
    ObjList args = new ArrayList<HiObject*>();
    args->add(rhs);
    return find_and_call(lhs, args, ST(add));
}

HiObject* Klass::find_and_call(HiObject* lhs, ObjList args, HiObject* func_name) {
    HiObject* func = lhs->getattr(func_name);
    if (func != Universe::HiNone) {
        if (!args)
            args = new ArrayList<HiObject*>();

        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class ");
    lhs->klass()->name()->print();
    printf(" Error : unsupport operation for class ");
    assert(false);
    return Universe::HiNone;
}
