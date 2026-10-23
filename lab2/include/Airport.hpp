#pragma once

#include <string>

#include "City.hpp"

/**
 * @brief Хранит сведения об аэропорте, его коде, названии и городе расположения.
 */
class Airport {
public:
    /**
     * @brief Создаёт объект класса Airport и инициализирует его исходные данные.
     * @param code код.
     * @param name название объекта.
     * @param city город.
     */
    Airport(std::string code, std::string name, City city);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `code_`.
     */
    const std::string& code() const { return code_; }
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает город, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `city_`.
     */
    const City& city() const { return city_; }
    /**
     * @brief Формирует краткую подпись объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string label() const;
    /**
     * @brief Проверяет, является ли перелёт между аэропортами международным.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isInternationalTo(const Airport& other) const;

private:
    std::string code_;
    std::string name_;
    City city_;
};
