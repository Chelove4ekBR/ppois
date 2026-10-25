#pragma once

#include <string>

/**
 * @brief Представляет аппаратный телеметрический датчик (температуры, напряжения или скорости вентилятора).
 */
class HardwareSensor {
public:
    /**
     * @brief Создаёт аппаратный датчик.
     * @param sensorName Имя датчика.
     * @param sensorType Тип ("Temperature", "Voltage", "Fan").
     * @param currentValue Начальное значение.
     * @param thresholdValue Аварийный порог срабатывания.
     */
    HardwareSensor(std::string sensorName, std::string sensorType,
                   double currentValue, double thresholdValue);

    /**
     * @brief Возвращает имя датчика.
     * @return Название сенсора.
     */
    const std::string& sensorName() const { return sensorName_; }

    /**
     * @brief Возвращает тип измерений датчика.
     * @return Тип ("Temperature", "Voltage", "Fan").
     */
    const std::string& sensorType() const { return sensorType_; }

    /**
     * @brief Возвращает текущее измеренное значение.
     * @return Показание датчика.
     */
    double currentValue() const { return currentValue_; }

    /**
     * @brief Возвращает пороговое значение.
     * @return Порог срабатывания.
     */
    double thresholdValue() const { return thresholdValue_; }

    /**
     * @brief Обновляет текущее показание датчика.
     * @param val Новое значение.
     */
    void updateValue(double val);

    /**
     * @brief Проверяет, превысило ли показание аварийный порог.
     * @return true, если датчик в состоянии тревоги (alarm).
     */
    bool isAlarm() const;

    /**
     * @brief Рассчитывает отклонение текущего значения от порога в процентах.
     * @return Процент отклонения.
     */
    double deviationPercent() const;

    /**
     * @brief Форматирует показание датчика в строку.
     * @return Текстовое представление.
     */
    std::string describe() const;

private:
    std::string sensorName_;
    std::string sensorType_;
    double currentValue_;
    double thresholdValue_;
};
