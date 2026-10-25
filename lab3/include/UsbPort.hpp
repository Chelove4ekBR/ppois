#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет аппаратный порт USB (Type-A / Type-C) с контролем питания и подключения.
 */
class UsbPort {
public:
    /**
     * @brief Создаёт порт USB.
     * @param portNumber Номер порта.
     * @param version Стандарт ("USB_2", "USB_3", "USB_C").
     * @param maxCurrentMa Максимальный ток питания в миллиамперах (мА).
     */
    UsbPort(uint32_t portNumber, std::string version = "USB_3", uint32_t maxCurrentMa = 900);

    /**
     * @brief Возвращает номер порта.
     * @return Числовой ID порта.
     */
    uint32_t portNumber() const { return portNumber_; }

    /**
     * @brief Возвращает стандарт USB.
     * @return Версия USB.
     */
    const std::string& version() const { return version_; }

    /**
     * @brief Возвращает предельный ток порта в мА.
     * @return Ток в мА.
     */
    uint32_t maxCurrentMa() const { return maxCurrentMa_; }

    /**
     * @brief Проверяет занятость порта.
     * @return true, если в порт вставлено устройство.
     */
    bool isOccupied() const { return occupied_; }

    /**
     * @brief Возвращает имя подключённого устройства.
     * @return Название периферии.
     */
    const std::string& connectedDevice() const { return connectedDevice_; }

    /**
     * @brief Возвращает текущее потребление тока.
     * @return Ток в мА.
     */
    uint32_t consumedCurrentMa() const { return consumedCurrentMa_; }

    /**
     * @brief Рассчитывает пропускную способность порта в Мбит/с.
     * @return Скорость передачи данных.
     */
    double speedMbps() const;

    /**
     * @brief Подключает устройство в USB-порт с учётом потребляемого тока.
     * @param device Название устройства.
     * @param currentMa Потребляемый ток в мА.
     */
    void plugDevice(std::string device, uint32_t currentMa);

    /**
     * @brief Извлекает устройство из USB-порта.
     */
    void unplugDevice();

    /**
     * @brief Проверяет доступность порта.
     * @return true, если порт свободен.
     */
    bool isAvailable() const;

    /**
     * @brief Рассчитывает остаток доступного тока по питанию порта.
     * @return Доступный ток в мА.
     */
    uint32_t availableCurrentMa() const;

private:
    uint32_t portNumber_;
    std::string version_;
    uint32_t maxCurrentMa_;
    bool occupied_;
    std::string connectedDevice_;
    uint32_t consumedCurrentMa_;
};
