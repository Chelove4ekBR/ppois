#pragma once

#include <string>

#include "Date.hpp"
#include "Money.hpp"

enum class PaymentMethod { Card, Cash, BankTransfer, Voucher };

std::string toString(PaymentMethod method);

class Payment {
public:
    Payment(int id, Money amount, PaymentMethod method, Date date);
    int id() const { return id_; }
    const Money& amount() const { return amount_; }
    PaymentMethod method() const { return method_; }
    const Date& date() const { return date_; }
    bool isCash() const;
    bool isOnOrBefore(const Date& deadline) const;
    Money refundPortion(int percent) const;
    std::string describe() const;

private:
    int id_;
    Money amount_;
    PaymentMethod method_;
    Date date_;
};
