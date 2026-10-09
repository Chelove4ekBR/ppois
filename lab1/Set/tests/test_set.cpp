#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../include/Set.h"
#include <sstream>

TEST_CASE("Node: конструкторы, сравнение и преобразование в строку", "[Node]")
{
    Set::Node atom_a("a");
    Set::Node atom_b("b");
    Set::Node atom_a_copy("a");

    SECTION("Атомы: равенство и неравенство")
    {
        REQUIRE(atom_a == atom_a_copy);
        REQUIRE(atom_a != atom_b);
        REQUIRE(atom_a.to_string() == "a");
        REQUIRE_FALSE(atom_a.is_set_node());
        REQUIRE(atom_a.get_atom() == "a");
    }

    SECTION("Вложенные множества: равенство и вывод")
    {
        Set::Node nested1(std::vector<Set::Node>{atom_a, atom_b});
        Set::Node nested2(std::vector<Set::Node>{atom_a, atom_b});
        Set::Node empty_nested(std::vector<Set::Node>{});

        REQUIRE(nested1 == nested2);
        REQUIRE(nested1 != empty_nested);
        REQUIRE(nested1.to_string() == "{a, b}");
        REQUIRE(empty_nested.to_string() == "{}");
        REQUIRE(nested1.is_set_node());
        REQUIRE(nested1.get_children().size() == 2);
        REQUIRE(empty_nested.is_set_node());
        REQUIRE(empty_nested.get_children().empty());
    }

    SECTION("Оператор < (порядок сортировки элементов)")
    {
        Set::Node nested(std::vector<Set::Node>{atom_a});

        REQUIRE(atom_a < nested);
        REQUIRE_FALSE(nested < atom_a);

        REQUIRE(atom_a < atom_b);
        REQUIRE_FALSE(atom_b < atom_a);
    }
}

TEST_CASE("Set: базовые методы и состояние", "[Set]")
{
    Set s;

    SECTION("Пустое множество")
    {
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);
    }

    SECTION("Добавление элементов и изменение размера")
    {
        s.add(Set::Node("x"));
        REQUIRE_FALSE(s.empty());
        REQUIRE(s.size() == 1);
    }

    SECTION("Очистка множества")
    {
        s.add(Set::Node("x"));
        s.clear();
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);
    }
}

TEST_CASE("Set: добавление, удаление и индексация []", "[Set]")
{
    Set s;
    Set::Node elem_a("a");
    Set::Node elem_b("b");

    SECTION("Добавление элементов и предотвращение дубликатов")
    {
        REQUIRE(s.add(elem_a));
        REQUIRE_FALSE(s.add(elem_a));
        REQUIRE(s.size() == 1);

        REQUIRE(s.contains(elem_a));
        REQUIRE(s[elem_a]);
        REQUIRE_FALSE(s.contains(elem_b));
        REQUIRE_FALSE(s[elem_b]);
    }

    SECTION("Удаление элементов")
    {
        s.add(elem_a);
        s.add(elem_b);

        REQUIRE(s.remove(elem_a));
        REQUIRE_FALSE(s.contains(elem_a));
        REQUIRE(s.size() == 1);

        REQUIRE_FALSE(s.remove(elem_a));
    }
}

TEST_CASE("Set: алгебраические операции", "[Set]")
{
    Set A = Set::parse("{a, b, c}");
    Set B = Set::parse("{b, c, d}");

    SECTION("Объединение (+ и +=)")
    {
        Set union_set = A + B;
        REQUIRE(union_set == Set::parse("{a, b, c, d}"));

        Set temp = A;
        temp += B;
        REQUIRE(temp == union_set);

        temp += Set::Node("e");
        REQUIRE(temp.contains(Set::Node("e")));
    }

    SECTION("Пересечение (* и *=)")
    {
        Set intersect_set = A * B;
        REQUIRE(intersect_set == Set::parse("{b, c}"));

        Set temp = A;
        temp *= B;
        REQUIRE(temp == intersect_set);
    }

    SECTION("Разность (- и -=)")
    {
        Set diff_set = A - B;
        REQUIRE(diff_set == Set::parse("{a}"));

        Set temp = A;
        temp -= B;
        REQUIRE(temp == diff_set);

        temp -= Set::Node("a");
        REQUIRE(temp.empty());
    }
}

TEST_CASE("Set: построение булеана (power_set)", "[Set]")
{
    SECTION("Булеан пустого множества")
    {
        Set empty_set;
        Set P = empty_set.power_set();
        REQUIRE(P.size() == 1);
        REQUIRE(P == Set::parse("{{}}"));
    }

    SECTION("Булеан множества из 2 элементов")
    {
        Set s = Set::parse("{a, b}");
        Set P = s.power_set();

        REQUIRE(P.size() == 4);
        REQUIRE(P.contains(Set::parse_Node("{}")));
        REQUIRE(P.contains(Set::parse_Node("{a}")));
        REQUIRE(P.contains(Set::parse_Node("{b}")));
        REQUIRE(P.contains(Set::parse_Node("{a, b}")));
    }
}

TEST_CASE("Set: парсинг и нормализация", "[Set]")
{
    SECTION("Сложное множество из условий лабораторной работы")
    {
        std::string str = "{a, b, c, {a, b}, {}, {a, {c}}}";
        Set s = Set::parse(str);

        REQUIRE(s.size() == 6);
        REQUIRE(s.to_string() == "{a, b, c, {}, {a, b}, {a, {c}}}");
    }

    SECTION("Обработка пробелов при парсинге")
    {
        Set s = Set::parse("{   x ,   { y,  z }  }");
        REQUIRE(s.size() == 2);
        REQUIRE(s.contains(Set::Node("x")));
        REQUIRE(s.contains(Set::parse_Node("{y, z}")));
    }

    SECTION("Парсинг отдельных элементов (parse_Node)")
    {
        Set::Node elem_atom = Set::parse_Node("  val  ");
        REQUIRE_FALSE(elem_atom.is_set_node());
        REQUIRE(elem_atom.get_atom() == "val");

        Set::Node elem_set = Set::parse_Node(" {1, 2} ");
        REQUIRE(elem_set.is_set_node());
        REQUIRE(elem_set.get_children().size() == 2);
    }
}

TEST_CASE("Set: работа с потоками ввода-вывода", "[Set]")
{
    SECTION("Запись в поток (operator<<)")
    {
        Set s = Set::parse("{a, b}");
        std::stringstream ss;
        ss << s;
        REQUIRE(ss.str() == "{a, b}");
    }

    SECTION("Чтение из потока (operator>>)")
    {
        std::stringstream ss("{x, y, {z}}");
        Set s;
        ss >> s;

        REQUIRE(s.size() == 3);
        REQUIRE(s.contains(Set::Node("x")));
        REQUIRE(s.contains(Set::parse_Node("{z}")));
    }
}