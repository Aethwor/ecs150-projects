#include <iostream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

size_t BUFFER_SIZE = 100000;

// Reads over standard input and converts it into a string.
string convertInputToString(int fileDesc) {
    string fileString = "";
    char inputChar;
    while(read(0, &inputChar, 1) > 0) {
        fileString += inputChar;
    }
    return fileString;
}

// Takes in a string and determines if it contains the search term up until a newline character.
// If there is no newline character, it returns the string as is.
// Otherwise, it modifies the string and only returns the substring after a newline character.
string findSearchTerm(string fileString, string searchTerm) {
    size_t newLinePos = fileString.find_first_of("\n");
    if (newLinePos != string::npos) {
        string lineSubstring = fileString.substr(0, newLinePos + 1);
        fileString = fileString.substr(newLinePos + 1, string::npos);
        size_t foundSearchTerm = lineSubstring.find(searchTerm);
        if (foundSearchTerm != string::npos) {
            char stdoutBuffer[lineSubstring.length()];
            strcpy(stdoutBuffer, lineSubstring.c_str());
            write(1, stdoutBuffer, lineSubstring.length());
        }
        return findSearchTerm(fileString, searchTerm);
    }
    return fileString;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << "wgrep: searchterm [file ...]" << endl;
        return 1;
    }

    string searchTerm(argv[1]);
    // If only a search term is specified, read from standard input.
    if (argc == 2) {
        string fileString = convertInputToString(0);
        findSearchTerm(fileString, searchTerm);
    }
    
    // Otherwise, opens the file(s) and iterates through it line by line.
    if (argc >= 3) {
        for (int i = 2; i < argc; i++) {
            int fileDesc = open(argv[i], O_RDONLY);
            if (fileDesc < 0) {
                cout << "wgrep: cannot open file" << endl;
                return 1;
            }

            size_t fileOffset = lseek(fileDesc, 0, SEEK_END);
            lseek(fileDesc, 0, SEEK_SET);
            
            char buffer[BUFFER_SIZE];
            string fileString = "";
            size_t totalBytesRead = 0;
            while(totalBytesRead < fileOffset) {
                size_t bytesRead = read(fileDesc, buffer, BUFFER_SIZE);
                string bufferString = string(buffer);
                fileString += bufferString;
                totalBytesRead += bytesRead;
                fileString = findSearchTerm(fileString, searchTerm);
            }
            close(fileDesc);
        }
    }
    
    return 0;
}