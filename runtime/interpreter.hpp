#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"

class Interpreter {
private:
    ArrayList<HiObject*>* _stack; // ArrayList *  是的 _stack 的类型 其中存储的是 HiObject* 类型的数据
    ArrayList<HiObject*>* _consts;
public:
    Interpreter();

    void run(CodeObject* codes);
};

#endif
