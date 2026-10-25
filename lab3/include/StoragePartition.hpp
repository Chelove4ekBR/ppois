#pragma once

#include <string>
#include "ByteSize.hpp"

/**
 * @brief Представляет логический раздел накопителя данных с файловой системой и точкой монтирования.
 */
class StoragePartition {
public:
    /**
     * @brief Создаёт дисковый раздел.
     * @param name Название раздела.
     * @param fileSystemType Тип файловой системы ("ext4", "NTFS", "FAT32").
     * @param capacity Полный объём раздела.
     * @param mountPoint Точка монтирования.
     */
    StoragePartition(std::string name, std::string fileSystemType,
                     ByteSize capacity, std::string mountPoint = "");

    /**
     * @brief Возвращает имя раздела.
     * @return Название раздела.
     */
    const std::string& name() const { return name_; }

    /**
     * @brief Возвращает тип файловой системы.
     * @return Тип ФС.
     */
    const std::string& fileSystemType() const { return fileSystemType_; }

    /**
     * @brief Возвращает полный объём раздела.
     * @return Объём ByteSize.
     */
    const ByteSize& capacity() const { return capacity_; }

    /**
     * @brief Возвращает объём занятого места.
     * @return Занятый объём ByteSize.
     */
    const ByteSize& usedBytes() const { return usedBytes_; }

    /**
     * @brief Возвращает точку монтирования.
     * @return Путь монтирования.
     */
    const std::string& mountPoint() const { return mountPoint_; }

    /**
     * @brief Рассчитывает доступный свободный объём раздела.
     * @return Свободный объём ByteSize.
     */
    ByteSize availableBytes() const;

    /**
     * @brief Записывает данные в раздел, увеличивая занятое пространство.
     * @param size Объём записываемых данных.
     */
    void writeBytes(ByteSize size);

    /**
     * @brief Освобождает занятое пространство в разделе.
     * @param size Объём удаляемых данных.
     */
    void freeBytes(ByteSize size);

    /**
     * @brief Форматирует раздел в новую файловую систему, очищая данные.
     * @param newFsType Новый тип ФС.
     */
    void format(std::string newFsType);

    /**
     * @brief Рассчитывает процент заполнения раздела.
     * @return Процент использования (0..100%).
     */
    double usagePercentage() const;

    /**
     * @brief Монтирует раздел в указанную точку монтирования.
     * @param point Путь монтирования.
     */
    void mount(std::string point);

private:
    std::string name_;
    std::string fileSystemType_;
    ByteSize capacity_;
    ByteSize usedBytes_;
    std::string mountPoint_;
};
