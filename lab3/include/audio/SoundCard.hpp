#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет звуковую карту (встроенный кодек или дискретную аудиокарту).
 */
class SoundCard {
public:
    /**
     * @brief Создаёт звуковую карту.
     * @param model Модель звукового контроллера.
     * @param maxSampleRateKhz Максимальная частота дискретизации в кГц.
     * @param bitDepth Разрядность ЦАП в битах.
     * @param channels Количество аудиоканалов (2 для стерео, 6 для 5.1, 8 для 7.1).
     * @param dacModel Модель микросхемы ЦАП.
     */
    SoundCard(std::string model = "Realtek ALC1220", uint32_t maxSampleRateKhz = 192,
              uint32_t bitDepth = 24, uint32_t channels = 8, std::string dacModel = "ESS SABRE");

    /**
     * @brief Возвращает модель звуковой карты.
     * @return Модель карты.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает максимальную частоту дискретизации.
     * @return Частота в кГц.
     */
    uint32_t maxSampleRateKhz() const { return maxSampleRateKhz_; }

    /**
     * @brief Возвращает разрядность аудио в битах.
     * @return Разрядность.
     */
    uint32_t bitDepth() const { return bitDepth_; }

    /**
     * @brief Возвращает число каналов.
     * @return Количество каналов.
     */
    uint32_t channels() const { return channels_; }

    /**
     * @brief Возвращает модель ЦАП.
     * @return Название ЦАП.
     */
    const std::string& dacModel() const { return dacModel_; }

    /**
     * @brief Проверяет поддержку аудио высокого разрешения (Hi-Res Audio).
     * @return true, если частота >= 96 кГц и разрядность >= 24 бит.
     */
    bool supportsHighResAudio() const;

    /**
     * @brief Проверяет поддержку пространственного звука (5.1 / 7.1).
     * @return true, если каналов 6 или более.
     */
    bool supportsSurround() const;

    /**
     * @brief Рассчитывает битрейт несжатого PCM-потока в кбит/с.
     * @return Битрейт.
     */
    double calculateAudioBitrateKbps() const;

    /**
     * @brief Эмулирует обработку звукового буфера сэмплами.
     * @param samples Число сэмплов.
     * @return Длительность звучания буфера в секундах.
     */
    double processAudioBuffer(uint32_t samples);

    /**
     * @brief Задаёт текущую частоту дискретизации.
     * @param rateKhz Частота в кГц.
     */
    void setSampleRate(uint32_t rateKhz);

private:
    std::string model_;
    uint32_t maxSampleRateKhz_;
    uint32_t bitDepth_;
    uint32_t channels_;
    std::string dacModel_;
};
