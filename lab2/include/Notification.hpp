#pragma once

#include <cstddef>
#include <string>

#include "Customer.hpp"
#include "Date.hpp"

/**
 * @brief Представляет уведомление клиента, дату отправки и состояние прочтения.
 */
class Notification {
public:
    /**
     * @brief Создаёт объект класса Notification и инициализирует его исходные данные.
     * @param recipient значение параметра «recipient».
     * @param message текст сообщения.
     * @param sentOn значение параметра «sent on».
     */
    Notification(CustomerPtr recipient, std::string message, Date sentOn);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `recipient_`.
     */
    const CustomerPtr& recipient() const { return recipient_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `message_`.
     */
    const std::string& message() const { return message_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `sentOn_`.
     */
    const Date& sentOn() const { return sentOn_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `read_`.
     */
    bool read() const { return read_; }
    /**
     * @brief Проверяет, является ли уведомление непрочитанным.
     * @return Результат проверки условия.
     */
    bool isUnread() const;
    /**
     * @brief Отмечает уведомление как прочитанное.
     */
    void markRead();
    /**
     * @brief Формирует краткий текстовый фрагмент с заданным максимальным размером.
     * @param maxLength максимальная длина текста.
     * @return Текстовое представление или строковое значение.
     */
    std::string preview(std::size_t maxLength) const;

private:
    CustomerPtr recipient_;
    std::string message_;
    Date sentOn_;
    bool read_;
};
