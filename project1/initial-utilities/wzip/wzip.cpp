#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

size_t BUFFER_SIZE = 10000;

string translateLine(string line) {
    string writeString = "";
    if(line.length() == 0) {
        return "";
    }

    size_t currentPos = 0;
    char currentChar = line[currentPos];
    unsigned int numOfRepetitions = 0;
    while(currentChar == line[0]) {
        numOfRepetitions += 1;
        currentPos += 1;
        if (currentPos >= line.length()) {
            break;
        }
        currentChar = line[currentPos];
    }

    char intBuf[4];

    intBuf[3] = (numOfRepetitions & 4294967294) >> 24;
    numOfRepetitions -=  (intBuf[3] << 24);
    intBuf[2] = (numOfRepetitions & 16777215) >> 16;
    numOfRepetitions -=  (intBuf[2] << 16);
    intBuf[1] = (numOfRepetitions & 65535) >> 8;
    numOfRepetitions -=  (intBuf[1] << 8);
    intBuf[0] = numOfRepetitions & 255;
    numOfRepetitions -=  intBuf[0];

    if(numOfRepetitions != 0) {
        cerr << "Error in translate line." << endl;
    }

    writeString += intBuf[0];
    writeString += intBuf[1];
    writeString += intBuf[2];
    writeString += intBuf[3];
    writeString += line[0];

    if(currentPos < line.length()) {
        string newLineString = line.substr(currentPos, string::npos);
        writeString += translateLine(newLineString);
    }
    
    char writeBuffer[writeString.length()];
    for(size_t i = 0; i < writeString.length(); i++) {
        writeBuffer[i] = writeString[i];
    }
    write(1, writeBuffer, writeString.length());
    return "";
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "wzip: file1 [file2 ...]" << endl;
        return 1;
    }

    char currentChar = 0;
    string currentString = "";
    for (int i = 1; i < argc; i++) {
        int fileDesc = open(argv[i], O_RDONLY);
        if (fileDesc < 0) {
                cerr << "wzip: cannot open file" << endl;
                return 1;
            }

        char buffer[BUFFER_SIZE];
        int bytes = read(fileDesc, buffer, BUFFER_SIZE);
        if(i == 1) {
            currentChar = buffer[0];
        }
        while(bytes > 0) {
            string bufferString(buffer);
            currentString += bufferString;
            size_t newLinePos = currentString.find('\n');
            while(newLinePos != string::npos) {
                string lineString = currentString.substr(0, newLinePos);
                translateLine(lineString);
                char newLine[5] = {1, 0, 0, 0,'\n'};
                write(1, newLine, 5);
                currentString = currentString.substr(newLinePos + 1, string::npos);
                newLinePos = currentString.find('\n');
            }

            size_t firstNotOfPos = currentString.find_first_not_of(currentChar);
            while(firstNotOfPos != string::npos) {
                string sameCharString = currentString.substr(0, firstNotOfPos);
                translateLine(sameCharString);
                currentChar = currentString[firstNotOfPos];
                currentString = currentString.substr(firstNotOfPos, string::npos);
                firstNotOfPos = currentString.find_first_not_of(currentChar);
            }

            bytes = read(fileDesc, buffer, BUFFER_SIZE);
        }
        close(fileDesc);
    }
    translateLine(currentString);
    return 0;
}