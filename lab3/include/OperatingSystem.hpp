#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "DeviceDriver.hpp"
#include "Process.hpp"
#include "ProcessScheduler.hpp"
#include "VirtualMemoryManager.hpp"

/**
 * @brief Представляет операционную систему компьютера, управляющую процессами, драйверами и памятью.
 */
class OperatingSystem {
public:
    /**
     * @brief Создаёт операционную систему.
     * @param osName Название ОС ("Ubuntu Linux", "Windows 11 Pro").
     * @param kernelVersion Версия ядра.
     * @param is64Bit 64-битная разрядность архитектуры.
     */
    OperatingSystem(std::string osName = "Ubuntu Linux 24.04 LTS",
                    std::string kernelVersion = "6.8.0-generic", bool is64Bit = true);

    /**
     * @brief Возвращает название ОС.
     * @return Имя операционной системы.
     */
    const std::string& osName() const { return osName_; }

    /**
     * @brief Возвращает версию ядра.
     * @return Версия ядра.
     */
    const std::string& kernelVersion() const { return kernelVersion_; }

    /**
     * @brief Проверяет 64-битную разрядность ОС.
     * @return true, если ОС 64-битная.
     */
    bool is64Bit() const { return is64Bit_; }

    /**
     * @brief Проверяет состояние загрузки ОС.
     * @return true, если ОС запущена.
     */
    bool isSystemBooted() const { return booted_; }

    /**
     * @brief Возвращает планировщик процессов.
     * @return Ссылка на ProcessScheduler.
     */
    ProcessScheduler& scheduler() { return scheduler_; }

    /**
     * @brief Возвращает планировщик процессов.
     * @return Константная ссылка на ProcessScheduler.
     */
    const ProcessScheduler& scheduler() const { return scheduler_; }

    /**
     * @brief Возвращает менеджер виртуальной памяти.
     * @return Ссылка на VirtualMemoryManager.
     */
    VirtualMemoryManager& vmm() { return vmm_; }

    /**
     * @brief Возвращает менеджер виртуальной памяти.
     * @return Константная ссылка на VirtualMemoryManager.
     */
    const VirtualMemoryManager& vmm() const { return vmm_; }

    /**
     * @brief Возвращает список драйверов.
     * @return Вектор DeviceDriver.
     */
    const std::vector<DeviceDriver>& drivers() const { return drivers_; }

    /**
     * @brief Загружает и инициализирует операционную систему.
     */
    void boot();

    /**
     * @brief Корректно останавливает операционную систему и завершает процессы.
     */
    void shutdown();

    /**
     * @brief Устанавливает драйвер устройства в ОС.
     * @param drv Драйвер.
     */
    void installDriver(DeviceDriver drv);

    /**
     * @brief Запускает прикладную программу / процесс.
     * @param name Название приложения.
     * @param memory Требуемая оперативная память.
     * @param priority Приоритет выполнения.
     * @return Указатель на запущенный Process.
     */
    ProcessPtr launchApplication(const std::string& name, ByteSize memory, uint32_t priority = 10);

    /**
     * @brief Завершает приложение по PID.
     * @param pid Идентификатор процесса.
     */
    void terminateProcess(uint32_t pid);

    /**
     * @brief Возвращает количество установленных драйверов.
     * @return Число драйверов.
     */
    uint32_t installedDriverCount() const;

private:
    std::string osName_;
    std::string kernelVersion_;
    bool is64Bit_;
    bool booted_;
    ProcessScheduler scheduler_;
    std::vector<DeviceDriver> drivers_;
    VirtualMemoryManager vmm_;
};
