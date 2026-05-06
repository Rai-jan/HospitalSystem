#include "hospitalException.h"

const char* HospitalException::what() {
    return message;
}
void HospitalException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 200) {
        length = 199; // Truncate if message is too long
    }
    copyarray(message, msg, length);
}