#pragma once

#include <cstdint>
#include "ClockFrequency.hpp"

/**
 * @brief Представляет отдельное вычислительное ядро центрального процессора.
 */
class CpuCore {
public:
    /**
     * @brief Создаёт ядро процессора.
     * @param coreId Порядковый номер ядра.
     * @param baseFrequency Базовая тактовая частота ядра.
     */
    CpuCore(uint32_t coreId, ClockFrequency baseFrequency);

    /**
     * @brief Возвращает идентификатор ядра.
     * @return Идентификатор ядра.
     */
    uint32_t coreId() const { return coreId_; }

    /**
     * @brief Возвращает базовую тактовую частоту.
     * @return Базовая частота.
     */
    const ClockFrequency& baseFrequency() const { return baseFrequency_; }

    /**
     * @brief Возвращает текущую тактовую частоту.
     * @return Текущая частота.
     */
    const ClockFrequency& currentFrequency() const { return currentFrequency_; }

    /**
     * @brief Возвращает процент текущей нагрузки на ядро.
     * @return Загрузка ядра (0..100%).
     */
    double usagePercent() const { return usagePercent_; }

    /**
     * @brief Проверяет активность ядра.
     * @return true, если ядро включено.
     */
    bool isActive() const { return active_; }

    /**
     * @brief Включает или отключает ядро.
     * @param active Флаг активности.
     */
    void setActive(bool active);

    /**
     * @brief Устанавливает текущую загрузку ядра.
     * @param percent Процент загрузки.
     */
    void setUsage(double percent);

    /**
     * @brief Понижает частоту ядра на коэффициент троттлинга.
     * @param factor Множитель частоты (0.0 < factor < 1.0).
     */
    void throttle(double factor);

    /**
     * @brief Сбрасывает текущую частоту ядра до базовой.
     */
    void resetFrequency();

    /**
     * @brief Эмулирует выполнение процессорных тактов.
     * @param cycles Количество инструкций/тактов.
     * @return Фактически затраченное виртуальное время в секундах.
     */
    double executeCycles(uint64_t cycles);

private:
    uint32_t coreId_;
    ClockFrequency baseFrequency_;
    ClockFrequency currentFrequency_;
    double usagePercent_;
    bool active_;
};
