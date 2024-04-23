#include <iostream>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        int fileDesc = open(argv[i], O_RDONLY);  
        if (fileDesc < 0) {
            cout << "wcat: cannot open file" << endl;
            return 1;
        }

        size_t fileOffset = lseek(fileDesc, 0, SEEK_END);
        lseek(fileDesc, 0, SEEK_SET);

        size_t BUFFER_SIZE = 1000000;
        char buffer[BUFFER_SIZE];
        size_t bytesRead = 0;
        while (bytesRead < fileOffset) {
            int currentBytesRead = read(fileDesc, buffer, BUFFER_SIZE);
            write(1, buffer, currentBytesRead);
            bytesRead += currentBytesRead;
        }
        close(fileDesc);
    }
    return 0;
}