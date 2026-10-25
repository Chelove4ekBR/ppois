#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Базовое исключение для ошибок предметной области компьютера.
 */
class ComputerException : public std::runtime_error {
public:
    /**
     * @brief Создаёт объект исключения ComputerException.
     * @param message Текст сообщения об ошибке.
     */
    explicit ComputerException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @brief Исключение, сообщающее об отсутствии требуемого компонента.
 */
class ComponentNotFoundException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о несовместимости аппаратных компонентов.
 */
class IncompatibleHardwareException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о перегреве аппаратного узла выше критического порога.
 */
class OverheatingException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о перегрузке блока питания.
 */
class PowerSupplyOverloadException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о нехватке оперативной памяти или адресного пространства.
 */
class InsufficientMemoryException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о переполнении дискового раздела или накопителя.
 */
class StorageFullException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о сбое или критическом износе накопителя данных.
 */
class DriveFailureException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее, что указанный процесс не найден.
 */
class ProcessNotFoundException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о некорректном сетевом или аппаратном адресе.
 */
class InvalidAddressException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, связанное с сетевыми операциями и передачей пакетов.
 */
class NetworkException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о попытке обращения к неподключённому устройству.
 */
class DeviceNotConnectedException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее об ошибке инициализации или работы драйвера устройства.
 */
class DriverFailedException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о провале самотестирования BIOS POST при включении.
 */
class BiosPostFailedException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о недопустимой конфигурации или параметре.
 */
class InvalidConfigurationException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о попытке занять уже занятый порт или слот расширения.
 */
class PortOccupiedException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о недопустимом доступе к сегменту памяти.
 */
class SegmentationFaultException : public ComputerException {
public:
    using ComputerException::ComputerException;
};

/**
 * @brief Исключение, сообщающее о полном разряде батареи источника бесперебойного питания.
 */
class BatteryDepletedException : public ComputerException {
public:
    using ComputerException::ComputerException;
};
