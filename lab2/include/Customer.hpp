#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Country.hpp"
#include "DateRange.hpp"
#include "LoyaltyAccount.hpp"
#include "Passport.hpp"
#include "Person.hpp"
#include "Visa.hpp"

class Customer : public Person {
public:
    Customer(int customerId, Person person, Passport passport);
    int customerId() const { return customerId_; }
    const Passport& passport() const { return passport_; }
    const std::vector<Visa>& visas() const { return visas_; }
    LoyaltyAccount& loyalty() { return loyalty_; }
    const LoyaltyAccount& loyalty() const { return loyalty_; }
    void addVisa(const Visa& visa);
    bool hasVisaFor(const std::string& countryCode, const DateRange& trip) const;
    void ensureCanEnter(const Country& country, const DateRange& trip) const;

private:
    int customerId_;
    Passport passport_;
    std::vector<Visa> visas_;
    LoyaltyAccount loyalty_;
};

using CustomerPtr = std::shared_ptr<Customer>;
