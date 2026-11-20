#pragma once

#include <cstdint>

/**
 * @brief Представляет источник бесперебойного питания (ИБП / UPS) для защиты от перебоев в электросети.
 */
class UninterruptiblePowerSupply {
public:
    /**
     * @brief Создаёт ИБП.
     * @param capacityVa Полная мощность в вольт-амперах (ВА).
     * @param connectedLoadWatt Подключённая нагрузка в ваттах.
     * @param batteryPercent Начальный заряд батареи (0..100%).
     */
    explicit UninterruptiblePowerSupply(uint32_t capacityVa = 1500, uint32_t connectedLoadWatt = 400,
                                        uint32_t batteryPercent = 100);

    /**
     * @brief Возвращает мощность ИБП в ВА.
     * @return Ёмкость в ВА.
     */
    uint32_t capacityVa() const { return capacityVa_; }

    /**
     * @brief Возвращает текущий заряд батареи в процентах.
     * @return Заряд (0..100%).
     */
    uint32_t batteryPercent() const { return batteryPercent_; }

    /**
     * @brief Проверяет работу от аккумулятора.
     * @return true, если внешнее питание отключено.
     */
    bool isOnBattery() const { return onBattery_; }

    /**
     * @brief Возвращает текущую нагрузку в ваттах.
     * @return Нагрузка в Вт.
     */
    uint32_t connectedLoadWatt() const { return connectedLoadWatt_; }

    /**
     * @brief Эмулирует отключение электроснабжения в сети (переход на батарею).
     */
    void simulatePowerOutage();

    /**
     * @brief Восстанавливает подачу сетевого питания от розетки.
     */
    void restoreMainsPower();

    /**
     * @brief Рассчитывает прогнозируемое время автономной работы в минутах при текущей нагрузке.
     * @return Оставшееся время в минутах.
     */
    double estimatedRuntimeMinutes() const;

    /**
     * @brief Заряжает батарею ИБП.
     * @param percent Добавляемый процент заряда.
     */
    void chargeBattery(uint32_t percent);

    /**
     * @brief Эмулирует разрядку батареи в течение заданного числа минут.
     * @param minutes Длительность работы от батареи.
     */
    void discharge(uint32_t minutes);

private:
    uint32_t capacityVa_;
    uint32_t batteryPercent_;
    bool onBattery_;
    uint32_t connectedLoadWatt_;
};
