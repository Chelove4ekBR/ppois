#include <catch2/catch_test_macros.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::usd;

template <typename TourType>
static std::shared_ptr<TourType> tourAt(TravelAgency &agency, std::size_t index)
{
    return std::dynamic_pointer_cast<TourType>(agency.catalog().tours().at(index));
}

TEST_CASE("Activity cost and duration", "[tours]")
{
    const Activity dive("Diving", 120, usd(5000), true);
    CHECK(dive.isLongerThan(60));
    CHECK_FALSE(dive.isLongerThan(120));
    CHECK(dive.costForGroup(3).cents() == 15000);
    CHECK(dive.describe() == "Diving (120 min, 50.00 USD)");
}

TEST_CASE("Itinerary day and totals", "[tours]")
{
    ItineraryDay day(1, "Arrival");
    day.addActivity(Activity("Walk", 90, usd(0), true));
    day.addActivity(Activity("Dinner", 120, usd(4000), false));
    CHECK(day.totalMinutes() == 210);
    CHECK(day.totalCost().cents() == 4000);
    CHECK_FALSE(day.isOverloaded());
    day.addActivity(Activity("Marathon", 600, usd(0), true));
    CHECK(day.isOverloaded());
}

TEST_CASE("Itinerary enforces day order and finds busiest day", "[tours]")
{
    Itinerary itinerary("Plan", City("Rome", support::visaFreeCountry(), 1));
    CHECK_THROWS_AS(itinerary.busiestDay(), TravelException);
    ItineraryDay first(1, "A");
    first.addActivity(Activity("Short", 30, usd(100), false));
    ItineraryDay second(2, "B");
    second.addActivity(Activity("Long", 300, usd(200), false));
    itinerary.addDay(first);
    itinerary.addDay(second);
    CHECK_THROWS_AS(itinerary.addDay(ItineraryDay(5, "Gap")), TravelException);
    CHECK(itinerary.totalDays() == 2);
    CHECK(itinerary.totalCost().cents() == 300);
    CHECK(itinerary.busiestDay().dayNumber() == 2);
}

TEST_CASE("BeachTour pricing", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const auto beach = tourAt<BeachTour>(agency, 0);
    CHECK(beach->pricePerPerson().cents() == 85000);
    CHECK(beach->priceFor(2).cents() == 170000);
    CHECK(beach->hasHotelWithStars(5));
    CHECK(beach->hasCapacityFor(20));
    CHECK_FALSE(beach->hasCapacityFor(21));
    CHECK(beach->isSuitableForSeason(Date(2026, 7, 1)));
    CHECK_FALSE(beach->isSuitableForSeason(Date(2026, 1, 1)));
    CHECK(beach->describe().find("Makadi Bay") != std::string::npos);
    CHECK(beach->itinerary().totalDays() == 1);
}

TEST_CASE("ExcursionTour pricing", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const auto excursion = tourAt<ExcursionTour>(agency, 1);
    CHECK(excursion->pricePerPerson().cents() == 31600);
    CHECK(excursion->guideSpeaksLanguage());
    CHECK(excursion->isEasy());
    CHECK_FALSE(tourAt<ExcursionTour>(agency, 4)->isEasy());
    CHECK(excursion->describe().find("Elena Guide") != std::string::npos);
}

TEST_CASE("CruiseTour cabin categories", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const auto cruise = tourAt<CruiseTour>(agency, 2);
    CHECK(cruise->cabinMultiplierPercent() == 130);
    CHECK(cruise->pricePerPerson().cents() == 195000);
    CHECK(cruise->portCount() == 2U);
    CHECK(cruise->visitsCity("Naples"));
    CHECK_FALSE(cruise->visitsCity("Oslo"));
    CHECK(cruise->describe().find("Mediterranean Star") != std::string::npos);
    const Tour base = *agency.catalog().tours().at(2);
    const auto ship = std::make_shared<Ship>("Star", 10, 2, 20);
    CHECK(CruiseTour(base, ship, "Inside", {}).cabinMultiplierPercent() == 100);
    CHECK(CruiseTour(base, ship, "Suite", {}).cabinMultiplierPercent() == 200);
    CHECK_THROWS_AS(CruiseTour(base, ship, "Attic", {}).pricePerPerson(), TravelException);
}

TEST_CASE("SkiTour pricing", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const auto ski = tourAt<SkiTour>(agency, 3);
    CHECK(ski->skiPassCost().cents() == 24000);
    CHECK(ski->pricePerPerson().cents() == 94000);
    CHECK(ski->isForBeginners());
    CHECK(ski->resortName() == "Axamer Lizum");
    CHECK(ski->describe().find("Ski tour") != std::string::npos);
}

TEST_CASE("Tour constructor rejects non-positive values", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const Tour base = *agency.catalog().tours().front();
    CHECK_THROWS_AS(Tour(9, "Bad", base.destination(), 0, usd(1), 5, base.itinerary()), TravelException);
    CHECK_THROWS_AS(Tour(9, "Bad", base.destination(), 3, usd(1), 0, base.itinerary()), TravelException);
}

TEST_CASE("Departure seat management and timing", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    const DeparturePtr departure = agency.catalog().findDeparture(4);
    CHECK(departure->freeSeats() == 12);
    departure->reserveSeats(12);
    CHECK(departure->isSoldOut());
    CHECK_THROWS_AS(departure->reserveSeats(1), TourFullException);
    departure->releaseSeats(20);
    CHECK(departure->bookedSeats() == 0);
    CHECK(departure->startsWithin(support::today(), 60));
    CHECK_FALSE(departure->startsWithin(support::today(), 10));
    CHECK_FALSE(departure->startsWithin(daysAfter(100), 60));
    CHECK_FALSE(departure->isFinished(support::today()));
    CHECK(departure->isFinished(daysAfter(100)));
}

TEST_CASE("Base Tour pricePerPerson and methods", "[tours]")
{
    const City cairoCity("Cairo", support::visaFreeCountry(), 2);
    Destination dest(cairoCity, "Cairo City", {10, 11});
    Itinerary itinerary("Cairo Plan", cairoCity);

    // Прямой вызов методов базового класса Tour (не переопределенных в подклассах)
    Tour baseTour(100, "Base Cairo", dest, 5, usd(50000), 10, itinerary);
    CHECK(baseTour.pricePerPerson().cents() == 50000);
    CHECK(baseTour.priceFor(2).cents() == 100000);
    CHECK(baseTour.hasCapacityFor(10));
    CHECK_FALSE(baseTour.hasCapacityFor(11));
    CHECK(baseTour.isSuitableForSeason(Date(2026, 10, 15)));
    CHECK_FALSE(baseTour.isSuitableForSeason(Date(2026, 1, 15)));
    CHECK(baseTour.describe().find("Base Cairo") != std::string::npos);
}

TEST_CASE("Catalog search and lookup", "[tours]")
{
    TravelAgency agency = support::makeAgency();
    TourCatalog &catalog = agency.catalog();

    // 1. Поиск туров (первый, промежуточный, несуществующий)
    CHECK(catalog.findTour(1)->id() == 1);
    CHECK(catalog.findTour(2)->name() == "Istanbul Classic");
    CHECK(catalog.findTour(5)->id() == 5);
    CHECK_THROWS_AS(catalog.findTour(99), TourNotFoundException);

    // 2. Поиск отправлений (существующие и несуществующие)
    CHECK(catalog.findDeparture(1)->id() == 1);
    CHECK(catalog.findDeparture(3)->id() == 3);
    CHECK_THROWS_AS(catalog.findDeparture(99), TourNotFoundException);

    // 3. Поиск по городу (совпадение и отсутствие результатов)
    CHECK(catalog.findByCity("Rome").size() == 1U);
    CHECK(catalog.findByCity("NonExistingCity").empty());

    // 4. Доступные по бюджету (все подходят, часть подходит, ни один не подходит)
    CHECK(catalog.findAffordable(usd(10000000)).size() == 5U);
    CHECK(catalog.findAffordable(usd(100000)).size() == 3U);
    CHECK(catalog.findAffordable(usd(1000)).empty());

    // 5. Доступные отправления: обычные и отсечение по дате
    CHECK(catalog.availableDepartures(support::today()).size() == 5U);
    CHECK(catalog.availableDepartures(daysAfter(45)).size() == 3U);

    // 6. Закрытие ветки !isSoldOut() == false (тур будущий, но мест нет)
    DeparturePtr dep = catalog.findDeparture(1);
    dep->reserveSeats(dep->freeSeats());
    CHECK(dep->isSoldOut());
    CHECK(catalog.availableDepartures(support::today()).size() == 4U);

    // 7. Поведение на пустом каталоге и прямое добавление отправления
    TourCatalog emptyCatalog;
    CHECK_THROWS_AS(emptyCatalog.findTour(1), TourNotFoundException);
    CHECK_THROWS_AS(emptyCatalog.findDeparture(1), TourNotFoundException);
    CHECK(emptyCatalog.findByCity("Rome").empty());
    CHECK(emptyCatalog.findAffordable(usd(100000)).empty());
    CHECK(emptyCatalog.availableDepartures(support::today()).empty());

    const City testCity("Rome", support::visaFreeCountry(), 1);
    Destination testDest(testCity, "Rome City", {5});
    Itinerary testItinerary("Itinerary", testCity);
    auto directTour = std::make_shared<Tour>(10, "Direct Tour", testDest, 3, usd(20000), 5, testItinerary);
    emptyCatalog.addTour(directTour);
    CHECK(emptyCatalog.findTour(10) == directTour);

    auto directDep = emptyCatalog.addDeparture(directTour, DateRange(daysAfter(10), daysAfter(13)));
    CHECK(directDep->id() == 1);
    CHECK(emptyCatalog.findDeparture(1) == directDep);
}

TEST_CASE("Itinerary ordering branch and overloaded limit", "[tours]")
{
    // Ветка сравнения в busiestDay, когда первый день длиннее второго
    Itinerary revItinerary("Reverse", City("Rome", support::visaFreeCountry(), 1));
    ItineraryDay day1(1, "Long day");
    day1.addActivity(Activity("Long Walk", 300, usd(0), true));
    ItineraryDay day2(2, "Short day");
    day2.addActivity(Activity("Short Walk", 60, usd(0), true));
    revItinerary.addDay(day1);
    revItinerary.addDay(day2);
    CHECK(revItinerary.busiestDay().dayNumber() == 1);

    // Граничное значение перегрузки (ровно 720 минут — не перегружен)
    ItineraryDay boundaryDay(1, "Boundary");
    boundaryDay.addActivity(Activity("Exact", 720, usd(0), false));
    CHECK_FALSE(boundaryDay.isOverloaded());
}