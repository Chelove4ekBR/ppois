#pragma once

#include <string>
#include <vector>

#include "Activity.hpp"
#include "Money.hpp"

/**
 * @brief Описывает один день маршрута, заметки и список запланированных мероприятий.
 */
class ItineraryDay {
public:
    /**
     * @brief Создаёт объект класса ItineraryDay и инициализирует его исходные данные.
     * @param dayNumber порядковый номер дня.
     * @param notes значение параметра «notes».
     */
    ItineraryDay(int dayNumber, std::string notes);
    /**
     * @brief Возвращает числовое представление даты в днях.
     * @return Значение сохранённого атрибута `dayNumber_`.
     */
    int dayNumber() const { return dayNumber_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `notes_`.
     */
    const std::string& notes() const { return notes_; }
    /**
     * @brief Возвращает список мероприятий.
     * @return Ссылка на сохранённый атрибут `activities_`.
     */
    const std::vector<Activity>& activities() const { return activities_; }
    /**
     * @brief Добавляет мероприятие в план дня.
     * @param activity значение параметра «activity».
     */
    void addActivity(const Activity& activity);
    /**
     * @brief Рассчитывает суммарную длительность в минутах.
     * @return Рассчитанное числовое значение.
     */
    int totalMinutes() const;
    /**
     * @brief Рассчитывает общую стоимость.
     * @return Рассчитанная денежная сумма.
     */
    Money totalCost() const;
    /**
     * @brief Проверяет условие «overloaded» для объекта.
     * @return Результат проверки условия.
     */
    bool isOverloaded() const;

private:
    int dayNumber_;
    std::string notes_;
    std::vector<Activity> activities_;
};
