#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет клавиатуру как периферийное устройство ввода.
 */
class Keyboard {
public:
    /**
     * @brief Создаёт клавиатуру.
     * @param model Модель клавиатуры.
     * @param layout Раскладка клавиш ("QWERTY", "JCUKEN").
     * @param isMechanical Механический тип переключателей.
     * @param backlightActive Активность подсветки RGB.
     */
    Keyboard(std::string model = "Mechanical Keyboard", std::string layout = "QWERTY",
             bool isMechanical = true, bool backlightActive = true);

    /**
     * @brief Возвращает модель клавиатуры.
     * @return Модель.
     */
    const std::string& model() const { return model_; }

    /**
     * @brief Возвращает текущую раскладку.
     * @return Код раскладки.
     */
    const std::string& layout() const { return layout_; }

    /**
     * @brief Проверяет механический тип переключателей.
     * @return true, если механическая.
     */
    bool isMechanical() const { return isMechanical_; }

    /**
     * @brief Проверяет включена ли подсветка.
     * @return true, если подсветка работает.
     */
    bool isBacklightActive() const { return backlightActive_; }

    /**
     * @brief Возвращает счётчик нажатий клавиш.
     * @return Количество нажатий.
     */
    uint32_t pressedKeysCount() const { return pressedKeysCount_; }

    /**
     * @brief Эмулирует нажатие клавиши.
     * @param key Название клавиши.
     */
    void pressKey(const std::string& key);

    /**
     * @brief Переключает язык/раскладку клавиатуры.
     * @param newLayout Новая раскладка.
     */
    void switchLayout(std::string newLayout);

    /**
     * @brief Переключает состояние подсветки клавиатуры.
     */
    void toggleBacklight();

    /**
     * @brief Сбрасывает счётчик нажатий клавиш.
     */
    void resetKeys();

    /**
     * @brief Возвращает краткое описание устройства.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    std::string model_;
    std::string layout_;
    bool isMechanical_;
    bool backlightActive_;
    uint32_t pressedKeysCount_;
};
