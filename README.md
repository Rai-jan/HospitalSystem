# MediCore Hospital Management System

## How to Compile

```bash
g++ -std=c++17 -o medicore \
  Utility.cpp HospitalException.cpp FileNotFoundException.cpp \
  InsufficientFundsException.cpp InvalidInputException.cpp SlotUnavailableException.cpp \
  Validator.cpp Person.cpp Patient.cpp Doctor.cpp Appointment.cpp Bill.cpp \
  Prescription.cpp FileHandler.cpp Admin.cpp PatientMenu.cpp DoctorMenu.cpp main.cpp
```

## How to Run

```bash
./medicore
```

## Default Credentials

- **Admin password:** `Admin123`
- Patient and Doctor accounts are created by Admin and stored in `patients.txt` / `doctors.txt`.

## Data Files (auto-created)

| File | Contents |
|------|----------|
| `patients.txt` | Patient records |
| `doctors.txt` | Doctor records |
| `appointments.txt` | Appointment records |
| `bills.txt` | Billing records |
| `prescriptions.txt` | Prescription records |
| `discharged.txt` | Archived discharged patient data |
| `security_log.txt` | Failed login / account lockout events |

## File Format

All files use comma-separated values (CSV), one record per line.

## Design Notes

- `balance` is stored as `long long` (whole PKR). Entering a balance with > 18 digits prints an error.
- `std::vector`, `std::string`, `strcmp`, `strtok` are **not used** anywhere.
- All data persists across restarts via file I/O.
- Custom `Storage<T>` template replaces `std::vector`.
- Operator overloads: `Patient +=`, `Patient -=`, `Patient ==`, `Doctor ==`, `Appointment ==` (conflict check), `<<` on Patient, Doctor, Appointment.

## Class Hierarchy

```
Person (abstract)
├── Patient
└── Doctor
Admin (standalone, not inheriting Person for simplicity of auth)

HospitalException (base)
├── FileNotFoundException
├── InsufficientFundsException
├── InvalidInputException
└── SlotUnavailableException

Storage<T>          (template, header-only)
FileHandler         (static methods, file I/O)
Validator           (static validation methods)
Appointment, Bill, Prescription  (data classes)
PatientMenu, DoctorMenu          (menu logic)
```
