#include "Utility.h"
#include <iostream>
#include <ctime>
#include <limits>

int getLength(const char* str) {
    if(str == nullptr) return 0;
    int length = 0;
    while(str[length] != '\0') length++;
    return length;
}

void getPart(char* dest, const char* str, int start, int end) {
    if(str == nullptr || dest == nullptr || start < 0 || end <= start) {
        dest[0] = '\0';
        return;
    }
    int length = end - start;
    for(int i = 0; i < length; i++) dest[i] = str[i + start];
    dest[length] = '\0';
}

int CharToInt(const char* str) {
    if(str == nullptr || str[0] == '\0') return 0;
    int result = 0;
    int i = 0;
    bool negative = false;
    if(str[0] == '-') { negative = true; i = 1; }
    for(; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') return result;
        result = result * 10 + (str[i] - '0');
    }
    return negative ? -result : result;
}

long long CharToLongLong(const char* str) {
    if(str == nullptr || str[0] == '\0') return 0LL;
    long long result = 0LL;
    int i = 0;
    bool negative = false;
    if(str[0] == '-') { negative = true; i = 1; }
    for(; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') return result;
        result = result * 10 + (str[i] - '0');
    }
    return negative ? -result : result;
}

float CharToFloat(const char* str) {
    if(str == nullptr || str[0] == '\0') return 0.0f;
    float result = 0.0f;
    float fraction = 0.0f;
    int i = 0;
    bool hasDot = false;
    float divisor = 10.0f;
    bool negative = false;
    if(str[0] == '-') { negative = true; i = 1; }
    for(; str[i] != '\0'; i++) {
        if(str[i] == '.') { hasDot = true; continue; }
        if(str[i] < '0' || str[i] > '9') break;
        if(hasDot) { fraction += (str[i] - '0') / divisor; divisor *= 10.0f; }
        else { result = result * 10 + (str[i] - '0'); }
    }
    result += fraction;
    return negative ? -result : result;
}

bool isDigit(const char* str) {
    if(str == nullptr || str[0] == '\0') return false;
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

void copyarray(char* dest, const char* src, int length) {
    if(src == nullptr || dest == nullptr) return;
    for(int i = 0; i < length; i++) dest[i] = src[i];
    dest[length] = '\0';
}

const char* IntToCChar(int num) {
    static char str[20];
    int index = 0;
    if(num == 0) { str[index++] = '0'; str[index] = '\0'; return str; }
    if(num < 0) { str[index++] = '-'; num = -num; }
    int temp = num, digits = 0;
    while(temp > 0) { temp /= 10; digits++; }
    for(int i = index + digits - 1; i >= index; i--) {
        str[i] = (num % 10) + '0'; num /= 10;
    }
    index += digits;
    str[index] = '\0';
    return str;
}

const char* LongLongToCChar(long long num) {
    static char str[25];
    int index = 0;
    if(num == 0) { str[index++] = '0'; str[index] = '\0'; return str; }
    if(num < 0) { str[index++] = '-'; num = -num; }
    long long temp = num; int digits = 0;
    while(temp > 0) { temp /= 10; digits++; }
    for(int i = index + digits - 1; i >= index; i--) {
        str[i] = (num % 10) + '0'; num /= 10;
    }
    index += digits;
    str[index] = '\0';
    return str;
}

bool contains(const char* text, const char* pattern) {
    if (!*pattern) return true;
    for(int i = 0; text[i] != '\0'; i++) {
        int j = 0;
        while(text[i+j] != '\0' && pattern[j] != '\0' && text[i+j] == pattern[j]) j++;
        if(pattern[j] == '\0') return true;
    }
    return false;
}

void toLowerStr(char* dest, const char* src) {
    int i = 0;
    for(; src[i] != '\0'; i++) {
        if(src[i] >= 'A' && src[i] <= 'Z') dest[i] = src[i] + 32;
        else dest[i] = src[i];
    }
    dest[i] = '\0';
}

bool strEqual(const char* a, const char* b) {
    if(a == nullptr || b == nullptr) return false;
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0') {
        if(a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

void getTodayDate(char* buf) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int d = now->tm_mday, m = now->tm_mon + 1, y = now->tm_year + 1900;
    // DD-MM-YYYY
    buf[0] = '0' + d/10; buf[1] = '0' + d%10; buf[2] = '-';
    buf[3] = '0' + m/10; buf[4] = '0' + m%10; buf[5] = '-';
    buf[6] = '0' + y/1000; buf[7] = '0' + (y/100)%10;
    buf[8] = '0' + (y/10)%10; buf[9] = '0' + y%10; buf[10] = '\0';
}

// Compare DD-MM-YYYY dates. Returns -1 if d1<d2, 0 if equal, 1 if d1>d2
int compareDates(const char* d1, const char* d2) {
    // year
    char y1[5], y2[5], m1[3], m2[3], day1[3], day2[3];
    getPart(day1, d1, 0, 2); getPart(m1, d1, 3, 5); getPart(y1, d1, 6, 10);
    getPart(day2, d2, 0, 2); getPart(m2, d2, 3, 5); getPart(y2, d2, 6, 10);
    int yr1 = CharToInt(y1), yr2 = CharToInt(y2);
    int mo1 = CharToInt(m1), mo2 = CharToInt(m2);
    int da1 = CharToInt(day1), da2 = CharToInt(day2);
    if(yr1 != yr2) return yr1 < yr2 ? -1 : 1;
    if(mo1 != mo2) return mo1 < mo2 ? -1 : 1;
    if(da1 != da2) return da1 < da2 ? -1 : 1;
    return 0;
}

int daysBetween(const char* earlier, const char* later) {
    char y1[5], y2[5], m1[3], m2[3], d1[3], d2[3];
    getPart(d1, earlier, 0, 2); getPart(m1, earlier, 3, 5); getPart(y1, earlier, 6, 10);
    getPart(d2, later, 0, 2);   getPart(m2, later, 3, 5);   getPart(y2, later, 6, 10);
    struct tm t1 = {}, t2 = {};
    t1.tm_mday = CharToInt(d1); t1.tm_mon = CharToInt(m1) - 1; t1.tm_year = CharToInt(y1) - 1900;
    t2.tm_mday = CharToInt(d2); t2.tm_mon = CharToInt(m2) - 1; t2.tm_year = CharToInt(y2) - 1900;
    time_t time1 = mktime(&t1), time2 = mktime(&t2);
    double diff = difftime(time2, time1);
    return (int)(diff / 86400.0);
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}
