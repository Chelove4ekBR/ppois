#pragma once

#include "Customer.hpp"
#include "DateRange.hpp"
#include "Hotel.hpp"
#include "MealPlan.hpp"
#include "Money.hpp"
#include "Room.hpp"

/**
 * @brief Представляет бронирование номера в отеле, включая гостя, период проживания и план питания.
 */
class HotelReservation {
public:
    /**
     * @brief Создаёт объект класса HotelReservation и инициализирует его исходные данные.
     * @param id идентификатор объекта.
     * @param hotel отель.
     * @param room номер отеля.
     * @param guest гость.
     * @param period период дат.
     * @param mealPlan значение параметра «meal plan».
     * @param guestCount количество гостей.
     */
    HotelReservation(int id, HotelPtr hotel, RoomPtr room, CustomerPtr guest, DateRange period, MealPlan mealPlan,
                     int guestCount);
    /**
     * @brief Возвращает идентификатор объекта.
     * @return Значение сохранённого атрибута `id_`.
     */
    int id() const { return id_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `hotel_`.
     */
    const HotelPtr& hotel() const { return hotel_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `room_`.
     */
    const RoomPtr& room() const { return room_; }
    /**
     * @brief Возвращает гостя, для которого оформлено бронирование.
     * @return Ссылка на сохранённый атрибут `guest_`.
     */
    const CustomerPtr& guest() const { return guest_; }
    /**
     * @brief Возвращает период, связанный с объектом.
     * @return Ссылка на сохранённый атрибут `period_`.
     */
    const DateRange& period() const { return period_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `mealPlan_`.
     */
    const MealPlan& mealPlan() const { return mealPlan_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `guestCount_`.
     */
    int guestCount() const { return guestCount_; }
    /**
     * @brief Возвращает количество ночей в бронировании.
     * @return Рассчитанное числовое значение.
     */
    int nights() const;
    /**
     * @brief Рассчитывает общую стоимость.
     * @return Рассчитанная денежная сумма.
     */
    Money totalCost() const;
    /**
     * @brief Отменяет объект или связанную с ним операцию.
     */
    void cancel();

private:
    int id_;
    HotelPtr hotel_;
    RoomPtr room_;
    CustomerPtr guest_;
    DateRange period_;
    MealPlan mealPlan_;
    int guestCount_;
};
