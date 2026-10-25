#pragma once

#include <cstdint>
#include <string>

#include "ClockFrequency.hpp"
#include "ThermalZone.hpp"

/**
 * @brief Представляет графический процессор (GPU) для рендеринга и шейдерных вычислений.
 */
class Gpu {
public:
    /**
     * @brief Создаёт графический процессор.
     * @param model Модель графического чипа.
     * @param cudaCores Количество вычислительных блоков / ядер CUDA / потоковых процессоров.
     * @param baseClock Базовая частота ядра.
     * @param tdpWatt Тепловыделение в ваттах.
     */
    Gpu(std::string model, uint32_t cudaCores, ClockFrequency baseClock, uint32_t tdpWatt = 250);

    /**
     * @brief Возвращает модель GPU.
     * @return Модель чипа.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает число ядер/вычислительных блоков.
     * @return Количество ядер.
     */
    uint32_t cudaCores() const { return cudaCores_; }

    /**
     * @brief Возвращает базовую частоту.
     * @return Базовая тактовая частота.
     */
    const ClockFrequency& baseClock() const { return baseClock_; }

    /**
     * @brief Возвращает текущую частоту.
     * @return Текущая частота.
     */
    const ClockFrequency& currentClock() const { return currentClock_; }

    /**
     * @brief Возвращает тепловыделение (TDP).
     * @return Значение в ваттах.
     */
    uint32_t tdpWatt() const { return tdpWatt_; }

    /**
     * @brief Возвращает термозону видеочипа.
     * @return Ссылка на ThermalZone.
     */
    ThermalZone& thermalZone() { return thermalZone_; }

    /**
     * @brief Возвращает термозону видеочипа.
     * @return Константная ссылка на ThermalZone.
     */
    const ThermalZone& thermalZone() const { return thermalZone_; }

    /**
     * @brief Эмулирует рендеринг кадра сцены с заданным числом треугольников/полигонов.
     * @param polygonCount Число полигонов.
     * @return Время отрисовки кадра в миллисекундах.
     */
    double renderFrame(uint32_t polygonCount);

    /**
     * @brief Выполняет параллельные вычислительные шейдеры (GPGPU/CUDA).
     * @param workUnits Количество рабочих единиц (воркетов).
     * @return Время выполнения в миллисекундах.
     */
    double computeShader(uint32_t workUnits);

    /**
     * @brief Разгоняет графический процессор на заданный процент.
     * @param percent Процент разгона.
     */
    void overclock(double percent);

    /**
     * @brief Снижает температуру видеочипа.
     * @param deltaC Градусы Цельсия.
     */
    void coolDown(double deltaC);

    /**
     * @brief Проверяет перегрев видеочипа.
     * @return true, если температура выше критической или активен троттлинг.
     */
    bool isOverheating() const;

private:
    std::string model_;
    uint32_t cudaCores_;
    ClockFrequency baseClock_;
    ClockFrequency currentClock_;
    uint32_t tdpWatt_;
    ThermalZone thermalZone_;
};
