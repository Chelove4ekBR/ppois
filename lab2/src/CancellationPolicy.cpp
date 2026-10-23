#include "CancellationPolicy.hpp"

CancellationPolicy::CancellationPolicy(int freeDays, int earlyPenaltyPercent, int latePenaltyPercent,
                                       int lateThresholdDays)
    : freeDays_(freeDays),
      earlyPenaltyPercent_(earlyPenaltyPercent),
      latePenaltyPercent_(latePenaltyPercent),
      lateThresholdDays_(lateThresholdDays) {}

bool CancellationPolicy::isFreeCancellation(const Date &cancelDate, const Date &tripStart) const
{
    return penaltyPercentFor(cancelDate, tripStart) == 0;
}

int CancellationPolicy::penaltyPercentFor(const Date &cancelDate, const Date &tripStart) const
{
    const int daysBefore = cancelDate.daysUntil(tripStart);
    if (daysBefore >= freeDays_)
    {
        return 0;
    }
    return daysBefore >= lateThresholdDays_ ? earlyPenaltyPercent_ : latePenaltyPercent_;
}

Money CancellationPolicy::penaltyFor(const Money &paid, const Date &cancelDate, const Date &tripStart) const
{
    return paid.percentOf(penaltyPercentFor(cancelDate, tripStart));
}
