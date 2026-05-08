#pragma once
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"

class DoctorMenu {
    private:
        Doctor* doctor;
        Storage<Patient>& patients;
        Storage<Appointment>& appointments;
        Storage<Prescription>& prescriptions;
        Storage<Bill>& bills;

        void viewTodaysAppointments();
        void markComplete();
        void markNoShow();
        void writePrescription();
        void viewPatientHistory();

    public:
        DoctorMenu(Doctor* d,
                   Storage<Patient>& patients,
                   Storage<Appointment>& appointments,
                   Storage<Prescription>& prescriptions,
                   Storage<Bill>& bills);
        void run();
};
