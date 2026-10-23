#pragma once

#include "Date.hpp"
#include "Money.hpp"

class CancellationPolicy {
public:
    CancellationPolicy(int freeDays, int earlyPenaltyPercent, int latePenaltyPercent, int lateThresholdDays);
    int freeDays() const { return freeDays_; }
    int earlyPenaltyPercent() const { return earlyPenaltyPercent_; }
    int latePenaltyPercent() const { return latePenaltyPercent_; }
    int lateThresholdDays() const { return lateThresholdDays_; }
    bool isFreeCancellation(const Date& cancelDate, const Date& tripStart) const;
    int penaltyPercentFor(const Date& cancelDate, const Date& tripStart) const;
    Money penaltyFor(const Money& paid, const Date& cancelDate, const Date& tripStart) const;

private:
    int freeDays_;
    int earlyPenaltyPercent_;
    int latePenaltyPercent_;
    int lateThresholdDays_;
};
