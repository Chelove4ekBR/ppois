#pragma once

#include <cstdint>
#include <string>

#include "ByteSize.hpp"
#include "StoragePartition.hpp"

/**
 * @brief Моделирует структуру файловой системы, кластеризацию и учет файлов на разделе.
 */
class FileSystem {
public:
    /**
     * @brief Создаёт файловую систему.
     * @param name Название ("ext4", "NTFS", "Btrfs").
     * @param clusterSize Размер кластера (обычно 4096 байт).
     * @param journaled Флаг поддержки журналирования.
     */
    FileSystem(std::string name = "ext4", ByteSize clusterSize = ByteSize::fromKiB(4),
               bool journaled = true);

    /**
     * @brief Возвращает название файловой системы.
     * @return Имя ФС.
     */
    const std::string& name() const { return name_; }

    /**
     * @brief Возвращает размер кластера.
     * @return Размер кластера ByteSize.
     */
    const ByteSize& clusterSize() const { return clusterSize_; }

    /**
     * @brief Возвращает количество файлов.
     * @return Число файлов.
     */
    uint32_t totalFiles() const { return totalFiles_; }

    /**
     * @brief Проверяет наличие журнала.
     * @return true, если ФС журналируемая.
     */
    bool isJournaled() const { return journaled_; }

    /**
     * @brief Создаёт файл заданного размера, округляя его до размера кластеров на разделе.
     * @param fileSize Логический размер файла.
     * @param partition Раздел для записи.
     */
    void createFile(ByteSize fileSize, StoragePartition& partition);

    /**
     * @brief Удаляет файл заданного размера и освобождает место на разделе.
     * @param fileSize Размер файла.
     * @param partition Раздел накопителя.
     */
    void deleteFile(ByteSize fileSize, StoragePartition& partition);

    /**
     * @brief Проверяет целостность файловой системы (fsck/chkdsk).
     * @return true, если ошибок не обнаружено.
     */
    bool checkConsistency() const;

    /**
     * @brief Рассчитывает уровень фрагментации данных.
     * @return Процент фрагментации (0..100%).
     */
    double fragmentationPercent() const;

    /**
     * @brief Рассчитывает необходимое количество кластеров для хранения файла.
     * @param size Размер файла.
     * @return Количество кластеров.
     */
    uint64_t calculateClustersNeeded(ByteSize size) const;

private:
    std::string name_;
    ByteSize clusterSize_;
    uint32_t totalFiles_;
    bool journaled_;
};
