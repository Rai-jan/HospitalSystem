#include"Utility.h"

int getLength(const char* str) {
    if(str == nullptr) {
        return 0;
    }
    int length = 0;
    while(str[length] != '\0') {
        length++;
    }
    return length;
}
void getPart(char* dest, const char* str, int start, int end) {
    if(str == nullptr || dest == nullptr || start < 0 || end <= start) {
        dest[0] = '\0'; // Return empty string on invalid input
        return;
    }
    int length = end - start;
    for(int i = 0; i < length; i++) {
        dest[i] = str[i + start];
    }
    dest[length] = '\0';
}
int CharToInt(const char* str) {
    int result = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') {
            return -999; // Invalid character
        }
        result = result * 10 + (str[i] - '0');
    }
    return result;
}
bool isDigit(const char* str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return true;
}
void copyarray(char* dest, const char* src, int length) {
    if(src == nullptr || dest == nullptr) {
        return; // Do nothing on invalid input
    }
    for(int i = 0; i < length; i++) {
        dest[i] = src[i];
    }
    dest[length] = '\0';
}