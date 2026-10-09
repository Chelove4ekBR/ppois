#include "../include/Matrix.h"

Matrix::Matrix(size_t rows, size_t cols, double value)
    : rows_count(rows), cols_count(cols),
      data(rows, std::vector<double>(cols, value)) {}

void Matrix::check_square() const
{
    if (rows_count != cols_count)
        throw std::logic_error("Операция определена только для квадратных матриц.");
}

void Matrix::check_same_size(const Matrix &other) const
{
    if (rows_count != other.rows_count || cols_count != other.cols_count)
        throw std::invalid_argument("Размеры матриц не совпадают.");
}

bool Matrix::is_zero_value(double value)
{
    return std::fabs(value) < EPSILON;
}

size_t Matrix::rows() const { return rows_count; }
size_t Matrix::cols() const { return cols_count; }

double &Matrix::operator()(size_t row, size_t col)
{
    if (row >= rows_count || col >= cols_count)
        throw std::out_of_range("Индекс выходит за границы матрицы.");
    return data[row][col];
}

double Matrix::operator()(size_t row, size_t col) const
{
    if (row >= rows_count || col >= cols_count)
        throw std::out_of_range("Индекс выходит за границы матрицы.");
    return data[row][col];
}

Matrix &Matrix::operator++()
{
    *this += 1.0;
    return *this;
}

Matrix Matrix::operator++(int)
{
    Matrix old = *this;
    *this += 1.0;
    return old;
}

Matrix &Matrix::operator--()
{
    *this -= 1.0;
    return *this;
}

Matrix Matrix::operator--(int)
{
    Matrix old = *this;
    *this -= 1.0;
    return old;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    Matrix result = *this;
    result += other;
    return result;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    check_same_size(other);
    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < cols_count; j++)
            data[i][j] += other.data[i][j];
    return *this;
}

Matrix Matrix::operator+(double number) const
{
    Matrix result = *this;
    result += number;
    return result;
}

Matrix &Matrix::operator+=(double number)
{
    for (auto &row : data)
        for (auto &value : row)
            value += number;
    return *this;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    Matrix result = *this;
    result -= other;
    return result;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    check_same_size(other);
    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < cols_count; j++)
            data[i][j] -= other.data[i][j];
    return *this;
}

Matrix Matrix::operator-(double number) const
{
    Matrix result = *this;
    result -= number;
    return result;
}

Matrix &Matrix::operator-=(double number) { return *this += -number; }

Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols_count != other.rows_count)
        throw std::invalid_argument(
            "Число столбцов левой матрицы не равно числу строк правой.");

    Matrix result(rows_count, other.cols_count);
    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < other.cols_count; j++)
            for (size_t k = 0; k < cols_count; k++)
                result.data[i][j] += data[i][k] * other.data[k][j];
    return result;
}

Matrix Matrix::operator*(double number) const
{
    Matrix result = *this;
    result *= number;
    return result;
}

Matrix &Matrix::operator*=(double number)
{
    for (auto &row : data)
        for (auto &value : row)
            value *= number;
    return *this;
}

Matrix Matrix::operator/(double number) const
{
    Matrix result = *this;
    result /= number;
    return result;
}

Matrix &Matrix::operator/=(double number)
{
    if (std::fabs(number) < EPSILON)
        throw std::domain_error("Деление матрицы на ноль.");

    for (auto &row : data)
        for (auto &value : row)
            value /= number;
    return *this;
}

Matrix Matrix::pow(int exponent) const
{
    check_square();
    if (exponent < 0)
        throw std::invalid_argument("Показатель степени должен быть неотрицательным.");

    Matrix result(rows_count, cols_count);
    for (size_t i = 0; i < rows_count; i++)
        result.data[i][i] = 1.0;

    for (int step = 0; step < exponent; step++)
        result = result * (*this);
    return result;
}

Matrix Matrix::operator^(int exponent) const { return pow(exponent); }

Matrix &Matrix::operator^=(int exponent)
{
    *this = pow(exponent);
    return *this;
}

double Matrix::determinant() const
{
    check_square();

    std::vector<std::vector<double>> work = data;
    double result = 1.0;

    for (size_t col = 0; col < rows_count; col++)
    {
        size_t pivot = col;
        for (size_t row = col + 1; row < rows_count; row++)
            if (std::fabs(work[row][col]) > std::fabs(work[pivot][col]))
                pivot = row;

        if (std::fabs(work[pivot][col]) < EPSILON)
            return 0.0;

        if (pivot != col)
        {
            std::swap(work[pivot], work[col]);
            result = -result;
        }

        result *= work[col][col];
        for (size_t row = col + 1; row < rows_count; row++)
        {
            double factor = work[row][col] / work[col][col];
            for (size_t j = col; j < rows_count; j++)
                work[row][j] -= factor * work[col][j];
        }
    }
    return result;
}

double Matrix::norm() const
{
    double sum = 0.0;
    for (const auto &row : data)
        for (double value : row)
            sum += value * value;
    return std::sqrt(sum);
}

bool Matrix::operator==(const Matrix &other) const
{
    if (rows_count != other.rows_count || cols_count != other.cols_count)
        return false;

    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < cols_count; j++)
            if (std::fabs(data[i][j] - other.data[i][j]) >= EPSILON)
                return false;
    return true;
}

bool Matrix::operator!=(const Matrix &other) const { return !(*this == other); }

void Matrix::resize(size_t new_rows, size_t new_cols)
{
    data.resize(new_rows);
    for (auto &row : data)
        row.resize(new_cols, 0.0);

    rows_count = new_rows;
    cols_count = new_cols;
}

void Matrix::load_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Не удалось открыть файл: " + filename);

    Matrix loaded;
    if (!(file >> loaded))
        throw std::invalid_argument("Неверный формат данных в файле.");

    *this = loaded;
}

void Matrix::save_to_file(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);

    file << rows_count << " " << cols_count << "\n";
    for (size_t i = 0; i < rows_count; i++)
    {
        for (size_t j = 0; j < cols_count; j++)
        {
            if (j > 0)
                file << " ";
            file << data[i][j];
        }
        file << "\n";
    }

    if (!file)
        throw std::runtime_error("Ошибка записи в файл: " + filename);
}

Matrix Matrix::submatrix(size_t first_row, size_t first_col,
                         size_t sub_rows, size_t sub_cols) const
{
    if (first_row + sub_rows > rows_count || first_col + sub_cols > cols_count)
        throw std::out_of_range("Подматрица выходит за границы матрицы.");

    Matrix result(sub_rows, sub_cols);
    for (size_t i = 0; i < sub_rows; i++)
        for (size_t j = 0; j < sub_cols; j++)
            result.data[i][j] = data[first_row + i][first_col + j];
    return result;
}

Matrix Matrix::transpose() const
{
    Matrix result(cols_count, rows_count);
    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < cols_count; j++)
            result.data[j][i] = data[i][j];
    return result;
}

bool Matrix::is_square() const { return rows_count == cols_count; }

bool Matrix::is_diagonal() const
{
    if (!is_square())
        return false;

    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = 0; j < cols_count; j++)
            if (i != j && !is_zero_value(data[i][j]))
                return false;
    return true;
}

bool Matrix::is_zero() const
{
    for (const auto &row : data)
        for (double value : row)
            if (!is_zero_value(value))
                return false;
    return true;
}

bool Matrix::is_identity() const
{
    if (!is_diagonal())
        return false;

    for (size_t i = 0; i < rows_count; i++)
        if (std::fabs(data[i][i] - 1.0) >= EPSILON)
            return false;
    return true;
}

bool Matrix::is_symmetric() const
{
    if (!is_square())
        return false;

    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = i + 1; j < cols_count; j++)
            if (std::fabs(data[i][j] - data[j][i]) >= EPSILON)
                return false;
    return true;
}

bool Matrix::is_upper_triangular() const
{
    if (!is_square())
        return false;

    for (size_t i = 1; i < rows_count; i++)
        for (size_t j = 0; j < i; j++)
            if (!is_zero_value(data[i][j]))
                return false;
    return true;
}

bool Matrix::is_lower_triangular() const
{
    if (!is_square())
        return false;

    for (size_t i = 0; i < rows_count; i++)
        for (size_t j = i + 1; j < cols_count; j++)
            if (!is_zero_value(data[i][j]))
                return false;
    return true;
}

std::ostream &operator<<(std::ostream &out, const Matrix &m)
{
    for (size_t i = 0; i < m.rows_count; i++)
    {
        for (size_t j = 0; j < m.cols_count; j++)
        {
            if (j > 0)
                out << " ";
            out << m.data[i][j];
        }
        if (i + 1 < m.rows_count)
            out << "\n";
    }
    return out;
}

std::istream &operator>>(std::istream &in, Matrix &m)
{
    size_t rows = 0, cols = 0;
    if (!(in >> rows >> cols))
        return in;

    Matrix loaded(rows, cols);
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            if (!(in >> loaded.data[i][j]))
                return in;

    m = loaded;
    return in;
}