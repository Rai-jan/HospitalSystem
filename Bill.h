#pragma once
#include "Utility.h"
#include "Types.h"
#include <iostream>

class Bill {
    private:
        int billId;
        int patientId;
        int appointmentId;
        float amount;
        BillStatus status;
        char date[11]; // DD-MM-YYYY
    public:
        Bill();
        Bill(int billId, int patientId, int appointmentId,
             float amount, BillStatus status, const char* date);

        int getBillId() const;
        int getPatientId() const;
        int getAppointmentId() const;
        float getAmount() const;
        int getId() const { return billId; }
        BillStatus getStatus() const;
        const char* getDate() const;

        void setStatus(BillStatus s);

        void display() const;

        static const char* statusToStr(BillStatus s);
        static BillStatus strToStatus(const char* s);
};
