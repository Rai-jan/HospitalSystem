#include "Prescription.h"
#include <iostream>

Prescription::Prescription()
    : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
    date[0] = '\0'; medication[0] = '\0'; notes[0] = '\0';
}

Prescription::Prescription(int pId, int aId, int patId, int docId,
                           const char* d, const char* med, const char* n)
    : prescriptionId(pId), appointmentId(aId), patientId(patId), doctorId(docId) {
    copyarray(date, d, getLength(d));
    int mLen = getLength(med); if(mLen >= 499) mLen = 499;
    copyarray(medication, med, mLen);
    int nLen = getLength(n); if(nLen >= 299) nLen = 299;
    copyarray(notes, n, nLen);
}

int Prescription::getPrescriptionId() const { return prescriptionId; }
int Prescription::getAppointmentId() const { return appointmentId; }
int Prescription::getPatientId() const { return patientId; }
int Prescription::getDoctorId() const { return doctorId; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMedication() const { return medication; }
const char* Prescription::getNotes() const { return notes; }

void Prescription::display() const {
    std::cout << "Prescription ID: " << prescriptionId
              << " | Appt ID: " << appointmentId
              << " | Date: " << date << std::endl
              << "  Medicines: " << medication << std::endl
              << "  Notes: " << notes << std::endl;
}
