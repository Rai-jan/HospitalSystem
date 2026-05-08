#pragma once

enum FileType {
    PATIENT,
    DISCHARGED,
    DOCTOR,
    APPOINTMENT,
    BILL,
    PRESCRIPTION,
    SECURITY_LOG
};

enum Gender {
    MALE,
    FEMALE
};

enum AppointmentStatus {
    PENDING,
    COMPLETED,
    CANCELLED,
    NO_SHOW
};

enum BillStatus {
    UNPAID,
    PAID,
    BILL_CANCELLED
};
