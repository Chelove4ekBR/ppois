#include <catch2/catch_test_macros.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::today;
using support::usd;

static City cairo() {
    return City("Cairo", support::visaFreeCountry(), 2);
}

TEST_CASE("Country describes visa rules", "[geo]") {
    CHECK(support::visaCountry().matchesCode("JP"));
    CHECK_FALSE(support::visaCountry().matchesCode("EG"));
    CHECK(support::visaCountry().describe() == "Japan (JP, JPY, visa required)");
    CHECK(support::visaFreeCountry().describe() == "Egypt (EG, EGP, no visa)");
}

TEST_CASE("City labels and time zones", "[geo]") {
    const City tokyo("Tokyo", support::visaCountry(), 9);
    CHECK(tokyo.label() == "Tokyo, Japan");
    CHECK(tokyo.isInCountry(support::visaCountry()));
    CHECK_FALSE(tokyo.isInCountry(support::visaFreeCountry()));
    CHECK(cairo().timeDifferenceHours(tokyo) == 7);
}

TEST_CASE("Airport validates code and direction", "[geo]") {
    const Airport cai = support::airportIn("CAI", support::visaFreeCountry());
    const Airport hrg = support::airportIn("HRG", support::visaFreeCountry());
    const Airport kix = support::airportIn("KIX", support::visaCountry());
    CHECK_THROWS_AS(Airport("TOOLONG", "x", cairo()), TravelException);
    CHECK_FALSE(cai.isInternationalTo(hrg));
    CHECK(cai.isInternationalTo(kix));
    CHECK(cai.label() == "CAI - CAI airport (CAI city)");
}

TEST_CASE("Attraction costs", "[geo]") {
    const Attraction museum("Museum", usd(1500), 90, true);
    CHECK_FALSE(museum.isFree());
    CHECK(Attraction("Park", usd(0), 30, false).isFree());
    CHECK(museum.costFor(3).cents() == 4500);
    CHECK(museum.fitsInto(90));
    CHECK_FALSE(museum.fitsInto(60));
}

TEST_CASE("Destination aggregates attractions", "[geo]") {
    Destination destination(cairo(), "Old city", {10, 11});
    CHECK_THROWS_AS(destination.cheapestAttraction(), TravelException);
    destination.addAttraction(Attraction("Pyramids", usd(3000), 180, false));
    destination.addAttraction(Attraction("Bazaar", usd(500), 60, false));
    CHECK(destination.totalAttractionCost().cents() == 3500);
    CHECK(destination.cheapestAttraction().name() == "Bazaar");
    CHECK(destination.attractionsWithin(90).size() == 1U);
    CHECK(destination.isBestSeason(Date(2026, 10, 1)));
    CHECK_FALSE(destination.isBestSeason(Date(2026, 7, 1)));
}

TEST_CASE("LoyaltyAccount tiers follow points", "[loyalty]") {
    LoyaltyAccount account;
    CHECK(account.tier() == "Bronze");
    CHECK(account.discountPercent() == 0);
    account.earnPoints(usd(500000));
    CHECK(account.points() == 500);
    CHECK(account.tier() == "Silver");
    CHECK(account.discountPercent() == 3);
    account.earnPoints(usd(1500000));
    CHECK(account.tier() == "Gold");
    CHECK(account.discountPercent() == 7);
    CHECK(account.lifetimeSpent().cents() == 2000000);
}

TEST_CASE("LoyaltyAccount redeem checks balance", "[loyalty]") {
    LoyaltyAccount account;
    account.earnPoints(usd(100000));
    account.redeemPoints(40);
    CHECK(account.points() == 60);
    CHECK_THROWS_AS(account.redeemPoints(61), PaymentException);
}

TEST_CASE("Person age and adulthood", "[people]") {
    const Person adult("Ivan", "Petrov", Date(1990, 12, 31), ContactInfo("a@b.c", "+7 900 123 45 67"), support::sampleAddress());
    const Person child("Petr", "Petrov", Date(2015, 10, 21), ContactInfo("a@b.c", "+7 900 123 45 67"), support::sampleAddress());
    CHECK(adult.fullName() == "Ivan Petrov");
    CHECK(adult.ageOn(today()) == 35);
    CHECK(child.ageOn(today()) == 11);
    CHECK(adult.isAdultOn(today()));
    CHECK_FALSE(child.isAdultOn(today()));
}

TEST_CASE("Customer travel document checks", "[people]") {
    CustomerPtr customer = support::makeCustomer();
    const DateRange trip(daysAfter(30), daysAfter(37));
    CHECK_NOTHROW(customer->ensureCanEnter(support::visaFreeCountry(), trip));
    CHECK_THROWS_AS(customer->ensureCanEnter(support::visaCountry(), trip), VisaRequiredException);
    customer->addVisa(Visa("JP", "tourist", DateRange(daysAfter(0), daysAfter(100))));
    CHECK(customer->hasVisaFor("JP", trip));
    CHECK_NOTHROW(customer->ensureCanEnter(support::visaCountry(), trip));
    CHECK(customer->visas().size() == 1U);
}

TEST_CASE("Customer expiring passport blocks trip", "[people]") {
    const Passport passport("AB123456", "RU", "Ivan Petrov", daysAfter(40));
    const Customer customer(7, support::makePerson("Ivan", "Petrov", 1990), passport);
    CHECK_THROWS_AS(
        customer.ensureCanEnter(support::visaFreeCountry(), DateRange(daysAfter(30), daysAfter(37))),
        ExpiredPassportException);
}

TEST_CASE("Employee salary and service", "[people]") {
    Employee employee(3, support::makePerson("Max", "Clerk", 1985), "Clerk", usd(100000), Date(2020, 10, 21));
    CHECK(employee.yearsOfService(today()) == 6);
    employee.raiseSalary(10);
    CHECK(employee.salary().cents() == 110000);
    CHECK(employee.annualSalary().cents() == 1320000);
    CHECK(employee.describeRole() == "Clerk: Max Clerk");
}

TEST_CASE("TourGuide languages and availability", "[people]") {
    TourGuide guide(4, support::makePerson("Elena", "Guide", 1988), usd(300000), daysAfter(-100), {"English"}, usd(8000));
    CHECK(guide.speaks("English"));
    CHECK_FALSE(guide.speaks("French"));
    guide.addLanguage("French");
    guide.addLanguage("French");
    CHECK(guide.languages().size() == 2U);
    const DateRange week(daysAfter(10), daysAfter(17));
    CHECK(guide.isAvailable(week));
    guide.assign(week);
    CHECK_FALSE(guide.isAvailable(DateRange(daysAfter(12), daysAfter(14))));
    CHECK_THROWS_AS(guide.assign(DateRange(daysAfter(12), daysAfter(14))), GuideUnavailableException);
    CHECK(guide.costForDays(3).cents() == 24000);
    CHECK(guide.describeRole() == "Guide Elena Guide (2 languages)");
}

TEST_CASE("Manager team management", "[people]") {
    Manager manager(1, support::makePerson("Olga", "Boss", 1980), usd(500000), daysAfter(-500), "Sales", 25);
    const auto clerk = std::make_shared<Employee>(3, support::makePerson("Max", "Clerk", 1985), "Clerk", usd(100000), daysAfter(-50));
    manager.addSubordinate(clerk);
    CHECK(manager.teamSize() == 1U);
    CHECK(manager.teamPayroll().cents() == 600000);
    CHECK(manager.canApproveDiscount(25));
    CHECK_FALSE(manager.canApproveDiscount(26));
    CHECK(manager.describeRole() == "Manager of Sales: Olga Boss");
}

TEST_CASE("Driver license and rest", "[people]") {
    DriverPtr driver = support::makeDriver();
    CHECK(driver->canDrive("D", today()));
    CHECK_FALSE(driver->canDrive("C", today()));
    CHECK_FALSE(driver->canDrive("D", daysAfter(4000)));
    driver->logDistance(300);
    CHECK_FALSE(driver->needsRest());
    driver->logDistance(200);
    CHECK(driver->needsRest());
    CHECK(driver->totalKm() == 500);
    driver->takeRest();
    CHECK(driver->kmSinceRest() == 0);
    CHECK_THROWS_AS(driver->logDistance(-1), TravelException);
    CHECK(driver->describeRole() == "Driver Boris Wheel (license BD)");
}

TEST_CASE("TravelGroup membership limits", "[people]") {
    TravelGroup group("Friends", support::makeCustomer(1), 5);
    CHECK(group.hasMember(1));
    group.addMember(support::makeCustomer(1));
    CHECK(group.members().size() == 1U);
    for (int id = 2; id <= 5; ++id) {
        group.addMember(support::makeCustomer(id));
    }
    CHECK(group.isFull());
    CHECK(group.isDiscountEligible());
    CHECK_THROWS_AS(group.addMember(support::makeCustomer(6)), TourFullException);
    group.removeMember(5);
    CHECK_FALSE(group.hasMember(5));
    CHECK_FALSE(group.isDiscountEligible());
    CHECK(group.leader()->customerId() == 1);
}
