#pragma once

#include <string>

#include "Customer.hpp"
#include "Tour.hpp"

/**
 * @brief Представляет отзыв клиента о туре с оценкой и текстом.
 */
class Review {
public:
    /**
     * @brief Создаёт объект класса Review и инициализирует его исходные данные.
     * @param author значение параметра «author».
     * @param tour тур.
     * @param rating оценка.
     * @param text текст.
     */
    Review(CustomerPtr author, TourPtr tour, int rating, std::string text);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `author_`.
     */
    const CustomerPtr& author() const { return author_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `tour_`.
     */
    const TourPtr& tour() const { return tour_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `rating_`.
     */
    int rating() const { return rating_; }
    /**
     * @brief Возвращает текстовое содержимое объекта.
     * @return Ссылка на сохранённый атрибут `text_`.
     */
    const std::string& text() const { return text_; }
    /**
     * @brief Проверяет, является ли отзыв положительным по правилам класса.
     * @return Результат проверки условия.
     */
    bool isPositive() const;
    /**
     * @brief Проверяет, оставлен ли отзыв указанным клиентом.
     * @param customerId идентификатор клиента.
     * @return Результат проверки условия.
     */
    bool isByCustomer(int customerId) const;
    /**
     * @brief Формирует краткую сводку объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string summary() const;

private:
    CustomerPtr author_;
    TourPtr tour_;
    int rating_;
    std::string text_;
};
