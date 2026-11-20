#pragma once

#include <string>

/**
 * @brief Контролирует температурный режим аппаратного узла, троттлинг и аварийное отключение.
 */
class ThermalZone {
public:
    /**
     * @brief Создаёт термозону с заданными порогами.
     * @param initialTempC Начальная температура в градусах Цельсия.
     * @param throttleTempC Порог включения троттлинга.
     * @param criticalTempC Критическая температура аварийного отключения.
     */
    ThermalZone(double initialTempC = 35.0, double throttleTempC = 85.0, double criticalTempC = 100.0);

    /**
     * @brief Возвращает текущую температуру.
     * @return Температура в °C.
     */
    double temperatureC() const { return temperatureC_; }

    /**
     * @brief Возвращает порог троттлинга.
     * @return Температура в °C.
     */
    double throttleTempC() const { return throttleTempC_; }

    /**
     * @brief Возвращает критическую температуру.
     * @return Температура в °C.
     */
    double criticalTempC() const { return criticalTempC_; }

    /**
     * @brief Изменяет температуру на заданную величину.
     * @param deltaC Приращение температуры (°C).
     */
    void updateTemperature(double deltaC);

    /**
     * @brief Устанавливает абсолютное значение температуры.
     * @param tempC Новая температура (°C).
     */
    void setTemperature(double tempC);

    /**
     * @brief Охлаждает узел на заданную величину градусов.
     * @param deltaC Снижение температуры (°C).
     */
    void cool(double deltaC);

    /**
     * @brief Проверяет, требуется ли троттлинг для предотвращения перегрева.
     * @return true, если температура превысила порог троттлинга.
     */
    bool isThrottling() const;

    /**
     * @brief Проверяет, достигнута ли критическая температура.
     * @return true, если температура достигла или превысила критический порог.
     */
    bool isCritical() const;

    /**
     * @brief Возвращает запас по температуре до критического порога.
     * @return Разница в °C.
     */
    double headroomC() const;

private:
    double temperatureC_;
    double throttleTempC_;
    double criticalTempC_;
};
