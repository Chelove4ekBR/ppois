#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "audio/SoundCard.hpp"
#include "chassis/ComputerCase.hpp"
#include "common/ByteSize.hpp"
#include "common/ClockFrequency.hpp"
#include "common/Exceptions.hpp"
#include "common/Voltage.hpp"
#include "cpu/Cpu.hpp"
#include "cpu/CpuCooler.hpp"
#include "graphics/Display.hpp"
#include "graphics/Gpu.hpp"
#include "graphics/GraphicsCard.hpp"
#include "memory/RamStick.hpp"
#include "motherboard/Bios.hpp"
#include "motherboard/Chipset.hpp"
#include "motherboard/Motherboard.hpp"
#include "network/IpAddress.hpp"
#include "network/NetworkPacket.hpp"
#include "os/ComputerTask.hpp"
#include "os/OperatingSystem.hpp"
#include "power/PowerSupplyUnit.hpp"
#include "storage/StorageDrive.hpp"
#include "storage/StoragePartition.hpp"
#include "system/Benchmark.hpp"
#include "system/Computer.hpp"
#include "system/HardwareDiagnostic.hpp"

namespace
{

    Computer buildDemoComputer()
    {
        Chipset chipset("Intel Z790", "LGA1700", 38, true);
        Bios bios("2.10", "American Megatrends Inc.", true, true);
        auto mobo = std::make_shared<Motherboard>("ASUS ROG MAXIMUS Z790", "ATX", "LGA1700", chipset, bios, 4);

        ClockFrequency cpuBaseClock = ClockFrequency::fromGHz(3.4);
        ByteSize l3Cache = ByteSize::fromMiB(36);
        auto cpu = std::make_shared<Cpu>("Intel Core i9-14900K", "LGA1700", 8, cpuBaseClock, l3Cache, 253);
        auto cooler = std::make_shared<CpuCooler>("Liquid", 360, 2400);
        cpu->attachCooler(cooler);

        PowerSupplyUnit psu("Seasonic Prime TX-1000", 1000, "80 PLUS Titanium", true);
        Computer pc("Рабочая станция Pro", mobo, cpu, psu);

        // RAM
        ClockFrequency ramFreq = ClockFrequency::fromMHz(6000);
        Voltage ramVolt(1350);
        RamStick stick1("RAM-SN001", ByteSize::fromGiB(16), ramFreq, "DDR5", 30, ramVolt);
        RamStick stick2("RAM-SN002", ByteSize::fromGiB(16), ramFreq, "DDR5", 30, ramVolt);
        pc.ram().installStick(stick1);
        pc.ram().installStick(stick2);

        // GPU
        ClockFrequency gpuClock = ClockFrequency::fromMHz(2500);
        Gpu gpuChip("NVIDIA GeForce RTX 4090", 16384, gpuClock, 450);
        auto gpuCard = std::make_shared<GraphicsCard>(gpuChip, ByteSize::fromGiB(24), 16);
        auto display = std::make_shared<Display>("LG UltraGear 4K OLED", 32.0, 3840, 2160, 240, true);
        display->turnOn();
        gpuCard->connectDisplay(1, display);
        pc.setGraphicsCard(gpuCard);

        // Storage
        StorageDrive nvmeRaw("Samsung 990 PRO", "S990P-NVME-001", "NVMe", ByteSize::fromGiB(2000), 7450, 6900);
        StoragePartition sysPart("Системный", "ext4", ByteSize::fromGiB(500), "/");
        StoragePartition dataPart("Данные", "ext4", ByteSize::fromGiB(1400), "/home");
        nvmeRaw.addPartition(sysPart);
        nvmeRaw.addPartition(dataPart);
        auto nvmeDrive = std::make_shared<StorageDrive>(nvmeRaw);
        pc.addStorageDrive(nvmeDrive);

        StorageDrive hddRaw("Seagate IronWolf Pro", "ST-HDD-002", "SATA", ByteSize::fromGiB(4000), 260, 250);
        StoragePartition backupPart("Резервные копии", "ext4", ByteSize::fromGiB(4000), "/mnt/backup");
        hddRaw.addPartition(backupPart);
        auto hddDrive = std::make_shared<StorageDrive>(hddRaw);
        pc.addStorageDrive(hddDrive);

        // Network & OS
        pc.connectNetwork(IpAddress({192, 168, 1, 150}, 24));
        pc.installOperatingSystem("Ubuntu Linux 24.04 LTS", "6.8.0-31-generic");

        return pc;
    }

    std::string prompt(const std::string &msg)
    {
        std::cout << msg << ": ";
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    int promptInt(const std::string &msg)
    {
        const std::string s = prompt(msg);
        if (s.empty())
        {
            return -1;
        }
        try
        {
            return std::stoi(s);
        }
        catch (...)
        {
            return -1;
        }
    }

    void printMenuHeader()
    {
        std::cout << "\n===================================================================\n"
                  << "        СИСТЕМА УПРАВЛЕНИЯ ПЕРСОНАЛЬНЫМ КОМПЬЮТЕРОМ                \n"
                  << "===================================================================\n"
                  << "   1. Сводка о системе (характеристики и компоненты)\n"
                  << "   2. Включить питание компьютера (Power ON / POST)\n"
                  << "   3. Выключить питание компьютера (Power OFF)\n"
                  << "   4. Перезагрузить компьютер (Reboot)\n"
                  << "   5. Текущее энергопотребление компонентов\n"
                  << "   6. Аппаратная диагностика (POST и датчики)\n"
                  << "   7. Тест производительности (Бенчмарк)\n"
                  << "   8. Выполнить вычислительную задачу (Нагрузочный тест)\n"
                  << "   9. Список запущенных процессов (Диспетчер задач)\n"
                  << "  10. Запустить новое приложение\n"
                  << "  11. Завершить процесс по PID\n"
                  << "  12. Выполнить квант планировщика процессов\n"
                  << "  13. Накопители данных и логические разделы\n"
                  << "  14. Добавить новый накопитель (SSD / HDD)\n"
                  << "  15. Состояние оперативной памяти (RAM)\n"
                  << "  16. Состояние вентиляции корпуса и настройка PWM\n"
                  << "  17. Мониторинг температурных зон (CPU и GPU)\n"
                  << "  18. Сетевое подключение и настройка IP\n"
                  << "  19. Отправить сетевой пакет (Ping / ICMP)\n"
                  << "  20. Статус USB-портов и подключение устройства\n"
                  << "  21. Отключить USB-устройство\n"
                  << "  22. Тест звуковой подсистемы (ЦАП / Буфер)\n"
                  << "  23. Просмотр системного журнала событий\n"
                  << "  24. Очистить системный журнал событий\n"
                  << "   0. Завершить работу программы\n"
                  << "===================================================================\n";
    }

} // namespace

int main()
{
    Computer pc = buildDemoComputer();

    std::cout << "Инициализация системы завершена.\n";

    while (true)
    {
        printMenuHeader();

        const int choice = promptInt("Выберите пункт меню (0-24)");
        if (choice == 0 || !std::cin)
        {
            break;
        }

        try
        {
            switch (choice)
            {
            case 1:
            {
                std::cout << "\n"
                          << pc.systemSummary() << "\n";
                break;
            }

            case 2:
            {
                pc.powerOn();
                std::cout << "\n[OK] Компьютер успешно включён! Статус питания: " << pc.powerState() << "\n";
                break;
            }

            case 3:
            {
                pc.powerOff();
                std::cout << "\n[OK] Компьютер выключен. Статус питания: " << pc.powerState() << "\n";
                break;
            }

            case 4:
            {
                pc.reboot();
                std::cout << "\n[OK] Компьютер успешно перезагружен. Статус питания: " << pc.powerState() << "\n";
                break;
            }

            case 5:
            {
                const uint32_t currentWatts = pc.calculateTotalPowerConsumption();
                const uint32_t maxWatts = pc.psu().ratedWattage();
                const double loadPercent = maxWatts > 0 ? (static_cast<double>(currentWatts) / maxWatts * 100.0) : 0.0;
                std::cout << "\n=== Энергопотребление системы ===\n"
                          << "Блок питания: " << pc.psu().model() << " (" << maxWatts << " Вт, "
                          << pc.psu().efficiencyRating() << ")\n"
                          << "Текущая нагрузка: " << currentWatts << " Вт ("
                          << std::fixed << std::setprecision(1) << loadPercent << "% от номинальной мощности БП)\n";
                break;
            }

            case 6:
            {
                auto diag = pc.runDiagnostics();
                std::cout << "\n"
                          << diag.generateReport() << "\n";
                break;
            }

            case 7:
            {
                std::string benchName = prompt("Введите название теста (Enter для значения по умолчанию)");
                if (benchName.empty())
                {
                    benchName = "Комплексный бенчмарк системы";
                }
                auto bench = pc.runBenchmark(benchName);
                std::cout << "\n"
                          << bench.summary() << "\n";
                break;
            }

            case 8:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "\n[Ошибка] Невозможно выполнить задачу: компьютер выключен. Сначала включите питание (пункт 2)!\n";
                    break;
                }
                std::string taskName = prompt("Название задачи (по умолчанию: 'Рендеринг 3D-сцены')");
                const int mcycles = promptInt("Объём вычислений в миллионах тактов (по умолчанию: 100)");
                const int memMb = promptInt("Требуемый объём памяти в МиБ (по умолчанию: 256)");
                ComputerTask task(101, taskName.empty() ? "Рендеринг 3D-сцены" : taskName,
                                  static_cast<uint64_t>(mcycles > 0 ? mcycles : 100) * 1000000ULL,
                                  ByteSize::fromMiB(memMb > 0 ? memMb : 256));
                const double timeSec = pc.executeWorkload(task);
                std::cout << "\n[OK] Вычислительная нагрузка успешно выполнена за "
                          << std::fixed << std::setprecision(3) << timeSec << " сек.!\n";
                break;
            }

            case 9:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "\n[Инфо] Компьютер выключен. Операционная система не активна.\n";
                    break;
                }
                const auto &procs = pc.os().scheduler().processes();
                std::cout << "\n=== Список процессов ОС (" << pc.os().osName() << ") ===\n"
                          << "Всего процессов: " << procs.size()
                          << " | Алгоритм диспетчеризации: " << pc.os().scheduler().algorithm() << "\n\n";
                std::cout << std::left << std::setw(8) << "PID"
                          << std::setw(28) << "Имя процесса"
                          << std::setw(16) << "Память"
                          << std::setw(12) << "Приоритет"
                          << std::setw(12) << "Статус" << "\n";
                std::cout << std::string(76, '-') << "\n";
                for (const auto &proc : procs)
                {
                    std::cout << std::left << std::setw(8) << proc->pid()
                              << std::setw(28) << proc->name()
                              << std::setw(16) << proc->memoryUsage().toString()
                              << std::setw(12) << proc->priority()
                              << std::setw(12) << proc->state() << "\n";
                }
                break;
            }

            case 10:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "\n[Ошибка] Компьютер выключен. Включите питание для запуска программ!\n";
                    break;
                }
                std::string appName = prompt("Название приложения (напр. 'Веб-браузер' или 'IDE')");
                const int memMb = promptInt("Выделяемая память в МиБ (по умолчанию: 128)");
                auto proc = pc.os().launchApplication(appName.empty() ? "Пользовательское приложение" : appName,
                                                      ByteSize::fromMiB(memMb > 0 ? memMb : 128));
                std::cout << "\n[OK] Приложение запущено: PID=" << proc->pid()
                          << " [" << proc->name() << "], выделено памяти: "
                          << proc->memoryUsage().toString() << "\n";
                break;
            }

            case 11:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "\n[Ошибка] Компьютер выключен.\n";
                    break;
                }
                const int pid = promptInt("Введите PID процесса для завершения");
                if (pid <= 0)
                {
                    std::cout << "[Отмена] Некорректный PID.\n";
                    break;
                }
                pc.os().terminateProcess(static_cast<uint32_t>(pid));
                std::cout << "\n[OK] Процесс с PID=" << pid << " успешно завершён.\n";
                break;
            }

            case 12:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "\n[Ошибка] Компьютер выключен.\n";
                    break;
                }
                double avgLoad = pc.os().scheduler().scheduleCycle();
                std::cout << "\n[OK] Квант планировщика (" << pc.os().scheduler().algorithm()
                          << ") выполнен. Средняя нагрузка процессов: "
                          << std::fixed << std::setprecision(1) << avgLoad << "%\n";
                break;
            }

            case 13:
            {
                std::cout << "\n=== Дисковая подсистема компьютера ===\n"
                          << "Количество физических накопителей: " << pc.drives().size() << "\n";
                for (size_t i = 0; i < pc.drives().size(); ++i)
                {
                    const auto &d = pc.drives()[i];
                    std::cout << "\n[" << (i + 1) << "] " << d->model() << " (" << d->interfaceType() << ")\n"
                              << "    Серийный номер: " << d->serialNumber() << "\n"
                              << "    Полная ёмкость: " << d->totalCapacity().toString() << "\n"
                              << "    Скорость последовательного доступа: Чтение " << d->readSpeedMBs()
                              << " МБ/с, Запись " << d->writeSpeedMBs() << " МБ/с\n"
                              << "    Здоровье S.M.A.R.T.: " << d->healthPercent() << "%\n"
                              << "    Неразмеченная область: " << d->unpartitionedCapacity().toString() << "\n";
                    if (!d->partitions().empty())
                    {
                        std::cout << "    Разделы:\n";
                        for (const auto &part : d->partitions())
                        {
                            std::cout << "      - " << part.name() << " (" << part.fileSystemType() << "): "
                                      << part.usedBytes().toString() << " / " << part.capacity().toString()
                                      << " [монтирование: '" << part.mountPoint() << "']\n";
                        }
                    }
                }
                break;
            }

            case 14:
            {
                std::string model = prompt("Модель накопителя (напр. 'Kingston KC3000' или Enter)");
                std::string iface = prompt("Интерфейс (NVMe / SATA, по умолчанию: NVMe)");
                const int sizeGb = promptInt("Объём накопителя в ГБ (по умолчанию: 1000)");
                auto newDrive = std::make_shared<StorageDrive>(
                    model.empty() ? "Kingston KC3000 NVMe" : model,
                    "SSD-SN" + std::to_string(rand() % 90000 + 10000),
                    iface.empty() ? "NVMe" : iface,
                    ByteSize::fromGiB(sizeGb > 0 ? sizeGb : 1000),
                    7000, 6000);
                pc.addStorageDrive(newDrive);
                std::cout << "\n[OK] Накопитель " << newDrive->model() << " ("
                          << newDrive->totalCapacity().toString() << ") успешно подключён!\n";
                break;
            }

            case 15:
            {
                std::cout << "\n=== Подсистема оперативной памяти (RAM) ===\n"
                          << "Занято слотов: " << pc.ram().installedCount() << " из " << pc.ram().maxSlots() << "\n"
                          << "Суммарный объём: " << pc.ram().totalCapacity().toString() << "\n"
                          << "Результирующая частота: " << pc.ram().operatingFrequency().toMHz() << " МГц\n"
                          << "Многоканальный режим: " << (pc.ram().isMultiChannel() ? "Активен" : "Неактивен") << "\n"
                          << "Пропускная способность: " << std::fixed << std::setprecision(1) << pc.ram().totalBandwidthMBs() << " МБ/с\n\n";
                for (size_t i = 0; i < pc.ram().sticks().size(); ++i)
                {
                    const auto &s = pc.ram().sticks()[i];
                    std::cout << "  Слот #" << (i + 1) << ": " << s.serialNumber() << " | " << s.generation() << " | "
                              << s.capacity().toString() << " | " << s.frequency().toMHz() << " МГц | CL"
                              << s.casLatency() << " | " << s.voltage().toVolts() << " В\n";
                }
                break;
            }

            case 16:
            {
                std::cout << "\n=== Корпус и вентиляция ===\n"
                          << "Модель корпуса: " << pc.computerCase().model()
                          << " (Форм-фактор: " << pc.computerCase().formFactorSupported() << ")\n"
                          << "Количество корпусных вентиляторов: " << pc.computerCase().fans().size() << "\n"
                          << "Суммарный воздушный поток: " << pc.computerCase().totalAirflowCfm() << " CFM\n"
                          << "Средний уровень шума: " << std::fixed << std::setprecision(1)
                          << pc.computerCase().averageNoiseDb() << " дБ\n";
                const int duty = promptInt("Задать скорость вентиляторов PWM в % (0-100, Enter - не менять)");
                if (duty >= 0 && duty <= 100)
                {
                    pc.computerCase().setAllFansPwm(static_cast<uint32_t>(duty));
                    std::cout << "\n[OK] Скорость вентиляторов установлена в " << duty << "%!\n"
                              << "Новый воздушный поток: " << pc.computerCase().totalAirflowCfm()
                              << " CFM, шум: " << pc.computerCase().averageNoiseDb() << " дБ\n";
                }
                break;
            }

            case 17:
            {
                std::cout << "\n=== Мониторинг температурных зон ===\n";
                const auto &cpuThermal = pc.cpu()->thermalZone();
                std::cout << "Процессор: " << pc.cpu()->model() << "\n"
                          << "  Текущая температура: " << cpuThermal.temperatureC() << " °C\n"
                          << "  Порог троттлинга:    " << cpuThermal.throttleTempC() << " °C\n"
                          << "  Критический предел:   " << cpuThermal.criticalTempC() << " °C\n";
                if (pc.cpu()->cooler())
                {
                    std::cout << "  Кулер процессора:    " << pc.cpu()->cooler()->coolerType()
                              << " (" << pc.cpu()->cooler()->fanRpm() << " об/мин из "
                              << pc.cpu()->cooler()->maxFanRpm() << " макс)\n";
                }
                if (pc.graphicsCard())
                {
                    const auto &gpuThermal = pc.graphicsCard()->gpu().thermalZone();
                    std::cout << "Видеокарта: " << pc.graphicsCard()->gpu().model() << "\n"
                              << "  Температура кристалла GPU: " << gpuThermal.temperatureC() << " °C\n"
                              << "  Порог троттлинга:          " << gpuThermal.throttleTempC() << " °C\n";
                }
                break;
            }

            case 18:
            {
                std::cout << "\n=== Сетевой адаптер ===\n"
                          << "Имя интерфейса:  " << pc.networkAdapter().adapterName() << "\n"
                          << "Физический MAC:  " << pc.networkAdapter().macAddress().toString() << "\n"
                          << "Текущий IP-адрес: " << pc.networkAdapter().ipAddress().toString() << "\n"
                          << "Скорость линка:  " << pc.networkAdapter().maxSpeedMbps() << " Мбит/с\n"
                          << "Состояние линка: " << (pc.networkAdapter().isLinkUp() ? "Подключено (UP)" : "Отключено (DOWN)") << "\n"
                          << "Объём трафика:   " << pc.networkAdapter().totalBytesTransferred() << " байт\n";
                std::string newIp = prompt("\nЗадать новый IP-адрес для подключения (напр. '192.168.1.180', Enter - пропустить)");
                if (!newIp.empty())
                {
                    pc.connectNetwork(IpAddress::fromString(newIp));
                    std::cout << "\n[OK] Сетевой адаптер успешно подключён с IP: " << newIp << "\n";
                }
                break;
            }

            case 19:
            {
                std::string targetIpStr = prompt("Целевой IP-адрес для отправки (по умолчанию: 192.168.1.1)");
                if (targetIpStr.empty())
                {
                    targetIpStr = "192.168.1.1";
                }
                IpAddress targetIp = IpAddress::fromString(targetIpStr);
                NetworkPacket packet(pc.networkAdapter().ipAddress(), targetIp, 64, "ICMP", 64);
                pc.networkAdapter().sendPacket(packet);
                std::cout << "\n[OK] Сетевой пакет отправлен: " << packet.describe() << "\n";
                break;
            }

            case 20:
            {
                std::cout << "\n=== Состояние USB-портов (" << pc.usbHub().hubName() << ") ===\n"
                          << "Подключено устройств: " << pc.usbHub().connectedDevicesCount()
                          << " из " << pc.usbHub().ports().size() << "\n"
                          << "Суммарный ток: " << pc.usbHub().totalCurrentDrawMa() << " мА\n\n";
                for (const auto &port : pc.usbHub().ports())
                {
                    std::cout << "  Порт #" << port.portNumber() << " [" << port.version() << "]: "
                              << (port.isOccupied() ? ("Занят -> '" + port.connectedDevice() + "' (" + std::to_string(port.consumedCurrentMa()) + " мА)") : "Свободен")
                              << " | макс. лимит: " << port.maxCurrentMa() << " мА\n";
                }
                std::string devName = prompt("\nПодключить устройство (введите имя устройства или Enter для пропуска)");
                if (!devName.empty())
                {
                    const int port = promptInt("Номер порта (1-6)");
                    const int currentMa = promptInt("Потребляемый ток в мА (по умолчанию: 250)");
                    pc.plugUsbDevice(static_cast<uint32_t>(port > 0 ? port : 1), devName,
                                     static_cast<uint32_t>(currentMa > 0 ? currentMa : 250));
                    std::cout << "\n[OK] Устройство '" << devName << "' подключено к порту #" << (port > 0 ? port : 1) << "!\n";
                }
                break;
            }

            case 21:
            {
                const int port = promptInt("Номер порта для извлечения устройства (1-6)");
                if (port <= 0)
                {
                    std::cout << "[Отмена] Некорректный номер порта.\n";
                    break;
                }
                pc.unplugUsbDevice(static_cast<uint32_t>(port));
                std::cout << "\n[OK] Устройство отключено от порта #" << port << ".\n";
                break;
            }

            case 22:
            {
                std::cout << "\n=== Звуковая подсистема ===\n"
                          << "Модель аудиокарты:      " << pc.soundCard().model() << "\n"
                          << "Аудиочип ЦАП:           " << pc.soundCard().dacModel() << "\n"
                          << "Число каналов:          " << pc.soundCard().channels()
                          << (pc.soundCard().supportsSurround() ? " (Surround 5.1/7.1)" : " (Стерео)") << "\n"
                          << "Разрядность ЦАП:        " << pc.soundCard().bitDepth() << " бит\n"
                          << "Частота дискретизации:  " << pc.soundCard().maxSampleRateKhz() << " кГц\n"
                          << "Поддержка Hi-Res Audio: " << (pc.soundCard().supportsHighResAudio() ? "Да" : "Нет") << "\n"
                          << "Битрейт PCM-потока:     " << pc.soundCard().calculateAudioBitrateKbps() << " кбит/с\n";
                double duration = pc.soundCard().processAudioBuffer(48000);
                std::cout << "Обработан тестовый аудиобуфер (48000 сэмплов). Длительность звучания: "
                          << std::fixed << std::setprecision(2) << duration << " сек.\n";
                break;
            }

            case 23:
            {
                std::cout << "\n=== Системный журнал событий (" << pc.logger().totalEntries()
                          << " записей, ошибок: " << pc.logger().errorCount() << ") ===\n";
                for (const auto &entry : pc.logger().entries())
                {
                    std::cout << entry.format() << "\n";
                }
                break;
            }

            case 24:
            {
                pc.logger().clear();
                std::cout << "\n[OK] Системный журнал событий успешно очищен.\n";
                break;
            }

            default:
                std::cout << "\n[Предупреждение] Неизвестный пункт меню (" << choice << "). Пожалуйста, выберите число от 0 до 24.\n";
                break;
            }
        }
        catch (const ComputerException &ex)
        {
            std::cout << "\n[Ошибка системы]: " << ex.what() << "\n";
        }
        catch (const std::exception &ex)
        {
            std::cout << "\n[Исключение]: " << ex.what() << "\n";
        }
    }

    return 0;
}
