#include "TourGuide.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"
#include "Tour.hpp"

TourGuide::TourGuide(int employeeId, Person person, Money salary, Date hireDate, std::vector<std::string> languages,
                     Money dailyRate)
    : Employee(employeeId, std::move(person), "Tour guide", std::move(salary), hireDate),
      languages_(std::move(languages)),
      dailyRate_(std::move(dailyRate)) {}

bool TourGuide::speaks(const std::string& language) const {
    return std::find(languages_.begin(), languages_.end(), language) != languages_.end();
}

void TourGuide::addLanguage(const std::string& language) {
    if (!speaks(language)) {
        languages_.push_back(language);
    }
}

bool TourGuide::isAvailable(const DateRange& period) const {
    return std::none_of(busyPeriods_.begin(), busyPeriods_.end(),
                        [&](const DateRange& busy) { return busy.overlaps(period); });
}

void TourGuide::assign(const DateRange& period) {
    if (!isAvailable(period)) {
        throw GuideUnavailableException(fullName() + " is busy during " + period.toString());
    }
    busyPeriods_.push_back(period);
}

Money TourGuide::costForDays(int days) const {
    return dailyRate_.times(days);
}

std::string TourGuide::describeRole() const {
    return "Guide " + fullName() + " (" + std::to_string(languages_.size()) + " languages)";
}
