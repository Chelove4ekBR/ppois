#pragma once

#include <cstdint>
#include <string>
#include "ByteSize.hpp"

/**
 * @brief Представляет веб-камеру для захвата видеопотока и фотографий.
 */
class Webcam {
public:
    /**
     * @brief Создаёт веб-камеру.
     * @param model Модель веб-камеры.
     * @param resolutionWidth Разрешение по горизонтали.
     * @param resolutionHeight Разрешение по вертикали.
     * @param fps Частота захвата кадров в секунду.
     */
    Webcam(std::string model = "HD Pro Webcam", uint32_t resolutionWidth = 1920,
           uint32_t resolutionHeight = 1080, uint32_t fps = 60);

    /**
     * @brief Возвращает модель камеры.
     * @return Модель.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает ширину кадра.
     * @return Пиксели по горизонтали.
     */
    uint32_t resolutionWidth() const { return resolutionWidth_; }

    /**
     * @brief Возвращает высоту кадра.
     * @return Пиксели по вертикали.
     */
    uint32_t resolutionHeight() const { return resolutionHeight_; }

    /**
     * @brief Возвращает кадровую частоту.
     * @return FPS.
     */
    uint32_t fps() const { return fps_; }

    /**
     * @brief Проверяет активность захвата/трансляции видео.
     * @return true, если стрим активен.
     */
    bool isStreaming() const { return streaming_; }

    /**
     * @brief Запускает захват видеопотока.
     */
    void startStream();

    /**
     * @brief Останавливает видеопоток.
     */
    void stopStream();

    /**
     * @brief Рассчитывает несжатый битрейт видеопотока в МБ/с.
     * @return Поток данных в МБ/с.
     */
    double bandwidthRequiredMBs() const;

    /**
     * @brief Эмулирует захват статичного фотоснимка и рассчитывает размер файла.
     * @return Размер фотоснимка в ByteSize.
     */
    ByteSize capturePhoto() const;

    /**
     * @brief Формирует описание веб-камеры.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    std::string model_;
    uint32_t resolutionWidth_;
    uint32_t resolutionHeight_;
    uint32_t fps_;
    bool streaming_;
};
