#include "Admin.h"
#include <iostream>

const char* Admin::ADMIN_PASSWORD = "IamAdmin123"; // In a real system, this should be securely stored 

Admin::Admin() : Person(0, "Admin", "00000000000", Admin::ADMIN_PASSWORD) {}

void Admin::display() const {
    std::cout << "Admin: " << name << std::endl;
}

bool Admin::authenticate(const char* password) {
    return strEqual(password, Admin::ADMIN_PASSWORD);
}

void Admin::runMenu(Storage<Patient>& patients, Storage<Doctor>& doctors,
                    Storage<Appointment>& appointments, Storage<Bill>& bills,
                    Storage<Prescription>& prescriptions) {
    int choice = 0;
    do {
        std::cout << "\n===================================\n";
        std::cout << "   Admin Panel -- MediCore\n";
        std::cout << "===================================\n";
        std::cout << " 1. Add Doctor\n";
        std::cout << " 2. Remove Doctor\n";
        std::cout << " 3. Add Patient\n";
        std::cout << " 4. Remove Patient\n";
        std::cout << " 5. View All Patients\n";
        std::cout << " 6. View All Doctors\n";
        std::cout << " 7. View All Appointments\n";
        std::cout << " 8. View Unpaid Bills\n";
        std::cout << " 9. Discharge Patient\n";
        std::cout << "10. View Security Log\n";
        std::cout << "11. Generate Daily Report\n";
        std::cout << "12. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        clearInputBuffer();

        switch(choice) {
            case 1:  addDoctor(doctors); break;
            case 2:  removeDoctor(doctors, appointments); break;
            case 3:  addPatient(patients); break;
            case 4:  removePatient(patients, appointments, bills, prescriptions); break;
            case 5:  viewAllPatients(patients, bills); break;
            case 6:  viewAllDoctors(doctors); break;
            case 7:  viewAllAppointments(appointments, patients, doctors); break;
            case 8:  viewUnpaidBills(bills, patients); break;
            case 9:  dischargePatient(patients, appointments, bills, prescriptions); break;
            case 10: viewSecurityLog(); break;
            case 11: generateDailyReport(appointments, bills, patients, doctors); break;
            case 12: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while(choice != 12);
}

void Admin::addDoctor(Storage<Doctor>& doctors) {
    char name[51], spec[51], contact[12], password[51];
    float fee = 0.0f;

    std::cout << "Enter doctor name (max 50 chars): ";
    std::cin.getline(name, 51);
    if(!Validator::validateName(name)) { std::cout << "Invalid name.\n"; return; }

    std::cout << "Enter specialization (max 50 chars): ";
    std::cin.getline(spec, 51);
    if(!Validator::validateName(spec)) { std::cout << "Invalid specialization.\n"; return; }

    std::cout << "Enter contact (11 digits): ";
    std::cin.getline(contact, 12);
    if(!Validator::validateContact(contact)) { std::cout << "Invalid contact.\n"; return; }

    std::cout << "Enter password (min 6 chars, uppercase+lowercase+digit): ";
    std::cin.getline(password, 51);
    if(!Validator::validatePassword(password)) { std::cout << "Invalid password.\n"; return; }

    char feeStr[20];
    std::cout << "Enter consultation fee (PKR): ";
    std::cin.getline(feeStr, 20);
    if(!Validator::validatePositiveFloat(feeStr)) { std::cout << "Invalid fee.\n"; return; }
    fee = CharToFloat(feeStr);

    int newId = FileHandler::getMaxId(DOCTOR) + 1;
    Doctor* d = new Doctor(newId, name, spec, contact, password, fee);
    doctors.add(d);
    FileHandler::AddDoctor(newId, name, spec, contact, password, fee);
    std::cout << "Doctor added successfully. ID: " << newId << "\n";
}

void Admin::removeDoctor(Storage<Doctor>& doctors, Storage<Appointment>& appointments) {
    viewAllDoctors(doctors);
    if(doctors.getCount() == 0) return;

    char idStr[10];
    std::cout << "Enter Doctor ID to remove: ";
    std::cin.getline(idStr, 10);
    int docId = CharToInt(idStr);

    Doctor* doc = doctors.findById(docId);
    if(!doc) { std::cout << "Doctor not found.\n"; return; }

    // Check pending appointments
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getDoctorId() == docId && a->getStatus() == PENDING) {
            std::cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first.\n";
            return;
        }
    }

    doctors.deleteById(docId);
    FileHandler::saveDoctors(doctors);
    std::cout << "Doctor removed.\n";
}

void Admin::addPatient(Storage<Patient>& patients) {
    char name[51], contact[12], password[51], ageStr[5], balStr[30];
    char genderChar;

    std::cout << "Enter patient name (max 50 chars): ";
    std::cin.getline(name, 51);
    if(!Validator::validateName(name)) { std::cout << "Invalid name.\n"; return; }

    std::cout << "Enter age: ";
    std::cin.getline(ageStr, 5);
    if(!Validator::validateAge(ageStr)) { std::cout << "Invalid age.\n"; return; }
    int age = CharToInt(ageStr);

    std::cout << "Enter gender (M/F): ";
    std::cin >> genderChar; clearInputBuffer();
    Gender g;
    try { g = Validator::validateGender(genderChar); }
    catch(InvalidInputException& e) { e.what(); return; }

    std::cout << "Enter contact (11 digits): ";
    std::cin.getline(contact, 12);
    if(!Validator::validateContact(contact)) { std::cout << "Invalid contact.\n"; return; }

    std::cout << "Enter password (min 6, uppercase+lowercase+digit): ";
    std::cin.getline(password, 51);
    if(!Validator::validatePassword(password)) { std::cout << "Invalid password.\n"; return; }

    std::cout << "Enter initial balance (PKR): ";
    std::cin.getline(balStr, 30);
    long long balance = 0;
    if(!Validator::validateBalance(balStr, balance)) {
        if(getLength(balStr) > 18) {
            std::cout << "Error: Balance is too large to handle.\n";
        } else {
            std::cout << "Invalid balance amount.\n";
        }
        return;
    }

    char gc = (g == MALE) ? 'M' : 'F';
    int newId = FileHandler::getMaxId(PATIENT) + 1;
    Patient* p = new Patient(newId, name, age, gc, contact, password, balance);
    patients.add(p);
    FileHandler::AddPatient(newId, name, age, gc, contact, password, balance);
    std::cout << "Patient added successfully. ID: " << newId << "\n";
}

void Admin::removePatient(Storage<Patient>& patients, Storage<Appointment>& appointments,
                           Storage<Bill>& bills, Storage<Prescription>& prescriptions) {
    viewAllPatients(patients, bills);
    if(patients.getCount() == 0) return;

    char idStr[10];
    std::cout << "Enter Patient ID to remove: ";
    std::cin.getline(idStr, 10);
    int patId = CharToInt(idStr);

    Patient* pat = patients.findById(patId);
    if(!pat) { std::cout << "Patient not found.\n"; return; }

    // Check pending
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getPatientId() == patId && a->getStatus() == PENDING) {
            std::cout << "Cannot remove patient with pending appointments.\n";
            return;
        }
    }
    // Check unpaid bills
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getPatientId() == patId && b->getStatus() == UNPAID) {
            std::cout << "Cannot remove patient with unpaid bills.\n";
            return;
        }
    }

    // Delete all related records
    for(int i = prescriptions.getCount() - 1; i >= 0; i--) {
        if(prescriptions.get(i)->getPatientId() == patId) prescriptions.deleteAt(i);
    }
    for(int i = bills.getCount() - 1; i >= 0; i--) {
        if(bills.get(i)->getPatientId() == patId) bills.deleteAt(i);
    }
    for(int i = appointments.getCount() - 1; i >= 0; i--) {
        if(appointments.get(i)->getPatientId() == patId) appointments.deleteAt(i);
    }
    patients.deleteById(patId);

    FileHandler::savePatients(patients);
    FileHandler::saveAppointments(appointments);
    FileHandler::saveBills(bills);
    FileHandler::savePrescriptions(prescriptions);
    std::cout << "Patient removed successfully.\n";
}

void Admin::viewAllPatients(const Storage<Patient>& patients, const Storage<Bill>& bills) {
    if(patients.getCount() == 0) { std::cout << "No patients found.\n"; return; }
    std::cout << "\n--- All Patients ---\n";
    for(int i = 0; i < patients.getCount(); i++) {
        Patient* p = patients.get(i);
        int unpaid = 0;
        for(int j = 0; j < bills.getCount(); j++) {
            if(bills.get(j)->getPatientId() == p->getId() && bills.get(j)->getStatus() == UNPAID)
                unpaid++;
        }
        std::cout << *p << " | Unpaid Bills: " << unpaid << "\n";
    }
}

void Admin::viewAllDoctors(const Storage<Doctor>& doctors) {
    if(doctors.getCount() == 0) { std::cout << "No doctors found.\n"; return; }
    std::cout << "\n--- All Doctors ---\n";
    for(int i = 0; i < doctors.getCount(); i++) {
        std::cout << *(doctors.get(i)) << "\n";
    }
}

void Admin::viewAllAppointments(const Storage<Appointment>& appointments,
                                 const Storage<Patient>& patients,
                                 const Storage<Doctor>& doctors) {
    if(appointments.getCount() == 0) { std::cout << "No appointments found.\n"; return; }

    // Sort by date descending - work on a temp index array (no extra alloc needed, just print sorted)
    // We'll do a simple display with inline sort of indices
    int n = appointments.getCount();
    int* idx = new int[n];
    for(int i = 0; i < n; i++) idx[i] = i;
    // Bubble sort descending
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            const char* d1 = appointments.get(idx[j])->getDate();
            const char* d2 = appointments.get(idx[j+1])->getDate();
            if(compareDates(d1, d2) < 0) { int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp; }
        }
    }
    std::cout << "\n--- All Appointments ---\n";
    for(int i = 0; i < n; i++) {
        Appointment* a = appointments.get(idx[i]);
        const char* patName = "Unknown";
        const char* docName = "Unknown";
        Patient* pat = patients.findById(a->getPatientId());
        Doctor*  doc = doctors.findById(a->getDoctorId());
        if(pat) patName = pat->getName();
        if(doc) docName = doc->getName();
        std::cout << "ID: " << a->getAppointmentId()
                  << " | Patient: " << patName
                  << " | Doctor: " << docName
                  << " | Date: " << a->getDate()
                  << " | Time: " << a->getTimeSlot()
                  << " | Status: " << Appointment::statusToStr(a->getStatus()) << "\n";
    }
    delete[] idx;
}

void Admin::viewUnpaidBills(const Storage<Bill>& bills, const Storage<Patient>& patients) {
    char today[11];
    getTodayDate(today);
    bool found = false;
    std::cout << "\n--- Unpaid Bills ---\n";
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getStatus() != UNPAID) continue;
        found = true;
        const char* patName = "Unknown";
        Patient* pat = patients.findById(b->getPatientId());
        if(pat) patName = pat->getName();
        int daysOld = daysBetween(b->getDate(), today);
        std::cout << "Bill ID: " << b->getBillId()
                  << " | Patient: " << patName
                  << " | Amount: PKR " << b->getAmount()
                  << " | Date: " << b->getDate();
        if(daysOld > 7) std::cout << " [OVERDUE]";
        std::cout << "\n";
    }
    if(!found) std::cout << "No unpaid bills.\n";
}

void Admin::dischargePatient(Storage<Patient>& patients, Storage<Appointment>& appointments,
                              Storage<Bill>& bills, Storage<Prescription>& prescriptions) {
    char idStr[10];
    std::cout << "Enter Patient ID: ";
    std::cin.getline(idStr, 10);
    int patId = CharToInt(idStr);

    Patient* pat = patients.findById(patId);
    if(!pat) { std::cout << "Patient not found.\n"; return; }

    for(int i = 0; i < bills.getCount(); i++) {
        if(bills.get(i)->getPatientId() == patId && bills.get(i)->getStatus() == UNPAID) {
            std::cout << "Cannot discharge patient with unpaid bills.\n"; return;
        }
    }
    for(int i = 0; i < appointments.getCount(); i++) {
        if(appointments.get(i)->getPatientId() == patId && appointments.get(i)->getStatus() == PENDING) {
            std::cout << "Cannot discharge patient with pending appointments.\n"; return;
        }
    }

    // Archive to discharged.txt
    FileHandler::AddDischargedPatient(pat->getId(), pat->getName(), pat->getAge(),
                                       pat->getGender(), pat->getContact(),
                                       pat->getPassword(), pat->getBalance());

    // Archive appointments, bills, prescriptions to discharged.txt (same file as simple CSV lines)
    // Per spec: copy all their rows from each file into discharged.txt
    {
        std::ofstream f("discharged.txt", std::ios::app);
        for(int i = 0; i < appointments.getCount(); i++) {
            Appointment* a = appointments.get(i);
            if(a->getPatientId() != patId) continue;
            f << "APT," << a->getAppointmentId() << "," << a->getPatientId() << ","
              << a->getDoctorId() << "," << a->getDate() << "," << a->getTimeSlot()
              << "," << Appointment::statusToStr(a->getStatus()) << "\n";
        }
        for(int i = 0; i < bills.getCount(); i++) {
            Bill* b = bills.get(i);
            if(b->getPatientId() != patId) continue;
            f << "BILL," << b->getBillId() << "," << b->getPatientId() << ","
              << b->getAppointmentId() << "," << b->getAmount() << ","
              << Bill::statusToStr(b->getStatus()) << "," << b->getDate() << "\n";
        }
        for(int i = 0; i < prescriptions.getCount(); i++) {
            Prescription* p = prescriptions.get(i);
            if(p->getPatientId() != patId) continue;
            f << "PRESC," << p->getPrescriptionId() << "," << p->getAppointmentId() << ","
              << p->getPatientId() << "," << p->getDoctorId() << "," << p->getDate()
              << "," << p->getMedication() << "," << p->getNotes() << "\n";
        }
        f.close();
    }

    // Remove from live data
    for(int i = prescriptions.getCount()-1; i >= 0; i--)
        if(prescriptions.get(i)->getPatientId() == patId) prescriptions.deleteAt(i);
    for(int i = bills.getCount()-1; i >= 0; i--)
        if(bills.get(i)->getPatientId() == patId) bills.deleteAt(i);
    for(int i = appointments.getCount()-1; i >= 0; i--)
        if(appointments.get(i)->getPatientId() == patId) appointments.deleteAt(i);
    patients.deleteById(patId);

    FileHandler::savePatients(patients);
    FileHandler::saveAppointments(appointments);
    FileHandler::saveBills(bills);
    FileHandler::savePrescriptions(prescriptions);
    std::cout << "Patient discharged and archived successfully.\n";
}

void Admin::viewSecurityLog() {
    FileHandler::printSecurityLog();
}

void Admin::generateDailyReport(const Storage<Appointment>& appointments,
                                  const Storage<Bill>& bills,
                                  const Storage<Patient>& patients,
                                  const Storage<Doctor>& doctors) {
    char today[11];
    getTodayDate(today);
    std::cout << "\n=== Daily Report for " << today << " ===\n";

    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(!strEqual(a->getDate(), today)) continue;
        total++;
        switch(a->getStatus()) {
            case PENDING:   pending++; break;
            case COMPLETED: completed++; break;
            case NO_SHOW:   noshow++; break;
            case CANCELLED: cancelled++; break;
        }
    }
    std::cout << "Total appointments today: " << total
              << " (Pending: " << pending << " Completed: " << completed
              << " No-show: " << noshow << " Cancelled: " << cancelled << ")\n";

    float revenue = 0.0f;
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(strEqual(b->getDate(), today) && b->getStatus() == PAID)
            revenue += b->getAmount();
    }
    std::cout << "Revenue collected today (paid bills): PKR " << revenue << "\n";

    // Patients with outstanding unpaid bills
    std::cout << "\nPatients with outstanding unpaid bills:\n";
    bool anyUnpaid = false;
    for(int i = 0; i < patients.getCount(); i++) {
        Patient* p = patients.get(i);
        float owed = 0.0f;
        for(int j = 0; j < bills.getCount(); j++) {
            Bill* b = bills.get(j);
            if(b->getPatientId() == p->getId() && b->getStatus() == UNPAID)
                owed += b->getAmount();
        }
        if(owed > 0) {
            std::cout << "  " << p->getName() << " | Total Owed: PKR " << owed << "\n";
            anyUnpaid = true;
        }
    }
    if(!anyUnpaid) std::cout << "  None\n";

    // Doctor-wise summary
    std::cout << "\nDoctor-wise summary for today:\n";
    for(int i = 0; i < doctors.getCount(); i++) {
        Doctor* d = doctors.get(i);
        int dc = 0, dp = 0, dn = 0;
        for(int j = 0; j < appointments.getCount(); j++) {
            Appointment* a = appointments.get(j);
            if(a->getDoctorId() != d->getId() || !strEqual(a->getDate(), today)) continue;
            if(a->getStatus() == COMPLETED) dc++;
            else if(a->getStatus() == PENDING) dp++;
            else if(a->getStatus() == NO_SHOW) dn++;
        }
        if(dc + dp + dn > 0) {
            std::cout << "  Dr. " << d->getName()
                      << " | Completed: " << dc << " | Pending: " << dp
                      << " | No-show: " << dn << "\n";
        }
    }
}
