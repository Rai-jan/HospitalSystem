#include "HospitalException.h"

class SlotUnavailableException : public HospitalException {
    private:
        char message[200];
    public:
        SlotUnavailableException(const char* );
        const char* what() override ;
        void setMessage(const char* ) override;
};
