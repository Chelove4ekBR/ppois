#include "Money.hpp"

#include <cstdio>
#include <cstdlib>
#include <utility>

#include "Exceptions.hpp"

namespace
{

    constexpr long long kCentsPerUnit = 100;
    constexpr std::size_t kMoneyBufferSize = 32;

}

Money::Money(long long cents, std::string currency) : cents_(cents), currency_(std::move(currency))
{
    if (currency_.empty())
    {
        throw InvalidMoneyException("Currency must not be empty");
    }
}

void Money::requireSameCurrency(const Money &other) const
{
    if (currency_ != other.currency_)
    {
        throw CurrencyMismatchException("Cannot combine " + currency_ + " with " + other.currency_);
    }
}

Money Money::plus(const Money &other) const
{
    requireSameCurrency(other);
    return Money(cents_ + other.cents_, currency_);
}

Money Money::minus(const Money &other) const
{
    requireSameCurrency(other);
    return Money(cents_ - other.cents_, currency_);
}

Money Money::times(int factor) const
{
    return Money(cents_ * factor, currency_);
}

Money Money::dividedBy(int divisor) const
{
    if (divisor == 0)
    {
        throw InvalidMoneyException("Division by zero");
    }
    return Money(cents_ / divisor, currency_);
}

Money Money::percentOf(int percent) const
{
    return Money(cents_ * percent / kPercentBase, currency_);
}

bool Money::isGreaterThan(const Money &other) const
{
    requireSameCurrency(other);
    return cents_ > other.cents_;
}

bool Money::isNegative() const
{
    return cents_ < 0;
}

std::string Money::toString() const
{
    const long long absolute = std::llabs(cents_);
    char buffer[kMoneyBufferSize];
    std::snprintf(buffer, sizeof(buffer), "%s%lld.%02lld ", cents_ < 0 ? "-" : "", absolute / kCentsPerUnit,
                  absolute % kCentsPerUnit);
    return std::string(buffer) + currency_;
}
