#include <iostream>
#include <string>
#include "Set.h"

void print_menu()
{
    std::cout << "\n=== МЕНЮ (Теория множеств) ===\n"
              << "1.  Ввести множество A (например, {a, b, c, {a, b}, {}, {a, {c}}})\n"
              << "2.  Ввести множество B\n"
              << "3.  Показать множества A и B\n"
              << "4.  Проверить A на пустоту\n"
              << "5.  Мощность множества A\n"
              << "6.  Добавить элемент в A\n"
              << "7.  Удалить элемент из A\n"
              << "8.  Проверить принадлежность элемента к A (оператор [])\n"
              << "9.  Объединение (A + B)\n"
              << "10. Пересечение (A * B)\n"
              << "11. Разность (A - B)\n"
              << "12. Построить булеан множества A\n"
              << "13. Проверить равенство (A == B)\n"
              << "0.  Выход\n"
              << "Выберите действие: ";
}

int main()
{
    Set A, B;
    int choice;

    while (true)
    {
        print_menu();
        if (!(std::cin >> choice) || choice == 0)
            break;

        std::cin.ignore(10000, '\n'); // Очистка буфера ввода

        switch (choice)
        {
        case 1:
        {
            std::cout << "Введите множество A: ";
            std::string input;
            std::getline(std::cin, input);
            A = Set::parse(input);
            std::cout << "A установлено: " << A << "\n";
            break;
        }
        case 2:
        {
            std::cout << "Введите множество B: ";
            std::string input;
            std::getline(std::cin, input);
            B = Set::parse(input);
            std::cout << "B установлено: " << B << "\n";
            break;
        }
        case 3:
            std::cout << "A = " << A << "\n";
            std::cout << "B = " << B << "\n";
            break;

        case 4:
            if (A.empty())
                std::cout << "Множество A пустое.\n";
            else
                std::cout << "Множество A не пустое.\n";
            break;

        case 5:
            std::cout << "Мощность множества A: " << A.size() << "\n";
            break;

        case 6:
        {
            std::cout << "Введите элемент для добавления (атом 'x' или множество '{x, y}'): ";
            std::string input;
            std::getline(std::cin, input);
            Set::Element elem = Set::parse_element(input);
            if (A.add(elem))
                std::cout << "Элемент добавлен. A = " << A << "\n";
            else
                std::cout << "Элемент уже принадлежит множеству A.\n";
            break;
        }
        case 7:
        {
            std::cout << "Введите элемент для удаления: ";
            std::string input;
            std::getline(std::cin, input);
            Set::Element elem = Set::parse_element(input);
            if (A.remove(elem))
                std::cout << "Элемент удален. A = " << A << "\n";
            else
                std::cout << "Элемент не найден в A.\n";
            break;
        }
        case 8:
        {
            std::cout << "Введите элемент для проверки: ";
            std::string input;
            std::getline(std::cin, input);
            Set::Element elem = Set::parse_element(input);
            if (A[elem])
                std::cout << "Элемент ПРИНАДЛЕЖИТ множеству A.\n";
            else
                std::cout << "Элемент НЕ принадлежит множеству A.\n";
            break;
        }
        case 9:
            std::cout << "A + B = " << (A + B) << "\n";
            break;

        case 10:
            std::cout << "A * B = " << (A * B) << "\n";
            break;

        case 11:
            std::cout << "A - B = " << (A - B) << "\n";
            break;

        case 12:
            std::cout << "Булеан множества A:\n"
                      << A.power_set() << "\n";
            break;

        case 13:
            if (A == B)
                std::cout << "Множества A и B равны.\n";
            else
                std::cout << "Множества A и B НЕ равны.\n";
            break;

        default:
            std::cout << "Неверный пункт меню.\n";
            break;
        }
    }

    std::cout << "Завершение работы.\n";
    return 0;
}