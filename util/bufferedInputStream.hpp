#ifndef BUFFERED_INPUT_STREAM_HPP
#define BUFFERED_INPUT_STREAM_HPP

#include <stdio.h>

#define BUFFER_LEN 256

class BufferedInputStream {
private:
    FILE* fp; // 定义文件指针
    char szBuffer[BUFFER_LEN];  // 定义字节数组
    unsigned short index;  // 定义无符号下标

public:
    // 初始化 类
    BufferedInputStream(char const* filename) {
        printf("filename: %s\n",filename);
        fp = fopen(filename, "rb");
        fread(szBuffer, BUFFER_LEN * sizeof(char), 1, fp);
        index = 0;
    }

    ~BufferedInputStream() {
        close();
    }

    char read() {
        if (index < BUFFER_LEN)
            return szBuffer[index++];
        else {
            index = 0;
            fread(szBuffer, BUFFER_LEN * sizeof(char), 1, fp);
            return szBuffer[index++];
        }
    }

    int read_int() {
        int b1 = read() & 0xff;
        int b2 = read() & 0xff;
        int b3 = read() & 0xff;
        int b4 = read() & 0xff;

        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    void unread() {
        index--;
    }

    void close() {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
    }
};

#endif