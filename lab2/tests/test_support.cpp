#include "test_support.hpp"

#include <vector>

namespace support
{

    namespace
    {

        constexpr int kPassportValidityDays = 3650;
        constexpr int kLicenseValidityDays = 3650;
        constexpr int kFirstDepartureOffsetDays = 30;
        constexpr int kDepartureStepDays = 10;
        constexpr int kYearDays = 365;
        constexpr int kFreeCancellationDays = 30;
        constexpr int kEarlyPenaltyPercent = 20;
        constexpr int kLatePenaltyPercent = 50;
        constexpr int kLateThresholdDays = 7;
        constexpr int kDiscountPercent = 10;
        constexpr int kDiscountUses = 100;
        constexpr int kManagerDiscountLimit = 25;

        struct TourSpec
        {
            int id;
            std::string name;
            int days;
            long long priceCents;
            int capacity;
        };

        Tour makeBaseTour(const TourSpec &spec, const City &city)
        {
            Destination destination(city, "Popular destination: " + city.name(), {5, 6, 7, 8, 9});
            destination.addAttraction(Attraction("City highlights", Money(2500), 120, false));
            Itinerary itinerary(spec.name + " itinerary", city);
            ItineraryDay firstDay(1, "Arrival and welcome dinner");
            firstDay.addActivity(Activity("Welcome walk", 90, Money(0), true));
            itinerary.addDay(firstDay);
            return Tour(spec.id, spec.name, destination, spec.days, Money(spec.priceCents), spec.capacity, itinerary);
        }

        TourGuidePtr makeGuide(const Date &now)
        {
            const std::vector<std::string> languages{"English", "Russian"};
            return std::make_shared<TourGuide>(10, makePerson("Elena", "Guide", 1988), Money(300000), now.addDays(-kYearDays),
                                               languages, Money(8000));
        }

        HotelPtr makeHotel()
        {
            HotelPtr hotel = std::make_shared<Hotel>("Sea Breeze", 5, Address("Egypt", "Hurghada", "Corniche 10", "84511"));
            hotel->addRoom(101, 2, Money(9000));
            hotel->addRoom(102, 3, Money(12000));
            return hotel;
        }

        void addBeachTour(TravelCompany &agency)
        {
            const Country egypt("Egypt", "EG", "EGP", false);
            const Tour beach = makeBaseTour({1, "Red Sea Relax", 7, 80000, 20}, City("Hurghada", egypt, 2));
            agency.catalog().addTour(std::make_shared<BeachTour>(beach, "Makadi Bay", makeHotel(), true));
        }

        void addIstanbulTour(TravelCompany &agency, const TourGuidePtr &guide)
        {
            const Country turkey("Turkey", "TR", "TRY", false);
            const Tour istanbul = makeBaseTour({2, "Istanbul Classic", 3, 30000, 15}, City("Istanbul", turkey, 3));
            agency.catalog().addTour(std::make_shared<ExcursionTour>(istanbul, guide, "English", 4));
        }

        void addCruiseTour(TravelCompany &agency)
        {
            const Country italy("Italy", "IT", "EUR", false);
            const ShipPtr ship = std::make_shared<Ship>("Mediterranean Star", 200, 8, 400);
            const std::vector<City> ports{City("Naples", italy, 1), City("Palermo", italy, 1)};
            const Tour cruise = makeBaseTour({3, "Mediterranean Cruise", 10, 150000, 30}, City("Rome", italy, 1));
            agency.catalog().addTour(std::make_shared<CruiseTour>(cruise, ship, "Ocean", ports));
        }

        void addSkiTour(TravelCompany &agency)
        {
            const Country austria("Austria", "AT", "EUR", false);
            const Tour ski = makeBaseTour({4, "Tyrol Ski Week", 6, 70000, 12}, City("Innsbruck", austria, 1));
            agency.catalog().addTour(std::make_shared<SkiTour>(ski, "Axamer Lizum", Money(4000), 2));
        }

        void addKyotoTour(TravelCompany &agency, const TourGuidePtr &guide)
        {
            const Country japan("Japan", "JP", "JPY", true);
            const Tour kyoto = makeBaseTour({5, "Kyoto Temples", 5, 120000, 10}, City("Kyoto", japan, 9));
            agency.catalog().addTour(std::make_shared<ExcursionTour>(kyoto, guide, "English", 8));
        }

        void addTours(TravelCompany &agency, const Date &now)
        {
            const TourGuidePtr guide = makeGuide(now);
            addBeachTour(agency);
            addIstanbulTour(agency, guide);
            addCruiseTour(agency);
            addSkiTour(agency);
            addKyotoTour(agency, guide);
        }

        void addDepartures(TravelCompany &agency, const Date &now)
        {
            int offset = kFirstDepartureOffsetDays;
            for (const TourPtr &tour : agency.catalog().tours())
            {
                const Date start = now.addDays(offset);
                agency.catalog().addDeparture(tour, DateRange(start, start.addDays(tour->durationDays())));
                offset += kDepartureStepDays;
            }
        }

        void addOffice(TravelCompany &agency, const Date &now)
        {
            const ManagerPtr manager = std::make_shared<Manager>(1, makePerson("Olga", "Boss", 1980), Money(500000),
                                                                 now.addDays(-kYearDays), "Sales", kManagerDiscountLimit);
            Office office("Moscow office", sampleAddress(), manager);
            office.hire(makeGuide(now));
            agency.addOffice(office);
        }

    } // namespace

    Date today()
    {
        return Date(2026, 10, 21);
    }

    Date daysAfter(int days)
    {
        return today().addDays(days);
    }

    Money usd(long long cents)
    {
        return Money(cents);
    }

    Address sampleAddress()
    {
        return Address("Russia", "Moscow", "Arbat 1", "119019");
    }

    Country visaFreeCountry()
    {
        return Country("Egypt", "EG", "EGP", false);
    }

    Country visaCountry()
    {
        return Country("Japan", "JP", "JPY", true);
    }

    Person makePerson(const std::string &firstName, const std::string &lastName, int birthYear)
    {
        const ContactInfo contact(firstName + "." + lastName + "@example.com", "+7 900 123 45 67");
        return Person(firstName, lastName, Date(birthYear, 1, 1), contact, sampleAddress());
    }

    CustomerPtr buildCustomer(int customerId, const std::string &firstName, const std::string &lastName)
    {
        const Passport passport("P" + std::to_string(customerId) + "123456", "RU", firstName + " " + lastName,
                                daysAfter(kPassportValidityDays));
        return std::make_shared<Customer>(customerId, makePerson(firstName, lastName, 1990), passport);
    }

    CustomerPtr makeCustomer(int id)
    {
        return buildCustomer(id, "Ivan", "Petrov");
    }

    DriverPtr makeDriver(const std::string &category)
    {
        const DrivingLicense license("DL-1", category, daysAfter(kLicenseValidityDays));
        return std::make_shared<Driver>(20, makePerson("Boris", "Wheel", 1980), usd(200000), daysAfter(-1000), license);
    }

    BusPtr makeBus(int capacity)
    {
        return std::make_shared<Bus>(5, "CityBus", capacity, usd(1000), "A123BC", makeDriver(), true);
    }

    Airport airportIn(const std::string &code, const Country &country)
    {
        return Airport(code, code + " airport", City(code + " city", country, 0));
    }

    TravelCompany buildAgency(const Date &now)
    {
        const CancellationPolicy policy(kFreeCancellationDays, kEarlyPenaltyPercent, kLatePenaltyPercent, kLateThresholdDays);
        const auto accountant = std::make_shared<Accountant>(2, makePerson("Irina", "Books", 1985), Money(250000),
                                                             now.addDays(-kYearDays), "ACC-2024-17");
        TravelCompany agency("Horizon Travel", policy, accountant);
        addTours(agency, now);
        addDepartures(agency, now);
        addOffice(agency, now);
        agency.addDiscount(Discount("WELCOME10", kDiscountPercent, DateRange(now, now.addDays(kYearDays)), kDiscountUses));
        agency.registerCustomer(buildCustomer(1, "Ivan", "Petrov"));
        agency.registerCustomer(buildCustomer(2, "Anna", "Smirnova"));
        return agency;
    }

    TravelCompany makeAgency()
    {
        return buildAgency(today());
    }

}