#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Accountant.hpp"
#include "Address.hpp"
#include "Attraction.hpp"
#include "BeachTour.hpp"
#include "Booking.hpp"
#include "CancellationPolicy.hpp"
#include "City.hpp"
#include "ContactInfo.hpp"
#include "Country.hpp"
#include "CruiseTour.hpp"
#include "Customer.hpp"
#include "Date.hpp"
#include "DateRange.hpp"
#include "Destination.hpp"
#include "Discount.hpp"
#include "ExcursionTour.hpp"
#include "Exceptions.hpp"
#include "Hotel.hpp"
#include "Itinerary.hpp"
#include "ItineraryDay.hpp"
#include "Manager.hpp"
#include "Money.hpp"
#include "Office.hpp"
#include "Passport.hpp"
#include "Payment.hpp"
#include "Person.hpp"
#include "Review.hpp"
#include "SalesReport.hpp"
#include "Ship.hpp"
#include "SkiTour.hpp"
#include "TourGuide.hpp"
#include "TravelAgency.hpp"

namespace
{

    constexpr int kPassportValidityDays = 3650;
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
    constexpr int kExitChoice = 0;
    constexpr int kReportDays = 365;
    constexpr long long kCentsPerUnit = 100;
    constexpr int kCashMethod = 2;
    constexpr int kTransferMethod = 3;
    constexpr int kVoucherMethod = 4;

    struct TourSpec
    {
        int id;
        std::string name;
        int days;
        long long priceCents;
        int capacity;
    };

    Address officeAddress()
    {
        return Address("Russia", "Moscow", "Tverskaya 1", "125009");
    }

    Person makePerson(const std::string &firstName, const std::string &lastName, int birthYear)
    {
        const ContactInfo contact(firstName + "." + lastName + "@example.com", "+7 900 123 45 67");
        return Person(firstName, lastName, Date(birthYear, 1, 1), contact, officeAddress());
    }

    CustomerPtr makeCustomer(int customerId, const std::string &firstName, const std::string &lastName, const Date &today)
    {
        const Passport passport("P" + std::to_string(customerId) + "123456", "RU", firstName + " " + lastName,
                                today.addDays(kPassportValidityDays));
        return std::make_shared<Customer>(customerId, makePerson(firstName, lastName, 1990), passport);
    }

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

    TourGuidePtr makeGuide(const Date &today)
    {
        const std::vector<std::string> languages{"English", "Russian"};
        return std::make_shared<TourGuide>(10, makePerson("Elena", "Guide", 1988), Money(300000), today.addDays(-kYearDays),
                                           languages, Money(8000));
    }

    HotelPtr makeHotel()
    {
        HotelPtr hotel = std::make_shared<Hotel>("Sea Breeze", 5, Address("Egypt", "Hurghada", "Corniche 10", "84511"));
        hotel->addRoom(101, 2, Money(9000));
        hotel->addRoom(102, 3, Money(12000));
        return hotel;
    }

    void addBeachTour(TravelAgency &agency)
    {
        const Country egypt("Egypt", "EG", "EGP", false);
        const Tour beach = makeBaseTour({1, "Red Sea Relax", 7, 80000, 20}, City("Hurghada", egypt, 2));
        agency.catalog().addTour(std::make_shared<BeachTour>(beach, "Makadi Bay", makeHotel(), true));
    }

    void addIstanbulTour(TravelAgency &agency, const TourGuidePtr &guide)
    {
        const Country turkey("Turkey", "TR", "TRY", false);
        const Tour istanbul = makeBaseTour({2, "Istanbul Classic", 3, 30000, 15}, City("Istanbul", turkey, 3));
        agency.catalog().addTour(std::make_shared<ExcursionTour>(istanbul, guide, "English", 4));
    }

    void addCruiseTour(TravelAgency &agency)
    {
        const Country italy("Italy", "IT", "EUR", false);
        const ShipPtr ship = std::make_shared<Ship>("Mediterranean Star", 200, 8, 400);
        const std::vector<City> ports{City("Naples", italy, 1), City("Palermo", italy, 1)};
        const Tour cruise = makeBaseTour({3, "Mediterranean Cruise", 10, 150000, 30}, City("Rome", italy, 1));
        agency.catalog().addTour(std::make_shared<CruiseTour>(cruise, ship, "Ocean", ports));
    }

    void addSkiTour(TravelAgency &agency)
    {
        const Country austria("Austria", "AT", "EUR", false);
        const Tour ski = makeBaseTour({4, "Tyrol Ski Week", 6, 70000, 12}, City("Innsbruck", austria, 1));
        agency.catalog().addTour(std::make_shared<SkiTour>(ski, "Axamer Lizum", Money(4000), 2));
    }

    void addKyotoTour(TravelAgency &agency, const TourGuidePtr &guide)
    {
        const Country japan("Japan", "JP", "JPY", true);
        const Tour kyoto = makeBaseTour({5, "Kyoto Temples", 5, 120000, 10}, City("Kyoto", japan, 9));
        agency.catalog().addTour(std::make_shared<ExcursionTour>(kyoto, guide, "English", 8));
    }

    void addTours(TravelAgency &agency, const Date &now)
    {
        const TourGuidePtr guide = makeGuide(now);
        addBeachTour(agency);
        addIstanbulTour(agency, guide);
        addCruiseTour(agency);
        addSkiTour(agency);
        addKyotoTour(agency, guide);
    }

    void addDepartures(TravelAgency &agency, const Date &today)
    {
        int offset = kFirstDepartureOffsetDays;
        for (const TourPtr &tour : agency.catalog().tours())
        {
            const Date start = today.addDays(offset);
            agency.catalog().addDeparture(tour, DateRange(start, start.addDays(tour->durationDays())));
            offset += kDepartureStepDays;
        }
    }

    void addOffice(TravelAgency &agency, const Date &today)
    {
        const ManagerPtr manager = std::make_shared<Manager>(1, makePerson("Olga", "Boss", 1980), Money(500000),
                                                             today.addDays(-kYearDays), "Sales", kManagerDiscountLimit);
        Office office("Moscow office", officeAddress(), manager);
        office.hire(makeGuide(today));
        agency.addOffice(office);
    }

    TravelAgency buildDemoAgency(const Date &today)
    {
        const CancellationPolicy policy(kFreeCancellationDays, kEarlyPenaltyPercent, kLatePenaltyPercent, kLateThresholdDays);
        const auto accountant = std::make_shared<Accountant>(2, makePerson("Irina", "Books", 1985), Money(250000),
                                                             today.addDays(-kYearDays), "ACC-2024-17");
        TravelAgency agency("Horizon Travel", policy, accountant);
        addTours(agency, today);
        addDepartures(agency, today);
        addOffice(agency, today);
        const DateRange discountPeriod(today, today.addDays(kYearDays));
        agency.addDiscount(Discount("WELCOME10", kDiscountPercent, discountPeriod, kDiscountUses));
        agency.registerCustomer(makeCustomer(1, "Ivan", "Petrov", today));
        agency.registerCustomer(makeCustomer(2, "Anna", "Smirnova", today));
        return agency;
    }

    PaymentMethod methodFromChoice(int choice)
    {
        switch (choice)
        {
        case kCashMethod:
            return PaymentMethod::Cash;
        case kTransferMethod:
            return PaymentMethod::BankTransfer;
        case kVoucherMethod:
            return PaymentMethod::Voucher;
        default:
            return PaymentMethod::Card;
        }
    }

    class CliApp
    {
    public:
        CliApp(TravelAgency &agency, Date today, std::istream &input, std::ostream &output)
            : agency_(agency), today_(today), input_(input), output_(output)
        {
            commands_ = {
                {1, {"List tours and departures", [this]
                     { listTours(); }}},
                {2, {"Register customer", [this]
                     { registerCustomer(); }}},
                {3, {"Create booking", [this]
                     { createBooking(); }}},
                {4, {"Pay booking", [this]
                     { payBooking(); }}},
                {5, {"Cancel booking", [this]
                     { cancelBooking(); }}},
                {6, {"Apply discount code", [this]
                     { applyDiscount(); }}},
                {7, {"Add review", [this]
                     { addReview(); }}},
                {8, {"Sales report", [this]
                     { showReport(); }}},
                {9, {"Show booking", [this]
                     { showBooking(); }}},
            };
        }

        void run()
        {
            while (input_)
            {
                printMenu();
                int choice = kExitChoice;
                try
                {
                    choice = readInt("Choose");
                }
                catch (const TravelException &)
                {
                    output_ << "Please enter a number\n";
                    continue;
                }
                if (choice == kExitChoice || !input_)
                {
                    break;
                }
                execute(choice);
            }
        }

    private:
        struct Command
        {
            std::string title;
            std::function<void()> handler;
        };

        void printMenu() const
        {
            output_ << "\n=== " << agency_.name() << " (" << today_.toString() << ") ===\n";
            for (const auto &[number, command] : commands_)
            {
                output_ << number << ". " << command.title << "\n";
            }
            output_ << kExitChoice << ". Exit\n";
        }

        std::string readLine(const std::string &prompt)
        {
            output_ << prompt << ": ";
            std::string line;
            std::getline(input_, line);
            return line;
        }

        int readInt(const std::string &prompt)
        {
            const std::string text = readLine(prompt);
            try
            {
                return std::stoi(text);
            }
            catch (const std::logic_error &)
            {
                throw TravelException("Number expected, got '" + text + "'");
            }
        }

        void execute(int choice)
        {
            const auto found = commands_.find(choice);
            if (found == commands_.end())
            {
                output_ << "Unknown command\n";
                return;
            }
            try
            {
                found->second.handler();
            }
            catch (const TravelException &error)
            {
                output_ << "Error: " << error.what() << "\n";
            }
        }

        void listTours()
        {
            for (const DeparturePtr &departure : agency_.catalog().departures())
            {
                output_ << "Departure " << departure->id() << ": " << departure->tour()->describe() << " | "
                        << departure->period().toString() << " | free seats " << departure->freeSeats() << "\n";
            }
        }

        void registerCustomer()
        {
            const int customerId = static_cast<int>(agency_.customers().size()) + 1;
            const std::string firstName = readLine("First name");
            const std::string lastName = readLine("Last name");
            agency_.registerCustomer(makeCustomer(customerId, firstName, lastName, today_));
            output_ << "Registered customer " << customerId << ": " << firstName << " " << lastName << "\n";
        }

        void createBooking()
        {
            const int customerId = readInt("Customer id");
            const int departureId = readInt("Departure id");
            const int participants = readInt("Participants");
            const BookingPtr booking = agency_.createBooking(customerId, departureId, participants, today_);
            output_ << "Booking " << booking->id() << " created, total " << booking->total().toString() << "\n";
        }

        void payBooking()
        {
            const int bookingId = readInt("Booking id");
            const int units = readInt("Amount (whole units)");
            const int method = readInt("Method (1 card, 2 cash, 3 transfer, 4 voucher)");
            agency_.payBooking(bookingId, Money(units * kCentsPerUnit), methodFromChoice(method), today_);
            output_ << "Amount due: " << agency_.findBooking(bookingId)->amountDue().toString() << "\n";
        }

        void cancelBooking()
        {
            const int bookingId = readInt("Booking id");
            const Money refund = agency_.cancelBooking(bookingId, today_);
            output_ << "Booking cancelled, refund " << refund.toString() << "\n";
        }

        void applyDiscount()
        {
            const int bookingId = readInt("Booking id");
            const std::string code = readLine("Discount code");
            agency_.applyDiscountCode(bookingId, code, today_);
            output_ << "New total: " << agency_.findBooking(bookingId)->total().toString() << "\n";
        }

        void addReview()
        {
            const int customerId = readInt("Customer id");
            const int tourId = readInt("Tour id");
            const int rating = readInt("Rating 1-5");
            const std::string text = readLine("Comment");
            const Review review(agency_.findCustomer(customerId), agency_.catalog().findTour(tourId), rating, text);
            agency_.addReview(review);
            output_ << "Thank you! Average rating: " << agency_.averageRating(tourId) << "\n";
        }

        void showReport()
        {
            const DateRange period(today_.addDays(-kReportDays), today_.addDays(kReportDays));
            const SalesReport report = agency_.generateReport("Annual report", period);
            output_ << report.title() << ": bookings " << report.bookingCount() << ", cancelled " << report.cancelledCount()
                    << ", revenue " << report.totalRevenue().toString() << "\n";
        }

        void showBooking()
        {
            const BookingPtr booking = agency_.findBooking(readInt("Booking id"));
            output_ << "Booking " << booking->id() << " for " << booking->customer()->fullName() << ": "
                    << toString(booking->status()) << ", total " << booking->total().toString() << ", paid "
                    << booking->totalPaid().toString() << "\n";
        }

        TravelAgency &agency_;
        Date today_;
        std::istream &input_;
        std::ostream &output_;
        std::map<int, Command> commands_;
    };

    Date currentDate()
    {
        const std::time_t now = std::time(nullptr);
        const std::tm *local = std::localtime(&now);
        return Date(local->tm_year + 1900,
                    static_cast<unsigned>(local->tm_mon + 1),
                    static_cast<unsigned>(local->tm_mday));
    }

}

int main()
{
    const Date today = currentDate();
    TravelAgency agency = buildDemoAgency(today);
    CliApp app(agency, today, std::cin, std::cout);
    app.run();
    return 0;
}
