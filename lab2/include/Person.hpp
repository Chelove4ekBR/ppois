#pragma once

#include <string>

#include "Address.hpp"
#include "ContactInfo.hpp"
#include "Date.hpp"

/**
 * @brief Хранит общие персональные данные: имя, фамилию, дату рождения, контакты и адрес.
 */
class Person {
public:
    /**
     * @brief Создаёт объект класса Person и инициализирует его исходные данные.
     * @param firstName имя.
     * @param lastName фамилия.
     * @param birthDate дата рождения.
     * @param contact контактные данные.
     * @param address адрес.
     */
    Person(std::string firstName, std::string lastName, Date birthDate, ContactInfo contact, Address address);
    /**
     * @brief Освобождает ресурсы базового объекта Person при уничтожении.
     */
    virtual ~Person() = default;
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `firstName_`.
     */
    const std::string& firstName() const { return firstName_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `lastName_`.
     */
    const std::string& lastName() const { return lastName_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `birthDate_`.
     */
    const Date& birthDate() const { return birthDate_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `contact_`.
     */
    const ContactInfo& contact() const { return contact_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `address_`.
     */
    const Address& address() const { return address_; }
    /**
     * @brief Формирует полное имя человека.
     * @return Текстовое представление или строковое значение.
     */
    std::string fullName() const;
    /**
     * @brief Рассчитывает возраст человека на указанную дату.
     * @param date дата.
     * @return Рассчитанное числовое значение.
     */
    int ageOn(const Date& date) const;
    /**
     * @brief Проверяет, достиг ли человек совершеннолетия на указанную дату.
     * @param date дата.
     * @return Результат проверки условия.
     */
    bool isAdultOn(const Date& date) const;

private:
    std::string firstName_;
    std::string lastName_;
    Date birthDate_;
    ContactInfo contact_;
    Address address_;
};
