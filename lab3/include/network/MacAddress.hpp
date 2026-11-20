#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Представляет физический MAC-адрес сетевого интерфейса (Ethernet/Wi-Fi).
 */
class MacAddress {
public:
    /**
     * @brief Создаёт MAC-адрес из 6 октетов.
     * @param octets Вектор из 6 байтов.
     */
    explicit MacAddress(std::vector<uint8_t> octets = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E});

    /**
     * @brief Парсит MAC-адрес из строки вида "AA:BB:CC:DD:EE:FF".
     * @param str Строковое представление.
     * @return Объект MacAddress.
     */
    static MacAddress fromString(const std::string& str);

    /**
     * @brief Возвращает байты адреса.
     * @return Вектор октетов.
     */
    const std::vector<uint8_t>& octets() const { return octets_; }

    /**
     * @brief Форматирует MAC-адрес в строку в шестнадцатеричном виде.
     * @return Строка вида "00:1A:2B:3C:4D:5E".
     */
    std::string toString() const;

    /**
     * @brief Проверяет, является ли адрес широковещательным (FF:FF:FF:FF:FF:FF).
     * @return true, если broadcast.
     */
    bool isBroadcast() const;

    /**
     * @brief Проверяет, является ли адрес групповым (multicast).
     * @return true, если multicast.
     */
    bool isMulticast() const;

    /**
     * @brief Сравнивает два MAC-адреса на равенство.
     * @param other Сравниваемый адрес.
     * @return true, если адреса идентичны.
     */
    bool matches(const MacAddress& other) const;

private:
    std::vector<uint8_t> octets_;
};
