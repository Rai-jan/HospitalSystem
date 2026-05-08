#pragma once
#include "HospitalException.h"
class FileNotFoundException : public HospitalException {
    public:
        FileNotFoundException(const char* msg);
        const char* what() override;
        void setMessage(const char* msg) override;
};
