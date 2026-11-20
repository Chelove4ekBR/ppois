#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет системную логику (чипсет) материнской платы.
 */
class Chipset {
public:
    /**
     * @brief Создаёт чипсет.
     * @param model Модель чипсета (например, "Intel Z790" или "AMD B650").
     * @param supportedSocket Совместимый сокет.
     * @param maxPcieLanes Максимальное число поддерживаемых линий PCIe.
     * @param overclockingSupported Поддержка разгона компонентов.
     */
    Chipset(std::string model = "B650", std::string supportedSocket = "AM5",
            uint32_t maxPcieLanes = 36, bool overclockingSupported = true);

    /**
     * @brief Возвращает название модели чипсета.
     * @return Модель чипсета.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает поддерживаемый сокет.
     * @return Название сокета.
     */
    const std::string& supportedSocket() const { return supportedSocket_; }

    /**
     * @brief Возвращает число линий PCIe.
     * @return Количество линий.
     */
    uint32_t maxPcieLanes() const { return maxPcieLanes_; }

    /**
     * @brief Проверяет поддержку разгона.
     * @return true, если чипсет поддерживает оверклокинг.
     */
    bool overclockingSupported() const { return overclockingSupported_; }

    /**
     * @brief Проверяет возможность выполнения разгона.
     * @return true, если разгон разрешён.
     */
    bool canOverclock() const { return overclockingSupported_; }

    /**
     * @brief Проверяет совместимость с указанным сокетом процессора.
     * @param socket Сокет процессора.
     * @return true, если сокет поддерживается.
     */
    bool supportsSocket(const std::string& socket) const;

    /**
     * @brief Проверяет, укладывается ли запрошенное число линий в лимит чипсета.
     * @param lanes Запрашиваемое количество линий.
     * @return true, если достаточно линий.
     */
    bool validatePcieLanes(uint32_t lanes) const;

    /**
     * @brief Формирует текстовое описание чипсета.
     * @return Строка с характеристиками.
     */
    std::string describe() const;

private:
    std::string model_;
    std::string supportedSocket_;
    uint32_t maxPcieLanes_;
    bool overclockingSupported_;
};
