#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Exceptions.hpp"
#include "FileSystem.hpp"
#include "HardDiskDrive.hpp"
#include "MemorySegment.hpp"
#include "RaidController.hpp"
#include "RamModule.hpp"
#include "RamStick.hpp"
#include "SolidStateDrive.hpp"
#include "StorageDrive.hpp"
#include "StoragePartition.hpp"
#include "VirtualMemoryManager.hpp"
#include "Voltage.hpp"

TEST_CASE("RamStick bandwidth and generation checks", "[ram_stick]") {
    RamStick stick("SN-DDR5-1", ByteSize::fromGiB(16), ClockFrequency::fromMHz(6000), "DDR5", 32, Voltage(1350));
    CHECK(stick.serialNumber() == "SN-DDR5-1");
    CHECK(stick.capacity().toGiB() == 16.0);
    CHECK(stick.frequency().toMHz() == 6000.0);
    CHECK(stick.generation() == "DDR5");
    CHECK(stick.casLatency() == 32);
    CHECK(stick.voltage().millivolts() == 1350);

    CHECK(stick.calculateBandwidthMBs() == 6000.0 * 8.0);
    CHECK(stick.matchesGeneration("DDR5"));
    CHECK_FALSE(stick.matchesGeneration("DDR4"));
    CHECK(stick.validateCompatibility(Voltage(1400)));
    CHECK_FALSE(stick.validateCompatibility(Voltage(1200)));
    CHECK_FALSE(stick.describe().empty());

    CHECK_THROWS_AS(RamStick("", ByteSize::fromGiB(8), ClockFrequency::fromMHz(3200)), InvalidConfigurationException);
}

TEST_CASE("RamModule multi-channel and slot capacity", "[ram_module]") {
    RamModule ram(2);
    CHECK(ram.maxSlots() == 2);
    CHECK(ram.installedCount() == 0);
    CHECK_FALSE(ram.isFull());
    CHECK_FALSE(ram.isMultiChannel());
    CHECK(ram.totalCapacity().bytes() == 0);

    RamStick s1("S1", ByteSize::fromGiB(8), ClockFrequency::fromMHz(5200), "DDR5");
    ram.installStick(s1);
    CHECK(ram.installedCount() == 1);
    CHECK(ram.totalCapacity().toGiB() == 8.0);
    CHECK_FALSE(ram.isMultiChannel());
    CHECK(ram.operatingFrequency().toMHz() == 5200.0);

    RamStick s2("S2", ByteSize::fromGiB(8), ClockFrequency::fromMHz(5600), "DDR5");
    ram.installStick(s2);
    CHECK(ram.installedCount() == 2);
    CHECK(ram.totalCapacity().toGiB() == 16.0);
    CHECK(ram.isFull());
    CHECK(ram.isMultiChannel());
    // Operates at the lower frequency of the two
    CHECK(ram.operatingFrequency().toMHz() == 5200.0);
    CHECK(ram.totalBandwidthMBs() > s1.calculateBandwidthMBs());

    // Slot overflow
    RamStick s3("S3", ByteSize::fromGiB(8), ClockFrequency::fromMHz(5200), "DDR5");
    CHECK_THROWS_AS(ram.installStick(s3), PortOccupiedException);

    // Generation mismatch
    RamModule ram4(4);
    ram4.installStick(s1);
    RamStick ddr4Stick("D4", ByteSize::fromGiB(8), ClockFrequency::fromMHz(3200), "DDR4");
    CHECK_THROWS_AS(ram4.installStick(ddr4Stick), IncompatibleHardwareException);

    ram.removeStick("S1");
    CHECK(ram.installedCount() == 1);
    CHECK_THROWS_AS(ram.removeStick("NonExistent"), ComponentNotFoundException);
    ram.removeStick("S2");
    CHECK(ram.installedCount() == 0);
    CHECK(ram.operatingFrequency().hertz() == 0);

    RamModule emptyRam(2);
    CHECK(emptyRam.totalBandwidthMBs() == 0.0);
    CHECK_THROWS_AS(RamModule(0), InvalidConfigurationException);

    RamModule ramLower(2);
    ramLower.installStick(RamStick("Fast", ByteSize::fromGiB(8), ClockFrequency::fromMHz(6000), "DDR5"));
    ramLower.installStick(RamStick("Slow", ByteSize::fromGiB(8), ClockFrequency::fromMHz(4800), "DDR5"));
    CHECK(ramLower.operatingFrequency().toMHz() == 4800.0);
}

TEST_CASE("MemorySegment permissions and address checks", "[memory_segment]") {
    MemorySegment seg(0x1000, ByteSize(4096), true, true, false);
    CHECK_THROWS_AS(MemorySegment(0x1000, ByteSize(0)), InvalidConfigurationException);
    CHECK(seg.startAddress() == 0x1000);
    CHECK(seg.endAddress() == 0x1000 + 4096 - 1);
    CHECK(seg.isReadable());
    CHECK(seg.isWritable());
    CHECK_FALSE(seg.isExecutable());

    CHECK_FALSE(seg.isAllocated());
    seg.allocate();
    CHECK(seg.isAllocated());
    CHECK_THROWS_AS(seg.allocate(), PortOccupiedException);
    seg.release();
    CHECK_FALSE(seg.isAllocated());

    CHECK_NOTHROW(seg.checkPermission(true, false));
    CHECK_THROWS_AS(seg.checkPermission(true, true), SegmentationFaultException);

    MemorySegment nonWritable(0x2000, ByteSize(1024), true, false, true);
    CHECK_THROWS_AS(nonWritable.checkPermission(true, false), SegmentationFaultException);

    CHECK(seg.containsAddress(0x1500));
    CHECK_FALSE(seg.containsAddress(0x5000));

    MemorySegment overlapSeg(0x1500, ByteSize(1024));
    CHECK(seg.overlaps(overlapSeg));
    MemorySegment farSeg(0x3000, ByteSize(1024));
    CHECK_FALSE(seg.overlaps(farSeg));
}

TEST_CASE("VirtualMemoryManager segment allocation and page faults", "[vmm]") {
    VirtualMemoryManager vmm(ByteSize::fromKiB(4), ByteSize::fromGiB(4));
    CHECK_THROWS_AS(VirtualMemoryManager(ByteSize(0), ByteSize::fromGiB(4)), InvalidConfigurationException);
    CHECK_THROWS_AS(vmm.allocateSegment(ByteSize(0), true, false), InvalidConfigurationException);
    CHECK(vmm.pageSize().bytes() == 4096);
    CHECK(vmm.availableSwap().toGiB() == 4.0);

    const uint64_t addr1 = vmm.allocateSegment(ByteSize::fromMiB(2), true, false);
    CHECK(addr1 > 0);
    CHECK(vmm.segments().size() == 1);
    CHECK(vmm.totalAllocatedBytes().toMiB() == 2.0);

    const auto& found = vmm.findSegment(addr1 + 100);
    CHECK(found.startAddress() == addr1);

    CHECK_THROWS_AS(vmm.findSegment(0x99999999ULL), InvalidAddressException);

    vmm.handlePageFault(ByteSize::fromMiB(512));
    CHECK(vmm.usedSwap().toMiB() == 512.0);

    CHECK_THROWS_AS(vmm.handlePageFault(ByteSize::fromGiB(10)), InsufficientMemoryException);

    vmm.freeSegment(addr1);
    CHECK(vmm.totalAllocatedBytes().bytes() == 0);
    CHECK_THROWS_AS(vmm.freeSegment(addr1), InvalidAddressException);
}

TEST_CASE("StoragePartition read, write and format", "[storage_partition]") {
    StoragePartition part("Data", "ext4", ByteSize::fromGiB(100), "/data");
    CHECK(part.name() == "Data");
    CHECK(part.fileSystemType() == "ext4");
    CHECK(part.capacity().toGiB() == 100.0);
    CHECK(part.usedBytes().bytes() == 0);
    CHECK(part.availableBytes().toGiB() == 100.0);
    CHECK(part.mountPoint() == "/data");

    part.writeBytes(ByteSize::fromGiB(30));
    CHECK(part.usedBytes().toGiB() == 30.0);
    CHECK(part.usagePercentage() == 30.0);

    part.freeBytes(ByteSize::fromGiB(10));
    CHECK(part.usedBytes().toGiB() == 20.0);

    part.freeBytes(ByteSize::fromGiB(100)); // clamp to 0
    CHECK(part.usedBytes().bytes() == 0);

    CHECK_THROWS_AS(part.writeBytes(ByteSize::fromGiB(150)), StorageFullException);

    part.format("Btrfs");
    CHECK(part.fileSystemType() == "Btrfs");
    CHECK(part.usedBytes().bytes() == 0);

    part.mount("/mnt/storage");
    CHECK(part.mountPoint() == "/mnt/storage");

    CHECK_THROWS_AS(StoragePartition("", "ext4", ByteSize::fromGiB(10)), InvalidConfigurationException);
    CHECK_THROWS_AS(StoragePartition("Data", "", ByteSize::fromGiB(10)), InvalidConfigurationException);
    CHECK_THROWS_AS(StoragePartition("Data", "ext4", ByteSize(0)), InvalidConfigurationException);
    CHECK_THROWS_AS(part.format(""), InvalidConfigurationException);
    CHECK_THROWS_AS(part.mount(""), InvalidConfigurationException);
}

TEST_CASE("StorageDrive partitioning and S.M.A.R.T.", "[storage_drive]") {
    StorageDrive drive("WD Red", "WDR-123", "SATA", ByteSize::fromGiB(1000), 200, 190);
    CHECK_THROWS_AS(StorageDrive("Model", "", "SATA", ByteSize::fromGiB(100), 100, 100), InvalidConfigurationException);
    CHECK_THROWS_AS(StorageDrive("Model", "SN", "SATA", ByteSize(0), 100, 100), InvalidConfigurationException);
    CHECK(drive.model() == "WD Red");
    CHECK(drive.serialNumber() == "WDR-123");
    CHECK(drive.interfaceType() == "SATA");
    CHECK(drive.totalCapacity().toGiB() == 1000.0);
    CHECK(drive.readSpeedMBs() == 200);
    CHECK(drive.writeSpeedMBs() == 190);
    CHECK(drive.healthPercent() == 100);
    CHECK(drive.performSelfTest());

    drive.addPartition(StoragePartition("P1", "NTFS", ByteSize::fromGiB(400)));
    drive.addPartition(StoragePartition("P2", "NTFS", ByteSize::fromGiB(500)));
    CHECK(drive.partitions().size() == 2);
    CHECK(drive.unpartitionedCapacity().toGiB() == 100.0);

    auto& p1 = drive.findPartition("P1");
    p1.writeBytes(ByteSize::fromGiB(100));
    CHECK(drive.totalUsedBytes().toGiB() == 100.0);

    CHECK_THROWS_AS(drive.addPartition(StoragePartition("P3", "NTFS", ByteSize::fromGiB(300))), StorageFullException);
    CHECK_THROWS_AS(drive.findPartition("Missing"), ComponentNotFoundException);

    drive.degradeHealth(20);
    CHECK(drive.healthPercent() == 80);
    CHECK_THROWS_AS(drive.degradeHealth(90), DriveFailureException);
}

TEST_CASE("HardDiskDrive mechanics and seek time", "[hdd]") {
    StorageDrive base("BarraCuda", "ST1000", "SATA", ByteSize::fromGiB(1000), 180, 160);
    HardDiskDrive hdd(base, 7200);
    CHECK(hdd.spindleSpeedRpm() == 7200);
    CHECK(hdd.badSectorsCount() == 0);
    CHECK(hdd.areHeadsParked());
    CHECK(hdd.seekTimeMs() > 5.0);

    hdd.unparkHeads();
    CHECK_FALSE(hdd.areHeadsParked());
    hdd.parkHeads();
    CHECK(hdd.areHeadsParked());

    hdd.remapBadSectors(5);
    CHECK(hdd.badSectorsCount() == 5);
    CHECK(hdd.drive().healthPercent() < 100);
    CHECK_THROWS_AS(hdd.remapBadSectors(600), DriveFailureException);
    CHECK_THROWS_AS(HardDiskDrive(hdd.drive(), 0), InvalidConfigurationException);
}

TEST_CASE("SolidStateDrive NAND endurance and TRIM", "[ssd]") {
    StorageDrive base("Crucial P5 Plus", "CT1000", "NVMe", ByteSize::fromGiB(1000), 6600, 5000);
    SolidStateDrive ssd(base, "TLC", 600);
    CHECK_THROWS_AS(SolidStateDrive(base, "TLC", 0), InvalidConfigurationException);
    CHECK(ssd.nandType() == "TLC");
    CHECK(ssd.tbwRatingTB() == 600);
    CHECK(ssd.terabytesWritten() == 0.0);
    CHECK(ssd.calculateRemainingLifePercent() == 100.0);

    ssd.writePayload(ByteSize::fromGiB(6000)); // 6 TB written
    CHECK(ssd.terabytesWritten() > 5.9);
    CHECK(ssd.calculateRemainingLifePercent() < 100.0);

    CHECK_NOTHROW(ssd.executeTrim());

    SolidStateDrive smallSsd(base, "QLC", 1);
    CHECK_THROWS_AS(smallSsd.writePayload(ByteSize::fromGiB(2000)), DriveFailureException);
    CHECK(smallSsd.calculateRemainingLifePercent() == 0.0);
}

TEST_CASE("RaidController redundancy and rebuild", "[raid]") {
    RaidController raid1(1);
    CHECK(raid1.raidLevel() == 1);
    CHECK(raid1.isFaultTolerant());

    auto d1 = std::make_shared<StorageDrive>("D1", "SN1", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    auto d2 = std::make_shared<StorageDrive>("D2", "SN2", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    raid1.attachDrive(d1);
    raid1.attachDrive(d2);

    CHECK(raid1.usableCapacity().toGiB() == 1000.0);
    CHECK(raid1.status() == "Optimal");

    raid1.handleDriveFailure("SN1");
    CHECK(raid1.status() == "Degraded");
    CHECK(raid1.drives().size() == 1);

    auto replacement = std::make_shared<StorageDrive>("D3", "SN3", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    raid1.rebuild(replacement);
    CHECK(raid1.status() == "Optimal");
    CHECK(raid1.drives().size() == 2);

    // RAID 5 tests
    RaidController raid5(5);
    auto r1 = std::make_shared<StorageDrive>("R1", "SR1", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    auto r2 = std::make_shared<StorageDrive>("R2", "SR2", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    auto r3 = std::make_shared<StorageDrive>("R3", "SR3", "NVMe", ByteSize::fromGiB(1000), 3000, 3000);
    raid5.attachDrive(r1);
    raid5.attachDrive(r2);
    raid5.attachDrive(r3);
    CHECK(raid5.usableCapacity().toGiB() == 2000.0); // (N-1) * Cap

    // Less than 3 drives for RAID 5
    RaidController raid5small(5);
    raid5small.attachDrive(r1);
    CHECK_THROWS_AS(raid5small.usableCapacity(), InvalidConfigurationException);

    CHECK_THROWS_AS(raid5.attachDrive(nullptr), ComponentNotFoundException);
    CHECK_THROWS_AS(raid5.rebuild(nullptr), ComponentNotFoundException);
    CHECK_THROWS_AS(raid5.handleDriveFailure("Unknown"), ComponentNotFoundException);
    CHECK_THROWS_AS(RaidController(99), InvalidConfigurationException);

    // Empty RAID capacity
    CHECK(RaidController(1).usableCapacity().bytes() == 0);

    // RAID 0 failure
    RaidController raid0(0);
    auto d4 = std::make_shared<StorageDrive>("D4", "SN4", "SATA", ByteSize::fromGiB(500), 500, 500);
    auto d5 = std::make_shared<StorageDrive>("D5", "SN5", "SATA", ByteSize::fromGiB(500), 500, 500);
    raid0.attachDrive(d4);
    raid0.attachDrive(d5);
    CHECK(raid0.usableCapacity().toGiB() == 1000.0);
    CHECK_THROWS_AS(raid0.handleDriveFailure("SN4"), DriveFailureException);
    CHECK(raid0.status() == "Failed");
    CHECK_THROWS_AS(raid0.rebuild(r1), DriveFailureException);
}

TEST_CASE("FileSystem clusters and fragmentation", "[filesystem]") {
    FileSystem fs("ext4", ByteSize(4096), true);
    CHECK(fs.name() == "ext4");
    CHECK(fs.clusterSize().bytes() == 4096);
    CHECK(fs.isJournaled());
    CHECK(fs.totalFiles() == 0);
    CHECK(fs.checkConsistency());

    CHECK(fs.calculateClustersNeeded(ByteSize(100)) == 1);
    CHECK(fs.calculateClustersNeeded(ByteSize(4096)) == 1);
    CHECK(fs.calculateClustersNeeded(ByteSize(4097)) == 2);
    CHECK(fs.calculateClustersNeeded(ByteSize(0)) == 0);

    StoragePartition part("Root", "ext4", ByteSize::fromGiB(10), "/");
    fs.createFile(ByteSize(10000), part); // requires 3 clusters = 12288 bytes
    CHECK(fs.totalFiles() == 1);
    CHECK(part.usedBytes().bytes() == 12288);

    fs.deleteFile(ByteSize(10000), part);
    CHECK(fs.totalFiles() == 0);
    CHECK(part.usedBytes().bytes() == 0);

    CHECK_THROWS_AS(fs.deleteFile(ByteSize(1000), part), ComponentNotFoundException);
    CHECK(fs.fragmentationPercent() == 0.0);

    fs.createFile(ByteSize(100), part);
    CHECK(fs.fragmentationPercent() > 0.0);

    FileSystem njFs("fat32", ByteSize(512), false);
    CHECK(njFs.checkConsistency());

    CHECK_THROWS_AS(FileSystem("", ByteSize(4096)), InvalidConfigurationException);
    CHECK_THROWS_AS(FileSystem("ext4", ByteSize(0)), InvalidConfigurationException);
}
