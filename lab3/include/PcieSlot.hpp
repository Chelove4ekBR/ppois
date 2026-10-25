#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет слот расширения PCI Express на материнской плате.
 */
class PcieSlot {
public:
    /**
     * @brief Создаёт слот PCIe.
     * @param slotId Номер слота.
     * @param generation Поколение PCIe (3, 4, 5).
     * @param laneCount Количество физических линий (например, 1, 4, 8, 16).
     */
    PcieSlot(uint32_t slotId, uint32_t generation = 4, uint32_t laneCount = 16);

    /**
     * @brief Возвращает номер слота.
     * @return Номер слота.
     */
    uint32_t slotId() const { return slotId_; }

    /**
     * @brief Возвращает поколение PCIe.
     * @return Номер поколения.
     */
    uint32_t generation() const { return generation_; }

    /**
     * @brief Возвращает число линий.
     * @return Количество линий.
     */
    uint32_t laneCount() const { return laneCount_; }

    /**
     * @brief Проверяет, занят ли слот.
     * @return true, если в слот вставлено устройство.
     */
    bool isOccupied() const { return occupied_; }

    /**
     * @brief Возвращает имя подключённого устройства.
     * @return Название устройства.
     */
    const std::string& connectedDevice() const { return connectedDevice_; }

    /**
     * @brief Пропускная способность слота в Гбит/с.
     * @return Пропускная способность.
     */
    double bandwidthGbps() const;

    /**
     * @brief Проверяет доступность слота для установки.
     * @return true, если слот свободен.
     */
    bool isAvailable() const;

    /**
     * @brief Проверяет, поддерживает ли слот требуемое число линий.
     * @param lanes Запрашиваемое число линий.
     * @return true, если слот вмещает устройство.
     */
    bool canAccommodateLanes(uint32_t lanes) const;

    /**
     * @brief Устанавливает устройство в слот.
     * @param device Название устройства.
     * @param neededLanes Требуемое число линий.
     */
    void plugDevice(std::string device, uint32_t neededLanes);

    /**
     * @brief Извлекает устройство из слота.
     */
    void unplugDevice();

private:
    uint32_t slotId_;
    uint32_t generation_;
    uint32_t laneCount_;
    bool occupied_;
    std::string connectedDevice_;
};
