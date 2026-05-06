#pragma once
#include "HospitalException.h"
class InsufficientFundsException : public HospitalException {
    private:
        char message[200];
    public:
        InsufficientFundsException(const char* );
        const char* what() override ;
        void setMessage(const char* ) override;
};