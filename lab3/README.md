# Laboratory Work №3. Large-Scale OOP Project
**Variant:** Computer

Row format: `Class | number of fields | number of methods | related classes`.

| Class | Fields | Methods | Associations (Related classes) |
|---|---|---|---|
| AudioOutput | 4 | 6 | — |
| Benchmark | 5 | 6 | Cpu, Gpu, StorageDrive |
| Bios | 5 | 5 | — |
| ByteSize | 1 | 10 | — |
| CaseFan | 5 | 5 | — |
| Chipset | 4 | 4 | — |
| ClockFrequency | 1 | 8 | — |
| Computer | 14 | 16 | Benchmark, ByteSize, ComputerCase, ComputerTask, Cpu, GraphicsCard, HardwareDiagnostic, IpAddress, Motherboard, NetworkAdapter, OperatingSystem, PowerSupplyUnit, RamModule, SoundCard, StorageDrive, SystemLogger, UsbHub |
| ComputerCase | 5 | 5 | CaseFan |
| ComputerTask | 6 | 5 | ByteSize |
| Cpu | 8 | 9 | ByteSize, ClockFrequency, CpuCooler, CpuCore, ThermalPaste, ThermalZone |
| CpuCooler | 5 | 5 | — |
| CpuCore | 5 | 5 | ClockFrequency |
| DeviceDriver | 4 | 5 | — |
| Display | 7 | 7 | — |
| FileSystem | 4 | 5 | ByteSize, StoragePartition |
| Gpu | 6 | 5 | ClockFrequency, ThermalZone |
| GraphicsCard | 5 | 7 | ByteSize, Display, Gpu, VideoPort |
| HardDiskDrive | 4 | 4 | StorageDrive |
| HardwareDiagnostic | 3 | 5 | Cpu, HardwareSensor, Motherboard, PowerSupplyUnit |
| HardwareSensor | 4 | 4 | — |
| IpAddress | 2 | 7 | — |
| Keyboard | 5 | 5 | — |
| MacAddress | 1 | 5 | — |
| MemorySegment | 6 | 6 | ByteSize |
| Motherboard | 9 | 6 | Bios, Chipset, Cpu, PcieBus, PowerSupplyUnit |
| Mouse | 5 | 5 | — |
| NetworkAdapter | 6 | 6 | IpAddress, MacAddress, NetworkPacket |
| NetworkConnection | 5 | 5 | IpAddress |
| NetworkPacket | 6 | 5 | IpAddress |
| OperatingSystem | 7 | 6 | ByteSize, DeviceDriver, Process, ProcessScheduler, VirtualMemoryManager |
| PcieBus | 2 | 6 | PcieSlot |
| PcieSlot | 5 | 5 | — |
| PowerSupplyUnit | 5 | 6 | — |
| Process | 6 | 6 | ByteSize |
| ProcessScheduler | 3 | 7 | ByteSize, Process |
| RaidController | 3 | 5 | ByteSize, StorageDrive |
| RamModule | 3 | 7 | ByteSize, ClockFrequency, RamStick |
| RamStick | 6 | 4 | ByteSize, ClockFrequency, Voltage |
| SolidStateDrive | 4 | 3 | ByteSize, StorageDrive |
| SoundCard | 5 | 5 | — |
| StorageDrive | 8 | 6 | ByteSize, StoragePartition |
| StoragePartition | 5 | 6 | ByteSize |
| SystemLogEntry | 4 | 3 | — |
| SystemLogger | 2 | 6 | SystemLogEntry |
| ThermalPaste | 3 | 4 | — |
| ThermalZone | 3 | 6 | — |
| UninterruptiblePowerSupply | 4 | 5 | — |
| UsbHub | 2 | 6 | UsbPort |
| UsbPort | 6 | 5 | — |
| VideoPort | 4 | 5 | — |
| VirtualMemoryManager | 5 | 6 | ByteSize, MemorySegment |
| Voltage | 1 | 6 | — |
| Webcam | 5 | 5 | ByteSize |
| WifiInterface | 4 | 5 | NetworkAdapter |

### Exceptions (18)

- ComputerException
- ComponentNotFoundException
- IncompatibleHardwareException
- OverheatingException
- PowerSupplyOverloadException
- InsufficientMemoryException
- StorageFullException
- DriveFailureException
- ProcessNotFoundException
- InvalidAddressException
- NetworkException
- DeviceNotConnectedException
- DriverFailedException
- BiosPostFailedException
- InvalidConfigurationException
- PortOccupiedException
- SegmentationFaultException
- BatteryDepletedException

### Final Statistics

| Indicator | Value |
|---|---|
| Classes | 55 |
| Fields | 244 |
| Behaviors | 308 |
| Associations | 86 |
| Exceptions | 18 |
| Code Coverage | 98.5% (all 55 classes ≥ 90%) |
