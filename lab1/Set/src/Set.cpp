#include "Set.h"

Set::Element::Element()
    : is_set(false) {}

Set::Element::Element(const std::string &atom_val)
    : is_set(false), atom(atom_val) {}

Set::Element::Element(const std::vector<Element> &children_val)
    : is_set(true), children(children_val) {}

bool Set::Element::operator==(const Element &other) const
{
    if (is_set != other.is_set)
        return false;
    if (!is_set)
        return atom == other.atom;
    return children == other.children;
}

bool Set::Element::operator!=(const Element &other) const
{
    return !(*this == other);
}

bool Set::Element::operator<(const Element &other) const
{

    if (is_set != other.is_set)
        return !is_set && other.is_set;

    if (!is_set)
        return atom < other.atom;

    return children < other.children;
}

std::string Set::Element::to_string() const
{
    if (!is_set)
        return atom;

    std::string res = "{";
    for (size_t i = 0; i < children.size(); i++)
    {
        if (i > 0)
            res += ", ";
        res += children[i].to_string();
    }
    res += "}";
    return res;
}

Set::Set() {}

Set::Set(const std::vector<Element> &elems)
    : elements(elems)
{
    normalize();
}

void Set::normalize()
{

    for (auto &item : elements)
    {
        if (item.is_set)
        {
            Set temp(item.children);
            temp.normalize();
            item.children = temp.elements;
        }
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        for (size_t j = i + 1; j < elements.size(); j++)
        {
            if (elements[j] < elements[i])
            {
                Element temp = elements[i];
                elements[i] = elements[j];
                elements[j] = temp;
            }
        }
    }

    std::vector<Element> unique_elems;
    for (const auto &item : elements)
    {
        if (unique_elems.empty() || unique_elems.back() != item)
        {
            unique_elems.push_back(item);
        }
    }
    elements = unique_elems;
}

bool Set::empty() const
{
    return elements.empty();
}

int Set::size() const
{
    return static_cast<int>(elements.size());
}

void Set::clear()
{
    elements.clear();
}

bool Set::contains(const Element &elem) const
{
    Element normalized = elem;
    if (normalized.is_set)
    {
        Set temp(normalized.children);
        temp.normalize();
        normalized.children = temp.elements;
    }

    for (const auto &item : elements)
    {
        if (item == normalized)
            return true;
    }
    return false;
}

bool Set::add(const Element &elem)
{
    Element normalized = elem;
    if (normalized.is_set)
    {
        Set temp(normalized.children);
        temp.normalize();
        normalized.children = temp.elements;
    }

    if (contains(normalized))
        return false;

    elements.push_back(normalized);
    normalize();
    return true;
}

bool Set::remove(const Element &elem)
{
    Element normalized = elem;
    if (normalized.is_set)
    {
        Set temp(normalized.children);
        temp.normalize();
        normalized.children = temp.elements;
    }

    for (size_t i = 0; i < elements.size(); i++)
    {
        if (elements[i] == normalized)
        {
            elements.erase(elements.begin() + i);
            return true;
        }
    }
    return false;
}

bool Set::operator[](const Element &elem) const
{
    return contains(elem);
}

Set Set::operator+(const Set &other) const
{
    Set res = *this;
    for (const auto &item : other.elements)
        res.add(item);
    return res;
}

Set &Set::operator+=(const Set &other)
{
    for (const auto &item : other.elements)
        add(item);
    return *this;
}

Set &Set::operator+=(const Element &elem)
{
    add(elem);
    return *this;
}

Set Set::operator*(const Set &other) const
{
    Set res;
    for (const auto &item : elements)
    {
        if (other.contains(item))
            res.add(item);
    }
    return res;
}

Set &Set::operator*=(const Set &other)
{
    *this = *this * other;
    return *this;
}

Set Set::operator-(const Set &other) const
{
    Set res;
    for (const auto &item : elements)
    {
        if (!other.contains(item))
            res.add(item);
    }
    return res;
}

Set &Set::operator-=(const Set &other)
{
    *this = *this - other;
    return *this;
}

Set &Set::operator-=(const Element &elem)
{
    remove(elem);
    return *this;
}

Set Set::power_set() const
{
    Set res;
    size_t n = elements.size();
    size_t total = 1ULL << n;

    for (size_t mask = 0; mask < total; mask++)
    {
        Element subset_elem;
        subset_elem.is_set = true;
        for (size_t i = 0; i < n; i++)
        {
            if (mask & (1ULL << i))
                subset_elem.children.push_back(elements[i]);
        }
        res.add(subset_elem);
    }
    return res;
}

bool Set::operator==(const Set &other) const
{
    return elements == other.elements;
}

bool Set::operator!=(const Set &other) const
{
    return !(*this == other);
}

std::string Set::to_string() const
{
    std::string res = "{";
    for (size_t i = 0; i < elements.size(); i++)
    {
        if (i > 0)
            res += ", ";
        res += elements[i].to_string();
    }
    res += "}";
    return res;
}

Set::Element Set::parse_element_recursive(const std::string &str, size_t &pos)
{
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Element();

    if (str[pos] == '{')
    {
        pos++;
        Element e;
        e.is_set = true;

        while (pos < str.length())
        {
            while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
                pos++;

            if (pos < str.length() && str[pos] == '}')
            {
                pos++;
                break;
            }

            e.children.push_back(parse_element_recursive(str, pos));

            while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
                pos++;

            if (pos < str.length() && str[pos] == ',')
                pos++;
            else if (pos < str.length() && str[pos] == '}')
            {
                pos++;
                break;
            }
        }
        return e;
    }
    else
    {
        size_t start = pos;
        while (pos < str.length() &&
               str[pos] != ',' && str[pos] != '{' && str[pos] != '}' &&
               !isspace(static_cast<unsigned char>(str[pos])))
        {
            pos++;
        }
        return Element(str.substr(start, pos - start));
    }
}

Set Set::parse(const std::string &str)
{
    size_t pos = 0;
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Set();

    Element root = parse_element_recursive(str, pos);
    Set s;
    if (root.is_set)
        s.elements = root.children;
    else
        s.add(root);

    s.normalize();
    return s;
}

Set::Element Set::parse_element(const std::string &str)
{
    size_t pos = 0;
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Element();

    if (str[pos] == '{')
    {
        Element e = parse_element_recursive(str, pos);
        Set temp(e.children);
        temp.normalize();
        e.children = temp.elements;
        return e;
    }
    else
    {
        size_t end = str.length();
        while (end > pos && isspace(static_cast<unsigned char>(str[end - 1])))
            end--;
        return Element(str.substr(pos, end - pos));
    }
}

std::ostream &operator<<(std::ostream &out, const Set &s)
{
    out << s.to_string();
    return out;
}

std::istream &operator>>(std::istream &in, Set &s)
{
    std::string line;
    std::getline(in, line);
    if (!line.empty())
        s = Set::parse(line);
    return in;
}