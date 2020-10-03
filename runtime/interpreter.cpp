#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "util/arrayList.hpp"
#include "util/map.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include <string.h>

#define PUSH(x)       _frame->stack()->add((x))
#define POP()         _frame->stack()->pop()
#define STACK_LEVEL() _frame->stack()->size()

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse


Interpreter::Interpreter() {
    // 初始化 解释器
    _builtins = new Map<HiObject*, HiObject*>();

    _builtins->put(new HiString("True"),     Universe::HiTrue);
    _builtins->put(new HiString("False"),    Universe::HiFalse);
    _builtins->put(new HiString("None"),     Universe::HiNone);
}

void Interpreter::build_frame(HiObject* callable) {
    FrameObject* frame = new FrameObject((FunctionObject*) callable);
    frame->set_sender(_frame);
    _frame = frame;
}

void Interpreter::leave_frame(HiObject* return_value) {
    if (!_frame->sender()) {
        delete _frame;
        _frame = NULL;
        return;
    }

    FrameObject* temp = _frame;
    _frame         = _frame->sender();
    PUSH(return_value);

    delete temp;
}

void Interpreter::run(CodeObject* codes) {

    _frame = new FrameObject(codes);


    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }

        Block* b;
        FunctionObject* fo;
        HiObject* v, * w; // 存储对象及属性

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

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                PUSH(w);
                break;

            case ByteCode::STORE_NAME:
                v = _frame->names()->get(op_arg);
                _frame->locals()->put(v, POP());
                break;

            case ByteCode::STORE_GLOBAL:
                v = _frame->names()->get(op_arg);
                _frame->globals()->put(v, POP());
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

             case ByteCode::MAKE_FUNCTION:
                v = POP();
                fo = new FunctionObject(v);
                fo->set_globals(_frame->globals());
                PUSH(fo);
                break;

            case ByteCode::CALL_FUNCTION:
                build_frame(POP());
                break;

            case ByteCode::RETURN_VALUE:
                leave_frame(POP());
                if(!_frame)
                    return;
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

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}