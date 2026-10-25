#pragma once

#include <cmath>
#include <cstdint>
#include <string>

/**
 * @brief Представляет манипулятор «мышь» как координатное устройство ввода.
 */
class Mouse {
public:
    /**
     * @brief Создаёт мышь.
     * @param model Модель мыши.
     * @param currentDpi Разрешающая способность оптического сенсора (DPI).
     * @param buttonCount Количество программируемых кнопок.
     * @param pollingRateHz Частота опроса порта в Гц.
     */
    Mouse(std::string model = "Gaming Mouse", uint32_t currentDpi = 1600,
          uint32_t buttonCount = 6, uint32_t pollingRateHz = 1000);

    /**
     * @brief Возвращает модель мыши.
     * @return Модель.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает текущее значение DPI.
     * @return DPI сенсора.
     */
    uint32_t currentDpi() const { return currentDpi_; }

    /**
     * @brief Возвращает число кнопок.
     * @return Количество кнопок.
     */
    uint32_t buttonCount() const { return buttonCount_; }

    /**
     * @brief Возвращает частоту опроса.
     * @return Частота в Гц.
     */
    uint32_t pollingRateHz() const { return pollingRateHz_; }

    /**
     * @brief Возвращает суммарное число кликов.
     * @return Число кликов.
     */
    uint32_t clickCount() const { return clickCount_; }

    /**
     * @brief Устанавливает разрешение сенсора (DPI).
     * @param dpi Новое значение DPI.
     */
    void setDpi(uint32_t dpi);

    /**
     * @brief Эмулирует нажатие кнопки мыши.
     * @param buttonId Номер кнопки (1..buttonCount).
     */
    void click(uint32_t buttonId);

    /**
     * @brief Эмулирует перемещение мыши и рассчитывает евклидово расстояние.
     * @param deltaX Смещение по горизонтали.
     * @param deltaY Смещение по вертикали.
     * @return Пройденная дистанция в условных единицах.
     */
    double move(int32_t deltaX, int32_t deltaY);

    /**
     * @brief Проверяет высокоточный режим сенсора (DPI >= 3200 и опрос >= 1000 Гц).
     * @return true, если режим высокой точности.
     */
    bool isHighPrecision() const;

    /**
     * @brief Формирует описание мыши.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    std::string model_;
    uint32_t currentDpi_;
    uint32_t buttonCount_;
    uint32_t pollingRateHz_;
    uint32_t clickCount_;
};
