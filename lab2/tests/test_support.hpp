#pragma once

#include <memory>
#include <string>

#include "Exceptions.hpp"
#include "Accountant.hpp"
#include "Activity.hpp"
#include "Address.hpp"
#include "Airport.hpp"
#include "Attraction.hpp"
#include "BeachTour.hpp"
#include "Booking.hpp"
#include "Bus.hpp"
#include "CancellationPolicy.hpp"
#include "City.hpp"
#include "Complaint.hpp"
#include "ContactInfo.hpp"
#include "Country.hpp"
#include "CruiseTour.hpp"
#include "Customer.hpp"
#include "Date.hpp"
#include "DateRange.hpp"
#include "Destination.hpp"
#include "Discount.hpp"
#include "Driver.hpp"
#include "DrivingLicense.hpp"
#include "Employee.hpp"
#include "ExcursionTour.hpp"
#include "Flight.hpp"
#include "Hotel.hpp"
#include "HotelReservation.hpp"
#include "InsuranceClaim.hpp"
#include "InsurancePolicy.hpp"
#include "Invoice.hpp"
#include "Itinerary.hpp"
#include "ItineraryDay.hpp"
#include "Ledger.hpp"
#include "LedgerEntry.hpp"
#include "LoyaltyAccount.hpp"
#include "Manager.hpp"
#include "MealPlan.hpp"
#include "Money.hpp"
#include "Notification.hpp"
#include "Office.hpp"
#include "Passport.hpp"
#include "Payment.hpp"
#include "Person.hpp"
#include "Review.hpp"
#include "Room.hpp"
#include "SalesReport.hpp"
#include "Ship.hpp"
#include "SkiTour.hpp"
#include "Tour.hpp"
#include "TourCatalog.hpp"
#include "TourDeparture.hpp"
#include "TourGuide.hpp"
#include "Train.hpp"
#include "Transfer.hpp"
#include "Transport.hpp"
#include "TransportTicket.hpp"
#include "TravelAgency.hpp"
#include "TravelGroup.hpp"
#include "Visa.hpp"

namespace support
{

    Date today();
    Date daysAfter(int days);
    Money usd(long long cents);
    Address sampleAddress();
    Country visaFreeCountry();
    Country visaCountry();
    Person makePerson(const std::string &firstName, const std::string &lastName, int birthYear);
    CustomerPtr buildCustomer(int customerId, const std::string &firstName, const std::string &lastName);
    CustomerPtr makeCustomer(int id = 1);
    DriverPtr makeDriver(const std::string &category = "BD");
    BusPtr makeBus(int capacity = 3);
    Airport airportIn(const std::string &code, const Country &country);
    TravelAgency buildAgency(const Date &today);
    TravelAgency makeAgency();

}
