#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

class TourGuide : public Employee {
public:
    TourGuide(int employeeId, Person person, Money salary, Date hireDate, std::vector<std::string> languages,
              Money dailyRate);
    const std::vector<std::string>& languages() const { return languages_; }
    const std::vector<DateRange>& busyPeriods() const { return busyPeriods_; }
    const Money& dailyRate() const { return dailyRate_; }
    bool speaks(const std::string& language) const;
    void addLanguage(const std::string& language);
    bool isAvailable(const DateRange& period) const;
    void assign(const DateRange& period);
    Money costForDays(int days) const;
    std::string describeRole() const override;

private:
    std::vector<std::string> languages_;
    std::vector<DateRange> busyPeriods_;
    Money dailyRate_;
};

using TourGuidePtr = std::shared_ptr<TourGuide>;
