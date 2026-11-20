#include "core/ContactInfo.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <utility>

#include "core/Exceptions.hpp"

namespace {

constexpr std::size_t kMinPhoneDigits = 7;
constexpr std::size_t kMaxPhoneDigits = 15;
constexpr std::size_t kVisiblePhoneDigits = 4;

std::string digitsOf(const std::string& phone) {
    std::string digits;
    std::copy_if(phone.begin(), phone.end(), std::back_inserter(digits),
                 [](unsigned char symbol) { return std::isdigit(symbol) != 0; });
    return digits;
}

}

ContactInfo::ContactInfo(std::string email, std::string phone) : email_(std::move(email)), phone_(std::move(phone)) {
    if (!isValidEmail(email_)) {
        throw InvalidContactException("Invalid email: " + email_);
    }
    if (!isValidPhone(phone_)) {
        throw InvalidContactException("Invalid phone: " + phone_);
    }
}

bool ContactInfo::isValidEmail(const std::string& email) {
    const auto atPosition = email.find('@');
    if (atPosition == std::string::npos || atPosition == 0 || email.find('@', atPosition + 1) != std::string::npos) {
        return false;
    }
    const auto dotPosition = email.find('.', atPosition);
    return dotPosition != std::string::npos && dotPosition > atPosition + 1 && dotPosition + 1 < email.size();
}

bool ContactInfo::isValidPhone(const std::string& phone) {
    const std::string digits = digitsOf(phone);
    return digits.size() >= kMinPhoneDigits && digits.size() <= kMaxPhoneDigits;
}

std::string ContactInfo::maskedPhone() const {
    const std::string digits = digitsOf(phone_);
    const std::string hidden(digits.size() - kVisiblePhoneDigits, '*');
    return hidden + digits.substr(digits.size() - kVisiblePhoneDigits);
}
