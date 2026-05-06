#include "SlotUnavailableException.h"

SlotUnavailableException::SlotUnavailableException(const char* msg)
    : HospitalException("Slot unavailable") {
    setMessage(msg);
}

const char* SlotUnavailableException::what() {
    HospitalException::what();
    std::cout<<std::endl;
    return message;
}

void SlotUnavailableException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 200) {
        length = 199; // Truncate if message is too long
    }
    copyarray(message, msg, length);
}

