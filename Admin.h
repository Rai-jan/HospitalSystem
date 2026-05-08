#pragma once
#include "Person.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"

class Admin : public Person {
    private:
        // Admin credentials are hardcoded (one admin)
        static const char* ADMIN_PASSWORD;
    public:
        Admin();
        void display() const override;

        static bool authenticate(const char* password);

        void runMenu(Storage<Patient>& patients, Storage<Doctor>& doctors,
                     Storage<Appointment>& appointments, Storage<Bill>& bills,
                     Storage<Prescription>& prescriptions);

        void addDoctor(Storage<Doctor>& doctors);
        void removeDoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments);
        void addPatient(Storage<Patient>& patients);
        void removePatient(Storage<Patient>& patients, Storage<Appointment>& appointments,
                           Storage<Bill>& bills, Storage<Prescription>& prescriptions);
        void viewAllPatients(const Storage<Patient>& patients, const Storage<Bill>& bills);
        void viewAllDoctors(const Storage<Doctor>& doctors);
        void viewAllAppointments(const Storage<Appointment>& appointments,
                                  const Storage<Patient>& patients,
                                  const Storage<Doctor>& doctors);
        void viewUnpaidBills(const Storage<Bill>& bills, const Storage<Patient>& patients);
        void dischargePatient(Storage<Patient>& patients, Storage<Appointment>& appointments,
                               Storage<Bill>& bills, Storage<Prescription>& prescriptions);
        void viewSecurityLog();
        void generateDailyReport(const Storage<Appointment>& appointments,
                                  const Storage<Bill>& bills,
                                  const Storage<Patient>& patients,
                                  const Storage<Doctor>& doctors);
};
