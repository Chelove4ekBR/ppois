#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Date.hpp"
#include "DateRange.hpp"
#include "Employee.hpp"
#include "Money.hpp"
#include "Person.hpp"

/**
 * @brief Представляет гида, его языки, занятые периоды и дневную ставку.
 */
class TourGuide : public Employee {
public:
    /**
     * @brief Создаёт объект класса TourGuide и инициализирует его исходные данные.
     * @param employeeId идентификатор сотрудника.
     * @param person персональные данные.
     * @param salary зарплата.
     * @param hireDate дата приёма на работу.
     * @param languages список языков.
     * @param dailyRate значение параметра «daily rate».
     */
    TourGuide(int employeeId, Person person, Money salary, Date hireDate, std::vector<std::string> languages,
              Money dailyRate);
    /**
     * @brief Возвращает список языков гида.
     * @return Ссылка на сохранённый атрибут `languages_`.
     */
    const std::vector<std::string>& languages() const { return languages_; }
    /**
     * @brief Возвращает периоды занятости гида.
     * @return Ссылка на сохранённый атрибут `busyPeriods_`.
     */
    const std::vector<DateRange>& busyPeriods() const { return busyPeriods_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `dailyRate_`.
     */
    const Money& dailyRate() const { return dailyRate_; }
    /**
     * @brief Проверяет, владеет ли гид указанным языком.
     * @param language название языка.
     * @return Результат проверки условия.
     */
    bool speaks(const std::string& language) const;
    /**
     * @brief Добавляет язык в список языков гида.
     * @param language название языка.
     */
    void addLanguage(const std::string& language);
    /**
     * @brief Проверяет доступность объекта на указанный период.
     * @param period период дат.
     * @return Результат проверки условия.
     */
    bool isAvailable(const DateRange& period) const;
    /**
     * @brief Назначает объект или сотрудника на указанный период.
     * @param period период дат.
     */
    void assign(const DateRange& period);
    /**
     * @brief Рассчитывает стоимость работы гида за указанное число дней.
     * @param days количество дней.
     * @return Рассчитанная денежная сумма.
     */
    Money costForDays(int days) const;
    /**
     * @brief Возвращает текстовое описание роли сотрудника.
     * @return Текстовое представление или строковое значение.
     */
    std::string describeRole() const override;

private:
    std::vector<std::string> languages_;
    std::vector<DateRange> busyPeriods_;
    Money dailyRate_;
};

/**
 * @brief Удобный алиас умного указателя на тип `TourGuide`.
 */
using TourGuidePtr = std::shared_ptr<TourGuide>;
