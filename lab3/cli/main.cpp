#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Benchmark.hpp"
#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Computer.hpp"
#include "ComputerTask.hpp"
#include "Cpu.hpp"
#include "CpuCooler.hpp"
#include "Display.hpp"
#include "Exceptions.hpp"
#include "GraphicsCard.hpp"
#include "HardDiskDrive.hpp"
#include "IpAddress.hpp"
#include "Motherboard.hpp"
#include "NetworkPacket.hpp"
#include "PowerSupplyUnit.hpp"
#include "RamStick.hpp"
#include "SolidStateDrive.hpp"
#include "StorageDrive.hpp"
#include "Voltage.hpp"

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
        Computer pc("Workstation Pro", mobo, cpu, psu);

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
        StoragePartition sysPart("System", "ext4", ByteSize::fromGiB(500), "/");
        StoragePartition dataPart("Data", "ext4", ByteSize::fromGiB(1400), "/home");
        nvmeRaw.addPartition(sysPart);
        nvmeRaw.addPartition(dataPart);
        auto nvmeDrive = std::make_shared<StorageDrive>(nvmeRaw);
        pc.addStorageDrive(nvmeDrive);

        StorageDrive hddRaw("Seagate IronWolf Pro", "ST-HDD-002", "SATA", ByteSize::fromGiB(4000), 260, 250);
        StoragePartition backupPart("Backup", "ext4", ByteSize::fromGiB(4000), "/mnt/backup");
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
            return 0;
        }
        try
        {
            return std::stoi(s);
        }
        catch (...)
        {
            return 0;
        }
    }

} // namespace

int main()
{
    Computer pc = buildDemoComputer();

    std::cout << "=== Computer System Management CLI ===\n";
    std::cout << "Loaded system: " << pc.name() << "\n\n";

    while (true)
    {
        std::cout << "1. Show System Summary\n"
                  << "2. Power ON / Boot Computer\n"
                  << "3. Power OFF Computer\n"
                  << "4. Run Hardware Diagnostics\n"
                  << "5. Run Performance Benchmark\n"
                  << "6. Launch Application (Process)\n"
                  << "7. Terminate Process\n"
                  << "8. Execute Workload Task\n"
                  << "9. Plug USB Device\n"
                  << "10. Send Network Packet\n"
                  << "11. View System Event Logs\n"
                  << "0. Exit\n";

        const int choice = promptInt("Select option");
        if (choice == 0 || !std::cin)
        {
            break;
        }

        try
        {
            switch (choice)
            {
            case 1:
                std::cout << pc.systemSummary() << "\n";
                break;

            case 2:
                pc.powerOn();
                std::cout << "Computer powered on successfully! State: " << pc.powerState() << "\n";
                break;

            case 3:
                pc.powerOff();
                std::cout << "Computer powered off. State: " << pc.powerState() << "\n";
                break;

            case 4:
            {
                auto diag = pc.runDiagnostics();
                std::cout << diag.generateReport() << "\n";
                break;
            }

            case 5:
            {
                std::string benchName = prompt("Enter benchmark name (or press Enter for default)");
                if (benchName.empty())
                {
                    benchName = "Standard Compute & Graphics Benchmark";
                }
                auto bench = pc.runBenchmark(benchName);
                std::cout << "\n"
                          << bench.summary() << "\n";
                break;
            }

            case 6:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "Cannot launch application: computer is OFF. Power on first!\n";
                    break;
                }
                std::string appName = prompt("Application name");
                const int memMb = promptInt("Memory requirement (MiB)");
                auto proc = pc.os().launchApplication(appName, ByteSize::fromMiB(memMb > 0 ? memMb : 128));
                std::cout << "Launched process: PID=" << proc->pid() << " [" << proc->name()
                          << "], Memory=" << proc->memoryUsage().toString() << "\n";
                break;
            }

            case 7:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "Computer is OFF.\n";
                    break;
                }
                const int pid = promptInt("Process PID to terminate");
                pc.os().terminateProcess(static_cast<uint32_t>(pid));
                std::cout << "Process PID=" << pid << " terminated.\n";
                break;
            }

            case 8:
            {
                if (pc.powerState() != "ON")
                {
                    std::cout << "Computer is OFF. Power on first!\n";
                    break;
                }
                std::string taskName = prompt("Task name");
                const int mcycles = promptInt("Required megacycles (e.g. 100)");
                ComputerTask task(101, taskName.empty() ? "Rendering Workload" : taskName,
                                  static_cast<uint64_t>(mcycles > 0 ? mcycles : 50) * 1000000ULL,
                                  ByteSize::fromMiB(256));
                const double time = pc.executeWorkload(task);
                std::cout << "Workload completed in " << time << " seconds!\n";
                break;
            }

            case 9:
            {
                const int port = promptInt("USB Port number (1-6)");
                std::string devName = prompt("Device name (e.g. 'USB Flash Drive')");
                const int currentMa = promptInt("Power requirement (mA, e.g. 250)");
                pc.plugUsbDevice(static_cast<uint32_t>(port > 0 ? port : 1),
                                 devName.empty() ? "External Drive" : devName,
                                 static_cast<uint32_t>(currentMa > 0 ? currentMa : 200));
                std::cout << "Device connected to port #" << port << "!\n";
                break;
            }

            case 10:
            {
                std::string targetIpStr = prompt("Target IP (e.g. 192.168.1.1)");
                if (targetIpStr.empty())
                {
                    targetIpStr = "192.168.1.1";
                }
                IpAddress targetIp = IpAddress::fromString(targetIpStr);
                NetworkPacket packet(pc.networkAdapter().ipAddress(), targetIp, 64, "ICMP", 64);
                pc.networkAdapter().sendPacket(packet);
                std::cout << "Sent: " << packet.describe() << "\n";
                break;
            }

            case 11:
            {
                std::cout << "\n=== System Event Log (" << pc.logger().totalEntries() << " entries) ===\n";
                for (const auto &entry : pc.logger().entries())
                {
                    std::cout << entry.format() << "\n";
                }
                break;
            }

            default:
                std::cout << "Unknown option. Please try again.\n";
                break;
            }
        }
        catch (const ComputerException &ex)
        {
            std::cout << "Error: " << ex.what() << "\n";
        }
    }

    return 0;
}
