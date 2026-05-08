#pragma once
#include "Utility.h"
#include "Types.h"
#include <iostream>

class Appointment {
    private:
        int appointmentId;
        int patientId;
        int doctorId;
        char date[11];       // DD-MM-YYYY
        char timeSlot[6];    // HH:MM
        AppointmentStatus status;
    public:
        Appointment();
        Appointment(int appId, int patientId, int doctorId,
                    const char* date, const char* timeSlot, AppointmentStatus status);

        int getAppointmentId() const;
        int getPatientId() const;
        int getDoctorId() const;
        const char* getDate() const;
        const char* getTimeSlot() const;
        int getId() const { return appointmentId; }
        AppointmentStatus getStatus() const;

        void setStatus(AppointmentStatus s);

        // Two appointments conflict if same doctor, date, time and neither is cancelled
        bool operator==(const Appointment& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Appointment& a);
        void display() const;

        static const char* statusToStr(AppointmentStatus s);
        static AppointmentStatus strToStatus(const char* s);
};
// For Storage<T> compatibility
