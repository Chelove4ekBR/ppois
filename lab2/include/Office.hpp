#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Address.hpp"
#include "Employee.hpp"
#include "Manager.hpp"
#include "Money.hpp"

/**
 * @brief Представляет офис туристической компании, руководителя, адрес и сотрудников.
 */
class Office {
public:
    /**
     * @brief Создаёт объект класса Office и инициализирует его исходные данные.
     * @param name название объекта.
     * @param address адрес.
     * @param manager руководитель.
     */
    Office(std::string name, Address address, ManagerPtr manager);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `address_`.
     */
    const Address& address() const { return address_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `manager_`.
     */
    const ManagerPtr& manager() const { return manager_; }
    /**
     * @brief Возвращает список сотрудников офиса.
     * @return Ссылка на сохранённый атрибут `staff_`.
     */
    const std::vector<EmployeePtr>& staff() const { return staff_; }
    /**
     * @brief Добавляет сотрудника в штат офиса.
     * @param employee сотрудник.
     */
    void hire(const EmployeePtr& employee);
    /**
     * @brief Возвращает количество сотрудников офиса.
     * @return Рассчитанное числовое значение.
     */
    std::size_t staffCount() const;
    /**
     * @brief Проверяет наличие признака «staff» у объекта.
     * @param employeeId идентификатор сотрудника.
     * @return Результат проверки условия.
     */
    bool hasStaff(int employeeId) const;
    /**
     * @brief Рассчитывает фонд оплаты труда сотрудников офиса.
     * @return Рассчитанная денежная сумма.
     */
    Money payroll() const;

private:
    std::string name_;
    Address address_;
    ManagerPtr manager_;
    std::vector<EmployeePtr> staff_;
};
