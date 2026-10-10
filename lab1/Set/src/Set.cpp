#include "../include/Set.h"

Set::Node::Node()
    : is_set(false) {}

Set::Node::Node(const std::string &atom_val)
    : is_set(false), atom(atom_val) {}

Set::Node::Node(const std::vector<Node> &children_val)
    : is_set(true), children(children_val) {}

bool Set::Node::is_set_node() const
{
    return is_set;
}

const std::string &Set::Node::get_atom() const
{
    return atom;
}

const std::vector<Set::Node> &Set::Node::get_children() const
{
    return children;
}

bool Set::Node::operator==(const Node &other) const
{
    if (is_set != other.is_set)
        return false;
    if (!is_set)
        return atom == other.atom;
    return children == other.children;
}

bool Set::Node::operator!=(const Node &other) const
{
    return !(*this == other);
}

bool Set::Node::operator<(const Node &other) const
{

    if (is_set != other.is_set)
        return !is_set && other.is_set;

    if (!is_set)
        return atom < other.atom;

    return children < other.children;
}

std::string Set::Node::to_string() const
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

Set::Set(const std::vector<Node> &elems)
    : Nodes(elems)
{
    normalize();
}

void Set::normalize()
{

    for (auto &item : Nodes)
    {
        if (item.is_set)
        {
            Set temporarySetForNorm(item.children);
            temporarySetForNorm.normalize();
            item.children = temporarySetForNorm.Nodes;
        }
    }

    std::sort(Nodes.begin(), Nodes.end());

    auto it = std::unique(Nodes.begin(), Nodes.end());
    Nodes.erase(it, Nodes.end());
}

bool Set::empty() const
{
    return Nodes.empty();
}

int Set::size() const
{
    return static_cast<int>(Nodes.size());
}

void Set::clear()
{
    Nodes.clear();
}

bool Set::contains(const Node &elem) const
{
    Node normalized = elem;
    if (normalized.is_set)
    {
        Set temporarySetForNorm(normalized.children);
        temporarySetForNorm.normalize();
        normalized.children = temporarySetForNorm.Nodes;
    }

    return std::binary_search(Nodes.begin(), Nodes.end(), normalized);
}

bool Set::add(const Node &elem)
{
    Node normalized = elem;
    if (normalized.is_set)
    {
        Set temporarySetForNorm(normalized.children);
        temporarySetForNorm.normalize();
        normalized.children = temporarySetForNorm.Nodes;
    }

    if (contains(normalized))
        return false;

    Nodes.push_back(normalized);
    normalize();
    return true;
}

bool Set::remove(const Node &elem)
{
    Node normalized = elem;
    if (normalized.is_set)
    {
        Set temporarySetForNorm(normalized.children);
        temporarySetForNorm.normalize();
        normalized.children = temporarySetForNorm.Nodes;
    }

    auto it = std::find(Nodes.begin(), Nodes.end(), normalized);
    if (it != Nodes.end())
    {
        Nodes.erase(it);
        return true;
    }
    return false;
}

bool Set::operator[](const Node &elem) const
{
    return contains(elem);
}

Set Set::operator+(const Set &other) const
{
    Set res;
    std::set_union(
        Nodes.begin(), Nodes.end(),
        other.Nodes.begin(), other.Nodes.end(),
        std::back_inserter(res.Nodes));
    return res;
}

Set &Set::operator+=(const Set &other)
{
    for (const auto &item : other.Nodes)
        add(item);
    return *this;
}

Set &Set::operator+=(const Node &elem)
{
    add(elem);
    return *this;
}

Set Set::operator*(const Set &other) const
{
    Set res;
    std::set_intersection(
        Nodes.begin(), Nodes.end(),
        other.Nodes.begin(), other.Nodes.end(),
        std::back_inserter(res.Nodes));
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
    std::set_difference(
        Nodes.begin(), Nodes.end(),
        other.Nodes.begin(), other.Nodes.end(),
        std::back_inserter(res.Nodes));
    return res;
}

Set &Set::operator-=(const Set &other)
{
    *this = *this - other;
    return *this;
}

Set &Set::operator-=(const Node &elem)
{
    remove(elem);
    return *this;
}

Set Set::power_set() const
{
    Set res;

    Node empty_subset;
    empty_subset.is_set = true;
    res.Nodes.push_back(empty_subset);

    size_t total_subsets = static_cast<size_t>(std::pow(2, Nodes.size()));
    res.Nodes.reserve(total_subsets);

    for (const auto &item : Nodes)
    {
        size_t current_size = res.Nodes.size();

        for (size_t i = 0; i < current_size; ++i)
        {
            Node new_subset = res.Nodes[i];

            new_subset.children.push_back(item);

            res.Nodes.push_back(new_subset);
        }
    }

    res.normalize();
    return res;
}

bool Set::operator==(const Set &other) const
{
    return Nodes == other.Nodes;
}

bool Set::operator!=(const Set &other) const
{
    return !(*this == other);
}

std::string Set::to_string() const
{
    std::string res = "{";
    for (size_t i = 0; i < Nodes.size(); i++)
    {
        if (i > 0)
            res += ", ";
        res += Nodes[i].to_string();
    }
    res += "}";
    return res;
}

Set::Node Set::parse_Node_recursive(const std::string &str, size_t &pos)
{
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Node();

    if (str[pos] == '{')
    {
        pos++;
        Node e;
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

            e.children.push_back(parse_Node_recursive(str, pos));

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
        return Node(str.substr(start, pos - start));
    }
}

Set Set::parse(const std::string &str)
{
    size_t pos = 0;
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Set();

    Node root = parse_Node_recursive(str, pos);
    Set s;
    if (root.is_set)
        s.Nodes = root.children;
    else
        s.add(root);

    s.normalize();
    return s;
}

Set::Node Set::parse_Node(const std::string &str)
{
    size_t pos = 0;
    while (pos < str.length() && isspace(static_cast<unsigned char>(str[pos])))
        pos++;

    if (pos >= str.length())
        return Node();

    if (str[pos] == '{')
    {
        Node e = parse_Node_recursive(str, pos);
        Set temporarySetForNorm(e.children);
        temporarySetForNorm.normalize();
        e.children = temporarySetForNorm.Nodes;
        return e;
    }
    else
    {
        size_t end = str.length();
        while (end > pos && isspace(static_cast<unsigned char>(str[end - 1])))
            end--;
        return Node(str.substr(pos, end - pos));
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