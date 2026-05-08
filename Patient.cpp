#include "Patient.h"

Patient::Patient() : Person(), age(0), gender('M'), balance(0) {}

Patient::Patient(int id, const char* name, int age, char gender,
                 const char* contact, const char* password, long long balance)
    : Person(id, name, contact, password), age(age), gender(gender), balance(balance) {}

int Patient::getAge() const { return age; }
char Patient::getGender() const { return gender; }
long long Patient::getBalance() const { return balance; }

void Patient::setAge(int a) { age = a; }
void Patient::setGender(char g) { gender = g; }
void Patient::setBalance(long long b) { balance = b; }

Patient& Patient::operator-=(long long amount) {
    balance -= amount;
    return *this;
}

Patient& Patient::operator+=(long long amount) {
    balance += amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Patient& p) {
    os << "ID: " << p.id << " | Name: " << p.name
       << " | Age: " << p.age << " | Gender: " << p.gender
       << " | Contact: " << p.contact << " | Balance: PKR " << p.balance;
    return os;
}

void Patient::display() const {
    std::cout << *this << std::endl;
}
