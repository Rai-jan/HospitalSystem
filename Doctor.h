#pragma once
#include "Person.h"
#include <iostream>

class Doctor : public Person {
    private:
        char specialization[51];
        float fee;
    public:
        Doctor();
        Doctor(int id, const char* name, const char* specialization,
               const char* contact, const char* password, float fee);

        const char* getSpecialization() const;
        float getFee() const;

        void setSpecialization(const char* spec);
        void setFee(float f);

        bool operator==(const Doctor& other) const;
        friend std::ostream& operator<<(std::ostream& os, const Doctor& d);

        void display() const override;
};
