#pragma once

#include <string>

#include "Date.hpp"

/**
 * @brief Хранит сведения о водительском удостоверении, категории и дате окончания действия.
 */
class DrivingLicense {
public:
    /**
     * @brief Создаёт объект класса DrivingLicense и инициализирует его исходные данные.
     * @param number номер объекта.
     * @param category категория.
     * @param expiry дата окончания действия.
     */
    DrivingLicense(std::string number, std::string category, Date expiry);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `number_`.
     */
    const std::string& number() const { return number_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `category_`.
     */
    const std::string& category() const { return category_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `expiry_`.
     */
    const Date& expiry() const { return expiry_; }
    /**
     * @brief Проверяет, разрешает ли удостоверение указанную категорию.
     * @param requested запрошенная категория.
     * @return Результат проверки условия.
     */
    bool allowsCategory(const std::string& requested) const;
    /**
     * @brief Проверяет действие объекта на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isValidOn(const Date& date) const;

private:
    std::string number_;
    std::string category_;
    Date expiry_;
};
