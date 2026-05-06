#pragma once
#include "Utility.h"
#include <time.h>
#include "Types.h"
#include "InvalidInputException.h"
class Validator {
    public:
        static bool validateID(const char*);
        static bool validateDate(const char*);
        static bool validateTimeSlot(const char*);
        static bool validateContact(const char*);
        static bool validatePassword(const char*);
        static bool validatePositiveFloat(const char*);
        static bool validateMenuChoice(int, int min, int max);
        static bool validateAge(const char*);
        static Gender validateGender(char);
};