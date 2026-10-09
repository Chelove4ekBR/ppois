#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <fstream>

/**
 * @brief Класс, представляющий вещественную матрицу произвольного размера.
 *
 * Элементы хранятся в динамическом двумерном векторе.
 * Поддерживает арифметику с матрицами и числами, инкремент/декремент всех элементов,
 * возведение в степень, вычисление детерминанта и нормы, а также изменение размера,
 * загрузку из файла, извлечение подматрицы, транспонирование и определение типа матрицы.
 */
class Matrix
{
private:
    static constexpr double EPSILON = 1e-9; ///< Погрешность при сравнении вещественных чисел.

    size_t rows_count = 0;                 ///< Число строк.
    size_t cols_count = 0;                 ///< Число столбцов.
    std::vector<std::vector<double>> data; ///< Элементы матрицы.

    /**
     * @brief Проверяет, что матрица квадратная.
     * @throw std::logic_error, если матрица не квадратная.
     */
    void check_square() const;

    /**
     * @brief Проверяет, что размеры матриц совпадают.
     * @throw std::invalid_argument, если размеры различаются.
     */
    void check_same_size(const Matrix &other) const;

    /**
     * @brief Проверяет, что число равно нулю с точностью EPSILON.
     */
    static bool is_zero_value(double value);

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустую матрицу 0x0.
     */
    Matrix() = default;

    /**
     * @brief Создает матрицу заданного размера, заполненную одним значением.
     * @param rows Число строк.
     * @param cols Число столбцов.
     * @param value Значение всех элементов (по умолчанию 0).
     */
    Matrix(size_t rows, size_t cols, double value = 0.0);

    /**
     * @brief Возвращает число строк.
     */
    size_t rows() const;

    /**
     * @brief Возвращает число столбцов.
     */
    size_t cols() const;

    /**
     * @brief Доступ к элементу для чтения и записи.
     * @throw std::out_of_range, если индексы выходят за границы матрицы.
     */
    double &operator()(size_t row, size_t col);

    /**
     * @brief Доступ к элементу только для чтения.
     * @throw std::out_of_range, если индексы выходят за границы матрицы.
     */
    double operator()(size_t row, size_t col) const;

    /**
     * @brief Префиксный инкремент: увеличивает каждый элемент на единицу.
     */
    Matrix &operator++();

    /**
     * @brief Постфиксный инкремент.
     * @return Копия матрицы до изменения.
     */
    Matrix operator++(int);

    /**
     * @brief Префиксный декремент: уменьшает каждый элемент на единицу.
     */
    Matrix &operator--();

    /**
     * @brief Постфиксный декремент.
     * @return Копия матрицы до изменения.
     */
    Matrix operator--(int);

    /**
     * @brief Сложение двух матриц одинакового размера.
     */
    Matrix operator+(const Matrix &other) const;
    Matrix &operator+=(const Matrix &other);

    /**
     * @brief Прибавление числа к каждому элементу матрицы.
     */
    Matrix operator+(double number) const;
    Matrix &operator+=(double number);

    /**
     * @brief Вычитание двух матриц одинакового размера.
     */
    Matrix operator-(const Matrix &other) const;
    Matrix &operator-=(const Matrix &other);

    /**
     * @brief Вычитание числа из каждого элемента матрицы.
     */
    Matrix operator-(double number) const;
    Matrix &operator-=(double number);

    /**
     * @brief Произведение двух матриц.
     * @throw std::invalid_argument, если число столбцов левой матрицы не равно числу строк правой.
     */
    Matrix operator*(const Matrix &other) const;

    /**
     * @brief Умножение каждого элемента матрицы на число.
     */
    Matrix operator*(double number) const;
    Matrix &operator*=(double number);

    /**
     * @brief Деление каждого элемента матрицы на число.
     * @throw std::domain_error при делении на ноль.
     */
    Matrix operator/(double number) const;
    Matrix &operator/=(double number);

    /**
     * @brief Возведение квадратной матрицы в неотрицательную целую степень.
     * @param exponent Показатель степени (0 дает единичную матрицу).
     * @throw std::logic_error, если матрица не квадратная.
     * @throw std::invalid_argument, если показатель отрицательный.
     */
    Matrix pow(int exponent) const;

    /**
     * @brief Возведение в степень (то же, что pow).
     * @note Приоритет ^ ниже, чем у + и *, поэтому выражение нужно брать в скобки.
     */
    Matrix operator^(int exponent) const;
    Matrix &operator^=(int exponent);

    /**
     * @brief Вычисляет детерминант методом Гаусса.
     * @throw std::logic_error, если матрица не квадратная.
     */
    double determinant() const;

    /**
     * @brief Вычисляет норму матрицы (корень из суммы квадратов всех элементов).
     */
    double norm() const;

    /**
     * @brief Проверка равенства: размеры и все элементы совпадают (с точностью EPSILON).
     */
    bool operator==(const Matrix &other) const;

    /**
     * @brief Проверка неравенства матриц.
     */
    bool operator!=(const Matrix &other) const;

    /**
     * @brief Изменяет число строк и столбцов.
     *
     * Существующие элементы, попадающие в новые границы, сохраняются,
     * новые элементы заполняются нулями, лишние отбрасываются.
     * @param new_rows Новое число строк.
     * @param new_cols Новое число столбцов.
     */
    void resize(size_t new_rows, size_t new_cols);

    /**
     * @brief Загружает матрицу из текстового файла.
     *
     * Формат файла: число строк, число столбцов, затем элементы по строкам.
     * При ошибке текущая матрица не изменяется.
     * @param filename Путь к файлу.
     * @throw std::runtime_error, если файл не удалось открыть.
     * @throw std::invalid_argument, если содержимое файла имеет неверный формат.
     */
    void load_from_file(const std::string &filename);

    /**
     * @brief Сохраняет матрицу в текстовый файл.
     *
     * Формат файла совпадает с форматом load_from_file:
     * число строк, число столбцов, затем элементы по строкам.
     * Полученный файл можно прочитать обратно через load_from_file.
     * @param filename Путь к файлу.
     * @throw std::runtime_error, если файл не удалось открыть для записи.
     */
    void save_to_file(const std::string &filename) const;

    /**
     * @brief Извлекает подматрицу заданного размера.
     * @param first_row Номер первой строки подматрицы (с нуля).
     * @param first_col Номер первого столбца подматрицы (с нуля).
     * @param sub_rows Число строк подматрицы.
     * @param sub_cols Число столбцов подматрицы.
     * @throw std::out_of_range, если подматрица выходит за границы матрицы.
     */
    Matrix submatrix(size_t first_row, size_t first_col,
                     size_t sub_rows, size_t sub_cols) const;

    /**
     * @brief Возвращает транспонированную матрицу.
     */
    Matrix transpose() const;

    /**
     * @brief Проверяет, что матрица квадратная.
     */
    bool is_square() const;

    /**
     * @brief Проверяет, что матрица диагональная (квадратная, все элементы вне главной диагонали нулевые).
     */
    bool is_diagonal() const;

    /**
     * @brief Проверяет, что матрица нулевая (все элементы равны нулю).
     */
    bool is_zero() const;

    /**
     * @brief Проверяет, что матрица единичная (диагональная, на диагонали стоят единицы).
     */
    bool is_identity() const;

    /**
     * @brief Проверяет, что матрица симметрическая (квадратная и равна своей транспонированной).
     */
    bool is_symmetric() const;

    /**
     * @brief Проверяет, что матрица верхняя треугольная (квадратная, нулевые элементы под диагональю).
     */
    bool is_upper_triangular() const;

    /**
     * @brief Проверяет, что матрица нижняя треугольная (квадратная, нулевые элементы над диагональю).
     */
    bool is_lower_triangular() const;

    /**
     * @brief Перегрузка оператора вывода: каждая строка матрицы на отдельной строке.
     */
    friend std::ostream &operator<<(std::ostream &out, const Matrix &m);

    /**
     * @brief Перегрузка оператора ввода.
     *
     * Формат: число строк, число столбцов, затем элементы по строкам.
     * При ошибке чтения матрица не изменяется.
     */
    friend std::istream &operator>>(std::istream &in, Matrix &m);
};