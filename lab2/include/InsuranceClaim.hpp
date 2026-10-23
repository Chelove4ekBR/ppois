#pragma once

#include <string>

#include "Date.hpp"
#include "InsurancePolicy.hpp"
#include "Money.hpp"

/**
 * @brief Возможные статусы рассмотрения страхового требования.
 */
enum class ClaimStatus {
    Pending, /**< Страховое требование ожидает рассмотрения. */
    Approved, /**< Страховое требование одобрено. */
    Rejected /**< Страховое требование отклонено. */
};

/**
 * @brief Представляет страховое требование, его сумму, описание и статус рассмотрения.
 */
class InsuranceClaim {
public:
    /**
     * @brief Создаёт объект класса InsuranceClaim и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param policy правила или полис.
     * @param amount денежная сумма.
     * @param description описание.
     */
    InsuranceClaim(int id, InsurancePolicyPtr policy, Money amount, std::string description);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `policy_`.
     */
    const InsurancePolicyPtr& policy() const { return policy_; }
    /**
     * @brief Возвращает денежную сумму объекта.
     * @return Ссылка на сохранённый атрибут `amount_`.
     */
    const Money& amount() const { return amount_; }
    /**
     * @brief Возвращает описание объекта.
     * @return Ссылка на сохранённый атрибут `description_`.
     */
    const std::string& description() const { return description_; }
    /**
     * @brief Возвращает текущее состояние объекта.
     * @return Значение сохранённого атрибута `status_`.
     */
    ClaimStatus status() const { return status_; }
    /**
     * @brief Проверяет, находится ли требование в ожидании рассмотрения.
     * @return Результат проверки условия.
     */
    bool isPending() const;
    /**
     * @brief Одобряет страховое требование с фиксацией даты решения.
     * @param date дата.
     */
    void approve(const Date& date);
    /**
     * @brief Отклоняет страховое требование.
     */
    void reject();

private:
    int id_;
    InsurancePolicyPtr policy_;
    Money amount_;
    std::string description_;
    ClaimStatus status_;
};
