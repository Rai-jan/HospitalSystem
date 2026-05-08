#pragma once
#include <ctime>

int getLength(const char*);
void getPart(char*, const char*, int, int);
int CharToInt(const char*);
long long CharToLongLong(const char*);
float CharToFloat(const char*);
bool isDigit(const char*);
void copyarray(char* dest, const char* src, int length);
const char* IntToCChar(int num);
const char* LongLongToCChar(long long num);
bool contains(const char*, const char*);
void toLowerStr(char* dest, const char* src);
bool strEqual(const char* a, const char* b);
void getTodayDate(char* buf);  // fills "DD-MM-YYYY"
int compareDates(const char* d1, const char* d2); // -1 < 0 == 1 >
int daysBetween(const char* earlier, const char* later);
void clearInputBuffer();
