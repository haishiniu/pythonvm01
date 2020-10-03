#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"

class FrameObject;
class Interpreter {
private:

    Map<HiObject*, HiObject*>*    _builtins;
    FrameObject*                  _frame;
    
    static Interpreter*   _instance;
    Interpreter();  // 初始化解释器 

public:
    static Interpreter* get_instance();

    void run(CodeObject* codes);  // 进行解释 执行
    void build_frame(HiObject* callable, ObjList args);
    void leave_frame(HiObject* ret_value);
    HiObject* call_virtual    (HiObject* func, ObjList args);
};

#endif
