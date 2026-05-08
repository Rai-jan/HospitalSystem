#include "Appointment.h"

Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0), status(PENDING) {
    date[0] = '\0'; timeSlot[0] = '\0';
}

Appointment::Appointment(int appId, int patId, int docId,
                         const char* d, const char* t, AppointmentStatus s)
    : appointmentId(appId), patientId(patId), doctorId(docId), status(s) {
    copyarray(date, d, getLength(d));
    copyarray(timeSlot, t, getLength(t));
}

int Appointment::getAppointmentId() const { return appointmentId; }
int Appointment::getPatientId() const { return patientId; }
int Appointment::getDoctorId() const { return doctorId; }
const char* Appointment::getDate() const { return date; }
const char* Appointment::getTimeSlot() const { return timeSlot; }
AppointmentStatus Appointment::getStatus() const { return status; }
void Appointment::setStatus(AppointmentStatus s) { status = s; }

bool Appointment::operator==(const Appointment& other) const {
    // Conflict: same doctor, same date, same time, neither cancelled
    if(status == CANCELLED || other.status == CANCELLED) return false;
    return doctorId == other.doctorId &&
           strEqual(date, other.date) &&
           strEqual(timeSlot, other.timeSlot);
}

std::ostream& operator<<(std::ostream& os, const Appointment& a) {
    os << "ID: " << a.appointmentId
       << " | PatientID: " << a.patientId
       << " | DoctorID: " << a.doctorId
       << " | Date: " << a.date
       << " | Time: " << a.timeSlot
       << " | Status: " << Appointment::statusToStr(a.status);
    return os;
}

void Appointment::display() const { std::cout << *this << std::endl; }

const char* Appointment::statusToStr(AppointmentStatus s) {
    switch(s) {
        case PENDING:   return "pending";
        case COMPLETED: return "completed";
        case CANCELLED: return "cancelled";
        case NO_SHOW:   return "no-show";
        default:        return "unknown";
    }
}

AppointmentStatus Appointment::strToStatus(const char* s) {
    if(strEqual(s, "pending"))   return PENDING;
    if(strEqual(s, "completed")) return COMPLETED;
    if(strEqual(s, "cancelled")) return CANCELLED;
    if(strEqual(s, "no-show"))   return NO_SHOW;
    return PENDING;
}
