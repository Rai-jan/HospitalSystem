#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const char* msg)
    : HospitalException("File not found") {
    setMessage(msg);
}
const char* FileNotFoundException::what() {
    HospitalException::what();
    std::cout<<std::endl;
    return message;
}
void FileNotFoundException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 200) {
        length = 199; // Truncate if message is too long
    }
    copyarray(message, msg, length);
}
