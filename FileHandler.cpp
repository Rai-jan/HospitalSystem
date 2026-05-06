#include "fileHandler.h"

bool FileHandler::AddDoctor( int id,const char* name,
    const char* specialization,const char* contact, 
    const char* password, float fee) {
    std::ofstream file;
    if(!appendFile(DOCTOR, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << id << "," << name << "," << specialization 
    << "," << contact << "," << password << "," << fee << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddPatient( int id,const char* name, int age,
    const char gender, const char* contact,const char* password,float balance) {
    std::ofstream file;
    if(!appendFile(PATIENT, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << id << "," << name << "," << age << "," << gender << "," 
    << contact << "," << password << "," << balance << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddAppointment( int App_id, int patientId, 
    int doctorId, const char* date, const char* time, const char* status) {
    std::ofstream file;
    if(!appendFile(APPOINTMENT, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << App_id << "," << patientId << "," << doctorId << "," 
    << date << "," << time << "," << status << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddDischargedPatient(int id,const char* name, int age,
    const char gender, const char* contact,const char* password,float balance) {
    std::ofstream file;
    if(!appendFile(DISCHARGED, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << id << "," << name << "," << age << "," << gender 
    << "," << contact << "," << password << "," << balance << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddBill( int billId, int patientId,int appointmentId, 
    float amount, const char* status,const char* date) {
    std::ofstream file;
    if(!appendFile(BILL, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << billId << "," << patientId << "," << appointmentId
     << "," << amount << "," << status << "," << date << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddPrescription( int prescriptionId, 
    int appointmentId,int patientId,int doctorId, const char* date,const char* medication,
     const char* notes) {
    std::ofstream file;
    if(!appendFile(PRESCRIPTION, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << prescriptionId << "," << appointmentId << "," << patientId << "," << doctorId << "," 
    << date << "," << medication << "," << notes << std::endl;
    file.close();
    return true;
}
bool FileHandler::AddSecurityLog(const char* timestamp, const char* role, int EnteredId, const char* result) {
    std::ofstream file;
    if(!appendFile(SECURITY_LOG, file)) {
        throw FileNotFoundException("Could not open file for appending");
    }
    file << timestamp << "," << role << "," << EnteredId << "," << result << std::endl;
    file.close();
    return true;
}