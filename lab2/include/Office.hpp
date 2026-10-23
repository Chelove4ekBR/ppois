#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Address.hpp"
#include "Employee.hpp"
#include "Manager.hpp"
#include "Money.hpp"

class Office {
public:
    Office(std::string name, Address address, ManagerPtr manager);
    const std::string& name() const { return name_; }
    const Address& address() const { return address_; }
    const ManagerPtr& manager() const { return manager_; }
    const std::vector<EmployeePtr>& staff() const { return staff_; }
    void hire(const EmployeePtr& employee);
    std::size_t staffCount() const;
    bool hasStaff(int employeeId) const;
    Money payroll() const;

private:
    std::string name_;
    Address address_;
    ManagerPtr manager_;
    std::vector<EmployeePtr> staff_;
};
