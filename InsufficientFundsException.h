#pragma once
#include "HospitalException.h"
class InsufficientFundsException : public HospitalException {
    public:
        InsufficientFundsException(const char* msg);
        const char* what() override;
        void setMessage(const char* msg) override;
};
