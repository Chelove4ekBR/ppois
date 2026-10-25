#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "UsbPort.hpp"

/**
 * @brief Представляет USB-концентратор (хаб), объединяющий набор внешних портов.
 */
class UsbHub {
public:
    /**
     * @brief Создаёт USB-концентратор.
     * @param hubName Название хаба.
     * @param portCount Количество портов по умолчанию.
     */
    explicit UsbHub(std::string hubName = "Root USB Hub", uint32_t portCount = 6);

    /**
     * @brief Возвращает имя концентратора.
     * @return Название хаба.
     */
    const std::string& hubName() const { return hubName_; }

    /**
     * @brief Возвращает список портов хаба.
     * @return Вектор портов UsbPort.
     */
    const std::vector<UsbPort>& ports() const { return ports_; }

    /**
     * @brief Добавляет порт в концентратор.
     * @param port Порт USB.
     */
    void addPort(const UsbPort& port);

    /**
     * @brief Подключает устройство в указанный порт.
     * @param portNumber Номер порта.
     * @param device Название устройства.
     * @param currentMa Потребляемый ток в мА.
     */
    void plugDevice(uint32_t portNumber, const std::string& device, uint32_t currentMa);

    /**
     * @brief Извлекает устройство из указанного порта.
     * @param portNumber Номер порта.
     */
    void unplugDevice(uint32_t portNumber);

    /**
     * @brief Рассчитывает число подключённых в данный момент устройств.
     * @return Количество занятых портов.
     */
    uint32_t connectedDevicesCount() const;

    /**
     * @brief Рассчитывает суммарное потребление тока всеми подключёнными устройствами.
     * @return Суммарный ток в мА.
     */
    uint32_t totalCurrentDrawMa() const;

    /**
     * @brief Находит порт по его номеру.
     * @param portNumber Номер порта.
     * @return Ссылка на UsbPort.
     */
    UsbPort& findPort(uint32_t portNumber);

    /**
     * @brief Находит порт по его номеру.
     * @param portNumber Номер порта.
     * @return Константная ссылка на UsbPort.
     */
    const UsbPort& findPort(uint32_t portNumber) const;

private:
    std::string hubName_;
    std::vector<UsbPort> ports_;
};
