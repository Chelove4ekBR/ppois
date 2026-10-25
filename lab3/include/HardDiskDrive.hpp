#pragma once

#include <cstdint>
#include "StorageDrive.hpp"

/**
 * @brief Представляет электромеханический жёсткий диск (HDD) со шпинделем и магнитными головками.
 */
class HardDiskDrive {
public:
    /**
     * @brief Создаёт жёсткий диск.
     * @param drive Базовые параметры накопителя.
     * @param spindleSpeedRpm Скорость вращения шпинделя (5400, 7200, 10000 об/мин).
     */
    HardDiskDrive(StorageDrive drive, uint32_t spindleSpeedRpm = 7200);

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
     * @brief Возвращает скорость вращения шпинделя.
     * @return Скорость в об/мин.
     */
    uint32_t spindleSpeedRpm() const { return spindleSpeedRpm_; }

    /**
     * @brief Возвращает число обнаруженных битых секторов.
     * @return Количество сбойных секторов.
     */
    uint32_t badSectorsCount() const { return badSectorsCount_; }

    /**
     * @brief Проверяет факт парковки магнитных головок.
     * @return true, если головки запаркованы.
     */
    bool areHeadsParked() const { return headsParked_; }

    /**
     * @brief Рассчитывает среднее время позиционирования головок (Seek time) в миллисекундах.
     * @return Время в мс.
     */
    double seekTimeMs() const;

    /**
     * @brief Паркует считывающие головки для безопасной транспортировки или выключения.
     */
    void parkHeads();

    /**
     * @brief Распарковывает головки для перехода в рабочий режим.
     */
    void unparkHeads();

    /**
     * @brief Переназначает сбойные сектора на резервную область диска.
     * @param count Число переназначаемых секторов.
     */
    void remapBadSectors(uint32_t count);

private:
    StorageDrive drive_;
    uint32_t spindleSpeedRpm_;
    uint32_t badSectorsCount_;
    bool headsParked_;
};
