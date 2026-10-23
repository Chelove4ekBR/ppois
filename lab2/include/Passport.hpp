#pragma once

#include <string>

#include "Date.hpp"
#include "DateRange.hpp"

/**
 * @brief Хранит данные паспорта и позволяет проверять его действительность на период поездки.
 */
class Passport {
public:
    /**
     * @brief Создаёт объект класса Passport и инициализирует его исходные данные.
     * @param number номер объекта.
     * @param issuingCountry значение параметра «issuing country».
     * @param holderName значение параметра «holder name».
     * @param expiry дата окончания действия.
     */
    Passport(std::string number, std::string issuingCountry, std::string holderName, Date expiry);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `number_`.
     */
    const std::string& number() const { return number_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `issuingCountry_`.
     */
    const std::string& issuingCountry() const { return issuingCountry_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `holderName_`.
     */
    const std::string& holderName() const { return holderName_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `expiry_`.
     */
    const Date& expiry() const { return expiry_; }
    /**
     * @brief Проверяет, истёк ли срок действия объекта на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isExpiredOn(const Date& date) const;
    /**
     * @brief Проверяет действительность документа на весь период поездки.
     * @param trip период поездки.
     * @return Результат проверки условия.
     */
    bool isValidForTrip(const DateRange& trip) const;
    /**
     * @brief Проверяет действительность документа на поездку; при нарушении условия сообщает об ошибке.
     * @param trip период поездки.
     */
    void requireValidForTrip(const DateRange& trip) const;

private:
    std::string number_;
    std::string issuingCountry_;
    std::string holderName_;
    Date expiry_;
};
