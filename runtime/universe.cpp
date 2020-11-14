#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "runtime/module.hpp"
#include "runtime/interpreter.hpp"
#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"



// __init__ 
HiObject* Universe::HiTrue   = NULL;
HiObject* Universe::HiFalse  = NULL;
HiObject*  Universe::HiNone   = NULL;
ArrayList<Klass*>* Universe::klasses   = NULL;

Heap* Universe::heap          = NULL;
CodeObject* Universe::main_code = NULL;


void Universe::genesis() {
    heap = Heap::get_instance();
    klasses = new ArrayList<Klass*>();

    HiTrue       = new HiString("True");
    HiFalse      = new HiString("False");
    HiNone       = new HiString("None");
    
    // klasses = new ArrayList<Klass*>();  // 可以有但是从执行效果看没有也可以不影响后续的执行

    Klass* object_klass = ObjectKlass::get_instance();
    Klass* type_klass   = TypeKlass::get_instance();

    HiTypeObject* tp_obj = new HiTypeObject();
    tp_obj->set_own_klass(type_klass);  // tp_obj对象的 _own_klass 与 _klass 是同一个 type_klass; self create self

    HiTypeObject* obj_obj = new HiTypeObject();
    obj_obj->set_own_klass(object_klass);  // obj_obj 对象的 _own_klass 是 object_klass; _klass 是 type_klass; type_klass create object_klass

    type_klass->add_super(object_klass);  // type_klass 的父类 是 object_klass
    // do nothing for object klass
    //object_klass->add_super(NULL);

    IntegerKlass::get_instance()->initialize(); // 继承 object_klass
    StringKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
    ModuleKlass::get_instance()->initialize();


    type_klass->set_klass_dict(new HiDict());
    object_klass->set_klass_dict(new HiDict());

    type_klass->set_name(new HiString("type"));
    object_klass->set_name(new HiString("object"));

    IntegerKlass::get_instance()->order_supers();
    StringKlass::get_instance()->order_supers();
    DictKlass::get_instance()->order_supers();
    ListKlass::get_instance()->order_supers();
    type_klass->order_supers();

    FunctionKlass::get_instance()->order_supers();
    NativeFunctionKlass::get_instance()->order_supers();
    MethodKlass::get_instance()->order_supers();

    ModuleKlass::get_instance()->order_supers();

    Interpreter::get_instance()->initialize();

}

void Universe::destroy() {
}

void Universe::oops_do(OopClosure* closure) {
    closure->do_oop((HiObject**)&HiTrue);
    closure->do_oop((HiObject**)&HiFalse);
    closure->do_oop((HiObject**)&HiNone);

    closure->do_oop((HiObject**)&main_code);
    closure->do_array_list(&klasses);
}
