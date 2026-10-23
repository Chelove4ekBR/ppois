#pragma once

#include <cstddef>
#include <string>

#include "Customer.hpp"
#include "Date.hpp"

class Notification {
public:
    Notification(CustomerPtr recipient, std::string message, Date sentOn);
    const CustomerPtr& recipient() const { return recipient_; }
    const std::string& message() const { return message_; }
    const Date& sentOn() const { return sentOn_; }
    bool read() const { return read_; }
    bool isUnread() const;
    void markRead();
    std::string preview(std::size_t maxLength) const;

private:
    CustomerPtr recipient_;
    std::string message_;
    Date sentOn_;
    bool read_;
};
