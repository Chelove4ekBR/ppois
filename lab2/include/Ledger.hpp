#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.hpp"
#include "LedgerEntry.hpp"
#include "Money.hpp"

/**
 * @brief Ведёт журнал финансовых операций и рассчитывает доходы, расходы и баланс.
 */
class Ledger {
public:
    /**
     * @brief Создаёт объект класса Ledger и инициализирует его исходные данные.
     * @param name название объекта.
     */
    explicit Ledger(std::string name);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает записи журнала.
     * @return Ссылка на сохранённый атрибут `entries_`.
     */
    const std::vector<LedgerEntry>& entries() const { return entries_; }
    /**
     * @brief Добавляет финансовую запись в журнал.
     * @param entry значение параметра «entry».
     */
    void record(const LedgerEntry& entry);
    /**
     * @brief Рассчитывает текущий баланс финансового журнала.
     * @return Рассчитанная денежная сумма.
     */
    Money balance() const;
    /**
     * @brief Рассчитывает общую сумму доходов.
     * @return Рассчитанная денежная сумма.
     */
    Money totalIncome() const;
    /**
     * @brief Рассчитывает общую сумму расходов.
     * @return Рассчитанная денежная сумма.
     */
    Money totalExpenses() const;
    /**
     * @brief Подсчитывает записи журнала за указанную дату.
     * @param date дата.
     * @return Рассчитанное числовое значение.
     */
    std::size_t entriesOn(const Date& date) const;

private:
    std::string name_;
    std::vector<LedgerEntry> entries_;
};
