#pragma once
#include <iostream>
#include "HospitalException.h"
#include "Utility.h"
class InvalidInputException : public HospitalException {
    private:
        char imessage[200];
    public:
        InvalidInputException(const char* );
        const char* what() override ;
        void setMessage(const char* ) override;
};
