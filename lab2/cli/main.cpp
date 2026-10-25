#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "Accountant.hpp"
#include "BeachTour.hpp"
#include "CruiseTour.hpp"
#include "Customer.hpp"
#include "ExcursionTour.hpp"
#include "Exceptions.hpp"
#include "Hotel.hpp"
#include "Manager.hpp"
#include "Review.hpp"
#include "SalesReport.hpp"
#include "Ship.hpp"
#include "SkiTour.hpp"
#include "TourGuide.hpp"
#include "TravelCompany.hpp"

namespace
{

    Person makePerson(const std::string &first, const std::string &last, int birthYear)
    {
        return Person(first, last, Date(birthYear, 1, 1),
                      ContactInfo(first + "." + last + "@example.com", "+7 900 123 45 67"),
                      Address("Russia", "Moscow", "Tverskaya 1", "125009"));
    }

    CustomerPtr makeCustomer(int id, const std::string &first, const std::string &last, const Date &today)
    {
        Passport passport("P" + std::to_string(id) + "123456", "RU", first + " " + last, today.addDays(3650));
        return std::make_shared<Customer>(id, makePerson(first, last, 1990), passport);
    }

    Tour makeBaseTour(int id, const std::string &name, int days, long long priceCents, int capacity, const City &city)
    {
        Destination dest(city, "Popular destination: " + city.name(), {5, 6, 7, 8, 9});
        dest.addAttraction(Attraction("City highlights", Money(2500), 120, false));
        Itinerary itinerary(name + " itinerary", city);
        ItineraryDay day1(1, "Arrival and welcome dinner");
        day1.addActivity(Activity("Welcome walk", 90, Money(0), true));
        itinerary.addDay(day1);
        return Tour(id, name, dest, days, Money(priceCents), capacity, itinerary);
    }

    TravelCompany buildDemoAgency(const Date &today)
    {
        CancellationPolicy policy(30, 20, 50, 7);
        auto accountant = std::make_shared<Accountant>(2, makePerson("Irina", "Books", 1985), Money(250000),
                                                       today.addDays(-365), "ACC-2024-17");
        TravelCompany agency("Travel Company", policy, accountant);

        auto guide = std::make_shared<TourGuide>(10, makePerson("Elena", "Guide", 1988), Money(300000),
                                                 today.addDays(-365), std::vector<std::string>{"English", "Russian"}, Money(8000));

        // Beach tour
        Country egypt("Egypt", "EG", "EGP", false);
        auto hotel = std::make_shared<Hotel>("Sea Breeze", 5, Address("Egypt", "Hurghada", "Corniche 10", "84511"));
        hotel->addRoom(101, 2, Money(9000));
        hotel->addRoom(102, 3, Money(12000));
        agency.catalog().addTour(std::make_shared<BeachTour>(
            makeBaseTour(1, "Red Sea Relax", 7, 80000, 20, City("Hurghada", egypt, 2)), "Makadi Bay", hotel, true));

        // Istanbul tour
        Country turkey("Turkey", "TR", "TRY", false);
        agency.catalog().addTour(std::make_shared<ExcursionTour>(
            makeBaseTour(2, "Istanbul Classic", 3, 30000, 15, City("Istanbul", turkey, 3)), guide, "English", 4));

        // Cruise tour
        Country italy("Italy", "IT", "EUR", false);
        auto ship = std::make_shared<Ship>("Mediterranean Star", 200, 8, 400);
        agency.catalog().addTour(std::make_shared<CruiseTour>(
            makeBaseTour(3, "Mediterranean Cruise", 10, 150000, 30, City("Rome", italy, 1)),
            ship, "Ocean", std::vector<City>{City("Naples", italy, 1), City("Palermo", italy, 1)}));

        // Ski tour
        Country austria("Austria", "AT", "EUR", false);
        agency.catalog().addTour(std::make_shared<SkiTour>(
            makeBaseTour(4, "Tyrol Ski Week", 6, 70000, 12, City("Innsbruck", austria, 1)), "Axamer Lizum", Money(4000), 2));

        // Kyoto tour
        Country japan("Japan", "JP", "JPY", true);
        agency.catalog().addTour(std::make_shared<ExcursionTour>(
            makeBaseTour(5, "Kyoto Temples", 5, 120000, 10, City("Kyoto", japan, 9)), guide, "English", 8));

        // Departures
        int offset = 30;
        for (const auto &tour : agency.catalog().tours())
        {
            Date start = today.addDays(offset);
            agency.catalog().addDeparture(tour, DateRange(start, start.addDays(tour->durationDays())));
            offset += 10;
        }

        // Office
        auto manager = std::make_shared<Manager>(1, makePerson("Olga", "Boss", 1980), Money(500000),
                                                 today.addDays(-365), "Sales", 25);
        Office office("Moscow office", Address("Russia", "Moscow", "Tverskaya 1", "125009"), manager);
        office.hire(guide);
        agency.addOffice(office);

        // Discounts and customers
        agency.addDiscount(Discount("WELCOME10", 10, DateRange(today, today.addDays(365)), 100));
        agency.registerCustomer(makeCustomer(1, "Ivan", "Petrov", today));
        agency.registerCustomer(makeCustomer(2, "Anna", "Smirnova", today));

        return agency;
    }

    PaymentMethod methodFromChoice(int choice)
    {
        switch (choice)
        {
        case 2:
            return PaymentMethod::Cash;
        case 3:
            return PaymentMethod::BankTransfer;
        case 4:
            return PaymentMethod::Voucher;
        default:
            return PaymentMethod::Card;
        }
    }

    std::string prompt(const std::string &msg)
    {
        std::cout << msg << ": ";
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    int promptInt(const std::string &msg)
    {
        std::string text = prompt(msg);
        try
        {
            return std::stoi(text);
        }
        catch (const std::logic_error &)
        {
            throw TravelException("Number expected, got '" + text + "'");
        }
    }

    Date currentDate()
    {
        std::time_t now = std::time(nullptr);
        std::tm *local = std::localtime(&now);
        return Date(local->tm_year + 1900,
                    static_cast<unsigned>(local->tm_mon + 1),
                    static_cast<unsigned>(local->tm_mday));
    }

}

int main()
{
    const Date today = currentDate();
    TravelCompany agency = buildDemoAgency(today);

    while (std::cin)
    {
        std::cout << "\n"
                  << agency.name() << " (" << today.toString() << ")\n"
                  << "1. List tours and departures\n"
                  << "2. Register customer\n"
                  << "3. Create booking\n"
                  << "4. Pay booking\n"
                  << "5. Cancel booking\n"
                  << "6. Apply discount code\n"
                  << "7. Add review\n"
                  << "8. Sales report\n"
                  << "9. Show booking\n"
                  << "0. Exit\n";

        int choice = 0;
        try
        {
            choice = promptInt("Choose");
        }
        catch (const TravelException &)
        {
            std::cout << "Please enter a number\n";
            continue;
        }

        if (choice == 0 || !std::cin)
        {
            break;
        }

        try
        {
            switch (choice)
            {
            case 1:
                for (const auto &dep : agency.catalog().departures())
                {
                    std::cout << "Departure " << dep->id() << ": " << dep->tour()->describe() << " | "
                              << dep->period().toString() << " | free seats " << dep->freeSeats() << "\n";
                }
                break;

            case 2:
            {
                int id = static_cast<int>(agency.customers().size()) + 1;
                std::string first = prompt("First name");
                std::string last = prompt("Last name");
                agency.registerCustomer(makeCustomer(id, first, last, today));
                std::cout << "Registered customer " << id << ": " << first << " " << last << "\n";
                break;
            }

            case 3:
            {
                int customerId = promptInt("Customer id");
                int departureId = promptInt("Departure id");
                int participants = promptInt("Participants");
                auto booking = agency.createBooking(customerId, departureId, participants, today);
                std::cout << "Booking " << booking->id() << " created, total " << booking->total().toString() << "\n";
                break;
            }

            case 4:
            {
                int bookingId = promptInt("Booking id");
                int units = promptInt("Amount (whole units)");
                int method = promptInt("Method (1 card, 2 cash, 3 transfer, 4 voucher)");
                agency.payBooking(bookingId, Money(units * 100), methodFromChoice(method), today);
                std::cout << "Amount due: " << agency.findBooking(bookingId)->amountDue().toString() << "\n";
                break;
            }

            case 5:
            {
                int bookingId = promptInt("Booking id");
                Money refund = agency.cancelBooking(bookingId, today);
                std::cout << "Booking cancelled, refund " << refund.toString() << "\n";
                break;
            }

            case 6:
            {
                int bookingId = promptInt("Booking id");
                std::string code = prompt("Discount code");
                agency.applyDiscountCode(bookingId, code, today);
                std::cout << "New total: " << agency.findBooking(bookingId)->total().toString() << "\n";
                break;
            }

            case 7:
            {
                int customerId = promptInt("Customer id");
                int tourId = promptInt("Tour id");
                int rating = promptInt("Rating 1-5");
                std::string comment = prompt("Comment");
                agency.addReview(Review(agency.findCustomer(customerId), agency.catalog().findTour(tourId), rating, comment));
                std::cout << "Thank you! Average rating: " << agency.averageRating(tourId) << "\n";
                break;
            }

            case 8:
            {
                SalesReport report = agency.generateReport("Annual report", DateRange(today.addDays(-365), today.addDays(365)));
                std::cout << report.title() << ": bookings " << report.bookingCount()
                          << ", cancelled " << report.cancelledCount()
                          << ", revenue " << report.totalRevenue().toString() << "\n";
                break;
            }

            case 9:
            {
                auto booking = agency.findBooking(promptInt("Booking id"));
                std::cout << "Booking " << booking->id() << " for " << booking->customer()->fullName() << ": "
                          << toString(booking->status()) << ", total " << booking->total().toString()
                          << ", paid " << booking->totalPaid().toString() << "\n";
                break;
            }

            default:
                std::cout << "Unknown command\n";
                break;
            }
        }
        catch (const TravelException &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    return 0;
}
