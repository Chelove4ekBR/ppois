#pragma once

#include <string>

inline constexpr const char* kDefaultCurrency = "USD";
inline constexpr int kPercentBase = 100;

/**
 * @brief Представляет денежную сумму в минимальных денежных единицах и валюту; предоставляет арифметические операции.
 */
class Money {
public:
    /**
     * @brief Создаёт объект класса Money и инициализирует его исходные данные.
     * @param cents сумма в центах или минимальных денежных единицах.
     * @param currency валюта.
     */
    explicit Money(long long cents = 0, std::string currency = kDefaultCurrency);
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `cents_`.
     */
    long long cents() const { return cents_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `currency_`.
     */
    const std::string& currency() const { return currency_; }
    /**
     * @brief Складывает денежные суммы в одной валюте.
     * @param other объект для сравнения.
     * @return Рассчитанная денежная сумма.
     */
    Money plus(const Money& other) const;
    /**
     * @brief Вычитает денежные суммы в одной валюте.
     * @param other объект для сравнения.
     * @return Рассчитанная денежная сумма.
     */
    Money minus(const Money& other) const;
    /**
     * @brief Умножает денежную сумму на целый коэффициент.
     * @param factor множитель.
     * @return Рассчитанная денежная сумма.
     */
    Money times(int factor) const;
    /**
     * @brief Делит денежную сумму на целый делитель.
     * @param divisor делитель.
     * @return Рассчитанная денежная сумма.
     */
    Money dividedBy(int divisor) const; 
    /**
     * @brief Рассчитывает указанный процент от денежной суммы.
     * @param percent процент.
     * @return Рассчитанная денежная сумма.
     */
    Money percentOf(int percent) const;
    /**
     * @brief Сравнивает денежную сумму с другой суммой той же валюты.
     * @param other объект для сравнения.
     * @return Результат проверки условия.
     */
    bool isGreaterThan(const Money& other) const;
    /**
     * @brief Проверяет, является ли денежная сумма отрицательной.
     * @return Результат проверки условия.
     */
    bool isNegative() const;
    /**
     * @brief Преобразует значение или объект в строковое представление.
     * @return Текстовое представление или строковое значение.
     */
    std::string toString() const;

private:
    void requireSameCurrency(const Money& other) const;

    long long cents_;
    std::string currency_;
};
