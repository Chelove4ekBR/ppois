#include "Payment.hpp"

#include <utility>

std::string toString(PaymentMethod method) {
    switch (method) {
        case PaymentMethod::Card:
            return "Card";
        case PaymentMethod::Cash:
            return "Cash";
        case PaymentMethod::BankTransfer:
            return "BankTransfer";
        case PaymentMethod::Voucher:
            return "Voucher";
    }
    return "Unknown";
}

Payment::Payment(int id, Money amount, PaymentMethod method, Date date)
    : id_(id), amount_(std::move(amount)), method_(method), date_(date) {}

bool Payment::isCash() const {
    return method_ == PaymentMethod::Cash;
}

bool Payment::isOnOrBefore(const Date& deadline) const {
    return !deadline.isBefore(date_);
}

Money Payment::refundPortion(int percent) const {
    return amount_.percentOf(percent);
}

std::string Payment::describe() const {
    return "Payment #" + std::to_string(id_) + " " + amount_.toString() + " by " + toString(method_) + " on " +
           date_.toString();
}
