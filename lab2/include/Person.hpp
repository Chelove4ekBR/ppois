#pragma once

#include <string>

#include "Address.hpp"
#include "ContactInfo.hpp"
#include "Date.hpp"

class Person {
public:
    Person(std::string firstName, std::string lastName, Date birthDate, ContactInfo contact, Address address);
    virtual ~Person() = default;
    const std::string& firstName() const { return firstName_; }
    const std::string& lastName() const { return lastName_; }
    const Date& birthDate() const { return birthDate_; }
    const ContactInfo& contact() const { return contact_; }
    const Address& address() const { return address_; }
    std::string fullName() const;
    int ageOn(const Date& date) const;
    bool isAdultOn(const Date& date) const;

private:
    std::string firstName_;
    std::string lastName_;
    Date birthDate_;
    ContactInfo contact_;
    Address address_;
};
