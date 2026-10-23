#include "Employee.hpp"

#include <utility>

namespace {

constexpr int kMonthsInYear = 12;
constexpr int kDaysInYear = 365;

}

Employee::Employee(int employeeId, Person person, std::string position, Money salary, Date hireDate)
    : Person(std::move(person)),
      employeeId_(employeeId),
      position_(std::move(position)),
      salary_(std::move(salary)),
      hireDate_(hireDate) {}

int Employee::yearsOfService(const Date& today) const {
    return hireDate_.daysUntil(today) / kDaysInYear;
}

void Employee::raiseSalary(int percent) {
    salary_ = salary_.plus(salary_.percentOf(percent));
}

Money Employee::annualSalary() const {
    return salary_.times(kMonthsInYear);
}

std::string Employee::describeRole() const {
    return position_ + ": " + fullName();
}
