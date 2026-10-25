#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "RamStick.hpp"

/**
 * @brief Управляет подсистемой оперативной памяти, слотами и многоканальным режимом.
 */
class RamModule {
public:
    /**
     * @brief Создаёт контроллер/подсистему оперативной памяти.
     * @param maxSlots Количество доступных слотов памяти.
     */
    explicit RamModule(uint32_t maxSlots = 4);

    /**
     * @brief Возвращает установленные модули памяти.
     * @return Вектор планок памяти.
     */
    const std::vector<RamStick>& sticks() const { return sticks_; }

    /**
     * @brief Возвращает максимальное число слотов.
     * @return Количество слотов.
     */
    uint32_t maxSlots() const { return maxSlots_; }

    /**
     * @brief Возвращает результирующую рабочую частоту подсистемы памяти.
     * @return Частота ClockFrequency.
     */
    const ClockFrequency& operatingFrequency() const { return operatingFrequency_; }

    /**
     * @brief Устанавливает планку памяти в свободный слот.
     * @param stick Модуль памяти.
     */
    void installStick(const RamStick& stick);

    /**
     * @brief Извлекает планку памяти по серийному номеру.
     * @param serialNumber Серийный номер.
     */
    void removeStick(const std::string& serialNumber);

    /**
     * @brief Рассчитывает суммарный объём оперативной памяти.
     * @return Суммарный объём ByteSize.
     */
    ByteSize totalCapacity() const;

    /**
     * @brief Возвращает количество установленных планок.
     * @return Число установленных модулей.
     */
    uint32_t installedCount() const;

    /**
     * @brief Проверяет, заполнены ли все слоты памяти.
     * @return true, если свободных слотов нет.
     */
    bool isFull() const;

    /**
     * @brief Проверяет, активен ли двух- или многоканальный режим памяти.
     * @return true, если установлено не менее 2 одинаковых модулей.
     */
    bool isMultiChannel() const;

    /**
     * @brief Рассчитывает суммарную эффективную пропускную способность подсистемы памяти.
     * @return Пропускная способность в МБ/с.
     */
    double totalBandwidthMBs() const;

private:
    void recalculateFrequency();

    std::vector<RamStick> sticks_;
    uint32_t maxSlots_;
    ClockFrequency operatingFrequency_;
};
