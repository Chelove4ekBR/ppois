#pragma once

#include <string>

inline constexpr const char* kDefaultCurrency = "USD";
inline constexpr int kPercentBase = 100;

class Money {
public:
    explicit Money(long long cents = 0, std::string currency = kDefaultCurrency);
    long long cents() const { return cents_; }
    const std::string& currency() const { return currency_; }
    Money plus(const Money& other) const;
    Money minus(const Money& other) const;
    Money times(int factor) const;
    Money dividedBy(int divisor) const; 
    Money percentOf(int percent) const;
    bool isGreaterThan(const Money& other) const;
    bool isNegative() const;
    std::string toString() const;

private:
    void requireSameCurrency(const Money& other) const;

    long long cents_;
    std::string currency_;
};
