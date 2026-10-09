#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../include/Matrix.h"
#include <sstream>
#include <fstream>
#include <cstdio>

TEST_CASE("Matrix constructors and basic accessors", "[matrix]")
{
    SECTION("Default constructor")
    {
        Matrix m;
        REQUIRE(m.rows() == 0);
        REQUIRE(m.cols() == 0);
        REQUIRE(m.is_square());
        REQUIRE(m.is_zero());
        REQUIRE(m.is_diagonal());
        REQUIRE(m.is_identity());
    }

    SECTION("Sized constructor")
    {
        Matrix m(2, 3, 5.0);
        REQUIRE(m.rows() == 2);
        REQUIRE(m.cols() == 3);
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j)
                REQUIRE(m(i, j) == 5.0);
    }

    SECTION("Copy constructor and assignment")
    {
        Matrix m1(2, 2, 1.0);
        Matrix m2 = m1;
        REQUIRE(m1 == m2);
        Matrix m3;
        m3 = m1;
        REQUIRE(m1 == m3);
    }
}

TEST_CASE("Matrix element access", "[matrix]")
{
    Matrix m(2, 2);
    m(0, 0) = 1.0;
    m(0, 1) = 2.0;
    m(1, 0) = 3.0;
    m(1, 1) = 4.0;

    REQUIRE(m(0, 0) == 1.0);
    REQUIRE(m(0, 1) == 2.0);
    REQUIRE(m(1, 0) == 3.0);
    REQUIRE(m(1, 1) == 4.0);

    SECTION("Out of range")
    {
        REQUIRE_THROWS_AS(m(2, 0), std::out_of_range);
        REQUIRE_THROWS_AS(m(0, 2), std::out_of_range);
        const Matrix &cm = m;
        REQUIRE_THROWS_AS(cm(2, 0), std::out_of_range);
    }
}

TEST_CASE("Matrix increment and decrement", "[matrix]")
{
    Matrix m(2, 2, 1.0);
    ++m;
    REQUIRE(m(0, 0) == 2.0);
    m++;
    REQUIRE(m(0, 0) == 3.0);
    --m;
    REQUIRE(m(0, 0) == 2.0);
    m--;
    REQUIRE(m(0, 0) == 1.0);

    Matrix old = m++;
    REQUIRE(old(0, 0) == 1.0);
    REQUIRE(m(0, 0) == 2.0);
    old = m--;
    REQUIRE(old(0, 0) == 2.0);
    REQUIRE(m(0, 0) == 1.0);
}

TEST_CASE("Matrix addition and subtraction", "[matrix]")
{
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 2.0);

    SECTION("Matrix + Matrix")
    {
        Matrix c = a + b;
        REQUIRE(c(0, 0) == 3.0);
        a += b;
        REQUIRE(a(0, 0) == 3.0);
    }

    SECTION("Matrix + scalar")
    {
        Matrix c = a + 5.0;
        REQUIRE(c(0, 0) == 6.0);
        a += 5.0;
        REQUIRE(a(0, 0) == 6.0);
    }

    SECTION("Matrix - Matrix")
    {
        Matrix c = b - a;
        REQUIRE(c(0, 0) == 1.0);
        b -= a;
        REQUIRE(b(0, 0) == 1.0);
    }

    SECTION("Matrix - scalar")
    {
        Matrix c = a - 0.5;
        REQUIRE(c(0, 0) == 0.5);
        a -= 0.5;
        REQUIRE(a(0, 0) == 0.5);
    }

    SECTION("Different sizes throw")
    {
        Matrix m1(2, 2), m2(2, 3);
        REQUIRE_THROWS_AS(m1 + m2, std::invalid_argument);
        REQUIRE_THROWS_AS(m1 += m2, std::invalid_argument);
        REQUIRE_THROWS_AS(m1 - m2, std::invalid_argument);
        REQUIRE_THROWS_AS(m1 -= m2, std::invalid_argument);
    }
}

TEST_CASE("Matrix multiplication and division", "[matrix]")
{
    Matrix a(2, 3, 1.0);
    Matrix b(3, 2, 2.0);

    SECTION("Matrix * Matrix")
    {
        Matrix c = a * b;
        REQUIRE(c.rows() == 2);
        REQUIRE(c.cols() == 2);
        REQUIRE(c(0, 0) == 6.0);
    }

    SECTION("Matrix * scalar")
    {
        Matrix c = a * 3.0;
        REQUIRE(c(0, 0) == 3.0);
        a *= 3.0;
        REQUIRE(a(0, 0) == 3.0);
    }

    SECTION("Matrix / scalar")
    {
        Matrix c = a / 2.0;
        REQUIRE(c(0, 0) == 0.5);
        a /= 2.0;
        REQUIRE(a(0, 0) == 0.5);
    }

    SECTION("Division by zero throws")
    {
        REQUIRE_THROWS_AS(a / 0.0, std::domain_error);
        REQUIRE_THROWS_AS(a /= 0.0, std::domain_error);
    }

    SECTION("Invalid dimensions for multiplication")
    {
        Matrix m1(2, 2), m2(3, 3);
        REQUIRE_THROWS_AS(m1 * m2, std::invalid_argument);
    }
}

TEST_CASE("Matrix power", "[matrix]")
{
    Matrix m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    SECTION("pow 0 gives identity")
    {
        Matrix id = m.pow(0);
        REQUIRE(id(0, 0) == 1.0);
        REQUIRE(id(0, 1) == 0.0);
        REQUIRE(id(1, 0) == 0.0);
        REQUIRE(id(1, 1) == 1.0);
    }

    SECTION("pow 1 gives same")
    {
        Matrix p = m.pow(1);
        REQUIRE(p == m);
    }

    SECTION("pow 2")
    {
        Matrix p = m.pow(2);
        REQUIRE(p(0, 0) == 7.0);
        REQUIRE(p(0, 1) == 10.0);
        REQUIRE(p(1, 0) == 15.0);
        REQUIRE(p(1, 1) == 22.0);
    }

    SECTION("operator^")
    {
        Matrix p = m ^ 2;
        REQUIRE(p(0, 0) == 7.0);
        m ^= 2;
        REQUIRE(m(0, 0) == 7.0);
    }

    SECTION("Non-square throws")
    {
        Matrix rect(2, 3);
        REQUIRE_THROWS_AS(rect.pow(2), std::logic_error);
    }

    SECTION("Negative exponent throws")
    {
        REQUIRE_THROWS_AS(m.pow(-1), std::invalid_argument);
    }
}

TEST_CASE("Matrix determinant", "[matrix]")
{
    SECTION("2x2")
    {
        Matrix m(2, 2);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 0) = 3;
        m(1, 1) = 4;
        REQUIRE(m.determinant() == Catch::Approx(-2.0));
    }

    SECTION("2x2 with swap")
    {
        Matrix m(2, 2);
        m(0, 0) = 0;
        m(0, 1) = 1;
        m(1, 0) = 1;
        m(1, 1) = 0;
        REQUIRE(m.determinant() == Catch::Approx(-1.0));
    }

    SECTION("3x3 singular")
    {
        Matrix m(3, 3);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(0, 2) = 3;
        m(1, 0) = 4;
        m(1, 1) = 5;
        m(1, 2) = 6;
        m(2, 0) = 7;
        m(2, 1) = 8;
        m(2, 2) = 9;
        REQUIRE(m.determinant() == Catch::Approx(0.0).margin(1e-9));
    }

    SECTION("3x3 non-singular")
    {
        Matrix m(3, 3);
        m(0, 0) = 2;
        m(0, 1) = 1;
        m(0, 2) = 1;
        m(1, 0) = 1;
        m(1, 1) = 3;
        m(1, 2) = 2;
        m(2, 0) = 1;
        m(2, 1) = 0;
        m(2, 2) = 0;
        REQUIRE(m.determinant() == Catch::Approx(-1.0));
    }

    SECTION("Singular matrix")
    {
        Matrix m(2, 2, 0.0);
        REQUIRE(m.determinant() == 0.0);
    }

    SECTION("Non-square throws")
    {
        Matrix m(2, 3);
        REQUIRE_THROWS_AS(m.determinant(), std::logic_error);
    }
}

TEST_CASE("Matrix norm", "[matrix]")
{
    Matrix m(2, 2);
    m(0, 0) = 3;
    m(0, 1) = 4;
    m(1, 0) = 0;
    m(1, 1) = 0;
    REQUIRE(m.norm() == Catch::Approx(5.0));
}

TEST_CASE("Matrix equality", "[matrix]")
{
    Matrix a(2, 2, 1.0);
    Matrix b(2, 2, 1.0);
    Matrix c(2, 2, 2.0);

    REQUIRE(a == b);
    REQUIRE(a != c);

    Matrix d(2, 3, 1.0);
    REQUIRE(a != d);
}

TEST_CASE("Matrix resize", "[matrix]")
{
    Matrix m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    SECTION("Grow rows and cols")
    {
        m.resize(3, 3);
        REQUIRE(m.rows() == 3);
        REQUIRE(m.cols() == 3);
        REQUIRE(m(0, 0) == 1);
        REQUIRE(m(0, 1) == 2);
        REQUIRE(m(1, 0) == 3);
        REQUIRE(m(1, 1) == 4);
        REQUIRE(m(2, 0) == 0);
        REQUIRE(m(0, 2) == 0);
    }

    SECTION("Shrink rows and cols")
    {
        m.resize(1, 1);
        REQUIRE(m.rows() == 1);
        REQUIRE(m.cols() == 1);
        REQUIRE(m(0, 0) == 1);
    }

    SECTION("Change only rows")
    {
        m.resize(3, 2);
        REQUIRE(m.rows() == 3);
        REQUIRE(m.cols() == 2);
        REQUIRE(m(2, 0) == 0);
    }

    SECTION("Change only cols")
    {
        m.resize(2, 3);
        REQUIRE(m.rows() == 2);
        REQUIRE(m.cols() == 3);
        REQUIRE(m(0, 2) == 0);
    }
}

TEST_CASE("Matrix save to file", "[matrix]")
{
    SECTION("Round-trip save then load")
    {
        const char *filename = "test_matrix_save.txt";

        Matrix original(2, 3);
        original(0, 0) = 1.5;
        original(0, 1) = -2.0;
        original(0, 2) = 3.25;
        original(1, 0) = 0.0;
        original(1, 1) = 4.5;
        original(1, 2) = -0.75;

        original.save_to_file(filename);

        Matrix loaded;
        loaded.load_from_file(filename);

        REQUIRE(loaded.rows() == original.rows());
        REQUIRE(loaded.cols() == original.cols());
        REQUIRE(loaded == original);

        std::remove(filename);
    }

    SECTION("Saved content matches operator<< style")
    {
        const char *filename = "test_matrix_format.txt";

        Matrix m(2, 2);
        m(0, 0) = 1;
        m(0, 1) = 2;
        m(1, 0) = 3;
        m(1, 1) = 4;
        m.save_to_file(filename);

        std::ifstream in(filename);
        REQUIRE(in.good());

        size_t rows = 0, cols = 0;
        in >> rows >> cols;
        REQUIRE(rows == 2);
        REQUIRE(cols == 2);

        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j)
            {
                double value = 0.0;
                in >> value;
                REQUIRE(value == m(i, j));
            }

        in.close();
        std::remove(filename);
    }

    SECTION("Empty matrix saves and loads")
    {
        const char *filename = "test_matrix_empty.txt";

        Matrix empty;
        empty.save_to_file(filename);

        Matrix loaded(1, 1, 7.0);
        loaded.load_from_file(filename);

        REQUIRE(loaded.rows() == 0);
        REQUIRE(loaded.cols() == 0);
        REQUIRE(loaded.is_zero());

        std::remove(filename);
    }

    SECTION("Invalid path throws")
    {
        Matrix m(2, 2, 1.0);
        REQUIRE_THROWS_AS(
            m.save_to_file("/nonexistent_directory/file.txt"),
            std::runtime_error);
    }

    SECTION("Overwrite existing file")
    {
        const char *filename = "test_matrix_overwrite.txt";

        Matrix first(1, 1, 1.0);
        first.save_to_file(filename);

        Matrix second(2, 2, 9.0);
        second.save_to_file(filename);

        Matrix loaded;
        loaded.load_from_file(filename);

        REQUIRE(loaded.rows() == 2);
        REQUIRE(loaded.cols() == 2);
        REQUIRE(loaded(0, 0) == 9.0);
        REQUIRE(loaded(1, 1) == 9.0);

        std::remove(filename);
    }

    SECTION("Save after modifications reflects new values")
    {
        const char *filename = "test_matrix_modify.txt";

        Matrix m(2, 2, 1.0);
        ++m;
        m(0, 0) = 42.0;
        m.save_to_file(filename);

        Matrix loaded;
        loaded.load_from_file(filename);

        REQUIRE(loaded(0, 0) == 42.0);
        REQUIRE(loaded(0, 1) == 2.0);
        REQUIRE(loaded(1, 0) == 2.0);
        REQUIRE(loaded(1, 1) == 2.0);

        std::remove(filename);
    }
}

TEST_CASE("Matrix submatrix", "[matrix]")
{
    Matrix m(3, 3);
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            m(i, j) = static_cast<double>(i * 3 + j);

    SECTION("Valid submatrix")
    {
        Matrix sub = m.submatrix(0, 0, 2, 2);
        REQUIRE(sub.rows() == 2);
        REQUIRE(sub.cols() == 2);
        REQUIRE(sub(0, 0) == 0);
        REQUIRE(sub(0, 1) == 1);
        REQUIRE(sub(1, 0) == 3);
        REQUIRE(sub(1, 1) == 4);
    }

    SECTION("Out of range")
    {
        REQUIRE_THROWS_AS(m.submatrix(2, 2, 2, 2), std::out_of_range);
        REQUIRE_THROWS_AS(m.submatrix(0, 0, 4, 2), std::out_of_range);
        REQUIRE_THROWS_AS(m.submatrix(0, 0, 2, 4), std::out_of_range);
    }
}

TEST_CASE("Matrix transpose", "[matrix]")
{
    Matrix m(2, 3);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(1, 0) = 4;
    m(1, 1) = 5;
    m(1, 2) = 6;

    Matrix t = m.transpose();
    REQUIRE(t.rows() == 3);
    REQUIRE(t.cols() == 2);
    REQUIRE(t(0, 0) == 1);
    REQUIRE(t(1, 0) == 2);
    REQUIRE(t(2, 0) == 3);
    REQUIRE(t(0, 1) == 4);
    REQUIRE(t(1, 1) == 5);
    REQUIRE(t(2, 1) == 6);
}

TEST_CASE("Matrix type checks", "[matrix]")
{
    SECTION("is_square")
    {
        Matrix sq(2, 2);
        Matrix rect(2, 3);
        REQUIRE(sq.is_square());
        REQUIRE_FALSE(rect.is_square());
    }

    SECTION("is_diagonal")
    {
        Matrix diag(2, 2);
        diag(0, 0) = 1;
        diag(1, 1) = 2;
        REQUIRE(diag.is_diagonal());

        Matrix non_diag(2, 2, 1.0);
        REQUIRE_FALSE(non_diag.is_diagonal());

        Matrix rect(2, 3);
        REQUIRE_FALSE(rect.is_diagonal());
    }

    SECTION("is_zero")
    {
        Matrix zero(2, 2);
        REQUIRE(zero.is_zero());

        Matrix non_zero(2, 2, 1.0);
        REQUIRE_FALSE(non_zero.is_zero());

        Matrix almost_zero(2, 2, 1e-10);
        REQUIRE(almost_zero.is_zero());
    }

    SECTION("is_identity")
    {
        Matrix id(2, 2);
        id(0, 0) = 1;
        id(1, 1) = 1;
        REQUIRE(id.is_identity());

        Matrix non_id(2, 2, 1.0);
        REQUIRE_FALSE(non_id.is_identity());

        Matrix diag(2, 2);
        diag(0, 0) = 1;
        diag(1, 1) = 2;
        REQUIRE_FALSE(diag.is_identity());

        Matrix rect(2, 3);
        REQUIRE_FALSE(rect.is_identity());
    }

    SECTION("is_symmetric")
    {
        Matrix sym(2, 2);
        sym(0, 0) = 1;
        sym(0, 1) = 2;
        sym(1, 0) = 2;
        sym(1, 1) = 3;
        REQUIRE(sym.is_symmetric());

        Matrix non_sym(2, 2);
        non_sym(0, 0) = 1;
        non_sym(0, 1) = 2;
        non_sym(1, 0) = 3;
        non_sym(1, 1) = 4;
        REQUIRE_FALSE(non_sym.is_symmetric());

        Matrix rect(2, 3);
        REQUIRE_FALSE(rect.is_symmetric());
    }

    SECTION("is_upper_triangular")
    {
        Matrix upper(2, 2);
        upper(0, 0) = 1;
        upper(0, 1) = 2;
        upper(1, 0) = 0;
        upper(1, 1) = 3;
        REQUIRE(upper.is_upper_triangular());

        Matrix non_upper(2, 2, 1.0);
        REQUIRE_FALSE(non_upper.is_upper_triangular());

        Matrix rect(2, 3);
        REQUIRE_FALSE(rect.is_upper_triangular());
    }

    SECTION("is_lower_triangular")
    {
        Matrix lower(2, 2);
        lower(0, 0) = 1;
        lower(0, 1) = 0;
        lower(1, 0) = 2;
        lower(1, 1) = 3;
        REQUIRE(lower.is_lower_triangular());

        Matrix non_lower(2, 2, 1.0);
        REQUIRE_FALSE(non_lower.is_lower_triangular());

        Matrix rect(2, 3);
        REQUIRE_FALSE(rect.is_lower_triangular());
    }
}

TEST_CASE("Matrix stream operators", "[matrix]")
{
    Matrix m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    SECTION("Output")
    {
        std::ostringstream oss;
        oss << m;
        REQUIRE(oss.str() == "1 2\n3 4");
    }

    SECTION("Input")
    {
        std::istringstream iss("2 2\n1 2\n3 4");
        Matrix m2;
        iss >> m2;
        REQUIRE(m2.rows() == 2);
        REQUIRE(m2.cols() == 2);
        REQUIRE(m2(0, 0) == 1);
        REQUIRE(m2(1, 1) == 4);
    }

    SECTION("Input failure does not modify matrix")
    {
        Matrix m2(1, 1, 9.0);
        std::istringstream iss("2 2\n1 2\n3");
        iss >> m2;
        REQUIRE(m2(0, 0) == 9.0);
    }
}