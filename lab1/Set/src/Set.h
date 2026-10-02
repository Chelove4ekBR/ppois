#pragma once

#include <iostream>
#include <string>
#include <vector>

class Set
{
public:
    class Element
    {
    public:
        bool is_set;
        std::string atom;
        std::vector<Element> children;

        Element();
        Element(const std::string &atom_val);
        Element(const std::vector<Element> &children_val);

        bool operator==(const Element &other) const;
        bool operator!=(const Element &other) const;
        bool operator<(const Element &other) const;

        std::string to_string() const;
    };

private:
    std::vector<Element> elements;

    void normalize();

    static Element parse_element_recursive(const std::string &str, size_t &pos);

public:
    Set();
    Set(const std::vector<Element> &elems);
    Set(const Set &other) = default;
    Set &operator=(const Set &other) = default;
    ~Set() = default;

    bool empty() const;
    int size() const;
    void clear();

    bool contains(const Element &elem) const;
    bool add(const Element &elem);
    bool remove(const Element &elem);

    bool operator[](const Element &elem) const;

    Set operator+(const Set &other) const;
    Set &operator+=(const Set &other);
    Set &operator+=(const Element &elem);

    Set operator*(const Set &other) const;
    Set &operator*=(const Set &other);

    Set operator-(const Set &other) const;
    Set &operator-=(const Set &other);
    Set &operator-=(const Element &elem);

    Set power_set() const;

    bool operator==(const Set &other) const;
    bool operator!=(const Set &other) const;

    std::string to_string() const;
    static Set parse(const std::string &str);
    static Element parse_element(const std::string &str);

    friend std::ostream &operator<<(std::ostream &out, const Set &s);
    friend std::istream &operator>>(std::istream &in, Set &s);
};
