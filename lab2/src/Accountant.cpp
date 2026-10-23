#include "Accountant.hpp"

#include <utility>

namespace {

constexpr int kInvoiceTermDays = 14;

}

Accountant::Accountant(int employeeId, Person person, Money salary, Date hireDate, std::string certificateNumber)
    : Employee(employeeId, std::move(person), "Accountant", std::move(salary), hireDate),
      certificateNumber_(std::move(certificateNumber)),
      processedPayments_(0),
      invoiceCounter_(0) {}

Invoice Accountant::issueInvoice(const BookingPtr& booking, const Date& issueDate) {
    ++invoiceCounter_;
    return Invoice(invoiceCounter_, booking, issueDate, issueDate.addDays(kInvoiceTermDays));
}

void Accountant::recordPayment(Ledger& ledger, const Payment& payment) {
    ledger.record(LedgerEntry("Payment #" + std::to_string(payment.id()), payment.amount(), payment.date(), true));
    ++processedPayments_;
}

void Accountant::recordRefund(Ledger& ledger, const Money& amount, const Date& date) {
    ledger.record(LedgerEntry("Refund", amount, date, false));
}

std::string Accountant::describeRole() const {
    return "Accountant " + fullName() + " (certificate " + certificateNumber_ + ")";
}
