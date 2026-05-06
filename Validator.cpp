#include "Validator.h"
#include "Utility.h"
#include <time.h>
bool Validator::validateID(const char* id) {
    if(id == nullptr) {
        return false;
    }
    if(!isDigit(id)) {
        return false;
    }
    int id = CharToInt(id);
    if(id <= 0) {
        return false;
    }

    return true; // Placeholder
}
bool Validator::validateDate(const char* date) {
    if(date == nullptr) {
        return false;
    }
    if(getLength(date) != 10) {
        return false;
    }
    //Validate date
    char dayStr[3];
    getPart(date, 0, 2);
    int day = CharToInt(dayStr);
    char monthStr[3];
    getPart(date, 3, 5);
    int month = CharToInt(monthStr);
    char yearStr[5];
    getPart(date, 6, 10);
    int year = CharToInt(yearStr);
    time_t t = time(0); 

    tm* now = localtime(&t);

    int current_year = now->tm_year + 1900;
    if(day<0 || day>31 || month<1 || month>12 || year<current_year || year>current_year + 1) {
        return false;
    }
    return true; // Placeholder
}

bool Validator::validateTimeSlot(const char* timeSlot) {
    if(timeSlot == nullptr) {
        return false;
    }
    if(getLength(timeSlot) != 5) {
        return false;
    }
    //Validate time slot
    char hourStr[3];
    getPart(timeSlot, 0, 2);
    int hour = CharToInt(hourStr);
    char minuteStr[3];
    getPart(timeSlot, 3, 5);
    int minute = CharToInt(minuteStr);
    if(hour<9 || hour>15 || minute!=0) {
        return false;
    }
    return true; // Placeholder
}
bool Validator::validateContact(const char* contact) {
    if(contact == nullptr) {
        return false;
    }
    if(getLength(contact) != 11) {
        return false;
    }
    if(!isDigit(contact)) {
        return false;
    }
    return true; // Placeholder
}
bool Validator::validatePassword(const char* password) {
    if(password == nullptr) {
        return false;
    }
    if(getLength(password) < 6) {
        return false;
    }
    int length = getLength(password);
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for(int i = 0; i < length; i++) {
        if(password[i] >= 'A' && password[i] <= 'Z') {
            hasUpper = true;
        } else if(password[i] >= 'a' && password[i] <= 'z') {
            hasLower = true;
        } else if(password[i] >= '0' && password[i] <= '9') {
            hasDigit = true;
        }
    }
    if(!hasUpper || !hasLower || !hasDigit) {
        return false;
    }
    const char space = ' ' ;
    for(int i =0; i < length - 1; i++) {
        if(password[i] == space && password[i+1] == space) {
            return false;
        }
    }
    return true; // Placeholder
}
