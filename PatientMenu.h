#pragma once
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"
#include "SlotUnavailableException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"

class PatientMenu {
    private:
        Patient* patient;
        Storage<Patient>& patients;
        Storage<Doctor>& doctors;
        Storage<Appointment>& appointments;
        Storage<Bill>& bills;
        Storage<Prescription>& prescriptions;

        void bookAppointment();
        void cancelAppointment();
        void viewAppointments();
        void viewMedicalRecords();
        void viewBills();
        void payBill();
        void topUpBalance();

        bool isSlotTaken(int doctorId, const char* date, const char* timeSlot);
        void displayAvailableSlots(int doctorId, const char* date);

    public:
        PatientMenu(Patient* p,
                    Storage<Patient>& patients,
                    Storage<Doctor>& doctors,
                    Storage<Appointment>& appointments,
                    Storage<Bill>& bills,
                    Storage<Prescription>& prescriptions);

        void run();
};
