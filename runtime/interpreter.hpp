#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"

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
        }  // 初始化 Block

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level  = b._level;
    }
};


class Interpreter {
private:
    ArrayList<HiObject*>* _stack; // ArrayList *  是的 _stack 的类型 其中存储的是 HiObject* 类型的数据
    ArrayList<HiObject*>* _consts;

    ArrayList<Block*>* _loop_stack; // 记录循环中 SETUP_LOOP 参数
public:
    Interpreter(); // 初始化解释器 

    void run(CodeObject* codes);  // 进行解释 执行
};

#endif
