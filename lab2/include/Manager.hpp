#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Date.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

/**
 * @brief Представляет руководителя подразделения, список подчинённых и лимит одобряемой скидки.
 */
class Manager : public Employee {
public:
    /**
     * @brief Создаёт объект класса Manager и инициализирует его исходные данные.
     * @param employeeId идентификатор сотрудника.
     * @param person персональные данные.
     * @param salary зарплата.
     * @param hireDate дата приёма на работу.
     * @param department название подразделения.
     * @param discountLimitPercent максимальный процент скидки.
     */
    Manager(int employeeId, Person person, Money salary, Date hireDate, std::string department,
            int discountLimitPercent);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `subordinates_`.
     */
    const std::vector<std::shared_ptr<Employee>>& subordinates() const { return subordinates_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `department_`.
     */
    const std::string& department() const { return department_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `discountLimitPercent_`.
     */
    int discountLimitPercent() const { return discountLimitPercent_; }
    /**
     * @brief Добавляет сотрудника в список подчинённых руководителя.
     * @param employee сотрудник.
     */
    void addSubordinate(const std::shared_ptr<Employee>& employee);
    /**
     * @brief Проверяет, может ли руководитель одобрить скидку заданного размера.
     * @param percent процент.
     * @return Результат проверки условия.
     */
    bool canApproveDiscount(int percent) const;
    /**
     * @brief Возвращает количество сотрудников в команде руководителя.
     * @return Рассчитанное числовое значение.
     */
    std::size_t teamSize() const;
    /**
     * @brief Рассчитывает совокупный фонд оплаты труда команды.
     * @return Рассчитанная денежная сумма.
     */
    Money teamPayroll() const;
    /**
     * @brief Возвращает текстовое описание роли сотрудника.
     * @return Текстовое представление или строковое значение.
     */
    std::string describeRole() const override;

private:
    std::vector<std::shared_ptr<Employee>> subordinates_;
    std::string department_;
    int discountLimitPercent_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Manager`.
 */
using ManagerPtr = std::shared_ptr<Manager>;
