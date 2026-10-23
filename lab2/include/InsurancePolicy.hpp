#pragma once

#include <memory>
#include <string>

#include "Customer.hpp"
#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

class InsurancePolicy {
public:
    InsurancePolicy(std::string policyNumber, CustomerPtr insured, Money coverage, DateRange period, Money premium);
    const std::string& policyNumber() const { return policyNumber_; }
    const CustomerPtr& insured() const { return insured_; }
    const Money& coverage() const { return coverage_; }
    const DateRange& period() const { return period_; }
    const Money& premium() const { return premium_; }
    const Money& claimsTotal() const { return claimsTotal_; }
    static Money premiumFor(const Money& tripCost, int travelerAge);
    bool isActiveOn(const Date& date) const;
    Money remainingCoverage() const;
    void registerClaim(const Money& amount, const Date& date);

private:
    std::string policyNumber_;
    CustomerPtr insured_;
    Money coverage_;
    DateRange period_;
    Money premium_;
    Money claimsTotal_;
};

using InsurancePolicyPtr = std::shared_ptr<InsurancePolicy>;
