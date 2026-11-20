#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет выходной видеоразъём видеокарты (HDMI, DisplayPort).
 */
class VideoPort {
public:
    /**
     * @brief Создаёт видеопорт.
     * @param portId Номер порта.
     * @param portType Тип интерфейса ("HDMI", "DisplayPort").
     * @param version Версия стандарта (например, "2.1" или "1.4").
     */
    VideoPort(uint32_t portId, std::string portType = "DisplayPort", std::string version = "1.4");

    /**
     * @brief Возвращает номер порта.
     * @return Идентификатор порта.
     */
    uint32_t portId() const { return portId_; }

    /**
     * @brief Возвращает тип разъёма.
     * @return Тип порта.
     */
    const std::string& portType() const { return portType_; }

    /**
     * @brief Возвращает версию стандарта.
     * @return Строка версии.
     */
    const std::string& version() const { return version_; }

    /**
     * @brief Проверяет подключение кабеля монитора.
     * @return true, если подключен дисплей.
     */
    bool isConnected() const { return connected_; }

    /**
     * @brief Рассчитывает предельную пропускную способность видеовыхода в Гбит/с.
     * @return Пропускная способность.
     */
    double maxBandwidthGbps() const;

    /**
     * @brief Проверяет, способен ли порт выдать заданное разрешение и частоту развёртки.
     * @param width Ширина в пикселях.
     * @param height Высота в пикселях.
     * @param refreshHz Частота обновления экрана в Гц.
     * @return true, если видеорежим поддерживается.
     */
    bool supportsResolution(uint32_t width, uint32_t height, uint32_t refreshHz) const;

    /**
     * @brief Подключает кабель дисплея к порту.
     */
    void connectCable();

    /**
     * @brief Отключает кабель дисплея.
     */
    void disconnectCable();

    /**
     * @brief Формирует текстовое описание порта.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    uint32_t portId_;
    std::string portType_;
    std::string version_;
    bool connected_;
};
