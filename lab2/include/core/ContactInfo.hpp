#pragma once

#include <string>

/**
 * @brief Хранит контактные данные и предоставляет базовую проверку адреса электронной почты и номера телефона.
 */
class ContactInfo {
public:
    /**
     * @brief Создаёт объект класса ContactInfo и инициализирует его исходные данные.
     * @param email адрес электронной почты.
     * @param phone номер телефона.
     */
    ContactInfo(std::string email, std::string phone);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `email_`.
     */
    const std::string& email() const { return email_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `phone_`.
     */
    const std::string& phone() const { return phone_; }
    /**
     * @brief Проверяет формат адреса электронной почты.
     * @param email адрес электронной почты.
     * @return Результат проверки условия.
     */
    static bool isValidEmail(const std::string& email);
    /**
     * @brief Проверяет формат номера телефона.
     * @param phone номер телефона.
     * @return Результат проверки условия.
     */
    static bool isValidPhone(const std::string& phone);
    /**
     * @brief Возвращает маскированное представление номера телефона.
     * @return Текстовое представление или строковое значение.
     */
    std::string maskedPhone() const;

private:
    std::string email_;
    std::string phone_;
};
