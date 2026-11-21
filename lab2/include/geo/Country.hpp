#pragma once

#include <string>

/**
 * @brief Описывает страну, её ISO-код, валюту и признак необходимости визы.
 */
class Country {
public:
    /**
     * @brief Создаёт объект класса Country и инициализирует его исходные данные.
     * @param name название объекта.
     * @param isoCode код страны по ISO.
     * @param currency валюта.
     * @param visaRequired признак необходимости визы.
     */
    Country(std::string name, std::string isoCode, std::string currency, bool visaRequired);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `isoCode_`.
     */
    const std::string& isoCode() const { return isoCode_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `currency_`.
     */
    const std::string& currency() const { return currency_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `visaRequired_`.
     */
    bool visaRequired() const { return visaRequired_; }
    /**
     * @brief Проверяет совпадение кода со значением страны.
     * @param code код.
     * @return Результат проверки условия.
     */
    bool matchesCode(const std::string& code) const;
    /**
     * @brief Формирует текстовое описание объекта.
     * @return Текстовое представление или строковое значение.
     */
    std::string describe() const;

private:
    std::string name_;
    std::string isoCode_;
    std::string currency_;
    bool visaRequired_;
};
