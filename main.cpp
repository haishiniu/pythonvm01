#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "runtime/interpreter.hpp"


int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("vm need a parameter : filename\n");
        return 0;
    }

    BufferedInputStream stream(argv[1]);
    // printf("magic number is 0x%x\n", stream.read_int());
    BinaryFileParser parser(&stream);
    CodeObject* main_code = parser.parse();
    Interpreter interpreter;
    interpreter.run(main_code);
    return 0;
}
