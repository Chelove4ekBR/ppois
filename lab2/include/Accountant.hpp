#pragma once

#include <string>

#include "Booking.hpp"
#include "Date.hpp"
#include "Employee.hpp"
#include "Invoice.hpp"
#include "Ledger.hpp"
#include "Money.hpp"
#include "Payment.hpp"
#include "Person.hpp"

class Accountant : public Employee {
public:
    Accountant(int employeeId, Person person, Money salary, Date hireDate, std::string certificateNumber);
    const std::string& certificateNumber() const { return certificateNumber_; }
    int processedPayments() const { return processedPayments_; }
    int invoiceCounter() const { return invoiceCounter_; }
    Invoice issueInvoice(const BookingPtr& booking, const Date& issueDate);
    void recordPayment(Ledger& ledger, const Payment& payment);
    void recordRefund(Ledger& ledger, const Money& amount, const Date& date);
    std::string describeRole() const override;

private:
    std::string certificateNumber_;
    int processedPayments_;
    int invoiceCounter_;
};
