#include "PatientMenu.h"
#include <iostream>

static const char* SLOTS[] = {"09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00"};
static const int NUM_SLOTS = 8;

PatientMenu::PatientMenu(Patient* p,
                         Storage<Patient>& pts, Storage<Doctor>& docs,
                         Storage<Appointment>& apps, Storage<Bill>& bls,
                         Storage<Prescription>& prescs)
    : patient(p), patients(pts), doctors(docs),
      appointments(apps), bills(bls), prescriptions(prescs) {}

void PatientMenu::run() {
    int choice = 0;
    do {
        std::cout << "\n===================================\n";
        std::cout << "Welcome, " << patient->getName() << "\n";
        std::cout << "Balance: PKR " << patient->getBalance() << "\n";
        std::cout << "===================================\n";
        std::cout << "1. Book Appointment\n";
        std::cout << "2. Cancel Appointment\n";
        std::cout << "3. View My Appointments\n";
        std::cout << "4. View My Medical Records\n";
        std::cout << "5. View My Bills\n";
        std::cout << "6. Pay Bill\n";
        std::cout << "7. Top Up Balance\n";
        std::cout << "8. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        clearInputBuffer();
        switch(choice) {
            case 1: bookAppointment(); break;
            case 2: cancelAppointment(); break;
            case 3: viewAppointments(); break;
            case 4: viewMedicalRecords(); break;
            case 5: viewBills(); break;
            case 6: payBill(); break;
            case 7: topUpBalance(); break;
            case 8: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while(choice != 8);
}

bool PatientMenu::isSlotTaken(int doctorId, const char* date, const char* timeSlot) {
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getDoctorId() == doctorId &&
           strEqual(a->getDate(), date) &&
           strEqual(a->getTimeSlot(), timeSlot) &&
           a->getStatus() != CANCELLED)
            return true;
    }
    return false;
}

void PatientMenu::displayAvailableSlots(int doctorId, const char* date) {
    std::cout << "Available slots: ";
    bool any = false;
    for(int i = 0; i < NUM_SLOTS; i++) {
        if(!isSlotTaken(doctorId, date, SLOTS[i])) {
            std::cout << SLOTS[i] << " ";
            any = true;
        }
    }
    if(!any) std::cout << "None";
    std::cout << "\n";
}

void PatientMenu::bookAppointment() {
    char specSearch[51];
    std::cout << "Enter specialization to search: ";
    std::cin.getline(specSearch, 51);

    char specLower[51];
    toLowerStr(specLower, specSearch);

    // Display matching doctors
    bool found = false;
    for(int i = 0; i < doctors.getCount(); i++) {
        Doctor* d = doctors.get(i);
        char docSpecLower[51];
        toLowerStr(docSpecLower, d->getSpecialization());
        if(strEqual(docSpecLower, specLower)) {
            std::cout << "ID: " << d->getId() << " | " << d->getName()
                      << " | " << d->getSpecialization() << " | Fee: PKR " << d->getFee() << "\n";
            found = true;
        }
    }
    if(!found) { std::cout << "No doctors available for that specialization.\n"; return; }

    char docIdStr[10];
    std::cout << "Enter Doctor ID: ";
    std::cin.getline(docIdStr, 10);
    int docId = CharToInt(docIdStr);
    Doctor* doc = doctors.findById(docId);
    if(!doc) { std::cout << "Doctor not found.\n"; return; }

    // Validate date with up to 3 attempts
    char date[11];
    int attempts = 0;
    bool validDate = false;
    while(attempts < 3 && !validDate) {
        std::cout << "Enter date (DD-MM-YYYY): ";
        std::cin.getline(date, 11);
        if(Validator::validateDate(date)) { validDate = true; }
        else { std::cout << "Invalid date. Use format DD-MM-YYYY.\n"; attempts++; }
    }
    if(!validDate) { std::cout << "Too many invalid attempts.\n"; return; }

    // Check balance >= fee
    if(patient->getBalance() < (long long)doc->getFee()) {
        try { throw InsufficientFundsException("Insufficient funds to book this appointment."); }
        catch(InsufficientFundsException& e) { e.what(); return; }
    }

    // Show and pick slot
    char timeSlot[6];
    int slotAttempts = 0;
    bool booked = false;
    while(!booked) {
        displayAvailableSlots(docId, date);
        std::cout << "Enter time slot (e.g. 09:00): ";
        std::cin.getline(timeSlot, 6);
        if(!Validator::validateTimeSlot(timeSlot)) {
            std::cout << "Invalid time slot.\n"; slotAttempts++;
            if(slotAttempts >= 3) { std::cout << "Too many attempts.\n"; return; }
            continue;
        }
        if(isSlotTaken(docId, date, timeSlot)) {
            try { throw SlotUnavailableException("That slot is already booked. Please choose another."); }
            catch(SlotUnavailableException& e) { e.what(); }
            continue;
        }
        booked = true;
    }

    // Generate IDs
    int newAppId  = FileHandler::getMaxId(APPOINTMENT) + 1;
    int newBillId = FileHandler::getMaxId(BILL) + 1;

    // Deduct balance
    *patient -= (long long)doc->getFee();
    FileHandler::savePatients(patients);

    // Save appointment
    Appointment* app = new Appointment(newAppId, patient->getId(), docId, date, timeSlot, PENDING);
    appointments.add(app);
    FileHandler::AddAppointment(newAppId, patient->getId(), docId, date, timeSlot, "pending");

    // Save bill
    char today[11]; getTodayDate(today);
    Bill* bill = new Bill(newBillId, patient->getId(), newAppId, doc->getFee(), UNPAID, today);
    bills.add(bill);
    FileHandler::AddBill(newBillId, patient->getId(), newAppId, doc->getFee(), "unpaid", today);

    std::cout << "Appointment booked successfully. Appointment ID: " << newAppId << "\n";
}

void PatientMenu::cancelAppointment() {
    // Show pending appointments
    bool any = false;
    for(int i = 0; i < appointments.getCount(); i++) {
        Appointment* a = appointments.get(i);
        if(a->getPatientId() == patient->getId() && a->getStatus() == PENDING) {
            Doctor* d = doctors.findById(a->getDoctorId());
            std::cout << "ID: " << a->getAppointmentId()
                      << " | Doctor: " << (d ? d->getName() : "?")
                      << " | Date: " << a->getDate()
                      << " | Time: " << a->getTimeSlot() << "\n";
            any = true;
        }
    }
    if(!any) { std::cout << "You have no pending appointments.\n"; return; }

    char idStr[10];
    std::cout << "Enter Appointment ID to cancel: ";
    std::cin.getline(idStr, 10);
    int appId = CharToInt(idStr);

    Appointment* app = appointments.findById(appId);
    if(!app || app->getPatientId() != patient->getId() || app->getStatus() != PENDING) {
        std::cout << "Invalid appointment ID.\n"; return;
    }

    // Find fee via doctor
    Doctor* doc = doctors.findById(app->getDoctorId());
    long long fee = doc ? (long long)doc->getFee() : 0;

    app->setStatus(CANCELLED);
    FileHandler::saveAppointments(appointments);

    // Refund
    *patient += fee;
    FileHandler::savePatients(patients);

    // Cancel bill
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getAppointmentId() == appId && b->getStatus() == UNPAID) {
            b->setStatus(BILL_CANCELLED);
            break;
        }
    }
    FileHandler::saveBills(bills);

    std::cout << "Appointment cancelled. PKR " << fee << " refunded to your balance.\n";
}

void PatientMenu::viewAppointments() {
    // Collect patient's appointments
    int count = 0;
    for(int i = 0; i < appointments.getCount(); i++) {
        if(appointments.get(i)->getPatientId() == patient->getId()) count++;
    }
    if(count == 0) { std::cout << "No appointments found.\n"; return; }

    // Create a small index array and sort by date ascending
    int* idx = new int[count];
    int k = 0;
    for(int i = 0; i < appointments.getCount(); i++) {
        if(appointments.get(i)->getPatientId() == patient->getId()) idx[k++] = i;
    }
    // Bubble sort ascending
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(compareDates(appointments.get(idx[j])->getDate(),
                            appointments.get(idx[j+1])->getDate()) > 0) {
                int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp;
            }
        }
    }
    std::cout << "\n--- My Appointments ---\n";
    for(int i = 0; i < count; i++) {
        Appointment* a = appointments.get(idx[i]);
        Doctor* d = doctors.findById(a->getDoctorId());
        std::cout << "ID: " << a->getAppointmentId()
                  << " | Doctor: " << (d ? d->getName() : "?")
                  << " | Spec: " << (d ? d->getSpecialization() : "?")
                  << " | Date: " << a->getDate()
                  << " | Time: " << a->getTimeSlot()
                  << " | Status: " << Appointment::statusToStr(a->getStatus()) << "\n";
    }
    delete[] idx;
}

void PatientMenu::viewMedicalRecords() {
    int count = 0;
    for(int i = 0; i < prescriptions.getCount(); i++) {
        if(prescriptions.get(i)->getPatientId() == patient->getId()) count++;
    }
    if(count == 0) { std::cout << "No medical records found.\n"; return; }

    int* idx = new int[count];
    int k = 0;
    for(int i = 0; i < prescriptions.getCount(); i++) {
        if(prescriptions.get(i)->getPatientId() == patient->getId()) idx[k++] = i;
    }
    // Bubble sort descending (most recent first)
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(compareDates(prescriptions.get(idx[j])->getDate(),
                            prescriptions.get(idx[j+1])->getDate()) < 0) {
                int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp;
            }
        }
    }
    std::cout << "\n--- My Medical Records ---\n";
    for(int i = 0; i < count; i++) {
        Prescription* p = prescriptions.get(idx[i]);
        Doctor* d = doctors.findById(p->getDoctorId());
        std::cout << "Date: " << p->getDate()
                  << " | Doctor: " << (d ? d->getName() : "?")
                  << "\n  Medicines: " << p->getMedication()
                  << "\n  Notes: " << p->getNotes() << "\n";
    }
    delete[] idx;
}

void PatientMenu::viewBills() {
    float totalUnpaid = 0.0f;
    bool any = false;
    std::cout << "\n--- My Bills ---\n";
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getPatientId() != patient->getId()) continue;
        std::cout << "Bill ID: " << b->getBillId()
                  << " | Appt ID: " << b->getAppointmentId()
                  << " | Amount: PKR " << b->getAmount()
                  << " | Status: " << Bill::statusToStr(b->getStatus())
                  << " | Date: " << b->getDate() << "\n";
        if(b->getStatus() == UNPAID) totalUnpaid += b->getAmount();
        any = true;
    }
    if(!any) { std::cout << "No bills found.\n"; return; }
    std::cout << "Total outstanding unpaid: PKR " << totalUnpaid << "\n";
}

void PatientMenu::payBill() {
    bool anyUnpaid = false;
    std::cout << "\n--- Unpaid Bills ---\n";
    for(int i = 0; i < bills.getCount(); i++) {
        Bill* b = bills.get(i);
        if(b->getPatientId() == patient->getId() && b->getStatus() == UNPAID) {
            std::cout << "Bill ID: " << b->getBillId()
                      << " | Amount: PKR " << b->getAmount()
                      << " | Date: " << b->getDate() << "\n";
            anyUnpaid = true;
        }
    }
    if(!anyUnpaid) { std::cout << "No unpaid bills.\n"; return; }

    char idStr[10];
    std::cout << "Enter Bill ID to pay: ";
    std::cin.getline(idStr, 10);
    int billId = CharToInt(idStr);

    Bill* bill = bills.findById(billId);
    if(!bill || bill->getPatientId() != patient->getId() || bill->getStatus() != UNPAID) {
        std::cout << "Invalid bill ID.\n"; return;
    }

    if(patient->getBalance() < (long long)bill->getAmount()) {
        try { throw InsufficientFundsException("Insufficient balance to pay this bill."); }
        catch(InsufficientFundsException& e) { e.what(); return; }
    }

    *patient -= (long long)bill->getAmount();
    FileHandler::savePatients(patients);

    bill->setStatus(PAID);
    FileHandler::saveBills(bills);

    std::cout << "Bill paid successfully. Remaining balance: PKR " << patient->getBalance() << "\n";
}

void PatientMenu::topUpBalance() {
    int attempts = 0;
    while(attempts < 3) {
        char amtStr[30];
        std::cout << "Enter amount to add (PKR): ";
        std::cin.getline(amtStr, 30);
        long long amt = 0;
        try {
            if(!Validator::validateBalance(amtStr, amt) || amt <= 0) {
                if(getLength(amtStr) > 18) {
                    throw InvalidInputException("Balance is too large to handle.");
                }
                throw InvalidInputException("Amount must be a positive number greater than 0.");
            }
        } catch(InvalidInputException& e) {
            e.what(); attempts++; continue;
        }
        *patient += amt;
        FileHandler::savePatients(patients);
        std::cout << "Balance updated. New balance: PKR " << patient->getBalance() << "\n";
        return;
    }
    std::cout << "Too many invalid attempts. Returning to menu.\n";
}
