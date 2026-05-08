#include "Person.h"
#include <iostream>

Person::Person() : id(0) {
    name[0] = '\0'; contact[0] = '\0'; password[0] = '\0';
}

Person::Person(int id, const char* name, const char* contact, const char* password) : id(id) {
    copyarray(this->name, name, getLength(name));
    copyarray(this->contact, contact, getLength(contact));
    copyarray(this->password, password, getLength(password));
}

int Person::getId() const { return id; }
const char* Person::getName() const { return name; }
const char* Person::getContact() const { return contact; }
const char* Person::getPassword() const { return password; }

void Person::setId(int id) { this->id = id; }
void Person::setName(const char* n) { copyarray(name, n, getLength(n)); }
void Person::setContact(const char* c) { copyarray(contact, c, getLength(c)); }
void Person::setPassword(const char* p) { copyarray(password, p, getLength(p)); }

bool Person::checkPassword(const char* pwd) const {
    return strEqual(password, pwd);
}
