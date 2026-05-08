#pragma once
#include "Person.h"
#include <iostream>

class Patient : public Person {
    private:
        int age;
        char gender;   // 'M' or 'F'
        long long balance; // stored as PKR (whole number, long long for large amounts)
    public:
        Patient();
        Patient(int id, const char* name, int age, char gender,
                const char* contact, const char* password, long long balance);

        int getAge() const;
        char getGender() const;
        long long getBalance() const;

        void setAge(int age);
        void setGender(char g);
        void setBalance(long long b);

        // Operator overloads
        Patient& operator-=(long long amount);   // deduct balance
        Patient& operator+=(long long amount);   // add to balance
        bool operator==(const Patient& other) const; // compare by ID

        friend std::ostream& operator<<(std::ostream& os, const Patient& p);

        void display() const override;
};
