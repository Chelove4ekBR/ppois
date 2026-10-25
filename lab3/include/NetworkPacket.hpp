#pragma once

#include <cstdint>
#include <string>
#include "IpAddress.hpp"

/**
 * @brief Представляет сетевой пакет данных на сетевом/транспортном уровне (IP/TCP/UDP).
 */
class NetworkPacket {
public:
    /**
     * @brief Создаёт сетевой пакет.
     * @param sourceIp IP-адрес отправителя.
     * @param destIp IP-адрес получателя.
     * @param payloadBytes Размер полезной нагрузки в байтах.
     * @param protocol Транспортный протокол ("TCP", "UDP", "ICMP").
     * @param ttl Время жизни пакета (Time To Live).
     */
    NetworkPacket(IpAddress sourceIp, IpAddress destIp, uint32_t payloadBytes,
                  std::string protocol = "TCP", uint8_t ttl = 64);

    /**
     * @brief Возвращает IP-адрес отправителя.
     * @return Ссылка на IpAddress.
     */
    const IpAddress& sourceIp() const { return sourceIp_; }

    /**
     * @brief Возвращает IP-адрес получателя.
     * @return Ссылка на IpAddress.
     */
    const IpAddress& destIp() const { return destIp_; }

    /**
     * @brief Возвращает объём полезной нагрузки.
     * @return Размер в байтах.
     */
    uint32_t payloadBytes() const { return payloadBytes_; }

    /**
     * @brief Возвращает протокол.
     * @return Строка протокола.
     */
    const std::string& protocol() const { return protocol_; }

    /**
     * @brief Возвращает оставшееся время жизни пакета (TTL).
     * @return Значение TTL.
     */
    uint8_t ttl() const { return ttl_; }

    /**
     * @brief Возвращает контрольную сумму пакета.
     * @return Чексумма.
     */
    uint16_t checksum() const { return checksum_; }

    /**
     * @brief Уменьшает TTL пакета при прохождении узла маршрутизации.
     */
    void decrementTtl();

    /**
     * @brief Проверяет, исчерпан ли лимит жизни пакета (TTL == 0).
     * @return true, если пакет истёк.
     */
    bool isExpired() const;

    /**
     * @brief Рассчитывает контрольную сумму заголовка пакета.
     * @return Значение 16-битной контрольной суммы.
     */
    uint16_t calculateChecksum() const;

    /**
     * @brief Проверяет целостность пакета сверкой сохранённой контрольной суммы.
     * @return true, если контрольная сумма верна.
     */
    bool verifyChecksum() const;

    /**
     * @brief Формирует текстовое описание пакета.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    IpAddress sourceIp_;
    IpAddress destIp_;
    uint32_t payloadBytes_;
    std::string protocol_;
    uint8_t ttl_;
    uint16_t checksum_;
};
