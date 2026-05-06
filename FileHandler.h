#pragma once
#include "Types.h"
#include "Utility.h"
#include "FileNotFoundException.h"
#include<iostream>
#include<fstream>
const char* resolveFileName(FileType type) {
    switch(type) {
        case PATIENT: return "patients.txt";
        case DOCTOR: return "doctors.txt";
        case APPOINTMENT: return "appointments.txt";
        case BILL: return "bills.txt";
        case PRESCRIPTION: return "prescriptions.txt";
        default: throw FileNotFoundException("Invalid file type");
    }
}
class FileHandler {
    private:
        static bool readFile(FileType type, std::ifstream& file) {
            file.open(resolveFileName(type));
            if(!file.is_open()) {
                return false;
            }
            return true;
        }
        static bool writeFile(FileType type, std::ofstream& file) {
            file.open(resolveFileName(type), std::ios::out | std::ios::trunc);
            if(!file.is_open()) {
                return false;
            }
            return true;
        }
        static bool appendFile(FileType type, std::ofstream& file) {
            file.open(resolveFileName(type), std::ios::out | std::ios::app);
            if(!file.is_open()) {
                return false;
            }
            return true;
        }
        static bool findById(FileType type, int id, char* resultLine) {
            std::ifstream file;
            if(!readFile(type, file)) {
                return false;
            }

            char line[1000];
            while(file.getline(line, sizeof(line))) {
                int parsedId = CharToInt(line); // assumes ID is first field
                if(parsedId == id) {
                    copyarray(resultLine, line, getLength(line));
                    file.close();
                    return true;
                }
            }

            file.close();
            return false;
        }
    public:
            static bool AddDoctor( int , const char* , const char* ,const char* , const char* , float );

};
