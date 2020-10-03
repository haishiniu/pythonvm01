#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"

class FrameObject;
class Interpreter {
private:
    FrameObject* _frame;
public:
    Interpreter(); // 初始化解释器 

    void run(CodeObject* codes);  // 进行解释 执行
};

#endif
