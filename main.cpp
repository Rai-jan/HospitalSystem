#include <iostream>
#include <ctime>
#include "Types.h"
#include "Utility.h"
#include "Validator.h"
#include "FileHandler.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Admin.h"
#include "PatientMenu.h"
#include "DoctorMenu.h"
#include "HospitalException.h"
#include "FileNotFoundException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"
#include "SlotUnavailableException.h"

// ---------------------------------------------------------------------------
// Global in-memory stores (loaded once at startup)
// ---------------------------------------------------------------------------
Storage<Patient>      g_patients(128);
Storage<Doctor>       g_doctors(64);
Storage<Appointment>  g_appointments(512);
Storage<Bill>         g_bills(512);
Storage<Prescription> g_prescriptions(512);

// ---------------------------------------------------------------------------
// Helper: get current timestamp string for security log
// ---------------------------------------------------------------------------
static void getTimestamp(char* buf) {
    time_t t = time(0);
    tm* now = localtime(&t);
    // YYYY-MM-DD HH:MM:SS
    int y = now->tm_year + 1900, mo = now->tm_mon + 1, d = now->tm_mday;
    int h = now->tm_hour, mi = now->tm_min, s = now->tm_sec;
    buf[0]='0'+y/1000; buf[1]='0'+(y/100)%10; buf[2]='0'+(y/10)%10; buf[3]='0'+y%10;
    buf[4]='-';
    buf[5]='0'+mo/10; buf[6]='0'+mo%10; buf[7]='-';
    buf[8]='0'+d/10;  buf[9]='0'+d%10;  buf[10]=' ';
    buf[11]='0'+h/10; buf[12]='0'+h%10; buf[13]=':';
    buf[14]='0'+mi/10;buf[15]='0'+mi%10;buf[16]=':';
    buf[17]='0'+s/10; buf[18]='0'+s%10; buf[19]='\0';
}

// ---------------------------------------------------------------------------
// Login handlers
// ---------------------------------------------------------------------------
static void patientLogin() {
    char idStr[10], password[51];
    int failCount = 0;

    while(failCount < 3) {
        std::cout << "Enter Patient ID: ";
        std::cin.getline(idStr, 10);
        std::cout << "Enter Password: ";
        std::cin.getline(password, 51);

        int id = CharToInt(idStr);
        Patient* pat = g_patients.findById(id);
        if(pat && pat->checkPassword(password)) {
            PatientMenu menu(pat, g_patients, g_doctors, g_appointments, g_bills, g_prescriptions);
            menu.run();
            return;
        }
        failCount++;
        std::cout << "Invalid credentials. Attempts remaining: " << (3 - failCount) << "\n";
    }
    // Lock
    std::cout << "Account locked. Contact admin.\n";
    char ts[20]; getTimestamp(ts);
    int id = CharToInt(idStr);
    FileHandler::AddSecurityLog(ts, "Patient", id, "LOCKED");
}

static void doctorLogin() {
    char idStr[10], password[51];
    int failCount = 0;

    while(failCount < 3) {
        std::cout << "Enter Doctor ID: ";
        std::cin.getline(idStr, 10);
        std::cout << "Enter Password: ";
        std::cin.getline(password, 51);

        int id = CharToInt(idStr);
        Doctor* doc = g_doctors.findById(id);
        if(doc && doc->checkPassword(password)) {
            DoctorMenu menu(doc, g_patients, g_appointments, g_prescriptions, g_bills);
            menu.run();
            return;
        }
        failCount++;
        std::cout << "Invalid credentials. Attempts remaining: " << (3 - failCount) << "\n";
    }
    std::cout << "Account locked. Contact admin.\n";
    char ts[20]; getTimestamp(ts);
    int id = CharToInt(idStr);
    FileHandler::AddSecurityLog(ts, "Doctor", id, "LOCKED");
}

static void adminLogin() {
    char password[51];
    int failCount = 0;

    while(failCount < 3) {
        std::cout << "Enter Admin Password: ";
        std::cin.getline(password, 51);

        if(Admin::authenticate(password)) {
            Admin admin;
            admin.runMenu(g_patients, g_doctors, g_appointments, g_bills, g_prescriptions);
            return;
        }
        failCount++;
        std::cout << "Invalid credentials. Attempts remaining: " << (3 - failCount) << "\n";
    }
    std::cout << "Account locked. Contact admin.\n";
    char ts[20]; getTimestamp(ts);
    FileHandler::AddSecurityLog(ts, "Admin", 0, "LOCKED");
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main() {
    // Load all data from files
    try {
        FileHandler::loadPatients(g_patients);
        FileHandler::loadDoctors(g_doctors);
        FileHandler::loadAppointments(g_appointments);
        FileHandler::loadBills(g_bills);
        FileHandler::loadPrescriptions(g_prescriptions);
    } catch(FileNotFoundException& e) {
        // Files may not exist on first run; that's OK
        std::cout << "[Info] Some data files are missing - starting fresh.\n";
    }

    int choice = 0;
    do {
        std::cout << "\n===============================================\n";
        std::cout << " Welcome to MediCore Hospital Management System\n";
        std::cout << "===============================================\n";
        std::cout << "Login as:\n";
        std::cout << "1. Patient\n";
        std::cout << "2. Doctor\n";
        std::cout << "3. Admin\n";
        std::cout << "4. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        clearInputBuffer();

        switch(choice) {
            case 1: patientLogin(); break;
            case 2: doctorLogin();  break;
            case 3: adminLogin();   break;
            case 4: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while(choice != 4);

    return 0;
}
