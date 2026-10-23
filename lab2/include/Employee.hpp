#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Money.hpp"
#include "Person.hpp"

class Employee : public Person {
public:
    Employee(int employeeId, Person person, std::string position, Money salary, Date hireDate);
    int employeeId() const { return employeeId_; }
    const std::string& position() const { return position_; }
    const Money& salary() const { return salary_; }
    const Date& hireDate() const { return hireDate_; }
    int yearsOfService(const Date& today) const;
    void raiseSalary(int percent);
    Money annualSalary() const;
    virtual std::string describeRole() const;

private:
    int employeeId_;
    std::string position_;
    Money salary_;
    Date hireDate_;
};

using EmployeePtr = std::shared_ptr<Employee>;
