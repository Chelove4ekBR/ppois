#pragma once

#include <cstdint>

/**
 * @brief Представляет корпусный вентилятор охлаждения с ШИМ (PWM) управлением оборотами.
 */
class CaseFan {
public:
    /**
     * @brief Создаёт вентилятор корпуса.
     * @param diameterMm Диаметр крыльчатки в миллиметрах (120, 140).
     * @param maxRpm Максимальная скорость вращения в об/мин.
     * @param maxAirflowCfm Максимальный воздушный поток в кубических футах в минуту (CFM).
     */
    explicit CaseFan(uint32_t diameterMm = 120, uint32_t maxRpm = 1800, double maxAirflowCfm = 65.0);

    /**
     * @brief Возвращает диаметр вентилятора.
     * @return Размер в мм.
     */
    uint32_t diameterMm() const { return diameterMm_; }

    /**
     * @brief Возвращает текущую скорость вращения.
     * @return Обороты в минуту (RPM).
     */
    uint32_t currentRpm() const { return currentRpm_; }

    /**
     * @brief Возвращает максимальные обороты.
     * @return Максимальный RPM.
     */
    uint32_t maxRpm() const { return maxRpm_; }

    /**
     * @brief Возвращает скважность ШИМ (0..100%).
     * @return Процент ШИМ.
     */
    uint32_t pwmDuty() const { return pwmDuty_; }

    /**
     * @brief Возвращает максимальный воздушный поток.
     * @return Поток в CFM.
     */
    double maxAirflowCfm() const { return maxAirflowCfm_; }

    /**
     * @brief Задаёт уровень ШИМ-сигнала (0..100%).
     * @param duty Процент заполнения ШИМ.
     */
    void setPwmDuty(uint32_t duty);

    /**
     * @brief Рассчитывает текущий воздушный поток вентилятора в CFM.
     * @return Производительность в CFM.
     */
    double currentAirflowCfm() const;

    /**
     * @brief Рассчитывает уровень шума вентилятора в децибелах (дБ).
     * @return Шум в dB(A).
     */
    double noiseLevelDb() const;

    /**
     * @brief Останавливает вращение вентилятора.
     */
    void stop();

    /**
     * @brief Проверяет, вращается ли вентилятор.
     * @return true, если обороты больше нуля.
     */
    bool isSpinning() const;

private:
    uint32_t diameterMm_;
    uint32_t currentRpm_;
    uint32_t maxRpm_;
    uint32_t pwmDuty_;
    double maxAirflowCfm_;
};
