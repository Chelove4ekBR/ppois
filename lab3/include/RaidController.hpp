#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "StorageDrive.hpp"

/**
 * @brief Представляет аппаратный или программный RAID-контроллер массива дисков.
 */
class RaidController {
public:
    /**
     * @brief Создаёт RAID-контроллер.
     * @param raidLevel Уровень массива (0, 1, 5).
     */
    explicit RaidController(uint32_t raidLevel = 1);

    /**
     * @brief Возвращает уровень RAID.
     * @return Номер уровня массива (0, 1, 5).
     */
    uint32_t raidLevel() const { return raidLevel_; }

    /**
     * @brief Возвращает статус массива.
     * @return Строка статуса ("Optimal", "Degraded", "Failed").
     */
    const std::string& status() const { return status_; }

    /**
     * @brief Возвращает список подключённых накопителей.
     * @return Вектор указателей на StorageDrive.
     */
    const std::vector<StorageDrivePtr>& drives() const { return drives_; }

    /**
     * @brief Подключает накопитель к RAID-массиву.
     * @param drive Умный указатель на накопитель.
     */
    void attachDrive(StorageDrivePtr drive);

    /**
     * @brief Рассчитывает полезную ёмкость массива в зависимости от уровня RAID.
     * @return Полезный объём ByteSize.
     */
    ByteSize usableCapacity() const;

    /**
     * @brief Обрабатывает отказ диска по серийному номеру.
     * @param serialNumber Серийный номер отказавшего диска.
     */
    void handleDriveFailure(const std::string& serialNumber);

    /**
     * @brief Восстанавливает (rebuild) массив после замены сбойного диска.
     * @param replacementDrive Новый накопитель взамен сбойного.
     */
    void rebuild(StorageDrivePtr replacementDrive);

    /**
     * @brief Проверяет, обеспечивает ли текущая конфигурация отказоустойчивость.
     * @return true, если массив защищён от отказа одного диска.
     */
    bool isFaultTolerant() const;

private:
    uint32_t raidLevel_;
    std::vector<StorageDrivePtr> drives_;
    std::string status_;
};
