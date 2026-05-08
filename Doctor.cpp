#include "Doctor.h"

Doctor::Doctor() : Person(), fee(0.0f) { specialization[0] = '\0'; }

Doctor::Doctor(int id, const char* name, const char* spec,
               const char* contact, const char* password, float fee)
    : Person(id, name, contact, password), fee(fee) {
    copyarray(specialization, spec, getLength(spec));
}

const char* Doctor::getSpecialization() const { return specialization; }
float Doctor::getFee() const { return fee; }

void Doctor::setSpecialization(const char* s) { copyarray(specialization, s, getLength(s)); }
void Doctor::setFee(float f) { fee = f; }

bool Doctor::operator==(const Doctor& other) const { return id == other.id; }

std::ostream& operator<<(std::ostream& os, const Doctor& d) {
    os << "ID: " << d.id << " | Name: " << d.name
       << " | Specialization: " << d.specialization
       << " | Contact: " << d.contact << " | Fee: PKR " << d.fee;
    return os;
}

void Doctor::display() const { std::cout << *this << std::endl; }
