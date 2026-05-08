#include "FileNotFoundException.h"
FileNotFoundException::FileNotFoundException(const char* msg)
    : HospitalException(msg) {}
const char* FileNotFoundException::what() {
    std::cout << "FileNotFoundException: " << message << std::endl;
    return message;
}
void FileNotFoundException::setMessage(const char* msg) {
    HospitalException::setMessage(msg);
}
