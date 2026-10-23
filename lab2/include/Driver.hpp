#pragma once

#include <memory>
#include <string>

#include "Date.hpp"
#include "DrivingLicense.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

/**
 * @brief Представляет водителя — сотрудника с водительским удостоверением и учётом пройденного расстояния и отдыха.
 */
class Driver : public Employee {
public:
    /**
     * @brief Создаёт объект класса Driver и инициализирует его исходные данные.
     * @param employeeId идентификатор сотрудника.
     * @param person персональные данные.
     * @param salary зарплата.
     * @param hireDate дата приёма на работу.
     * @param license водительское удостоверение.
     */
    Driver(int employeeId, Person person, Money salary, Date hireDate, DrivingLicense license);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `license_`.
     */
    const DrivingLicense& license() const { return license_; }
    /**
     * @brief Возвращает суммарное расстояние, пройденное водителем.
     * @return Значение сохранённого атрибута `totalKm_`.
     */
    long long totalKm() const { return totalKm_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `kmSinceRest_`.
     */
    long long kmSinceRest() const { return kmSinceRest_; }
    /**
     * @brief Проверяет, может ли водитель управлять транспортом указанной категории на дату.
     * @param category категория.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool canDrive(const std::string& category, const Date& date) const;
    /**
     * @brief Добавляет пройденное расстояние в учёт водителя.
     * @param kilometers расстояние в километрах.
     */
    void logDistance(long long kilometers);
    /**
     * @brief Проверяет, требуется ли водителю отдых согласно текущему учёту.
     * @return Результат проверки условия.
     */
    bool needsRest() const;
    /**
     * @brief Отмечает отдых водителя и обновляет соответствующий учёт.
     */
    void takeRest();
    /**
     * @brief Возвращает текстовое описание роли сотрудника.
     * @return Текстовое представление или строковое значение.
     */
    std::string describeRole() const override;

private:
    DrivingLicense license_;
    long long totalKm_;
    long long kmSinceRest_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Driver`.
 */
using DriverPtr = std::shared_ptr<Driver>;
