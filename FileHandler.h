#pragma once
#include "Types.h"
#include "Utility.h"
#include "FileNotFoundException.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include <iostream>
#include <fstream>

inline const char* resolveFileName(FileType type) {
    switch(type) {
        case PATIENT:      return "patients.txt";
        case DISCHARGED:   return "discharged.txt";
        case DOCTOR:       return "doctors.txt";
        case APPOINTMENT:  return "appointments.txt";
        case BILL:         return "bills.txt";
        case PRESCRIPTION: return "prescriptions.txt";
        case SECURITY_LOG: return "security_log.txt";
        default: throw FileNotFoundException("Invalid file type");
    }
}

class FileHandler {
    private:
        static bool readFile(FileType type, std::ifstream& file) {
            file.open(resolveFileName(type));
            return file.is_open();
        }
        static bool writeFile(FileType type, std::ofstream& file) {
            file.open(resolveFileName(type), std::ios::out | std::ios::trunc);
            return file.is_open();
        }
        static bool appendFile(FileType type, std::ofstream& file) {
            file.open(resolveFileName(type), std::ios::out | std::ios::app);
            return file.is_open();
        }

        // Parse a CSV line into fields; returns number of fields parsed
        static int parseCSV(const char* line, char fields[][500], int maxFields);

    public:
        // --- Write operations ---
        static bool AddDoctor(int id, const char* name, const char* spec,
                              const char* contact, const char* password, float fee);
        static bool AddPatient(int id, const char* name, int age, char gender,
                               const char* contact, const char* password, long long balance);
        static bool AddAppointment(int appId, int patientId, int doctorId,
                                   const char* date, const char* time, const char* status);
        static bool AddBill(int billId, int patientId, int appointmentId,
                            float amount, const char* status, const char* date);
        static bool AddPrescription(int prescId, int appId, int patId, int docId,
                                    const char* date, const char* medication, const char* notes);
        static bool AddSecurityLog(const char* timestamp, const char* role,
                                   int enteredId, const char* result);
        static bool AddDischargedPatient(int id, const char* name, int age, char gender,
                                         const char* contact, const char* password, long long balance);

        // --- Load all records into Storage ---
        static void loadPatients(Storage<Patient>& store);
        static void loadDoctors(Storage<Doctor>& store);
        static void loadAppointments(Storage<Appointment>& store);
        static void loadBills(Storage<Bill>& store);
        static void loadPrescriptions(Storage<Prescription>& store);

        // --- Rewrite whole file from Storage ---
        static void savePatients(const Storage<Patient>& store);
        static void saveDoctors(const Storage<Doctor>& store);
        static void saveAppointments(const Storage<Appointment>& store);
        static void saveBills(const Storage<Bill>& store);
        static void savePrescriptions(const Storage<Prescription>& store);

        // --- Max ID helpers ---
        static int getMaxId(FileType type);

        // --- Security log ---
        static void printSecurityLog();
};
