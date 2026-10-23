#pragma once

#include <string>

/**
 * @brief Хранит адрес с указанием страны, города, улицы и почтового индекса и предоставляет операции сравнения адресов.
 */
class Address {
public:
    /**
     * @brief Создаёт объект класса Address и инициализирует его исходные данные.
     * @param country название страны.
     * @param city город.
     * @param street улица.
     * @param postalCode почтовый индекс.
     */
    Address(std::string country, std::string city, std::string street, std::string postalCode);
    /**
     * @brief Возвращает страну, связанную с объектом.
     * @return Ссылка на сохранённый атрибут `country_`.
     */
    const std::string& country() const { return country_; }
    /**
     * @brief Возвращает город, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `city_`.
     */
    const std::string& city() const { return city_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `street_`.
     */
    const std::string& street() const { return street_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `postalCode_`.
     */
    const std::string& postalCode() const { return postalCode_; }
    /**
     * @brief Формирует полное текстовое представление адреса.
     * @return Текстовое представление или строковое значение.
     */
    std::string fullText() const;
    /**
     * @brief Проверяет, относятся ли адреса к одному городу.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isSameCity(const Address& other) const;
    /**
     * @brief Проверяет, относятся ли адреса к одной стране.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isSameCountry(const Address& other) const;

private:
    std::string country_;
    std::string city_;
    std::string street_;
    std::string postalCode_;
};
