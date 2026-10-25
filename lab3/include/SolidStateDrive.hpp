#pragma once

#include <cstdint>
#include <string>
#include "ByteSize.hpp"
#include "StorageDrive.hpp"

/**
 * @brief Представляет твердотельный накопитель (SSD) на базе энергонезависимой флеш-памяти NAND.
 */
class SolidStateDrive {
public:
    /**
     * @brief Создаёт SSD накопитель.
     * @param drive Базовые характеристики диска.
     * @param nandType Тип ячеек ("SLC", "MLC", "TLC", "QLC").
     * @param tbwRatingTB Ресурс перезаписи TBW в терабайтах.
     */
    SolidStateDrive(StorageDrive drive, std::string nandType = "TLC", uint32_t tbwRatingTB = 600);

    /**
     * @brief Возвращает базовый накопитель.
     * @return Ссылка на StorageDrive.
     */
    StorageDrive& drive() { return drive_; }

    /**
     * @brief Возвращает базовый накопитель.
     * @return Константная ссылка на StorageDrive.
     */
    const StorageDrive& drive() const { return drive_; }

    /**
     * @brief Возвращает тип NAND памяти.
     * @return Тип ячеек флеш-памяти.
     */
    const std::string& nandType() const { return nandType_; }

    /**
     * @brief Возвращает ресурс гарантированной записи TBW.
     * @return Значение в ТБ.
     */
    uint32_t tbwRatingTB() const { return tbwRatingTB_; }

    /**
     * @brief Возвращает фактически записанный объём данных в ТБ.
     * @return Записанные данные в терабайтах.
     */
    double terabytesWritten() const { return terabytesWritten_; }

    /**
     * @brief Выполняет команду TRIM для очистки неиспользуемых блоков NAND.
     */
    void executeTrim();

    /**
     * @brief Рассчитывает остаточный ресурс ячеек памяти в процентах.
     * @return Ресурс (0..100%).
     */
    double calculateRemainingLifePercent() const;

    /**
     * @brief Эмулирует запись объёма данных на диск и учитывает износ ячеек.
     * @param payload Размер записываемых данных.
     */
    void writePayload(ByteSize payload);

private:
    StorageDrive drive_;
    std::string nandType_;
    uint32_t tbwRatingTB_;
    double terabytesWritten_;
};
