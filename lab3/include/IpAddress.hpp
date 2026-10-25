#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Представляет IPv4-адрес и маску подсети с проверкой принадлежности к подсети и RFC 1918.
 */
class IpAddress {
public:
    /**
     * @brief Создаёт IP-адрес.
     * @param octets 4 октета адреса.
     * @param subnetPrefix Префикс маски подсети CIDR (0..32).
     */
    explicit IpAddress(std::vector<uint8_t> octets = {192, 168, 1, 100}, uint8_t subnetPrefix = 24);

    /**
     * @brief Парсит IP-адрес из строки вида "192.168.1.1" или "192.168.1.1/24".
     * @param str Строковое представление IP.
     * @return Объект IpAddress.
     */
    static IpAddress fromString(const std::string& str);

    /**
     * @brief Возвращает 4 октета IP.
     * @return Вектор байтов.
     */
    const std::vector<uint8_t>& octets() const { return octets_; }

    /**
     * @brief Возвращает префикс подсети.
     * @return Длина префикса CIDR.
     */
    uint8_t subnetPrefix() const { return subnetPrefix_; }

    /**
     * @brief Форматирует IP-адрес в строку "A.B.C.D".
     * @return Текстовое представление.
     */
    std::string toString() const;

    /**
     * @brief Форматирует адрес с CIDR префиксом "A.B.C.D/N".
     * @return Текстовое представление с префиксом.
     */
    std::string toStringWithPrefix() const;

    /**
     * @brief Проверяет, является ли адрес локальным/серым (RFC 1918).
     * @return true, если адрес приватный.
     */
    bool isPrivate() const;

    /**
     * @brief Проверяет, является ли адрес петлевым (loopback, 127.0.0.0/8).
     * @return true, если loopback.
     */
    bool isLoopback() const;

    /**
     * @brief Проверяет, находятся ли два адреса в одной IP-подсети.
     * @param other Второй IP-адрес.
     * @return true, если адреса в одной подсети.
     */
    bool isInSameSubnet(const IpAddress& other) const;

    /**
     * @brief Возвращает адрес подсети (network address).
     * @return Объект IpAddress подсети.
     */
    IpAddress networkAddress() const;

private:
    std::vector<uint8_t> octets_;
    uint8_t subnetPrefix_;
};
