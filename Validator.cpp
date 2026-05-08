#include "Validator.h"
#include <climits>

bool Validator::validateID(const char* id) {
    if(id == nullptr || !isDigit(id)) return false;
    int idValue = CharToInt(id);
    return idValue > 0;
}

bool Validator::validateDate(const char* date) {
    if(date == nullptr || getLength(date) != 10) return false;
    if(date[2] != '-' || date[5] != '-') return false;
    char dayStr[3], monthStr[3], yearStr[5];
    getPart(dayStr, date, 0, 2);
    getPart(monthStr, date, 3, 5);
    getPart(yearStr, date, 6, 10);
    if(!isDigit(dayStr) || !isDigit(monthStr) || !isDigit(yearStr)) return false;
    int day = CharToInt(dayStr), month = CharToInt(monthStr), year = CharToInt(yearStr);
    time_t t = time(0);
    tm* now = localtime(&t);
    int current_year = now->tm_year + 1900;
    if(day < 1 || day > 31 || month < 1 || month > 12) return false;
    if(year < current_year || year > current_year + 1) return false;
    return true;
}

bool Validator::validateTimeSlot(const char* timeSlot) {
    if(timeSlot == nullptr || getLength(timeSlot) != 5) return false;
    if(timeSlot[2] != ':') return false;
    char hourStr[3], minuteStr[3];
    getPart(hourStr, timeSlot, 0, 2);
    getPart(minuteStr, timeSlot, 3, 5);
    if(!isDigit(hourStr) || !isDigit(minuteStr)) return false;
    int hour = CharToInt(hourStr), minute = CharToInt(minuteStr);
    // Valid slots: 09:00 to 16:00
    if(hour < 9 || hour > 16 || minute != 0) return false;
    return true;
}

bool Validator::validateContact(const char* contact) {
    if(contact == nullptr || getLength(contact) != 11) return false;
    return isDigit(contact);
}

bool Validator::validatePassword(const char* password) {
    if(password == nullptr || getLength(password) < 6) return false;
    int length = getLength(password);
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for(int i = 0; i < length; i++) {
        if(password[i] >= 'A' && password[i] <= 'Z') hasUpper = true;
        else if(password[i] >= 'a' && password[i] <= 'z') hasLower = true;
        else if(password[i] >= '0' && password[i] <= '9') hasDigit = true;
    }
    if(!hasUpper || !hasLower || !hasDigit) return false;
    for(int i = 0; i < length - 1; i++) {
        if(password[i] == ' ' && password[i+1] == ' ') return false;
    }
    return true;
}

bool Validator::validatePositiveFloat(const char* str) {
    if(str == nullptr || str[0] == '\0') return false;
    int length = getLength(str);
    bool hasDecimal = false;
    bool hasDigit = false;
    for(int i = 0; i < length; i++) {
        if(str[i] == '-') return false;
        if(str[i] == '.') {
            if(hasDecimal) return false;
            hasDecimal = true;
        } else if(str[i] >= '0' && str[i] <= '9') {
            hasDigit = true;
        } else {
            return false;
        }
    }
    if(!hasDigit) return false;
    float val = CharToFloat(str);
    return val > 0.0f;
}

bool Validator::validateMenuChoice(int choice, int min, int max) {
    return choice >= min && choice <= max;
}

bool Validator::validateAge(const char* age) {
    if(age == nullptr || !isDigit(age)) return false;
    int ageValue = CharToInt(age);
    return ageValue >= 0 && ageValue <= 120;
}

Gender Validator::validateGender(char gender) {
    if(gender == 'M' || gender == 'm') return MALE;
    if(gender == 'F' || gender == 'f') return FEMALE;
    throw InvalidInputException("Invalid gender. Enter M or F.");
}

bool Validator::validateName(const char* name, int maxLen) {
    if(name == nullptr) return false;
    int len = getLength(name);
    return len > 0 && len <= maxLen;
}

bool Validator::validateBalance(const char* str, long long& outBalance) {
    if(str == nullptr || str[0] == '\0') return false;
    // Check for negative
    if(str[0] == '-') return false;
    // Check all chars are digits or single dot
    int len = getLength(str);
    bool hasDot = false;
    bool hasDigit = false;
    for(int i = 0; i < len; i++) {
        if(str[i] == '.') {
            if(hasDot) return false;
            hasDot = true;
        } else if(str[i] >= '0' && str[i] <= '9') {
            hasDigit = true;
        } else {
            return false;
        }
    }
    if(!hasDigit) return false;
    // Parse integer part only for range check
    char intPart[30];
    int dotPos = -1;
    for(int i = 0; i < len; i++) { if(str[i] == '.') { dotPos = i; break; } }
    if(dotPos == -1) {
        copyarray(intPart, str, len);
    } else {
        getPart(intPart, str, 0, dotPos);
    }
    // Check if integer part fits in long long (max ~9.2 * 10^18)
    int intLen = getLength(intPart);
    if(intLen > 18) {
        // Too large
        return false;
    }
    long long val = CharToLongLong(str); // we'll just use the full thing
    outBalance = val;
    // Also check actual float part for cents (store as long long * 100 if needed, but we store as long long PKR)
    // For simplicity: balance stored as long long (whole PKR)
    if(val < 0) return false;
    return true;
}
