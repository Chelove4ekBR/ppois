#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет объём данных в байтах и поддерживает конвертацию, арифметику и форматирование.
 */
class ByteSize {
public:
    /**
     * @brief Создаёт объект объёма данных.
     * @param bytes Количество байт.
     */
    explicit ByteSize(uint64_t bytes = 0);

    /**
     * @brief Создаёт объект из килобайт (KiB, 1024 байта).
     * @param kib Количество килобайт.
     * @return Объект ByteSize.
     */
    static ByteSize fromKiB(uint64_t kib);

    /**
     * @brief Создаёт объект из мегабайт (MiB, 1024^2 байта).
     * @param mib Количество мегабайт.
     * @return Объект ByteSize.
     */
    static ByteSize fromMiB(uint64_t mib);

    /**
     * @brief Создаёт объект из гигабайт (GiB, 1024^3 байта).
     * @param gib Количество гигабайт.
     * @return Объект ByteSize.
     */
    static ByteSize fromGiB(uint64_t gib);

    /**
     * @brief Возвращает количество байт.
     * @return Значение в байтах.
     */
    uint64_t bytes() const { return bytes_; }

    /**
     * @brief Переводит значение в килобайты.
     * @return Значение в KiB.
     */
    double toKiB() const;

    /**
     * @brief Переводит значение в мегабайты.
     * @return Значение в MiB.
     */
    double toMiB() const;

    /**
     * @brief Переводит значение в гигабайты.
     * @return Значение в GiB.
     */
    double toGiB() const;

    /**
     * @brief Складывает два объёма памяти.
     * @param other Второе слагаемое.
     * @return Суммарный объём.
     */
    ByteSize plus(const ByteSize& other) const;

    /**
     * @brief Вычитает один объём памяти из другого.
     * @param other Вычитаемый объём.
     * @return Разность объёмов.
     */
    ByteSize minus(const ByteSize& other) const;

    /**
     * @brief Умножает объём памяти на множитель.
     * @param factor Множитель.
     * @return Результат умножения.
     */
    ByteSize times(uint32_t factor) const;

    /**
     * @brief Проверяет, строго ли больше текущий объём другого.
     * @param other Сравниваемый объём.
     * @return true, если текущий объём больше.
     */
    bool isGreaterThan(const ByteSize& other) const;

    /**
     * @brief Форматирует объём данных в человекочитаемую строку.
     * @return Строковое представление (например, "16.00 GiB" или "512.00 MiB").
     */
    std::string toString() const;

private:
    uint64_t bytes_;
};
