#include "SlotUnavailableException.h"
SlotUnavailableException::SlotUnavailableException(const char* msg)
    : HospitalException(msg) {}
const char* SlotUnavailableException::what() {
    std::cout << "SlotUnavailableException: " << message << std::endl;
    return message;
}
void SlotUnavailableException::setMessage(const char* msg) {
    HospitalException::setMessage(msg);
}
