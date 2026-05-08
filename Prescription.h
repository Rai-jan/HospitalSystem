#pragma once
#include "Utility.h"
#include <iostream>

class Prescription {
    private:
        int prescriptionId;
        int appointmentId;
        int patientId;
        int doctorId;
        char date[11];
        char medication[500];
        char notes[300];
    public:
        Prescription();
        Prescription(int prescId, int appId, int patId, int docId,
                     const char* date, const char* medication, const char* notes);

        int getId() const { return prescriptionId; }
        int getPrescriptionId() const;
        int getAppointmentId() const;
        int getPatientId() const;
        int getDoctorId() const;
        const char* getDate() const;
        const char* getMedication() const;
        const char* getNotes() const;

        void display() const;
};
