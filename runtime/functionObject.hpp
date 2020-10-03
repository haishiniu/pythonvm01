#ifndef HI_FUNCTION_OBJECT_HPP
#define HI_FUNCTION_OBJECT_HPP

#include "code/codeObject.hpp"
#include "object/klass.hpp"

class FunctionKlass : public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;

public:
    static FunctionKlass* get_instance();

    virtual void print(HiObject* obj);
};


HiObject* len(ObjList args);

typedef HiObject* (*NativeFuncPointer)(ObjList args);

class FunctionObject : public HiObject {
friend class FunctionKlass;  // FunctionKlass and FrameObject is FunctionObject's friend
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString*   _func_name;  // function name
    Map<HiObject*, HiObject*>* _globals;
    ObjList     _defaults;

    NativeFuncPointer _native_func;
    unsigned int _flags;

public:
    FunctionObject(HiObject* code_object);

    FunctionObject(NativeFuncPointer nfp);

    FunctionObject(Klass* klass) {
        _func_code = NULL;
        _func_name = NULL;
        _flags     = 0;
        _globals = NULL;
        _defaults = NULL; 

        set_klass(klass);
    }
    
    HiString*  func_name()   { return _func_name; }
    int  flags()             { return _flags; }

    Map<HiObject*, HiObject*>* globals() { return _globals; }
    void set_globals(Map<HiObject*, HiObject*>* x) { _globals = x; }

    void set_default(ObjList defaults);
    ObjList defaults()       { return _defaults; }

    HiObject*  call(ObjList args);
};


class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;

public:
    static NativeFunctionKlass* get_instance();
};


#endif