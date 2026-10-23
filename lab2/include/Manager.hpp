#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Date.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

class Manager : public Employee {
public:
    Manager(int employeeId, Person person, Money salary, Date hireDate, std::string department,
            int discountLimitPercent);
    const std::vector<std::shared_ptr<Employee>>& subordinates() const { return subordinates_; }
    const std::string& department() const { return department_; }
    int discountLimitPercent() const { return discountLimitPercent_; }
    void addSubordinate(const std::shared_ptr<Employee>& employee);
    bool canApproveDiscount(int percent) const;
    std::size_t teamSize() const;
    Money teamPayroll() const;
    std::string describeRole() const override;

private:
    std::vector<std::shared_ptr<Employee>> subordinates_;
    std::string department_;
    int discountLimitPercent_;
};

using ManagerPtr = std::shared_ptr<Manager>;
