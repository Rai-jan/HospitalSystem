#include "Bill.h"
#include <iostream>

Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0f), status(UNPAID) {
    date[0] = '\0';
}

Bill::Bill(int bId, int pId, int aId, float amt, BillStatus s, const char* d)
    : billId(bId), patientId(pId), appointmentId(aId), amount(amt), status(s) {
    copyarray(date, d, getLength(d));
}

int Bill::getBillId() const { return billId; }
int Bill::getPatientId() const { return patientId; }
int Bill::getAppointmentId() const { return appointmentId; }
float Bill::getAmount() const { return amount; }
BillStatus Bill::getStatus() const { return status; }
const char* Bill::getDate() const { return date; }
void Bill::setStatus(BillStatus s) { status = s; }

void Bill::display() const {
    std::cout << "Bill ID: " << billId
              << " | Appt ID: " << appointmentId
              << " | Amount: PKR " << amount
              << " | Status: " << statusToStr(status)
              << " | Date: " << date << std::endl;
}

const char* Bill::statusToStr(BillStatus s) {
    switch(s) {
        case UNPAID:         return "unpaid";
        case PAID:           return "paid";
        case BILL_CANCELLED: return "cancelled";
        default:             return "unknown";
    }
}

BillStatus Bill::strToStatus(const char* s) {
    if(strEqual(s, "paid"))      return PAID;
    if(strEqual(s, "unpaid"))    return UNPAID;
    if(strEqual(s, "cancelled")) return BILL_CANCELLED;
    return UNPAID;
}
