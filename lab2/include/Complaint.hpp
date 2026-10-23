#pragma once

#include <string>

#include "Booking.hpp"
#include "Customer.hpp"

/**
 * @brief Представляет жалобу клиента на бронирование и хранит сведения о её разрешении.
 */
class Complaint {
public:
    /**
     * @brief Создаёт объект класса Complaint и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param customer клиент.
     * @param booking бронирование.
     * @param text текст.
     */
    Complaint(int id, CustomerPtr customer, BookingPtr booking, std::string text);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает клиента, связанного с объектом.
     * @return Ссылка на сохранённый атрибут `customer_`.
     */
    const CustomerPtr& customer() const { return customer_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `booking_`.
     */
    const BookingPtr& booking() const { return booking_; }
    /**
     * @brief Возвращает текстовое содержимое объекта.
     * @return Ссылка на сохранённый атрибут `text_`.
     */
    const std::string& text() const { return text_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `resolved_`.
     */
    bool resolved() const { return resolved_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `resolution_`.
     */
    const std::string& resolution() const { return resolution_; }
    /**
     * @brief Проверяет, остаётся ли жалоба открытой.
     * @return Результат проверки условия.
     */
    bool isOpen() const;
    /**
     * @brief Сохраняет решение по жалобе.
     * @param resolutionText текст решения по жалобе.
     */
    void resolve(const std::string& resolutionText);
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    int id_;
    CustomerPtr customer_;
    BookingPtr booking_;
    std::string text_;
    bool resolved_;
    std::string resolution_;
};
