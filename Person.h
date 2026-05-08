#pragma once
#include "Utility.h"
#include "Types.h"

class Person {
    protected:
        int id;
        char name[51];
        char contact[12];
        char password[51];
    public:
        Person();
        Person(int id, const char* name, const char* contact, const char* password);
        virtual ~Person() {}

        int getId() const;
        const char* getName() const;
        const char* getContact() const;
        const char* getPassword() const;

        void setId(int id);
        void setName(const char* name);
        void setContact(const char* contact);
        void setPassword(const char* password);

        bool checkPassword(const char* pwd) const;

        virtual void display() const = 0;
};
