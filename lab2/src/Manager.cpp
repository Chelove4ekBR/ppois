#include "Manager.hpp"

#include <utility>

Manager::Manager(int employeeId, Person person, Money salary, Date hireDate, std::string department,
                 int discountLimitPercent)
    : Employee(employeeId, std::move(person), "Manager", std::move(salary), hireDate),
      department_(std::move(department)),
      discountLimitPercent_(discountLimitPercent) {}

void Manager::addSubordinate(const std::shared_ptr<Employee>& employee) {
    subordinates_.push_back(employee);
}

bool Manager::canApproveDiscount(int percent) const {
    return percent <= discountLimitPercent_;
}

std::size_t Manager::teamSize() const {
    return subordinates_.size();
}

Money Manager::teamPayroll() const {
    Money payroll = salary();
    for (const auto& employee : subordinates_) {
        payroll = payroll.plus(employee->salary());
    }
    return payroll;
}

std::string Manager::describeRole() const {
    return "Manager of " + department_ + ": " + fullName();
}
