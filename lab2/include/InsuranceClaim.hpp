#pragma once

#include <string>

#include "Date.hpp"
#include "InsurancePolicy.hpp"
#include "Money.hpp"

enum class ClaimStatus { Pending, Approved, Rejected };

class InsuranceClaim {
public:
    InsuranceClaim(int id, InsurancePolicyPtr policy, Money amount, std::string description);
    int id() const { return id_; }
    const InsurancePolicyPtr& policy() const { return policy_; }
    const Money& amount() const { return amount_; }
    const std::string& description() const { return description_; }
    ClaimStatus status() const { return status_; }
    bool isPending() const;
    void approve(const Date& date);
    void reject();

private:
    int id_;
    InsurancePolicyPtr policy_;
    Money amount_;
    std::string description_;
    ClaimStatus status_;
};
