#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "StoragePartition.hpp"

/**
 * @brief Базовое представление физического накопителя данных (SSD/HDD).
 */
class StorageDrive {
public:
    /**
     * @brief Создаёт накопитель.
     * @param model Модель накопителя.
     * @param serialNumber Серийный номер.
     * @param interfaceType Интерфейс подключения ("NVMe", "SATA").
     * @param totalCapacity Полная ёмкость.
     * @param readSpeedMBs Скорость последовательного чтения в МБ/с.
     * @param writeSpeedMBs Скорость последовательной записи в МБ/с.
     */
    StorageDrive(std::string model, std::string serialNumber, std::string interfaceType,
                 ByteSize totalCapacity, uint32_t readSpeedMBs, uint32_t writeSpeedMBs);

    /**
     * @brief Возвращает модель накопителя.
     * @return Название модели.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает серийный номер.
     * @return Серийный номер.
     */
    const std::string& serialNumber() const { return serialNumber_; }

    /**
     * @brief Возвращает тип интерфейса.
     * @return Интерфейс накопителя.
     */
    const std::string& interfaceType() const { return interfaceType_; }

    /**
     * @brief Возвращает полный объём накопителя.
     * @return Ёмкость ByteSize.
     */
    const ByteSize& totalCapacity() const { return totalCapacity_; }

    /**
     * @brief Возвращает скорость чтения.
     * @return Скорость в МБ/с.
     */
    uint32_t readSpeedMBs() const { return readSpeedMBs_; }

    /**
     * @brief Возвращает скорость записи.
     * @return Скорость в МБ/с.
     */
    uint32_t writeSpeedMBs() const { return writeSpeedMBs_; }

    /**
     * @brief Возвращает процент здоровья (S.M.A.R.T. health).
     * @return Здоровье в процентах (0..100).
     */
    uint32_t healthPercent() const { return healthPercent_; }

    /**
     * @brief Возвращает список разделов диска.
     * @return Вектор разделов.
     */
    const std::vector<StoragePartition>& partitions() const { return partitions_; }

    /**
     * @brief Добавляет новый логический раздел на диск.
     * @param partition Раздел накопителя.
     */
    void addPartition(const StoragePartition& partition);

    /**
     * @brief Находит раздел по имени.
     * @param name Название раздела.
     * @return Ссылка на раздел.
     */
    StoragePartition& findPartition(const std::string& name);

    /**
     * @brief Рассчитывает суммарный занятый объём по всем разделам.
     * @return Занятый объём ByteSize.
     */
    ByteSize totalUsedBytes() const;

    /**
     * @brief Рассчитывает неразмеченный остаток ёмкости диска.
     * @return Свободный объём ByteSize.
     */
    ByteSize unpartitionedCapacity() const;

    /**
     * @brief Уменьшает показатель здоровья накопителя из-за износа.
     * @param amount Процент снижения.
     */
    void degradeHealth(uint32_t amount);

    /**
     * @brief Выполняет процедуру самодиагностики S.M.A.R.T.
     * @return true, если накопитель исправен.
     */
    bool performSelfTest();

private:
    std::string model_;
    std::string serialNumber_;
    std::string interfaceType_;
    ByteSize totalCapacity_;
    uint32_t readSpeedMBs_;
    uint32_t writeSpeedMBs_;
    uint32_t healthPercent_;
    std::vector<StoragePartition> partitions_;
};

using StorageDrivePtr = std::shared_ptr<StorageDrive>;
