#pragma once
#include "HospitalException.h"
class InvalidInputException : public HospitalException {
    public:
        InvalidInputException(const char* msg);
        const char* what() override;
        void setMessage(const char* msg) override;
};
