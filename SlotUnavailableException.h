#pragma once
#include "HospitalException.h"
class SlotUnavailableException : public HospitalException {
    public:
        SlotUnavailableException(const char* msg);
        const char* what() override;
        void setMessage(const char* msg) override;
};
