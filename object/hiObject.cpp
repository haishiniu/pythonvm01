#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/functionObject.hpp"

ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass::ObjectKlass() {
    set_super(NULL);
}

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == NULL)
        instance = new ObjectKlass();

    return instance;
}


void HiObject::print() {
    klass()->print(this);  // this <=> self
}

HiObject::HiObject() {
    _obj_dict = NULL;
}

HiObject* HiObject::greater(HiObject * rhs) {
    return klass()->greater(this, rhs);
}

HiObject* HiObject::add(HiObject * rhs) {
    return klass()->add(this, rhs);
}

HiObject* HiObject::sub(HiObject * rhs) {
    return klass()->sub(this, rhs);
}

HiObject* HiObject::mul(HiObject * rhs) {
    return klass()->mul(this, rhs);
}

HiObject* HiObject::div(HiObject * rhs) {
    return klass()->div(this, rhs);
}

HiObject* HiObject::mod(HiObject * rhs) {
    return klass()->mod(this, rhs);
}

HiObject* HiObject::less(HiObject * rhs) {
    return klass()->less(this, rhs);
}

HiObject* HiObject::equal(HiObject * rhs) {
    return klass()->equal(this, rhs);
}

HiObject* HiObject::not_equal(HiObject * rhs) {
    return klass()->not_equal(this, rhs);
}

HiObject* HiObject::ge(HiObject * rhs) {
    return klass()->ge(this, rhs);
}

HiObject* HiObject::le(HiObject * rhs) {
    return klass()->le(this, rhs);
}

HiObject* HiObject::getattr(HiObject* x) {
    HiObject* result = Universe::HiNone;

    if (_obj_dict != NULL) {
        result = _obj_dict->get(x);
        if (result != Universe::HiNone)
            return result;
    }

    result = klass()->klass_dict()->get(x);

    if (result == Universe::HiNone)
        return result;

    // Only klass attribute needs bind.
    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result, this);
    }
    return result;
}

HiObject* HiObject::setattr(HiObject* x, HiObject* y) {
    
    if (klass() == TypeKlass::get_instance()) {
        HiTypeObject* type_obj = (HiTypeObject*)this;
        type_obj->own_klass()->klass_dict()->put(x, y);
        return Universe::HiNone;
    }

    if (!_obj_dict)
        _obj_dict = new HiDict();

    _obj_dict->put(x, y);
    return Universe::HiNone;
}


HiObject* HiObject::subscr(HiObject* x) {
    return klass()->subscr(this, x);
}

void HiObject::store_subscr(HiObject* x, HiObject* y) {
    klass()->store_subscr(this, x, y);
}

HiObject* HiObject::contains(HiObject* x) {
    return klass()->contains(this, x);
}

void HiObject::del_subscr(HiObject* x) {
    klass()->del_subscr(this, x);
}

HiObject* HiObject::iter() {
    return klass()->iter(this);
}

/*
 * TypeObject is a special object
 */
TypeKlass* TypeKlass::instance = NULL;

TypeKlass* TypeKlass::get_instance() {
    if (instance == NULL)
        instance = new TypeKlass();

    return instance;
}

void TypeKlass::print(HiObject* obj) {
    assert(obj->klass() == (Klass*) this);
    printf("<type ");
    Klass* own_klass = ((HiTypeObject*)obj)->own_klass();

    HiDict* attr_dict = own_klass->klass_dict();
    if (attr_dict) {
        HiObject* mod = attr_dict->get((HiObject*)
                StringTable::get_instance()->mod_str);
        if (mod != Universe::HiNone) {
            mod->print();
            printf(".");
        }
    }

    own_klass->name()->print();
    printf(">");
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    // Klass to HiObject: one to one
    _own_klass = k; 
    k->set_type_object(this);
}
