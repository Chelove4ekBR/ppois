#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет блок питания компьютера (PSU), распределение мощности и контроль перегрузки.
 */
class PowerSupplyUnit {
public:
    /**
     * @brief Создаёт блок питания.
     * @param model Модель блока питания.
     * @param ratedWattage Номинальная выходная мощность в ваттах.
     * @param efficiencyRating Сертификат энергоэффективности (например, "80 PLUS Gold").
     * @param isModular Признак модульной конструкции кабелей.
     */
    PowerSupplyUnit(std::string model = "Generic PSU", uint32_t ratedWattage = 650,
                    std::string efficiencyRating = "80 PLUS Gold", bool isModular = true);

    /**
     * @brief Возвращает название модели.
     * @return Модель блока питания.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает номинальную мощность.
     * @return Мощность в ваттах.
     */
    uint32_t ratedWattage() const { return ratedWattage_; }

    /**
     * @brief Возвращает класс сертификации.
     * @return Строка сертификации.
     */
    const std::string& efficiencyRating() const { return efficiencyRating_; }

    /**
     * @brief Возвращает текущее суммарное энергопотребление.
     * @return Потребляемая мощность в ваттах.
     */
    uint32_t currentDrawWatt() const { return currentDrawWatt_; }

    /**
     * @brief Признак модульности кабелей.
     * @return true, если блок питания модульный.
     */
    bool isModular() const { return isModular_; }

    /**
     * @brief Запрашивает дополнительную мощность питания для компонента.
     * @param watts Мощность в ваттах.
     */
    void drawPower(uint32_t watts);

    /**
     * @brief Освобождает ранее занятую мощность питания.
     * @param watts Освобождаемая мощность в ваттах.
     */
    void releasePower(uint32_t watts);

    /**
     * @brief Проверяет, перегружен ли блок питания.
     * @return true, если текущая нагрузка превышает номинал.
     */
    bool isOverloaded() const;

    /**
     * @brief Рассчитывает оставшийся резерв мощности.
     * @return Свободная мощность в ваттах.
     */
    uint32_t remainingCapacityWatt() const;

    /**
     * @brief Возвращает процент текущей нагрузки на блок питания.
     * @return Процент загрузки (0..100+).
     */
    double loadPercentage() const;

    /**
     * @brief Рассчитывает коэффициент полезного действия (КПД) при текущей нагрузке.
     * @return КПД в процентах.
     */
    double calculateEfficiency() const;

private:
    std::string model_;
    uint32_t ratedWattage_;
    std::string efficiencyRating_;
    uint32_t currentDrawWatt_;
    bool isModular_;
};
