#pragma once
#include <iostream>
#include "Utility.h"
class HospitalException {
    private:
        char message[200];
    public:
        HospitalException(const char* ){}
        virtual const char* what();
        virtual void setMessage(const char* );
};