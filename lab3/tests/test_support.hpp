#pragma once

#include <memory>
#include <string>

#include "system/Benchmark.hpp"
#include "common/ByteSize.hpp"
#include "common/ClockFrequency.hpp"
#include "system/Computer.hpp"
#include "chassis/ComputerCase.hpp"
#include "os/ComputerTask.hpp"
#include "cpu/Cpu.hpp"
#include "cpu/CpuCooler.hpp"
#include "graphics/Display.hpp"
#include "common/Exceptions.hpp"
#include "graphics/GraphicsCard.hpp"
#include "system/HardwareDiagnostic.hpp"
#include "network/IpAddress.hpp"
#include "motherboard/Motherboard.hpp"
#include "network/NetworkAdapter.hpp"
#include "os/OperatingSystem.hpp"
#include "power/PowerSupplyUnit.hpp"
#include "memory/RamStick.hpp"
#include "storage/SolidStateDrive.hpp"
#include "storage/StorageDrive.hpp"
#include "common/Voltage.hpp"

namespace support {

MotherboardPtr makeMotherboard();
CpuPtr makeCpu();
PowerSupplyUnit makePsu(uint32_t watts = 750);
Computer makeComputer();

} // namespace support
