#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "Money.hpp"
#include "Person.hpp"

/**
 * @brief Представляет сотрудника туристического агентства, его должность, зарплату и дату приёма на работу.
 */
class Employee : public Person {
public:
    /**
     * @brief Создаёт объект класса Employee и инициализирует его исходные данные.
     * @param employeeId идентификатор сотрудника.
     * @param person персональные данные.
     * @param position значение параметра «position».
     * @param salary зарплата.
     * @param hireDate дата приёма на работу.
     */
    Employee(int employeeId, Person person, std::string position, Money salary, Date hireDate);
    /**
     * @brief Возвращает идентификатор сотрудника.
     * @return Значение сохранённого атрибута `employeeId_`.
     */
    int employeeId() const { return employeeId_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `position_`.
     */
    const std::string& position() const { return position_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `salary_`.
     */
    const Money& salary() const { return salary_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `hireDate_`.
     */
    const Date& hireDate() const { return hireDate_; }
    /**
     * @brief Рассчитывает стаж сотрудника на указанную дату.
     * @param today дата, на которую выполняется проверка.
     * @return Рассчитанное числовое значение.
     */
    int yearsOfService(const Date& today) const;
    /**
     * @brief Увеличивает зарплату сотрудника на заданный процент.
     * @param percent процент.
     */
    void raiseSalary(int percent);
    /**
     * @brief Рассчитывает годовую зарплату сотрудника.
     * @return Рассчитанная денежная сумма.
     */
    Money annualSalary() const;
    /**
     * @brief Возвращает текстовое описание роли сотрудника.
     * @return Текстовое представление или строковое значение.
     */
    virtual std::string describeRole() const;

private:
    int employeeId_;
    std::string position_;
    Money salary_;
    Date hireDate_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Employee`.
 */
using EmployeePtr = std::shared_ptr<Employee>;
