#pragma once

#include <cstdint>
#include <string>
#include "NetworkAdapter.hpp"

/**
 * @brief Представляет беспроводной сетевой адаптер Wi-Fi с поддержкой SSID и уровня сигнала.
 */
class WifiInterface {
public:
    /**
     * @brief Создаёт Wi-Fi интерфейс.
     * @param adapter Базовый сетевой адаптер.
     */
    explicit WifiInterface(NetworkAdapter adapter);

    /**
     * @brief Возвращает сетевой адаптер.
     * @return Ссылка на NetworkAdapter.
     */
    NetworkAdapter& adapter() { return adapter_; }

    /**
     * @brief Возвращает сетевой адаптер.
     * @return Константная ссылка на NetworkAdapter.
     */
    const NetworkAdapter& adapter() const { return adapter_; }

    /**
     * @brief Возвращает имя текущей подключённой Wi-Fi сети (SSID).
     * @return Название сети.
     */
    const std::string& currentSsid() const { return currentSsid_; }

    /**
     * @brief Возвращает уровень сигнала в дБм (dBm).
     * @return Уровень сигнала в dBm.
     */
    int32_t signalStrengthDbm() const { return signalStrengthDbm_; }

    /**
     * @brief Проверяет факт подключения к Wi-Fi сети.
     * @return true, если подключено к точке доступа.
     */
    bool isConnected() const { return connected_; }

    /**
     * @brief Подключается к беспроводной сети по SSID и паролю.
     * @param ssid Имя сети.
     * @param password Пароль доступа.
     */
    void connectSsid(std::string ssid, const std::string& password);

    /**
     * @brief Отключается от текущей беспроводной сети.
     */
    void disconnectWifi();

    /**
     * @brief Обновляет измеренный уровень сигнала.
     * @param dbm Новое значение в dBm (-90..-30).
     */
    void updateSignalStrength(int32_t dbm);

    /**
     * @brief Рассчитывает качество сигнала в процентах (0..100%).
     * @return Качество связи.
     */
    double signalQualityPercent() const;

private:
    NetworkAdapter adapter_;
    std::string currentSsid_;
    int32_t signalStrengthDbm_;
    bool connected_;
};
