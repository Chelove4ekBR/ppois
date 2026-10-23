#include "Office.hpp"

#include <algorithm>
#include <utility>

Office::Office(std::string name, Address address, ManagerPtr manager)
    : name_(std::move(name)), address_(std::move(address)), manager_(std::move(manager)) {}

void Office::hire(const EmployeePtr& employee) {
    staff_.push_back(employee);
    manager_->addSubordinate(employee);
}

std::size_t Office::staffCount() const {
    return staff_.size();
}

bool Office::hasStaff(int employeeId) const {
    return std::any_of(staff_.begin(), staff_.end(),
                       [employeeId](const EmployeePtr& employee) { return employee->employeeId() == employeeId; });
}

Money Office::payroll() const {
    return manager_->teamPayroll();
}
