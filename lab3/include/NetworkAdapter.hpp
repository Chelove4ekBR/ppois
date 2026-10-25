#pragma once

#include <cstdint>
#include <string>

#include "IpAddress.hpp"
#include "MacAddress.hpp"
#include "NetworkPacket.hpp"

/**
 * @brief Представляет сетевой адаптер (NIC - Network Interface Card) Ethernet или Wi-Fi.
 */
class NetworkAdapter {
public:
    /**
     * @brief Создаёт сетевой адаптер.
     * @param adapterName Название интерфейса (например, "eth0" или "wlan0").
     * @param mac MAC-адрес интерфейса.
     * @param ip IP-адрес интерфейса.
     * @param maxSpeedMbps Максимальная скорость сетевого линка в Мбит/с.
     */
    NetworkAdapter(std::string adapterName = "eth0", MacAddress mac = MacAddress(),
                   IpAddress ip = IpAddress(), uint32_t maxSpeedMbps = 1000);

    /**
     * @brief Возвращает имя адаптера.
     * @return Название интерфейса.
     */
    const std::string& adapterName() const { return adapterName_; }

    /**
     * @brief Возвращает MAC-адрес.
     * @return Ссылка на MacAddress.
     */
    const MacAddress& macAddress() const { return macAddress_; }

    /**
     * @brief Возвращает текущий IP-адрес.
     * @return Ссылка на IpAddress.
     */
    const IpAddress& ipAddress() const { return ipAddress_; }

    /**
     * @brief Возвращает скорость линка.
     * @return Скорость в Мбит/с.
     */
    uint32_t maxSpeedMbps() const { return maxSpeedMbps_; }

    /**
     * @brief Проверяет активность сетевого кабеля/соединения.
     * @return true, если link up.
     */
    bool isLinkUp() const { return linkUp_; }

    /**
     * @brief Возвращает суммарный объём переданных и принятых байт.
     * @return Число байт трафика.
     */
    uint64_t totalBytesTransferred() const { return totalBytesTransferred_; }

    /**
     * @brief Назначает новый IP-адрес сетевому адаптеру.
     * @param ip Новый IP-адрес.
     */
    void setIpAddress(IpAddress ip);

    /**
     * @brief Устанавливает сетевое соединение (подключает кабель/линк).
     */
    void connectLink();

    /**
     * @brief Разрывает физический линк сети.
     */
    void disconnectLink();

    /**
     * @brief Отправляет сетевой пакет через адаптер.
     * @param packet Передаваемый пакет.
     */
    void sendPacket(NetworkPacket& packet);

    /**
     * @brief Принимает входящий сетевой пакет.
     * @param packet Входящий пакет.
     */
    void receivePacket(const NetworkPacket& packet);

    /**
     * @brief Переводит максимальную скорость в Гбит/с.
     * @return Скорость в Гбит/с.
     */
    double bandwidthGbps() const;

private:
    std::string adapterName_;
    MacAddress macAddress_;
    IpAddress ipAddress_;
    uint32_t maxSpeedMbps_;
    bool linkUp_;
    uint64_t totalBytesTransferred_;
};
