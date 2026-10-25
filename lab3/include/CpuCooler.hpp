#pragma once

#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Представляет систему охлаждения процессора (воздушный кулер или СЖО).
 */
class CpuCooler {
public:
    /**
     * @brief Создаёт кулер процессора.
     * @param coolerType Тип охлаждения ("Air", "Liquid", "Passive").
     * @param maxTdpWatt Максимальный отводимый теплопакет в ваттах.
     * @param maxFanRpm Максимальная скорость вентилятора в об/мин.
     */
    CpuCooler(std::string coolerType = "Air", uint32_t maxTdpWatt = 150, uint32_t maxFanRpm = 2000);

    /**
     * @brief Возвращает тип охлаждения.
     * @return Строка типа кулера.
     */
    const std::string& coolerType() const { return coolerType_; }

    /**
     * @brief Возвращает максимальный рассеиваемый TDP.
     * @return Мощность в ваттах.
     */
    uint32_t maxTdpWatt() const { return maxTdpWatt_; }

    /**
     * @brief Возвращает текущие обороты вентилятора.
     * @return Скорость в об/мин.
     */
    uint32_t fanRpm() const { return fanRpm_; }

    /**
     * @brief Возвращает максимальные обороты вентилятора.
     * @return Скорость в об/мин.
     */
    uint32_t maxFanRpm() const { return maxFanRpm_; }

    /**
     * @brief Проверяет активность помпы жидкостного охлаждения.
     * @return true, если помпа работает.
     */
    bool isPumpActive() const { return pumpActive_; }

    /**
     * @brief Задаёт скорость вращения вентилятора.
     * @param rpm Обороты в минуту.
     */
    void setFanSpeed(uint32_t rpm);

    /**
     * @brief Останавливает вентилятор кулера.
     */
    void stopFan();

    /**
     * @brief Рассчитывает охлаждающий эффект и возвращает снижение температуры.
     * @param currentTemp Текущая температура процессора (°C).
     * @param tdpWatt Тепловыделение процессора в ваттах.
     * @return Снижение температуры (°C).
     */
    double calculateCoolingDelta(double currentTemp, uint32_t tdpWatt) const;

    /**
     * @brief Проверяет, достаточно ли кулера для заданного теплопакета процессора.
     * @param tdpWatt Тепловыделение процессора в ваттах.
     * @return true, если кулер справляется с охлаждением.
     */
    bool isSufficientFor(uint32_t tdpWatt) const;

    /**
     * @brief Возвращает процент от максимальной скорости вентилятора.
     * @return Значение от 0 до 100%.
     */
    double rpmPercentage() const;

private:
    std::string coolerType_;
    uint32_t maxTdpWatt_;
    uint32_t fanRpm_;
    uint32_t maxFanRpm_;
    bool pumpActive_;
};

using CpuCoolerPtr = std::shared_ptr<CpuCooler>;
