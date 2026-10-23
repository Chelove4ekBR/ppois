#include "InsurancePolicy.hpp"

#include <utility>

#include "Exceptions.hpp"

namespace
{

    constexpr int kBasePremiumPercent = 5;
    constexpr int kSeniorAge = 65;
    constexpr int kSeniorSurchargePercent = 3;

}

InsurancePolicy::InsurancePolicy(std::string policyNumber, CustomerPtr insured, Money coverage, DateRange period,
                                 Money premium)
    : policyNumber_(std::move(policyNumber)),
      insured_(std::move(insured)),
      coverage_(std::move(coverage)),
      period_(period),
      premium_(std::move(premium)),
      claimsTotal_() {}

Money InsurancePolicy::premiumFor(const Money &tripCost, int travelerAge)
{
    const int percent = kBasePremiumPercent + (travelerAge >= kSeniorAge ? kSeniorSurchargePercent : 0);
    return tripCost.percentOf(percent);
}

bool InsurancePolicy::isActiveOn(const Date &date) const
{
    return period_.contains(date);
}

Money InsurancePolicy::remainingCoverage() const
{
    return coverage_.minus(claimsTotal_);
}

void InsurancePolicy::registerClaim(const Money &amount, const Date &date)
{
    if (!isActiveOn(date))
    {
        throw InsuranceException("Policy " + policyNumber_ + " is not active on " + date.toString());
    }
    if (amount.isGreaterThan(remainingCoverage()))
    {
        throw InsuranceException("Claim exceeds remaining coverage of policy " + policyNumber_);
    }
    claimsTotal_ = claimsTotal_.plus(amount);
}
