#include "FileHandler.h"

// Parse a CSV line into fields array, returns count
int FileHandler::parseCSV(const char* line, char fields[][500], int maxFields) {
    int fieldIdx = 0;
    int charIdx = 0;
    int lineLen = getLength(line);
    for(int i = 0; i <= lineLen && fieldIdx < maxFields; i++) {
        if(line[i] == ',' || line[i] == '\0') {
            fields[fieldIdx][charIdx] = '\0';
            fieldIdx++;
            charIdx = 0;
        } else {
            fields[fieldIdx][charIdx++] = line[i];
        }
    }
    return fieldIdx;
}

bool FileHandler::AddDoctor(int id, const char* name, const char* spec,
                             const char* contact, const char* password, float fee) {
    std::ofstream file;
    if(!appendFile(DOCTOR, file)) throw FileNotFoundException("Could not open doctors.txt");
    file << id << "," << name << "," << spec << "," << contact << ","
         << password << "," << fee << "\n";
    file.close();
    return true;
}

bool FileHandler::AddPatient(int id, const char* name, int age, char gender,
                              const char* contact, const char* password, long long balance) {
    std::ofstream file;
    if(!appendFile(PATIENT, file)) throw FileNotFoundException("Could not open patients.txt");
    file << id << "," << name << "," << age << "," << gender << ","
         << contact << "," << password << "," << balance << "\n";
    file.close();
    return true;
}

bool FileHandler::AddAppointment(int appId, int patientId, int doctorId,
                                  const char* date, const char* time, const char* status) {
    std::ofstream file;
    if(!appendFile(APPOINTMENT, file)) throw FileNotFoundException("Could not open appointments.txt");
    file << appId << "," << patientId << "," << doctorId << ","
         << date << "," << time << "," << status << "\n";
    file.close();
    return true;
}

bool FileHandler::AddBill(int billId, int patientId, int appointmentId,
                           float amount, const char* status, const char* date) {
    std::ofstream file;
    if(!appendFile(BILL, file)) throw FileNotFoundException("Could not open bills.txt");
    file << billId << "," << patientId << "," << appointmentId << ","
         << amount << "," << status << "," << date << "\n";
    file.close();
    return true;
}

bool FileHandler::AddPrescription(int prescId, int appId, int patId, int docId,
                                   const char* date, const char* medication, const char* notes) {
    std::ofstream file;
    if(!appendFile(PRESCRIPTION, file)) throw FileNotFoundException("Could not open prescriptions.txt");
    file << prescId << "," << appId << "," << patId << "," << docId << ","
         << date << "," << medication << "," << notes << "\n";
    file.close();
    return true;
}

bool FileHandler::AddSecurityLog(const char* timestamp, const char* role,
                                  int enteredId, const char* result) {
    std::ofstream file;
    if(!appendFile(SECURITY_LOG, file)) throw FileNotFoundException("Could not open security_log.txt");
    file << timestamp << "," << role << "," << enteredId << "," << result << "\n";
    file.close();
    return true;
}

bool FileHandler::AddDischargedPatient(int id, const char* name, int age, char gender,
                                        const char* contact, const char* password, long long balance) {
    std::ofstream file;
    if(!appendFile(DISCHARGED, file)) throw FileNotFoundException("Could not open discharged.txt");
    file << id << "," << name << "," << age << "," << gender << ","
         << contact << "," << password << "," << balance << "\n";
    file.close();
    return true;
}

void FileHandler::loadPatients(Storage<Patient>& store) {
    std::ifstream file;
    if(!readFile(PATIENT, file)) return; // file may not exist yet
    char line[1000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        char fields[10][500];
        int n = parseCSV(line, fields, 10);
        if(n < 7) continue;
        int id = CharToInt(fields[0]);
        int age = CharToInt(fields[2]);
        char gender = fields[3][0];
        long long balance = CharToLongLong(fields[6]);
        Patient* p = new Patient(id, fields[1], age, gender, fields[4], fields[5], balance);
        store.add(p);
    }
    file.close();
}

void FileHandler::loadDoctors(Storage<Doctor>& store) {
    std::ifstream file;
    if(!readFile(DOCTOR, file)) return;
    char line[1000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        char fields[10][500];
        int n = parseCSV(line, fields, 10);
        if(n < 6) continue;
        int id = CharToInt(fields[0]);
        float fee = CharToFloat(fields[5]);
        Doctor* d = new Doctor(id, fields[1], fields[2], fields[3], fields[4], fee);
        store.add(d);
    }
    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& store) {
    std::ifstream file;
    if(!readFile(APPOINTMENT, file)) return;
    char line[1000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        char fields[10][500];
        int n = parseCSV(line, fields, 10);
        if(n < 6) continue;
        int appId = CharToInt(fields[0]);
        int patId = CharToInt(fields[1]);
        int docId = CharToInt(fields[2]);
        AppointmentStatus status = Appointment::strToStatus(fields[5]);
        Appointment* a = new Appointment(appId, patId, docId, fields[3], fields[4], status);
        store.add(a);
    }
    file.close();
}

void FileHandler::loadBills(Storage<Bill>& store) {
    std::ifstream file;
    if(!readFile(BILL, file)) return;
    char line[1000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        char fields[10][500];
        int n = parseCSV(line, fields, 10);
        if(n < 6) continue;
        int billId = CharToInt(fields[0]);
        int patId  = CharToInt(fields[1]);
        int appId  = CharToInt(fields[2]);
        float amount = CharToFloat(fields[3]);
        BillStatus status = Bill::strToStatus(fields[4]);
        Bill* b = new Bill(billId, patId, appId, amount, status, fields[5]);
        store.add(b);
    }
    file.close();
}

void FileHandler::loadPrescriptions(Storage<Prescription>& store) {
    std::ifstream file;
    if(!readFile(PRESCRIPTION, file)) return;
    char line[2000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        char fields[10][500];
        int n = parseCSV(line, fields, 10);
        if(n < 7) continue;
        int pId  = CharToInt(fields[0]);
        int aId  = CharToInt(fields[1]);
        int patId = CharToInt(fields[2]);
        int docId = CharToInt(fields[3]);
        Prescription* p = new Prescription(pId, aId, patId, docId,
                                            fields[4], fields[5], fields[6]);
        store.add(p);
    }
    file.close();
}

void FileHandler::savePatients(const Storage<Patient>& store) {
    std::ofstream file;
    if(!writeFile(PATIENT, file)) throw FileNotFoundException("Could not write patients.txt");
    for(int i = 0; i < store.getCount(); i++) {
        Patient* p = store.get(i);
        file << p->getId() << "," << p->getName() << "," << p->getAge() << ","
             << p->getGender() << "," << p->getContact() << "," << p->getPassword()
             << "," << p->getBalance() << "\n";
    }
    file.close();
}

void FileHandler::saveDoctors(const Storage<Doctor>& store) {
    std::ofstream file;
    if(!writeFile(DOCTOR, file)) throw FileNotFoundException("Could not write doctors.txt");
    for(int i = 0; i < store.getCount(); i++) {
        Doctor* d = store.get(i);
        file << d->getId() << "," << d->getName() << "," << d->getSpecialization()
             << "," << d->getContact() << "," << d->getPassword()
             << "," << d->getFee() << "\n";
    }
    file.close();
}

void FileHandler::saveAppointments(const Storage<Appointment>& store) {
    std::ofstream file;
    if(!writeFile(APPOINTMENT, file)) throw FileNotFoundException("Could not write appointments.txt");
    for(int i = 0; i < store.getCount(); i++) {
        Appointment* a = store.get(i);
        file << a->getAppointmentId() << "," << a->getPatientId() << ","
             << a->getDoctorId() << "," << a->getDate() << "," << a->getTimeSlot()
             << "," << Appointment::statusToStr(a->getStatus()) << "\n";
    }
    file.close();
}

void FileHandler::saveBills(const Storage<Bill>& store) {
    std::ofstream file;
    if(!writeFile(BILL, file)) throw FileNotFoundException("Could not write bills.txt");
    for(int i = 0; i < store.getCount(); i++) {
        Bill* b = store.get(i);
        file << b->getBillId() << "," << b->getPatientId() << ","
             << b->getAppointmentId() << "," << b->getAmount()
             << "," << Bill::statusToStr(b->getStatus()) << "," << b->getDate() << "\n";
    }
    file.close();
}

void FileHandler::savePrescriptions(const Storage<Prescription>& store) {
    std::ofstream file;
    if(!writeFile(PRESCRIPTION, file)) throw FileNotFoundException("Could not write prescriptions.txt");
    for(int i = 0; i < store.getCount(); i++) {
        Prescription* p = store.get(i);
        file << p->getPrescriptionId() << "," << p->getAppointmentId() << ","
             << p->getPatientId() << "," << p->getDoctorId() << "," << p->getDate()
             << "," << p->getMedication() << "," << p->getNotes() << "\n";
    }
    file.close();
}

int FileHandler::getMaxId(FileType type) {
    std::ifstream file;
    if(!readFile(type, file)) return 0;
    int maxId = 0;
    char line[2000];
    while(file.getline(line, sizeof(line))) {
        if(line[0] == '\0') continue;
        int id = CharToInt(line);
        if(id > maxId) maxId = id;
    }
    file.close();
    return maxId;
}

void FileHandler::printSecurityLog() {
    std::ifstream file;
    if(!readFile(SECURITY_LOG, file)) {
        std::cout << "No security events logged." << std::endl;
        return;
    }
    char line[500];
    bool hasData = false;
    while(file.getline(line, sizeof(line))) {
        if(line[0] != '\0') { std::cout << line << std::endl; hasData = true; }
    }
    file.close();
    if(!hasData) std::cout << "No security events logged." << std::endl;
}
