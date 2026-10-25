#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "CaseFan.hpp"

/**
 * @brief Представляет корпус системного блока с отсеками для накопителей и вентиляторами.
 */
class ComputerCase {
public:
    /**
     * @brief Создаёт корпус компьютера.
     * @param model Модель корпуса.
     * @param formFactorSupported Поддерживаемый форм-фактор материнских плат.
     * @param driveBaysCount Количество посадочных мест для накопителей 2.5"/3.5".
     * @param maxGpuLengthMm Максимальная допустимая длина видеокарты в мм.
     */
    ComputerCase(std::string model = "Mid-Tower Mesh", std::string formFactorSupported = "ATX",
                 uint32_t driveBaysCount = 4, uint32_t maxGpuLengthMm = 380);

    /**
     * @brief Возвращает модель корпуса.
     * @return Название модели.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает поддерживаемый форм-фактор.
     * @return Форм-фактор ("ATX", "Micro-ATX", "Mini-ITX").
     */
    const std::string& formFactorSupported() const { return formFactorSupported_; }

    /**
     * @brief Возвращает число отсеков под накопители.
     * @return Количество посадочных мест.
     */
    uint32_t driveBaysCount() const { return driveBaysCount_; }

    /**
     * @brief Возвращает максимальную длину видеокарты.
     * @return Длина в мм.
     */
    uint32_t maxGpuLengthMm() const { return maxGpuLengthMm_; }

    /**
     * @brief Возвращает список корпусных вентиляторов.
     * @return Вектор вентиляторов CaseFan.
     */
    const std::vector<CaseFan>& fans() const { return fans_; }

    /**
     * @brief Устанавливает корпусный вентилятор.
     * @param fan Вентилятор CaseFan.
     */
    void installFan(const CaseFan& fan);

    /**
     * @brief Рассчитывает суммарный воздушный поток всех вентиляторов корпуса в CFM.
     * @return Суммарный поток CFM.
     */
    double totalAirflowCfm() const;

    /**
     * @brief Проверяет, поместится ли видеокарта заданной длины в корпус.
     * @param lengthMm Длина видеокарты в мм.
     * @return true, если видеокарта помещается.
     */
    bool canFitGpu(uint32_t lengthMm) const;

    /**
     * @brief Задаёт уровень ШИМ для всех корпусных вентиляторов одновременно.
     * @param duty Процент заполнения ШИМ (0..100).
     */
    void setAllFansPwm(uint32_t duty);

    /**
     * @brief Рассчитывает суммарный уровень шума системы вентиляции корпуса.
     * @return Шум в дБ.
     */
    double averageNoiseDb() const;

private:
    std::string model_;
    std::string formFactorSupported_;
    uint32_t driveBaysCount_;
    uint32_t maxGpuLengthMm_;
    std::vector<CaseFan> fans_;
};
