#include "test_support.hpp"

namespace support {

MotherboardPtr makeMotherboard() {
    Chipset chipset("B650", "AM5", 36, true);
    Bios bios("1.0.0", "AMI", true, true);
    return std::make_shared<Motherboard>("MSI B650 Tomahawk", "ATX", "AM5", chipset, bios, 4);
}

CpuPtr makeCpu() {
    auto cpu = std::make_shared<Cpu>("AMD Ryzen 7 7800X3D", "AM5", 8,
                                     ClockFrequency::fromGHz(4.2), ByteSize::fromMiB(96), 120);
    cpu->attachCooler(std::make_shared<CpuCooler>("Air", 180, 2000));
    return cpu;
}

PowerSupplyUnit makePsu(uint32_t watts) {
    return PowerSupplyUnit("Corsair RM750x", watts, "80 PLUS Gold", true);
}

Computer makeComputer() {
    auto mobo = makeMotherboard();
    auto cpu = makeCpu();
    auto psu = makePsu(850);
    Computer pc("TestRig", mobo, cpu, psu);

    RamStick ram1("SN-R1", ByteSize::fromGiB(16), ClockFrequency::fromMHz(5600), "DDR5", 36, Voltage(1250));
    RamStick ram2("SN-R2", ByteSize::fromGiB(16), ClockFrequency::fromMHz(5600), "DDR5", 36, Voltage(1250));
    pc.ram().installStick(ram1);
    pc.ram().installStick(ram2);

    Gpu gpu("GeForce RTX 4070", 5888, ClockFrequency::fromMHz(2475), 200);
    auto gpuCard = std::make_shared<GraphicsCard>(gpu, ByteSize::fromGiB(12), 16);
    pc.setGraphicsCard(gpuCard);

    StorageDrive drive("WD Black SN850X", "SN850X-1TB", "NVMe", ByteSize::fromGiB(1000), 7300, 6300);
    drive.addPartition(StoragePartition("Root", "ext4", ByteSize::fromGiB(1000), "/"));
    pc.addStorageDrive(std::make_shared<StorageDrive>(drive));

    return pc;
}

} // namespace support
