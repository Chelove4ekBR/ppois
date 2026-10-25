#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет устройство вывода звука (динамики или наушники).
 */
class AudioOutput {
public:
    /**
     * @brief Создаёт аудиовыход.
     * @param deviceName Название устройства.
     * @param deviceType Тип устройства ("Speakers", "Headphones").
     * @param volumePercent Начальная громкость в процентах (0..100).
     */
    AudioOutput(std::string deviceName = "Stereo Speakers", std::string deviceType = "Speakers",
                uint32_t volumePercent = 50);

    /**
     * @brief Возвращает имя устройства.
     * @return Название.
     */
    const std::string& deviceName() const { return deviceName_; }

    /**
     * @brief Возвращает тип устройства.
     * @return Тип вывода.
     */
    const std::string& deviceType() const { return deviceType_; }

    /**
     * @brief Возвращает установленный уровень громкости.
     * @return Громкость (0..100).
     */
    uint32_t volumePercent() const { return volumePercent_; }

    /**
     * @brief Проверяет факт приглушения звука (Mute).
     * @return true, если звук заглушен.
     */
    bool isMuted() const { return muted_; }

    /**
     * @brief Устанавливает громкость воспроизведения.
     * @param percent Процент громкости (0..100).
     */
    void setVolume(uint32_t percent);

    /**
     * @brief Заглушает звук.
     */
    void mute();

    /**
     * @brief Восстанавливает воспроизведение звука.
     */
    void unmute();

    /**
     * @brief Переключает состояние приглушения звука.
     */
    void toggleMute();

    /**
     * @brief Рассчитывает реальную громкость с учётом флага mute.
     * @return Эффективная громкость (0, если заглушено, иначе volumePercent_).
     */
    uint32_t effectiveVolume() const;

    /**
     * @brief Эмулирует воспроизведение звукового фрагмента.
     * @param durationMs Длительность в миллисекундах.
     */
    void playSound(uint32_t durationMs);

private:
    std::string deviceName_;
    std::string deviceType_;
    uint32_t volumePercent_;
    bool muted_;
};
