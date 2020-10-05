#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/stringTable.hpp"
#include "util/arrayList.hpp"
#include "util/map.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include <string.h>

#define PUSH(x)       _frame->stack()->append((x))
#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()
#define STACK_LEVEL() _frame->stack()->size()

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse


Interpreter* Interpreter::_instance = NULL;

Interpreter* Interpreter::get_instance() {
    if (_instance == NULL) {
        _instance = new Interpreter();
    }

    return _instance;
}


Interpreter::Interpreter() {
    // 初始化 解释器
    _builtins = new HiDict();

    _builtins->put(new HiString("True"),     Universe::HiTrue);
    _builtins->put(new HiString("False"),    Universe::HiFalse);
    _builtins->put(new HiString("None"),     Universe::HiNone);

    _builtins->put(new HiString("len"),      new FunctionObject(len));
    _builtins->put(new HiString("type"),     new FunctionObject(type_of));
    _builtins->put(new HiString("isinstance"),new FunctionObject(isinstance));

    _builtins->put(new HiString("int"),      IntegerKlass::get_instance()->type_object());
    _builtins->put(new HiString("object"),   ObjectKlass::get_instance()->type_object());
    _builtins->put(new HiString("str"),      StringKlass::get_instance()->type_object());
    _builtins->put(new HiString("list"),     ListKlass::get_instance()->type_object());

    _builtins->put(new HiString("dict"),     DictKlass::get_instance()->type_object());
}

void Interpreter::build_frame(HiObject* callable, ObjList args) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject*)callable)->call(args));
    }
    else if (MethodObject::is_method(callable)) {
        MethodObject* method = (MethodObject*) callable;
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args);
    }
    else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject* frame = new FrameObject((FunctionObject*) callable, args);
        frame->set_sender(_frame);  // { _sender = x; }   FrameObject*  _sender;
        _frame = frame;
    }
    else if (callable->klass() == TypeKlass::get_instance()) {
        HiObject* inst = ((HiTypeObject*)callable)->own_klass()->
            allocate_instance(callable, args); 
        PUSH(inst);
    }
}

// void Interpreter::leave_frame(HiObject* return_value) {
//     if (!_frame->sender()) {
//         delete _frame;
//         _frame = NULL;
//         return;
//     }

//     FrameObject* temp = _frame;
//     _frame         = _frame->sender();
//     PUSH(return_value);

//     delete temp;
// }
void Interpreter::leave_frame() {
    destroy_frame();
    PUSH(_ret_value);
}

void Interpreter::destroy_frame() {
    FrameObject* temp = _frame;
    _frame = _frame->sender();

    delete temp;
}


HiObject* Interpreter::call_virtual(HiObject* func, ObjList args) {
    if (func->klass() == NativeFunctionKlass::get_instance()) {
        // we do not create a virtual frame, but native frame.
        return ((FunctionObject*)func)->call(args);
    }
    else if (MethodObject::is_method(func)) {
        MethodObject* method = (MethodObject*) func;
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        return call_virtual(method->func(), args);
    }
    else if (MethodObject::is_function(func)) {
        FrameObject* frame = new FrameObject((FunctionObject*) func, args);
        frame->set_entry_frame(true);
        enter_frame(frame);
        eval_frame();
        destroy_frame();
        return _ret_value;
    }

    return Universe::HiNone;
}

void Interpreter::enter_frame(FrameObject* frame) {
    frame->set_sender(_frame);
    _frame         = frame;
}

void Interpreter::run(CodeObject* codes) {
    _frame = new FrameObject(codes);
    eval_frame();
    destroy_frame();
}

void Interpreter::eval_frame() {

    Block* b;
    FunctionObject* fo;
    ArrayList<HiObject*>* args = NULL;
    HiObject* v, * w, *u;  // 存储对象及属性


    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }


        switch (op_code) {
                
            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::LOAD_CONST:
                PUSH(_frame->consts()->get(op_arg));
                break;

            case ByteCode::LOAD_NAME:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;
            
            case ByteCode::LOAD_FAST:
                PUSH(_frame->fast_locals()->get(op_arg));
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::LOAD_ATTR:
                v = POP();
                w = _frame->_names->get(op_arg);
                PUSH(v->getattr(w));
                break;

            case ByteCode::LOAD_LOCALS:
                PUSH(_frame->_locals);
                break;

            case ByteCode::STORE_NAME:
                v = _frame->names()->get(op_arg);
                _frame->locals()->put(v, POP());
                break;

            case ByteCode::STORE_FAST:
                _frame->_fast_locals->set(op_arg, POP());
                break;

            case ByteCode::STORE_GLOBAL:
                v = _frame->names()->get(op_arg);
                _frame->globals()->put(v, POP());
                break;

            case ByteCode::STORE_SUBSCR:
                u = POP();
                v = POP();
                w = POP();
                v->store_subscr(u, w);
                break;

            case ByteCode::STORE_MAP:
                w = POP();
                u = POP();
                v = TOP();
                ((HiDict*)v)->put(w, u);
                break;

            case ByteCode::STORE_ATTR:
                u = POP();
                v = _frame->_names->get(op_arg);
                w = POP();
                u->setattr(v, w);
                break;

            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;

            case ByteCode::BINARY_SUBTRACT:
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;

            case ByteCode::PRINT_ITEM:
                v = POP();
                v->print();
                break;

            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;

            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;

            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;

             case ByteCode::MAKE_FUNCTION:
                v = POP();
                fo = new FunctionObject(v);
                fo->set_globals(_frame->globals());
                
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                fo->set_default(args);

                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                
                PUSH(fo);
                break;

            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    args = new ArrayList<HiObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }

                build_frame(POP(), args);

                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                break;

            case ByteCode::RETURN_VALUE:
                _ret_value = POP();
                if (_frame->is_first_frame() ||
                        _frame->is_entry_frame())
                    return;
                leave_frame();
                break;

            case ByteCode::COMPARE_OP:
                w = POP();
                v = POP();

                switch(op_arg) {
                case ByteCode::GREATER:
                    PUSH(v->greater(w));
                    break;

                case ByteCode::LESS:
                    PUSH(v->less(w));
                    break;

                case ByteCode::EQUAL:
                    PUSH(v->equal(w));
                    break;

                case ByteCode::NOT_EQUAL:
                    PUSH(v->not_equal(w));
                    break;

                case ByteCode::GREATER_EQUAL:
                    PUSH(v->ge(w));
                    break;
               
                case ByteCode::LESS_EQUAL: 
                    PUSH(v->le(w));
                    break;

                case ByteCode::IN:
                    PUSH(v->contains(w));
                    break;

                case ByteCode::IS:
                    if (v == w)
                        PUSH(HI_TRUE);
                    else
                        PUSH(HI_FALSE);
                    break;

                case ByteCode::IS_NOT:
                    if (v == w)
                        PUSH(HI_TRUE);
                    else
                        PUSH(HI_FALSE);
                    break;


                default:
                    printf("Error: Unrecognized compare op %d\n", op_arg);

                }
                break;

            case ByteCode::DELETE_SUBSCR:
                w = POP();
                v = POP();
                v->del_subscr(w);
                break;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                if (v == Universe::HiFalse)
                    _frame->set_pc(op_arg);
                break;

            case ByteCode::JUMP_FORWARD:
                _frame->set_pc(op_arg + _frame->get_pc());
                break;

            case ByteCode::JUMP_ABSOLUTE:
                _frame->set_pc(op_arg);
                break;

            case ByteCode::SETUP_LOOP:
                _frame->loop_stack()->add(new Block(
                    op_code, _frame->get_pc() + op_arg,
                    STACK_LEVEL()));
                break;

            case ByteCode::POP_BLOCK:
                b = _frame->loop_stack()->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                break;
            
            case ByteCode::BREAK_LOOP:
                b = _frame->loop_stack()->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                _frame->set_pc(b->_target);
                break;
            
            case ByteCode::BUILD_TUPLE: // drop down, we need this

            case ByteCode::BUILD_LIST:
                v = new HiList();
                while (op_arg--) {
                    ((HiList*)v)->set(op_arg, POP());
                }
                PUSH(v);
                break;

            case ByteCode::BUILD_MAP:
                v = new HiDict();
                PUSH(v);
                break;

            case ByteCode::BUILD_CLASS:
                v = POP();
                u = POP();
                w = POP();
                v = Klass::create_klass(v, u, w);
                PUSH(v);
                break;

            case ByteCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;

            case ByteCode::FOR_ITER:
                v = TOP();
                w = v->getattr(StringTable::get_instance()->next_str);
                build_frame(w, NULL);

                if (TOP() == NULL) {
                    _frame->_pc += op_arg;
                    POP();
                }
                break;

            case ByteCode::UNPACK_SEQUENCE:
                v = POP();

                while (op_arg--) {
                    PUSH(v->subscr(new HiInteger(op_arg)));
                }
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}