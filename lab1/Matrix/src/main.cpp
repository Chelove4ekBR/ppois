#include <iostream>
#include <string>
#include "../include/Matrix.h"

void print_menu()
{
    std::cout << "1.  Ввести матрицу A\n"
              << "2.  Ввести матрицу B\n"
              << "3.  Показать матрицы A и B\n"
              << "4.  Префиксный инкремент (++A)\n"
              << "5.  Постфиксный инкремент (A++)\n"
              << "6.  Префиксный декремент (--A)\n"
              << "7.  Постфиксный декремент (A--)\n"
              << "8.  Сложение матриц (A + B)\n"
              << "9.  Вычитание матриц (A - B)\n"
              << "10. Произведение матриц (A * B)\n"
              << "11. Сложение матрицы с числом (A + число)\n"
              << "12. Вычитание числа из матрицы (A - число)\n"
              << "13. Умножение матрицы на число (A * число)\n"
              << "14. Деление матрицы на число (A / число)\n"
              << "15. Возведение матрицы A в степень (A ^ n)\n"
              << "16. Детерминант матрицы A\n"
              << "17. Норма матрицы A\n"
              << "18. Проверить равенство (A == B)\n"
              << "19. Изменить размер матрицы A\n"
              << "20. Загрузить матрицу A из файла\n"
              << "21. Сохранить матрицу A в файл\n"
              << "22. Извлечь подматрицу из A\n"
              << "23. Транспонировать матрицу A\n"
              << "24. Определить тип матрицы A\n"
              << "0.  Выход\n"
              << "Выберите действие: ";
}

Matrix read_matrix(const std::string &name)
{
    std::cout << "Введите матрицу " << name << " (строки, столбцы, элементы): ";
    Matrix loaded;
    if (!(std::cin >> loaded))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("Неверный формат матрицы.");
    }
    std::cin.ignore(10000, '\n');
    return loaded;
}

double read_number()
{
    std::cout << "Введите число: ";
    double number;
    if (!(std::cin >> number))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("Введено не число.");
    }
    std::cin.ignore(10000, '\n');
    return number;
}

size_t read_size(const std::string &prompt)
{
    std::cout << prompt;
    long long value;
    if (!(std::cin >> value) || value < 0)
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("Нужно ввести неотрицательное целое число.");
    }
    std::cin.ignore(10000, '\n');
    return static_cast<size_t>(value);
}

std::string yes_no(bool value)
{
    return value ? "да" : "нет";
}

void print_matrix_type(const Matrix &matrix)
{
    std::cout << "Квадратная: " << yes_no(matrix.is_square()) << "\n"
              << "Диагональная: " << yes_no(matrix.is_diagonal()) << "\n"
              << "Нулевая: " << yes_no(matrix.is_zero()) << "\n"
              << "Единичная: " << yes_no(matrix.is_identity()) << "\n"
              << "Симметрическая: " << yes_no(matrix.is_symmetric()) << "\n"
              << "Верхняя треугольная: " << yes_no(matrix.is_upper_triangular()) << "\n"
              << "Нижняя треугольная: " << yes_no(matrix.is_lower_triangular()) << "\n";
}

int main()
{
    Matrix A, B;
    int choice;

    while (true)
    {
        print_menu();
        if (!(std::cin >> choice) || choice == 0)
            break;

        std::cin.ignore(10000, '\n');

        try
        {
            switch (choice)
            {
            case 1:
                A = read_matrix("A");
                std::cout << "A установлена:\n"
                          << A << "\n";
                break;

            case 2:
                B = read_matrix("B");
                std::cout << "B установлена:\n"
                          << B << "\n";
                break;

            case 3:
                std::cout << "A =\n"
                          << A << "\n";
                std::cout << "B =\n"
                          << B << "\n";
                break;

            case 4:
                std::cout << "++A =\n"
                          << ++A << "\n";
                break;

            case 5:
                std::cout << "A++ вернул:\n"
                          << A++ << "\n";
                std::cout << "A стала:\n"
                          << A << "\n";
                break;

            case 6:
                std::cout << "--A =\n"
                          << --A << "\n";
                break;

            case 7:
                std::cout << "A-- вернул:\n"
                          << A-- << "\n";
                std::cout << "A стала:\n"
                          << A << "\n";
                break;

            case 8:
                std::cout << "A + B =\n"
                          << (A + B) << "\n";
                break;

            case 9:
                std::cout << "A - B =\n"
                          << (A - B) << "\n";
                break;

            case 10:
                std::cout << "A * B =\n"
                          << (A * B) << "\n";
                break;

            case 11:
                std::cout << "A + число =\n"
                          << (A + read_number()) << "\n";
                break;

            case 12:
                std::cout << "A - число =\n"
                          << (A - read_number()) << "\n";
                break;

            case 13:
                std::cout << "A * число =\n"
                          << (A * read_number()) << "\n";
                break;

            case 14:
                std::cout << "A / число =\n"
                          << (A / read_number()) << "\n";
                break;

            case 15:
            {
                int exponent = static_cast<int>(read_size("Введите степень (целое число >= 0): "));
                std::cout << "A ^ n =\n"
                          << (A ^ exponent) << "\n";
                break;
            }

            case 16:
                std::cout << "det(A) = " << A.determinant() << "\n";
                break;

            case 17:
                std::cout << "||A|| = " << A.norm() << "\n";
                break;

            case 18:
                if (A == B)
                    std::cout << "Матрицы A и B равны.\n";
                else
                    std::cout << "Матрицы A и B НЕ равны.\n";
                break;

            case 19:
            {
                size_t new_rows = read_size("Новое число строк: ");
                size_t new_cols = read_size("Новое число столбцов: ");
                A.resize(new_rows, new_cols);
                std::cout << "A после изменения размера:\n"
                          << A << "\n";
                break;
            }

            case 20:
            {
                std::cout << "Введите путь к файлу: ";
                std::string filename;
                std::getline(std::cin, filename);
                A.load_from_file(filename);
                std::cout << "A загружена:\n"
                          << A << "\n";
                break;
            }

            case 21:
            {
                std::cout << "Введите путь к файлу: ";
                std::string filename;
                std::getline(std::cin, filename);
                A.save_to_file(filename);
                std::cout << "Матрица A сохранена в файл: " << filename << "\n";
                break;
            }

            case 22:
            {
                size_t first_row = read_size("Номер первой строки (с 0): ");
                size_t first_col = read_size("Номер первого столбца (с 0): ");
                size_t sub_rows = read_size("Число строк подматрицы: ");
                size_t sub_cols = read_size("Число столбцов подматрицы: ");
                Matrix sub = A.submatrix(first_row, first_col, sub_rows, sub_cols);
                std::cout << "Подматрица:\n"
                          << sub << "\n";
                break;
            }

            case 23:
                std::cout << "Транспонированная A:\n"
                          << A.transpose() << "\n";
                break;

            case 24:
                print_matrix_type(A);
                break;

            default:
                std::cout << "Неверный пункт меню.\n";
                break;
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Ошибка: " << error.what() << "\n";
        }
    }

    std::cout << "Завершение работы.\n";
    return 0;
}