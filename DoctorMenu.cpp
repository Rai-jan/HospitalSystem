#include "DoctorMenu.h"
#include <iostream>

DoctorMenu::DoctorMenu(Doctor* d, Storage<Patient>& pts,
                       Storage<Appointment>& apps,
                       Storage<Prescription>& prescs,
                       Storage<Bill>& bls)
    : doctor(d), patients(pts), appointments(apps), prescriptions(prescs), bills(bls) {}

void DoctorMenu::run() {
    int choice = 0;
    do {
        std::cout << "\n===================================\n";
        std::cout << "Welcome, Dr. " << doctor->getName() << "\n";
        std::cout << "Specialization: " << doctor->getSpecialization() << "\n";
        std::cout << "===================================\n";
        std::cout << "1. View Today's Appointments\n";
        std::cout << "2. Mark Appointment Complete\n";
        std::cout << "3. Mark Appointment No-Show\n";
        std::cout << "4. Write Prescription\n";
        std::cout << "5. View Patient Medical History\n";
        std::cout << "6. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        clearInputBuffer();
        switch(choice) {
            case 1: viewTodaysAppointments(); break;
            case 2: markComplete(); break;
            case 3: markNoShow(); break;
            case 4: writePrescription(); break;
            case 5: viewPatientHistory(); break;
            case 6: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while(choice != 6);
}

void DoctorMenu::viewTodaysAppointments() {
    char today[11]; getTodayDate(today);
    int count = 0;
    // Collect indices
    int idx[200]; int k = 0;
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getDoctorId() == doctor->getId() && strEqual(a->getDate(), today)) {
            idx[k++] = i; count++;
        }
    }
    if(count == 0) { std::cout << "No appointments scheduled for today.\n"; return; }
    // Sort by time slot ascending (bubble)
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            const char* t1 = appointments.get(idx[j])->getTimeSlot();
            const char* t2 = appointments.get(idx[j+1])->getTimeSlot();
            bool swap = false;
            for(int m = 0; t1[m] != '\0' && t2[m] != '\0'; m++) {
                if(t1[m] > t2[m]) { swap = true; break; }
                if(t1[m] < t2[m]) break;
            }
            if(swap) { int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp; }
        }
    }
    std::cout << "\n--- Today's Appointments ---\n";
    for(int i = 0; i < count; i++) {
        Appointment* a = appointments.get(idx[i]);
        Patient* p = patients.findById(a->getPatientId());
        std::cout << "ID: " << a->getAppointmentId()
                  << " | Patient: " << (p ? p->getName() : "?")
                  << " | Time: " << a->getTimeSlot()
                  << " | Status: " << Appointment::statusToStr(a->getStatus()) << "\n";
    }
}

void DoctorMenu::markComplete() {
    viewTodaysAppointments();
    char today[11]; getTodayDate(today);
    char idStr[10];
    std::cout << "Enter Appointment ID: ";
    std::cin.getline(idStr, 10);
    int appId = CharToInt(idStr);
    Appointment* a = appointments.findById(appId);
    if(!a || a->getDoctorId() != doctor->getId() ||
       a->getStatus() != PENDING || !strEqual(a->getDate(), today)) {
        std::cout << "Invalid appointment ID.\n"; return;
    }
    a->setStatus(COMPLETED);
    FileHandler::saveAppointments(appointments);
    std::cout << "Appointment marked as completed.\n";
}

void DoctorMenu::markNoShow() {
    viewTodaysAppointments();
    char today[11]; getTodayDate(today);
    char idStr[10];
    std::cout << "Enter Appointment ID: ";
    std::cin.getline(idStr, 10);
    int appId = CharToInt(idStr);
    Appointment* a = appointments.findById(appId);
    if(!a || a->getDoctorId() != doctor->getId() ||
       a->getStatus() != PENDING || !strEqual(a->getDate(), today)) {
        std::cout << "Invalid appointment ID.\n"; return;
    }
    a->setStatus(NO_SHOW);
    FileHandler::saveAppointments(appointments);
    // Cancel bill
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getAppointmentId() == appId && b->getStatus() == UNPAID) {
            b->setStatus(BILL_CANCELLED); break;
        }
    }
    FileHandler::saveBills(bills);
    std::cout << "Appointment marked as no-show.\n";
}

void DoctorMenu::writePrescription() {
    char idStr[10];
    std::cout << "Enter Appointment ID: ";
    std::cin.getline(idStr, 10);
    int appId = CharToInt(idStr);
    Appointment* a = appointments.findById(appId);
    if(!a || a->getDoctorId() != doctor->getId() || a->getStatus() != COMPLETED) {
        std::cout << "Invalid appointment or not completed.\n"; return;
    }
    // Check if already written
    for(int i = 0; i < prescriptions.getCount(); i++) {
        if(prescriptions.get(i)->getAppointmentId() == appId) {
            std::cout << "Prescription already written for this appointment.\n"; return;
        }
    }

    char medication[500];
    char notes[300];
    std::cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    std::cin.getline(medication, 500);

    std::cout << "Enter notes (max 300 chars): ";
    std::cin.getline(notes, 300);

    // Truncate silently if needed (already handled by getline limits)
    char today[11]; getTodayDate(today);
    int newId = FileHandler::getMaxId(PRESCRIPTION) + 1;
    Prescription* p = new Prescription(newId, appId, a->getPatientId(),
                                        doctor->getId(), today, medication, notes);
    prescriptions.add(p);
    FileHandler::AddPrescription(newId, appId, a->getPatientId(),
                                  doctor->getId(), today, medication, notes);
    std::cout << "Prescription saved.\n";
}

void DoctorMenu::viewPatientHistory() {
    char idStr[10];
    std::cout << "Enter Patient ID: ";
    std::cin.getline(idStr, 10);
    int patId = CharToInt(idStr);

    Patient* pat = patients.findById(patId);
    if(!pat) { std::cout << "Access denied. You can only view records of your own patients.\n"; return; }

    // Check patient has at least one completed appointment with this doctor
    bool authorized = false;
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getPatientId() == patId && a->getDoctorId() == doctor->getId() && a->getStatus() == COMPLETED) {
            authorized = true; break;
        }
    }
    if(!authorized) {
        std::cout << "Access denied. You can only view records of your own patients.\n"; return;
    }

    // Collect prescriptions written by this doctor for this patient, sort descending
    int count = 0;
    for(int i = 0; i < prescriptions.getCount(); i++) {
        Prescription* p = prescriptions.get(i);
        if(p->getPatientId() == patId && p->getDoctorId() == doctor->getId()) count++;
    }
    if(count == 0) { std::cout << "No records found.\n"; return; }

    int* idx = new int[count];
    int k = 0;
    for(int i = 0; i < prescriptions.getCount(); i++) {
        Prescription* p = prescriptions.get(i);
        if(p->getPatientId() == patId && p->getDoctorId() == doctor->getId()) idx[k++] = i;
    }
    // Sort desc
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(compareDates(prescriptions.get(idx[j])->getDate(),
                            prescriptions.get(idx[j+1])->getDate()) < 0) {
                int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp;
            }
        }
    }
    std::cout << "\n--- Medical History for " << pat->getName() << " ---\n";
    for(int i = 0; i < count; i++) {
        prescriptions.get(idx[i])->display();
    }
    delete[] idx;
}
