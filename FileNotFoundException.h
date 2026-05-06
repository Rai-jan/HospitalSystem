#include"HospitalException.h"
class FileNotFoundException : public HospitalException {
    private:
        char message[200];
    public:
        FileNotFoundException(const char* );
        const char* what() override ;
        void setMessage(const char* ) override;
};