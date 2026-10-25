#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "Bios.hpp"
#include "Chipset.hpp"
#include "Cpu.hpp"
#include "PcieBus.hpp"
#include "PowerSupplyUnit.hpp"

/**
 * @brief Представляет материнскую плату компьютера, объединяющую процессор, чипсет, BIOS и шины.
 */
class Motherboard {
public:
    /**
     * @brief Создаёт материнскую плату.
     * @param model Модель платы.
     * @param formFactor Форм-фактор ("ATX", "Micro-ATX", "Mini-ITX").
     * @param socket Тип разъёма для процессора.
     * @param chipset Чипсет платы.
     * @param bios Базовая система ввода-вывода.
     * @param maxRamSlots Количество слотов для оперативной памяти.
     */
    Motherboard(std::string model, std::string formFactor, std::string socket,
                Chipset chipset, Bios bios, uint32_t maxRamSlots = 4);

    /**
     * @brief Возвращает модель материнской платы.
     * @return Название модели.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает форм-фактор.
     * @return Форм-фактор.
     */
    const std::string& formFactor() const { return formFactor_; }

    /**
     * @brief Возвращает сокет процессора.
     * @return Название сокета.
     */
    const std::string& socket() const { return socket_; }

    /**
     * @brief Возвращает чипсет материнской платы.
     * @return Ссылка на Chipset.
     */
    const Chipset& chipset() const { return chipset_; }

    /**
     * @brief Возвращает BIOS материнской платы.
     * @return Ссылка на Bios.
     */
    Bios& bios() { return bios_; }

    /**
     * @brief Возвращает BIOS материнской платы.
     * @return Константная ссылка на Bios.
     */
    const Bios& bios() const { return bios_; }

    /**
     * @brief Возвращает шину PCIe.
     * @return Ссылка на PcieBus.
     */
    PcieBus& pcieBus() { return pcieBus_; }

    /**
     * @brief Возвращает шину PCIe.
     * @return Константная ссылка на PcieBus.
     */
    const PcieBus& pcieBus() const { return pcieBus_; }

    /**
     * @brief Возвращает количество слотов оперативной памяти.
     * @return Число слотов RAM.
     */
    uint32_t maxRamSlots() const { return maxRamSlots_; }

    /**
     * @brief Возвращает установленный процессор.
     * @return Умный указатель на Cpu.
     */
    const CpuPtr& cpu() const { return cpu_; }

    /**
     * @brief Проверяет, подключено ли питание.
     * @return true, если питание подано.
     */
    bool isPowerConnected() const { return powerConnected_; }

    /**
     * @brief Устанавливает процессор в сокет материнской платы.
     * @param cpu Умный указатель на процессор.
     */
    void installCpu(CpuPtr cpu);

    /**
     * @brief Извлекает процессор из сокета.
     * @return Извлечённый процессор.
     */
    CpuPtr removeCpu();

    /**
     * @brief Подключает кабели питания от блока питания.
     * @param psu Блок питания.
     */
    void connectPower(const PowerSupplyUnit& psu);

    /**
     * @brief Отключает питание материнской платы.
     */
    void disconnectPower();

    /**
     * @brief Проверяет готовность материнской платы к старту системы.
     * @param hasRam Наличие оперативной памяти в слотах.
     * @return true, если плата готова к запуску.
     */
    bool isReadyToBoot(bool hasRam) const;

    /**
     * @brief Устанавливает плату расширения в шину PCIe.
     * @param deviceName Имя устройства.
     * @param lanes Число требуемых линий.
     * @return Номер занятого слота.
     */
    uint32_t installPcieDevice(const std::string& deviceName, uint32_t lanes);

private:
    std::string model_;
    std::string formFactor_;
    std::string socket_;
    Chipset chipset_;
    Bios bios_;
    PcieBus pcieBus_;
    uint32_t maxRamSlots_;
    CpuPtr cpu_;
    bool powerConnected_;
};

using MotherboardPtr = std::shared_ptr<Motherboard>;
