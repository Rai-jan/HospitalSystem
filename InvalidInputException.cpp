#include "InvalidInputException.h"


InvalidInputException::InvalidInputException(const char* msg)
    : HospitalException("Invalid input") {
    setMessage(msg);
}
const char* InvalidInputException::what() {
    HospitalException::what();
    std::cout<<std::endl;
    return imessage;
}
void InvalidInputException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 200) {
        length = 199; // Truncate if message is too long
    }
    copyarray(imessage, msg, length);
}