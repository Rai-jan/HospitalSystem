#pragma once
#include <iostream>
#include "Utility.h"

class HospitalException {
    protected:
        char message[200];
    public:
        HospitalException(const char* msg) {
            setMessage(msg);
        }
        virtual const char* what();
        virtual void setMessage(const char* msg);
};
