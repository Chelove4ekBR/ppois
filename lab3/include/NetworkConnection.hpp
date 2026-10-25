#pragma once

#include <cstdint>
#include <string>
#include "IpAddress.hpp"

/**
 * @brief Представляет активное TCP/UDP сетевое соединение с удалённым узлом (сокет).
 */
class NetworkConnection {
public:
    /**
     * @brief Создаёт сетевое соединение.
     * @param connectionId Идентификатор соединения.
     * @param remoteIp IP-адрес удалённого узла.
     * @param remotePort Порт удалённого сервиса.
     * @param state Начальное состояние ("ESTABLISHED").
     */
    NetworkConnection(uint32_t connectionId, IpAddress remoteIp, uint16_t remotePort,
                      std::string state = "ESTABLISHED");

    /**
     * @brief Возвращает идентификатор соединения.
     * @return Числовой ID.
     */
    uint32_t connectionId() const { return connectionId_; }

    /**
     * @brief Возвращает IP-адрес удалённого узла.
     * @return Ссылка на IpAddress.
     */
    const IpAddress& remoteIp() const { return remoteIp_; }

    /**
     * @brief Возвращает удалённый порт.
     * @return Номер порта.
     */
    uint16_t remotePort() const { return remotePort_; }

    /**
     * @brief Возвращает состояние соединения.
     * @return Строка состояния ("ESTABLISHED", "CLOSED").
     */
    const std::string& state() const { return state_; }

    /**
     * @brief Возвращает количество переданных байт.
     * @return Объём переданных данных.
     */
    uint64_t bytesTransferred() const { return bytesTransferred_; }

    /**
     * @brief Передаёт порцию данных через открытое соединение.
     * @param bytes Число отправляемых байт.
     */
    void transmitData(uint32_t bytes);

    /**
     * @brief Закрывает сетевое соединение.
     */
    void close();

    /**
     * @brief Проверяет, активно ли соединение.
     * @return true, если соединение открыто.
     */
    bool isOpen() const;

    /**
     * @brief Переоткрывает соединение на новый порт.
     * @param newPort Новый порт.
     */
    void reopen(uint16_t newPort);

    /**
     * @brief Формирует текстовое описание сетевого сокета.
     * @return Строка вида "Connection #1 to 192.168.1.1:80 [ESTABLISHED]".
     */
    std::string describe() const;

private:
    uint32_t connectionId_;
    IpAddress remoteIp_;
    uint16_t remotePort_;
    std::string state_;
    uint64_t bytesTransferred_;
};
