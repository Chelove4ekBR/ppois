#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "CpuCooler.hpp"
#include "CpuCore.hpp"
#include "ThermalPaste.hpp"
#include "ThermalZone.hpp"

/**
 * @brief Представляет центральный процессор с набором ядер, кеш-памятью, тепловым пакетом и кулером.
 */
class Cpu {
public:
    /**
     * @brief Создаёт процессор.
     * @param model Модель процессора.
     * @param socket Тип процессорного разъёма (сокета).
     * @param coreCount Количество ядер.
     * @param baseClock Базовая частота ядер.
     * @param l3Cache Объём кэш-памяти 3-го уровня.
     * @param tdpWatt Тепловыделение (TDP) в ваттах.
     */
    Cpu(std::string model, std::string socket, uint32_t coreCount, ClockFrequency baseClock,
        ByteSize l3Cache, uint32_t tdpWatt);

    /**
     * @brief Возвращает модель процессора.
     * @return Название модели.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает сокет процессора.
     * @return Тип сокета.
     */
    const std::string& socket() const { return socket_; }

    /**
     * @brief Возвращает список ядер.
     * @return Ссылка на вектор ядер.
     */
    const std::vector<CpuCore>& cores() const { return cores_; }

    /**
     * @brief Возвращает объём L3 кэша.
     * @return Объём кэша.
     */
    const ByteSize& l3CacheSize() const { return l3CacheSize_; }

    /**
     * @brief Возвращает тепловыделение (TDP).
     * @return Значение в ваттах.
     */
    uint32_t tdpWatt() const { return tdpWatt_; }

    /**
     * @brief Возвращает термозону процессора.
     * @return Ссылка на ThermalZone.
     */
    ThermalZone& thermalZone() { return thermalZone_; }

    /**
     * @brief Возвращает термозону процессора.
     * @return Константная ссылка на ThermalZone.
     */
    const ThermalZone& thermalZone() const { return thermalZone_; }

    /**
     * @brief Возвращает установленный кулер.
     * @return Умный указатель на CpuCooler.
     */
    const CpuCoolerPtr& cooler() const { return cooler_; }

    /**
     * @brief Возвращает термопасту процессора.
     * @return Ссылка на ThermalPaste.
     */
    ThermalPaste& thermalPaste() { return paste_; }

    /**
     * @brief Возвращает термопасту процессора.
     * @return Константная ссылка на ThermalPaste.
     */
    const ThermalPaste& thermalPaste() const { return paste_; }

    /**
     * @brief Возвращает общее число ядер.
     * @return Количество ядер.
     */
    uint32_t totalCores() const;

    /**
     * @brief Возвращает число активных ядер.
     * @return Количество активных ядер.
     */
    uint32_t activeCoresCount() const;

    /**
     * @brief Рассчитывает среднюю загрузку процессора.
     * @return Средняя загрузка в процентах (0..100%).
     */
    double averageUsage() const;

    /**
     * @brief Устанавливает кулер на процессор.
     * @param cooler Умный указатель на кулер.
     */
    void attachCooler(CpuCoolerPtr cooler);

    /**
     * @brief Задаёт загрузку для конкретного ядра.
     * @param coreIndex Индекс ядра.
     * @param percent Процент загрузки.
     */
    void setCoreUsage(uint32_t coreIndex, double percent);

    /**
     * @brief Включает или выключает ядро процессора по индексу.
     * @param coreIndex Индекс ядра.
     * @param active Флаг активности.
     */
    void setCoreActive(uint32_t coreIndex, bool active);

    /**
     * @brief Выполняет вычисления заданного объёма тактов, распределяя их по ядрам.
     * @param totalCycles Суммарное количество тактов задачи.
     * @return Время выполнения в секундах.
     */
    double executeTask(uint64_t totalCycles);

    /**
     * @brief Выполняет цикл охлаждения с помощью установленного кулера.
     */
    void coolDown();

    /**
     * @brief Проверяет, перегревается ли процессор.
     * @return true, если температура выше критической или активен троттлинг.
     */
    bool isOverheating() const;

    /**
     * @brief Разгоняет все ядра процессора на заданный процент.
     * @param percent Процент разгона.
     */
    void overclockAllCores(double percent);

private:
    std::string model_;
    std::string socket_;
    std::vector<CpuCore> cores_;
    ByteSize l3CacheSize_;
    uint32_t tdpWatt_;
    ThermalZone thermalZone_;
    CpuCoolerPtr cooler_;
    ThermalPaste paste_;
};

using CpuPtr = std::shared_ptr<Cpu>;
