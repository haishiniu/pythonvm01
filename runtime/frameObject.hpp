#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "util/arrayList.hpp"

class FunctionObject;
class HiList;

class Block {
public:
    unsigned char _type;
    unsigned int  _target;  // 记录跳转后的指令地址
    int  _level;  // 操作数的栈的深度

    Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    Block(unsigned char b_type,
            unsigned int b_target,
            int b_level):
        _type(b_type),
        _target(b_target),
        _level(b_level) {
        }

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level  = b._level;
    }
};

class FrameObject {
public:
    FrameObject(CodeObject* codes);
    FrameObject(FunctionObject* func, ObjList args);
    ~FrameObject() {};

    HiList* _stack;  // ArrayList *  是的 _stack 的类型 其中存储的是 HiObject* 类型的数据
    ArrayList<Block*>*    _loop_stack;  // 记录循环中 SETUP_LOOP 参数

    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;

    HiDict* _locals;
    HiDict* _globals;
    // ObjList                    _fast_locals;
    HiList*               _fast_locals;

    CodeObject*           _codes;
    FrameObject*          _sender;  // 记录调用者的栈帧--链表存储
    int                   _pc;
    bool                  _entry_frame;


public:
    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender()           { return _sender;}
    void set_pc(int x)              { _pc = x; }
    int  get_pc()                   { return _pc; }

    void set_entry_frame(bool x)    { _entry_frame = x; }
    bool is_entry_frame()           { return _entry_frame; }
    bool is_first_frame()           { return _sender == NULL; }

    HiList* stack()                 { return _stack; }
    ArrayList<Block*>* loop_stack()               { return _loop_stack; }
    ArrayList<HiObject*>* consts()                { return _consts; }
    ArrayList<HiObject*>* names()                 { return _names; }
    HiDict* locals()                              { return _locals; }
    HiDict* globals()                             { return _globals; }
    // ObjList fast_locals()                         { return _fast_locals; }
    HiList* fast_locals()           { return _fast_locals; }

    bool has_more_codes();
    unsigned char get_op_code();
    int  get_op_arg();

    void oops_do(OopClosure* f);
};

#endif