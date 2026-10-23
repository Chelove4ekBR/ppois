#pragma once

#include <string>

class ContactInfo {
public:
    ContactInfo(std::string email, std::string phone);
    const std::string& email() const { return email_; }
    const std::string& phone() const { return phone_; }
    static bool isValidEmail(const std::string& email);
    static bool isValidPhone(const std::string& phone);
    std::string maskedPhone() const;

private:
    std::string email_;
    std::string phone_;
};
