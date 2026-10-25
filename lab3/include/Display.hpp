#pragma once

#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Представляет монитор или дисплей, подключённый к компьютеру.
 */
class Display {
public:
    /**
     * @brief Создаёт монитор.
     * @param model Модель дисплея.
     * @param diagonalInches Диагональ экрана в дюймах.
     * @param resolutionWidth Горизонтальное разрешение в пикселях.
     * @param resolutionHeight Вертикальное разрешение в пикселях.
     * @param refreshRateHz Частота обновления экрана в Гц.
     * @param hdrEnabled Поддержка расширенного динамического диапазона HDR.
     */
    Display(std::string model, double diagonalInches = 27.0,
            uint32_t resolutionWidth = 2560, uint32_t resolutionHeight = 1440,
            uint32_t refreshRateHz = 144, bool hdrEnabled = true);

    /**
     * @brief Возвращает модель монитора.
     * @return Название модели.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает диагональ экрана.
     * @return Размер в дюймах.
     */
    double diagonalInches() const { return diagonalInches_; }

    /**
     * @brief Возвращает ширину в пикселях.
     * @return Число пикселей по горизонтали.
     */
    uint32_t resolutionWidth() const { return resolutionWidth_; }

    /**
     * @brief Возвращает высоту в пикселях.
     * @return Число пикселей по вертикали.
     */
    uint32_t resolutionHeight() const { return resolutionHeight_; }

    /**
     * @brief Возвращает частоту обновления.
     * @return Частота в Гц.
     */
    uint32_t refreshRateHz() const { return refreshRateHz_; }

    /**
     * @brief Проверяет включение режима HDR.
     * @return true, если HDR активен.
     */
    bool isHdrEnabled() const { return hdrEnabled_; }

    /**
     * @brief Проверяет питание монитора.
     * @return true, если дисплей включен.
     */
    bool isPowerOn() const { return powerOn_; }

    /**
     * @brief Включает монитор.
     */
    void turnOn();

    /**
     * @brief Выключает монитор.
     */
    void turnOff();

    /**
     * @brief Устанавливает разрешение экрана.
     * @param width Ширина в пикселях.
     * @param height Высота в пикселях.
     */
    void setResolution(uint32_t width, uint32_t height);

    /**
     * @brief Задаёт частоту обновления экрана.
     * @param hz Частота в Гц.
     */
    void setRefreshRate(uint32_t hz);

    /**
     * @brief Переключает режим HDR.
     */
    void toggleHdr();

    /**
     * @brief Отображает кадр с заданным порядковым номером.
     * @param frameNumber Номер кадра.
     */
    void displayFrame(uint64_t frameNumber);

    /**
     * @brief Рассчитывает соотношение сторон экрана.
     * @return Числовое соотношение (например, 16/9 = 1.777...).
     */
    double aspectRatio() const;

private:
    std::string model_;
    double diagonalInches_;
    uint32_t resolutionWidth_;
    uint32_t resolutionHeight_;
    uint32_t refreshRateHz_;
    bool hdrEnabled_;
    bool powerOn_;
};

using DisplayPtr = std::shared_ptr<Display>;
