#pragma once

#include <memory>
#include <string>

#include "Customer.hpp"
#include "Date.hpp"
#include "DateRange.hpp"
#include "Money.hpp"

/**
 * @brief Представляет страховой полис клиента, лимит покрытия, срок действия, премию и учтённые выплаты.
 */
class InsurancePolicy {
public:
    /**
     * @brief Создаёт объект класса InsurancePolicy и инициализирует его исходные данные.
     * @param policyNumber значение параметра «policy number».
     * @param insured значение параметра «insured».
     * @param coverage значение параметра «coverage».
     * @param period период дат.
     * @param premium значение параметра «premium».
     */
    InsurancePolicy(std::string policyNumber, CustomerPtr insured, Money coverage, DateRange period, Money premium);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `policyNumber_`.
     */
    const std::string& policyNumber() const { return policyNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `insured_`.
     */
    const CustomerPtr& insured() const { return insured_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `coverage_`.
     */
    const Money& coverage() const { return coverage_; }
    /**
     * @brief Возвращает период, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `period_`.
     */
    const DateRange& period() const { return period_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `premium_`.
     */
    const Money& premium() const { return premium_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `claimsTotal_`.
     */
    const Money& claimsTotal() const { return claimsTotal_; }
    /**
     * @brief Рассчитывает страховую премию с учётом стоимости поездки и возраста путешественника.
     * @param tripCost значение параметра «trip cost».
     * @param travelerAge значение параметра «traveler age».
     * @return Рассчитанная денежная сумма.
     */
    static Money premiumFor(const Money& tripCost, int travelerAge);
    /**
     * @brief Проверяет, действует ли объект на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isActiveOn(const Date& date) const;
    /**
     * @brief Рассчитывает оставшуюся сумму страхового покрытия.
     * @return Рассчитанная денежная сумма.
     */
    Money remainingCoverage() const;
    /**
     * @brief Регистрирует страховое требование и обновляет учёт выплат.
     * @param amount денежная сумма.
     * @param date дата.
     */
    void registerClaim(const Money& amount, const Date& date);

private:
    std::string policyNumber_;
    CustomerPtr insured_;
    Money coverage_;
    DateRange period_;
    Money premium_;
    Money claimsTotal_;
};

/**
 * @brief Удобный алиас умного указателя на тип `InsurancePolicy`.
 */
using InsurancePolicyPtr = std::shared_ptr<InsurancePolicy>;
