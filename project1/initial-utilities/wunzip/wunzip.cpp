#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "wunzip: file1 [file2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        int fileDesc = open(argv[i], O_RDONLY);
        if (fileDesc < 0) {
                cerr << "wunzip: cannot open file" << endl;
                return 1;
            }

        char buf[100000];
        size_t finishedRead = read(fileDesc, buf, 100000);
        string stringToWrite = "";
        while(finishedRead > 0) {
            for (size_t i = 0; i < finishedRead; i += 5) {
                unsigned int length = buf[i] + (256 * buf[i + 1]) + (65526 * buf[i + 2]) + (16777216 * buf[i + 3]);
                string uncompressedString(length, buf[i + 4]);
                stringToWrite += uncompressedString;
            }
            char writeBuffer[stringToWrite.length()];
            strcpy(writeBuffer, stringToWrite.c_str());
            write(1, writeBuffer, stringToWrite.length());
            finishedRead = read(fileDesc, buf, 100000);
            stringToWrite = "";
        }
        close(fileDesc);
    }
    return 0;
}