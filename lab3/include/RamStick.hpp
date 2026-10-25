#pragma once

#include <cstdint>
#include <string>

#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Voltage.hpp"

/**
 * @brief Представляет отдельный модуль (планку) оперативной памяти.
 */
class RamStick {
public:
    /**
     * @brief Создаёт планку оперативной памяти.
     * @param serialNumber Серийный номер.
     * @param capacity Объём памяти.
     * @param frequency Тактовая частота.
     * @param generation Поколение памяти ("DDR4", "DDR5").
     * @param casLatency Тайминг CAS Latency (CL).
     * @param voltage Рабочее напряжение.
     */
    RamStick(std::string serialNumber, ByteSize capacity, ClockFrequency frequency,
             std::string generation = "DDR5", uint32_t casLatency = 36,
             Voltage voltage = Voltage(1250));

    /**
     * @brief Возвращает серийный номер.
     * @return Строка серийного номера.
     */
    const std::string& serialNumber() const { return serialNumber_; }

    /**
     * @brief Возвращает объём памяти планки.
     * @return Объём ByteSize.
     */
    const ByteSize& capacity() const { return capacity_; }

    /**
     * @brief Возвращает частоту памяти.
     * @return Частота ClockFrequency.
     */
    const ClockFrequency& frequency() const { return frequency_; }

    /**
     * @brief Возвращает поколение памяти.
     * @return Поколение DDR.
     */
    const std::string& generation() const { return generation_; }

    /**
     * @brief Возвращает задержку CAS.
     * @return Значение CL.
     */
    uint32_t casLatency() const { return casLatency_; }

    /**
     * @brief Возвращает рабочее напряжение.
     * @return Напряжение Voltage.
     */
    const Voltage& voltage() const { return voltage_; }

    /**
     * @brief Рассчитывает теоретическую пропускную способность модуля в МБ/с.
     * @return Скорость в МБ/с.
     */
    double calculateBandwidthMBs() const;

    /**
     * @brief Проверяет совпадение поколения памяти.
     * @param gen Поколение (например, "DDR5").
     * @return true, если поколение совпадает.
     */
    bool matchesGeneration(const std::string& gen) const;

    /**
     * @brief Проверяет совместимость напряжения с порогом материнской платы.
     * @param maxSafeVoltage Максимально безопасное напряжение.
     * @return true, если модуль совместим.
     */
    bool validateCompatibility(const Voltage& maxSafeVoltage) const;

    /**
     * @brief Формирует краткое текстовое описание планки памяти.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    std::string serialNumber_;
    ByteSize capacity_;
    ClockFrequency frequency_;
    std::string generation_;
    uint32_t casLatency_;
    Voltage voltage_;
};
