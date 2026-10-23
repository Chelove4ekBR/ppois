#pragma once

#include "Date.hpp"
#include "Money.hpp"

/**
 * @brief Задаёт правила отмены бронирования: период бесплатной отмены и размеры возможных штрафов.
 */
class CancellationPolicy {
public:
    /**
     * @brief Создаёт объект класса CancellationPolicy и инициализирует его исходные данные.
     * @param freeDays количество дней бесплатной отмены.
     * @param earlyPenaltyPercent процент штрафа при ранней отмене.
     * @param latePenaltyPercent процент штрафа при поздней отмене.
     * @param lateThresholdDays порог поздней отмены в днях.
     */
    CancellationPolicy(int freeDays, int earlyPenaltyPercent, int latePenaltyPercent, int lateThresholdDays);
    /**
     * @brief Возвращает число дней, в течение которых отмена бесплатна.
     * @return Значение сохранённого атрибута `freeDays_`.
     */
    int freeDays() const { return freeDays_; }
    /**
     * @brief Возвращает процент штрафа при ранней отмене по правилам класса.
     * @return Значение сохранённого атрибута `earlyPenaltyPercent_`.
     */
    int earlyPenaltyPercent() const { return earlyPenaltyPercent_; }
    /**
     * @brief Возвращает процент штрафа при поздней отмене по правилам класса.
     * @return Значение сохранённого атрибута `latePenaltyPercent_`.
     */
    int latePenaltyPercent() const { return latePenaltyPercent_; }
    /**
     * @brief Возвращает порог в днях для определения поздней отмены.
     * @return Значение сохранённого атрибута `lateThresholdDays_`.
     */
    int lateThresholdDays() const { return lateThresholdDays_; }
    /**
     * @brief Проверяет, допускается ли бесплатная отмена поездки.
     * @param cancelDate дата отмены.
     * @param tripStart дата начала поездки.
     * @return Результат проверки условия.
     */
    bool isFreeCancellation(const Date& cancelDate, const Date& tripStart) const;
    /**
     * @brief Определяет процент штрафа при отмене поездки.
     * @param cancelDate дата отмены.
     * @param tripStart дата начала поездки.
     * @return Рассчитанное числовое значение.
     */
    int penaltyPercentFor(const Date& cancelDate, const Date& tripStart) const;
    /**
     * @brief Рассчитывает денежный штраф за отмену поездки.
     * @param paid уже уплаченная сумма.
     * @param cancelDate дата отмены.
     * @param tripStart дата начала поездки.
     * @return Рассчитанная денежная сумма.
     */
    Money penaltyFor(const Money& paid, const Date& cancelDate, const Date& tripStart) const;

private:
    int freeDays_;
    int earlyPenaltyPercent_;
    int latePenaltyPercent_;
    int lateThresholdDays_;
};
