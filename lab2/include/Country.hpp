#pragma once

#include <string>

class Country {
public:
    Country(std::string name, std::string isoCode, std::string currency, bool visaRequired);
    const std::string& name() const { return name_; }
    const std::string& isoCode() const { return isoCode_; }
    const std::string& currency() const { return currency_; }
    bool visaRequired() const { return visaRequired_; }
    bool matchesCode(const std::string& code) const;
    std::string describe() const;

private:
    std::string name_;
    std::string isoCode_;
    std::string currency_;
    bool visaRequired_;
};
