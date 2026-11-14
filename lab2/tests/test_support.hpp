#pragma once

#include <memory>
#include <string>

#include "core/Exceptions.hpp"
#include "staff/Accountant.hpp"
#include "tours/Activity.hpp"
#include "core/Address.hpp"
#include "geo/Airport.hpp"
#include "geo/Attraction.hpp"
#include "tours/BeachTour.hpp"
#include "booking/Booking.hpp"
#include "transport/Bus.hpp"
#include "booking/CancellationPolicy.hpp"
#include "geo/City.hpp"
#include "service/Complaint.hpp"
#include "core/ContactInfo.hpp"
#include "geo/Country.hpp"
#include "tours/CruiseTour.hpp"
#include "customer/Customer.hpp"
#include "core/Date.hpp"
#include "core/DateRange.hpp"
#include "geo/Destination.hpp"
#include "booking/Discount.hpp"
#include "staff/Driver.hpp"
#include "customer/DrivingLicense.hpp"
#include "staff/Employee.hpp"
#include "tours/ExcursionTour.hpp"
#include "transport/Flight.hpp"
#include "lodging/Hotel.hpp"
#include "lodging/HotelReservation.hpp"
#include "service/InsuranceClaim.hpp"
#include "service/InsurancePolicy.hpp"
#include "booking/Invoice.hpp"
#include "tours/Itinerary.hpp"
#include "tours/ItineraryDay.hpp"
#include "booking/Ledger.hpp"
#include "booking/LedgerEntry.hpp"
#include "customer/LoyaltyAccount.hpp"
#include "staff/Manager.hpp"
#include "lodging/MealPlan.hpp"
#include "core/Money.hpp"
#include "service/Notification.hpp"
#include "staff/Office.hpp"
#include "customer/Passport.hpp"
#include "booking/Payment.hpp"
#include "customer/Person.hpp"
#include "service/Review.hpp"
#include "lodging/Room.hpp"
#include "booking/SalesReport.hpp"
#include "transport/Ship.hpp"
#include "tours/SkiTour.hpp"
#include "tours/Tour.hpp"
#include "tours/TourCatalog.hpp"
#include "tours/TourDeparture.hpp"
#include "staff/TourGuide.hpp"
#include "transport/Train.hpp"
#include "transport/Transfer.hpp"
#include "transport/Transport.hpp"
#include "transport/TransportTicket.hpp"
#include "company/TravelCompany.hpp"
#include "customer/TravelGroup.hpp"
#include "customer/Visa.hpp"

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
    TravelCompany buildAgency(const Date &today);
    TravelCompany makeAgency();

}
