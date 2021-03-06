#include "runtime/stringTable.hpp"
#include "object/hiString.hpp"
#include "memory/oopClosure.hpp"


StringTable* StringTable::instance = NULL;

StringTable* StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

StringTable::StringTable() {
    next_str = new HiString("next");
    mod_str = new HiString("__module__");
    init_str = new HiString("__init__");
    add_str  = new HiString("__add__");
    len_str  = new HiString("__len__");
    call_str = new HiString("__call__");
    name_str = new HiString("__name__");
    getitem_str = new HiString("__getitem__");
    setitem_str = new HiString("__setitem__");
    getattr_str = new HiString("__getattr__");
    setattr_str = new HiString("__setattr__");
}

void StringTable::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&next_str);
    f->do_oop((HiObject**)&mod_str);
    f->do_oop((HiObject**)&init_str);
    f->do_oop((HiObject**)&add_str);
    f->do_oop((HiObject**)&len_str);
    f->do_oop((HiObject**)&call_str);
    f->do_oop((HiObject**)&name_str);
    f->do_oop((HiObject**)&getitem_str);
    f->do_oop((HiObject**)&setitem_str);
    f->do_oop((HiObject**)&setattr_str);
    f->do_oop((HiObject**)&getattr_str);
}


