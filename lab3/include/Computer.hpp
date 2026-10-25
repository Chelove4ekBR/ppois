#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "Benchmark.hpp"
#include "ByteSize.hpp"
#include "ComputerCase.hpp"
#include "ComputerTask.hpp"
#include "Cpu.hpp"
#include "GraphicsCard.hpp"
#include "HardwareDiagnostic.hpp"
#include "IpAddress.hpp"
#include "Motherboard.hpp"
#include "NetworkAdapter.hpp"
#include "OperatingSystem.hpp"
#include "PowerSupplyUnit.hpp"
#include "RamModule.hpp"
#include "SoundCard.hpp"
#include "StorageDrive.hpp"
#include "SystemLogger.hpp"
#include "UsbHub.hpp"

/**
 * @brief Главный координирующий класс предметной области «Компьютер».
 * Объединяет все аппаратные и программные компоненты системы: материнскую плату,
 * процессор, память, видеокарту, накопители, блок питания, сеть, ОС, телеметрию и диагностику.
 */
class Computer {
public:
    /**
     * @brief Создаёт персональный компьютер с базовой конфигурацией.
     * @param name Название компьютера/сборки.
     * @param mobo Материнская плата.
     * @param cpu Центральный процессор.
     * @param psu Блок питания.
     */
    Computer(std::string name, MotherboardPtr mobo, CpuPtr cpu, PowerSupplyUnit psu);

    /**
     * @brief Возвращает имя компьютера.
     * @return Название сборки.
     */
    const std::string& name() const { return name_; }

    /**
     * @brief Возвращает статус питания ("OFF", "POST", "ON", "ERROR").
     * @return Строка статуса.
     */
    const std::string& powerState() const { return powerState_; }

    /**
     * @brief Возвращает материнскую плату.
     * @return Умный указатель на Motherboard.
     */
    const MotherboardPtr& motherboard() const { return motherboard_; }

    /**
     * @brief Возвращает процессор.
     * @return Умный указатель на Cpu.
     */
    const CpuPtr& cpu() const { return cpu_; }

    /**
     * @brief Возвращает подсистему оперативной памяти.
     * @return Ссылка на RamModule.
     */
    RamModule& ram() { return ramModule_; }

    /**
     * @brief Возвращает подсистему оперативной памяти.
     * @return Константная ссылка на RamModule.
     */
    const RamModule& ram() const { return ramModule_; }

    /**
     * @brief Возвращает видеокарту.
     * @return Умный указатель на GraphicsCard.
     */
    const GraphicsCardPtr& graphicsCard() const { return graphicsCard_; }

    /**
     * @brief Задаёт видеокарту для компьютера.
     * @param gpuCard Видеокарта.
     */
    void setGraphicsCard(GraphicsCardPtr gpuCard);

    /**
     * @brief Возвращает список дисковых накопителей.
     * @return Вектор указателей на StorageDrive.
     */
    const std::vector<StorageDrivePtr>& drives() const { return drives_; }

    /**
     * @brief Возвращает блок питания.
     * @return Ссылка на PowerSupplyUnit.
     */
    PowerSupplyUnit& psu() { return psu_; }

    /**
     * @brief Возвращает блок питания.
     * @return Константная ссылка на PowerSupplyUnit.
     */
    const PowerSupplyUnit& psu() const { return psu_; }

    /**
     * @brief Возвращает корпус компьютера.
     * @return Ссылка на ComputerCase.
     */
    ComputerCase& computerCase() { return computerCase_; }

    /**
     * @brief Возвращает сетевой адаптер.
     * @return Ссылка на NetworkAdapter.
     */
    NetworkAdapter& networkAdapter() { return networkAdapter_; }

    /**
     * @brief Возвращает звуковую карту.
     * @return Ссылка на SoundCard.
     */
    SoundCard& soundCard() { return soundCard_; }

    /**
     * @brief Возвращает концентратор USB.
     * @return Ссылка на UsbHub.
     */
    UsbHub& usbHub() { return usbHub_; }

    /**
     * @brief Возвращает операционную систему.
     * @return Ссылка на OperatingSystem.
     */
    OperatingSystem& os() { return os_; }

    /**
     * @brief Возвращает операционную систему.
     * @return Константная ссылка на OperatingSystem.
     */
    const OperatingSystem& os() const { return os_; }

    /**
     * @brief Возвращает системный логгер.
     * @return Ссылка на SystemLogger.
     */
    SystemLogger& logger() { return logger_; }

    /**
     * @brief Возвращает системный логгер.
     * @return Константная ссылка на SystemLogger.
     */
    const SystemLogger& logger() const { return logger_; }

    /**
     * @brief Включает компьютер, проводит POST и загружает ОС.
     */
    void powerOn();

    /**
     * @brief Выключает компьютер и корректно останавливает ОС.
     */
    void powerOff();

    /**
     * @brief Перезагружает компьютер.
     */
    void reboot();

    /**
     * @brief Проверяет работоспособность компьютера.
     * @return true, если компьютер включен и работает без критических ошибок.
     */
    bool isOperational() const;

    /**
     * @brief Устанавливает новую операционную систему.
     * @param osName Название ОС.
     * @param kernelVer Версия ядра.
     */
    void installOperatingSystem(std::string osName, std::string kernelVer);

    /**
     * @brief Добавляет накопитель данных в систему.
     * @param drive Умный указатель на накопитель.
     */
    void addStorageDrive(StorageDrivePtr drive);

    /**
     * @brief Подключает USB-устройство к концентратору.
     * @param portNumber Номер порта.
     * @param device Название устройства.
     * @param currentMa Потребляемый ток в мА.
     */
    void plugUsbDevice(uint32_t portNumber, const std::string& device, uint32_t currentMa);

    /**
     * @brief Отключает USB-устройство от концентратора.
     * @param portNumber Номер порта.
     */
    void unplugUsbDevice(uint32_t portNumber);

    /**
     * @brief Подключает компьютер к сети с заданным IP-адресом.
     * @param ip IP-адрес сетевого адаптера.
     */
    void connectNetwork(const IpAddress& ip);

    /**
     * @brief Отключает сетевой интерфейс компьютера от сети.
     */
    void disconnectNetwork();

    /**
     * @brief Выполняет комплексную аппаратную диагностику всей системы.
     * @return Отчёт HardwareDiagnostic.
     */
    HardwareDiagnostic runDiagnostics();

    /**
     * @brief Запускает бенчмарк производительности компьютера.
     * @param benchName Название теста.
     * @return Результаты Benchmark.
     */
    Benchmark runBenchmark(const std::string& benchName);

    /**
     * @brief Выполняет вычислительную нагрузку (задачу).
     * @param task Вычислительная задача.
     * @return Время выполнения в секундах.
     */
    double executeWorkload(ComputerTask& task);

    /**
     * @brief Рассчитывает суммарное текущее энергопотребление всех компонентов в ваттах.
     * @return Мощность в ваттах.
     */
    uint32_t calculateTotalPowerConsumption() const;

    /**
     * @brief Формирует сводный текстовый отчёт о конфигурации и состоянии компьютера.
     * @return Текст сводки.
     */
    std::string systemSummary() const;

private:
    std::string name_;
    MotherboardPtr motherboard_;
    CpuPtr cpu_;
    RamModule ramModule_;
    GraphicsCardPtr graphicsCard_;
    std::vector<StorageDrivePtr> drives_;
    PowerSupplyUnit psu_;
    ComputerCase computerCase_;
    NetworkAdapter networkAdapter_;
    SoundCard soundCard_;
    UsbHub usbHub_;
    OperatingSystem os_;
    SystemLogger logger_;
    std::string powerState_;
};
