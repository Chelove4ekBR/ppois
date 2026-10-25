#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "PcieSlot.hpp"

/**
 * @brief Управляет шиной и совокупностью слотов PCI Express в системе.
 */
class PcieBus {
public:
    /**
     * @brief Создаёт шину PCIe.
     * @param maxTotalLanes Общий предел процессорных и чипсетных линий PCIe.
     */
    explicit PcieBus(uint32_t maxTotalLanes = 48);

    /**
     * @brief Возвращает список слотов.
     * @return Вектор слотов PCIe.
     */
    const std::vector<PcieSlot>& slots() const { return slots_; }

    /**
     * @brief Возвращает максимальное суммарное число линий.
     * @return Лимит линий.
     */
    uint32_t maxTotalLanes() const { return maxTotalLanes_; }

    /**
     * @brief Добавляет слот к шине.
     * @param slot Слот PCIe.
     */
    void addSlot(const PcieSlot& slot);

    /**
     * @brief Находит подходящий свободный слот и подключает в него устройство.
     * @param device Имя устройства.
     * @param lanes Необходимое число линий.
     * @return Идентификатор занятого слота.
     */
    uint32_t allocateSlot(const std::string& device, uint32_t lanes);

    /**
     * @brief Освобождает слот по его идентификатору.
     * @param slotId Идентификатор слота.
     */
    void releaseSlot(uint32_t slotId);

    /**
     * @brief Рассчитывает суммарное число занятых линий шины.
     * @return Количество занятых линий.
     */
    uint32_t totalOccupiedLanes() const;

    /**
     * @brief Проверяет наличие свободного слота, способного вместить устройство с заданным числом линий.
     * @param lanes Число линий.
     * @return true, если такой слот есть.
     */
    bool hasAvailableSlot(uint32_t lanes) const;

    /**
     * @brief Находит слот по номеру.
     * @param slotId Номер слота.
     * @return Ссылка на PcieSlot.
     */
    PcieSlot& findSlot(uint32_t slotId);

    /**
     * @brief Находит слот по номеру.
     * @param slotId Номер слота.
     * @return Константная ссылка на PcieSlot.
     */
    const PcieSlot& findSlot(uint32_t slotId) const;

private:
    std::vector<PcieSlot> slots_;
    uint32_t maxTotalLanes_;
};
