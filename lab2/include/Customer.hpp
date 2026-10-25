#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Country.hpp"
#include "DateRange.hpp"
#include "LoyaltyAccount.hpp"
#include "Passport.hpp"
#include "Person.hpp"
#include "Visa.hpp"

/**
 * @brief Представляет клиента туристической компании, его паспорт, визы и программу лояльности.
 */
class Customer : public Person {
public:
    /**
     * @brief Создаёт объект класса Customer и инициализирует его исходные данные.
     * @param customerId идентификатор клиента.
     * @param person персональные данные.
     * @param passport значение параметра «passport».
     */
    Customer(int customerId, Person person, Passport passport);
    /**
     * @brief Возвращает идентификатор клиента.
     * @return Значение сохранённого атрибута `customerId_`.
     */
    int customerId() const { return customerId_; }
    /**
     * @brief Возвращает паспорт клиента.
     * @return Ссылка на сохранённый атрибут `passport_`.
     */
    const Passport& passport() const { return passport_; }
    /**
     * @brief Возвращает список виз клиента.
     * @return Ссылка на сохранённый атрибут `visas_`.
     */
    const std::vector<Visa>& visas() const { return visas_; }
    /**
     * @brief Возвращает учётную запись программы лояльности клиента.
     * @return Ссылка на сохранённый атрибут `loyalty_`.
     */
    LoyaltyAccount& loyalty() { return loyalty_; }
    /**
     * @brief Возвращает учётную запись программы лояльности клиента.
     * @return Ссылка на сохранённый атрибут `loyalty_`.
     */
    const LoyaltyAccount& loyalty() const { return loyalty_; }
    /**
     * @brief Добавляет визу в список виз клиента.
     * @param visa значение параметра «visa».
     */
    void addVisa(const Visa& visa);
    /**
     * @brief Проверяет наличие визы, покрывающей указанную поездку.
     * @param countryCode код страны.
     * @param trip период поездки.
     * @return Результат проверки условия.
     */
    bool hasVisaFor(const std::string& countryCode, const DateRange& trip) const;
    /**
     * @brief Проверяет возможность въезда в указанную страну на период поездки.
     * @param country название страны.
     * @param trip период поездки.
     */
    void ensureCanEnter(const Country& country, const DateRange& trip) const;

private:
    int customerId_;
    Passport passport_;
    std::vector<Visa> visas_;
    LoyaltyAccount loyalty_;
};

/**
 * @brief Удобный алиас умного указателя на тип `Customer`.
 */
using CustomerPtr = std::shared_ptr<Customer>;
