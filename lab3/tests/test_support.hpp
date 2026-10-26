#pragma once

#include <memory>
#include <string>

#include "Benchmark.hpp"
#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Computer.hpp"
#include "ComputerCase.hpp"
#include "ComputerTask.hpp"
#include "Cpu.hpp"
#include "CpuCooler.hpp"
#include "Display.hpp"
#include "Exceptions.hpp"
#include "GraphicsCard.hpp"
#include "HardwareDiagnostic.hpp"
#include "IpAddress.hpp"
#include "Motherboard.hpp"
#include "NetworkAdapter.hpp"
#include "OperatingSystem.hpp"
#include "PowerSupplyUnit.hpp"
#include "RamStick.hpp"
#include "SolidStateDrive.hpp"
#include "StorageDrive.hpp"
#include "Voltage.hpp"

namespace support {

MotherboardPtr makeMotherboard();
CpuPtr makeCpu();
PowerSupplyUnit makePsu(uint32_t watts = 750);
Computer makeComputer();

} // namespace support
