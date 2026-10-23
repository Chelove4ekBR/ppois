#include "InsuranceClaim.hpp"

#include <utility>

#include "Exceptions.hpp"

InsuranceClaim::InsuranceClaim(int id, InsurancePolicyPtr policy, Money amount, std::string description)
    : id_(id),
      policy_(std::move(policy)),
      amount_(std::move(amount)),
      description_(std::move(description)),
      status_(ClaimStatus::Pending) {}

bool InsuranceClaim::isPending() const {
    return status_ == ClaimStatus::Pending;
}

void InsuranceClaim::approve(const Date& date) {
    if (!isPending()) {
        throw InsuranceException("Claim " + std::to_string(id_) + " was already processed");
    }
    policy_->registerClaim(amount_, date);
    status_ = ClaimStatus::Approved;
}

void InsuranceClaim::reject() {
    if (!isPending()) {
        throw InsuranceException("Claim " + std::to_string(id_) + " was already processed");
    }
    status_ = ClaimStatus::Rejected;
}
