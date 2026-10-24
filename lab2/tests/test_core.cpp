#include <catch2/catch_test_macros.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::today;
using support::usd;

TEST_CASE("Money arithmetic keeps currency", "[money]") {
    CHECK(usd(500).plus(usd(250)).cents() == 750);
    CHECK(usd(500).minus(usd(750)).cents() == -250);
    CHECK(usd(500).times(3).cents() == 1500);
    CHECK(usd(500).dividedBy(4).cents() == 125);
    CHECK(usd(1000).percentOf(15).cents() == 150);
    CHECK(usd(1).currency() == "USD");
}

TEST_CASE("Money comparison and sign", "[money]") {
    CHECK(usd(200).isGreaterThan(usd(100)));
    CHECK_FALSE(usd(100).isGreaterThan(usd(100)));
    CHECK(usd(-1).isNegative());
    CHECK_FALSE(usd(0).isNegative());
}

TEST_CASE("Money toString formats units and sign", "[money]") {
    CHECK(usd(12345).toString() == "123.45 USD");
    CHECK(usd(-150).toString() == "-1.50 USD");
}

TEST_CASE("Money rejects invalid operations", "[money]") {
    CHECK_THROWS_AS(Money(1, ""), InvalidMoneyException);
    CHECK_THROWS_AS(usd(1).dividedBy(0), InvalidMoneyException);
    CHECK_THROWS_AS(usd(1).plus(Money(1, "EUR")), CurrencyMismatchException);
    CHECK_THROWS_AS(usd(1).minus(Money(1, "EUR")), CurrencyMismatchException);
    CHECK_THROWS_AS(usd(1).isGreaterThan(Money(1, "EUR")), CurrencyMismatchException);
}

TEST_CASE("Date validates calendar", "[date]") {
    CHECK_THROWS_AS(Date(2026, 2, 30), InvalidDateException);
    CHECK_THROWS_AS(Date(2026, 13, 1), InvalidDateException);
    CHECK_NOTHROW(Date(2028, 2, 29));
    CHECK_THROWS_AS(Date(2027, 2, 29), InvalidDateException);
}

TEST_CASE("Date addDays crosses year boundary", "[date]") {
    const Date next = Date(2026, 12, 31).addDays(1);
    CHECK(next.toString() == "2027-01-01");
    CHECK(next.year() == 2027);
    CHECK(next.month() == 1U);
    CHECK(next.day() == 1U);
}

TEST_CASE("Date comparison helpers", "[date]") {
    CHECK(today().daysUntil(daysAfter(10)) == 10);
    CHECK(today().isBefore(daysAfter(1)));
    CHECK_FALSE(daysAfter(1).isBefore(today()));
    CHECK(today().isSameDay(Date(2026, 10, 21)));
    CHECK(Date::fromDayNumber(today().dayNumber()).toString() == "2026-10-21");
}

TEST_CASE("DateRange length, contains and overlap", "[daterange]") {
    const DateRange range(today(), daysAfter(7));
    CHECK(range.lengthInDays() == 7);
    CHECK(range.contains(today()));
    CHECK(range.contains(daysAfter(7)));
    CHECK_FALSE(range.contains(daysAfter(8)));
    CHECK(range.overlaps(DateRange(daysAfter(3), daysAfter(10))));
    CHECK_FALSE(range.overlaps(DateRange(daysAfter(7), daysAfter(9))));
    CHECK(range.toString() == "2026-10-21 .. 2026-10-28");
}

TEST_CASE("DateRange rejects reversed range", "[daterange]") {
    CHECK_THROWS_AS(DateRange(daysAfter(1), today()), InvalidDateException);
}

TEST_CASE("Address compares city and country", "[address]") {
    const Address moscow = support::sampleAddress();
    const Address sameCity("Russia", "Moscow", "Tverskaya 5", "125009");
    const Address otherCity("Russia", "Kazan", "Baumana 1", "420111");
    CHECK(moscow.isSameCity(sameCity));
    CHECK_FALSE(moscow.isSameCity(otherCity));
    CHECK(moscow.isSameCountry(otherCity));
    CHECK(moscow.fullText() == "Arbat 1, Moscow 119019, Russia");
}

TEST_CASE("ContactInfo validates email and phone", "[contactinfo]") {
    CHECK(ContactInfo::isValidEmail("a@b.c"));
    CHECK_FALSE(ContactInfo::isValidEmail("@b.c"));
    CHECK_FALSE(ContactInfo::isValidEmail("a@@b.c"));
    CHECK_FALSE(ContactInfo::isValidEmail("a@b"));
    CHECK_FALSE(ContactInfo::isValidEmail("a@.c"));
    CHECK_FALSE(ContactInfo::isValidEmail("a@b."));
    CHECK_FALSE(ContactInfo::isValidEmail("plain"));
    CHECK(ContactInfo::isValidPhone("+7 900 123 45 67"));
    CHECK_FALSE(ContactInfo::isValidPhone("12345"));
}

TEST_CASE("ContactInfo constructor rejects bad data", "[contactinfo]") {
    CHECK_THROWS_AS(ContactInfo("bad", "+7 900 123 45 67"), InvalidContactException);
    CHECK_THROWS_AS(ContactInfo("a@b.c", "123"), InvalidContactException);
    CHECK(ContactInfo("a@b.c", "+7 900 123 45 67").maskedPhone() == "*******4567");
}

TEST_CASE("Passport validates number", "[passport]") {
    CHECK_THROWS_AS(Passport("A1", "RU", "Ivan", daysAfter(1)), InvalidPassportException);
    CHECK_THROWS_AS(Passport("ABC-1234", "RU", "Ivan", daysAfter(1)), InvalidPassportException);
    CHECK_NOTHROW(Passport("AB123456", "RU", "Ivan", daysAfter(1)));
}

TEST_CASE("Passport expiry and trip validity", "[passport]") {
    const Passport passport("AB123456", "RU", "Ivan", Date(2026, 12, 31));
    const DateRange trip(Date(2026, 12, 10), Date(2026, 12, 20));
    CHECK_FALSE(passport.isExpiredOn(today()));
    CHECK(passport.isExpiredOn(Date(2027, 1, 1)));
    CHECK_FALSE(passport.isValidForTrip(trip));
    CHECK_THROWS_AS(passport.requireValidForTrip(trip), ExpiredPassportException);
    const Passport longLived("AB123456", "RU", "Ivan", Date(2035, 1, 1));
    CHECK_NOTHROW(longLived.requireValidForTrip(trip));
}

TEST_CASE("Visa covers only matching country and period", "[visa]") {
    const Visa visa("JP", "tourist", DateRange(daysAfter(0), daysAfter(60)));
    CHECK(visa.isActiveOn(daysAfter(5)));
    CHECK(visa.covers("JP", DateRange(daysAfter(10), daysAfter(20))));
    CHECK_FALSE(visa.covers("JP", DateRange(daysAfter(10), daysAfter(90))));
    CHECK_FALSE(visa.covers("US", DateRange(daysAfter(10), daysAfter(20))));
}

TEST_CASE("DrivingLicense category and validity", "[drivinglicense]") {
    const DrivingLicense license("DL-1", "BD", daysAfter(10));
    CHECK(license.allowsCategory("D"));
    CHECK_FALSE(license.allowsCategory("C"));
    CHECK(license.isValidOn(daysAfter(10)));
    CHECK_FALSE(license.isValidOn(daysAfter(11)));
}
