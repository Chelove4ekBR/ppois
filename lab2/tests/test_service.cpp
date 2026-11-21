#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::today;
using support::usd;

static BookingPtr bookingFor(TravelCompany &agency, int customerId = 1, int departureId = 1, int participants = 2)
{
    return agency.createBooking(customerId, departureId, participants, today());
}

TEST_CASE("Review validates rating and summarizes", "[service]")
{
    TravelCompany agency = support::makeAgency();
    const TourPtr tour = agency.catalog().findTour(1);
    const Review review(support::makeCustomer(), tour, 5, "Great");
    CHECK(review.isPositive());
    CHECK(review.isByCustomer(1));
    CHECK_FALSE(review.isByCustomer(2));
    CHECK(review.summary() == "Red Sea Relax: 5/5 by Ivan Petrov");
    CHECK_FALSE(Review(support::makeCustomer(), tour, 2, "Meh").isPositive());
    CHECK_THROWS_AS(Review(support::makeCustomer(), tour, 0, "x"), InvalidRatingException);
    CHECK_THROWS_AS(Review(support::makeCustomer(), tour, 6, "x"), InvalidRatingException);
}

TEST_CASE("Complaint resolution flow", "[service]")
{
    TravelCompany agency = support::makeAgency();
    const BookingPtr booking = bookingFor(agency);
    Complaint complaint(1, support::makeCustomer(), booking, "Noisy hotel");
    CHECK(complaint.isOpen());
    CHECK(complaint.describe() == "Complaint #1 about booking 1 (open)");
    complaint.resolve("Compensation paid");
    CHECK_FALSE(complaint.isOpen());
    CHECK(complaint.resolved());
    CHECK(complaint.describe() == "Complaint #1 about booking 1 (resolved: Compensation paid)");
}

TEST_CASE("Notification read state and preview", "[service]")
{
    Notification note(support::makeCustomer(), "Your booking is confirmed for next month", today());
    CHECK(note.isUnread());
    note.markRead();
    CHECK_FALSE(note.isUnread());
    CHECK(note.preview(100) == "Your booking is confirmed for next month");
    CHECK(note.preview(12) == "Your booking...");
}

TEST_CASE("Office staff and payroll", "[service]")
{
    const auto manager = std::make_shared<Manager>(1, support::makePerson("Olga", "Boss", 1980), usd(500000), daysAfter(-500), "Sales", 25);
    Office office("Main", support::sampleAddress(), manager);
    office.hire(std::make_shared<Employee>(3, support::makePerson("Max", "Clerk", 1985), "Clerk", usd(100000), daysAfter(-50)));
    CHECK(office.staffCount() == 1U);
    CHECK(office.hasStaff(3));
    CHECK_FALSE(office.hasStaff(4));
    CHECK(office.payroll().cents() == 600000);
    CHECK(office.manager()->teamSize() == 1U);
}

TEST_CASE("Agency booking happy path", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    const BookingPtr booking = bookingFor(agency);
    CHECK(booking->status() == BookingStatus::Confirmed);
    CHECK(agency.findBooking(booking->id()) == booking);
    CHECK(agency.unreadNotifications(1) == 1U);
    agency.payBooking(booking->id(), usd(100000), PaymentMethod::Card, today());
    CHECK_FALSE(booking->isFullyPaid());
    agency.payBooking(booking->id(), usd(70000), PaymentMethod::BankTransfer, today());
    CHECK(booking->status() == BookingStatus::Paid);
    CHECK(agency.ledger().balance().cents() == 170000);
    CHECK(agency.findCustomer(1)->loyalty().points() == 170);
    CHECK(agency.unreadNotifications(1) == 2U);
}

TEST_CASE("Agency lookup failures use dedicated exceptions", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    CHECK_THROWS_AS(agency.findCustomer(99), CustomerNotFoundException);
    CHECK_THROWS_AS(agency.findBooking(99), BookingNotFoundException);
    CHECK_THROWS_AS(bookingFor(agency, 99), CustomerNotFoundException);
    CHECK_THROWS_AS(bookingFor(agency, 1, 99), TourNotFoundException);
}

TEST_CASE("Agency visa is checked before booking", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    CHECK_THROWS_AS(bookingFor(agency, 1, 5, 1), VisaRequiredException);
    agency.findCustomer(1)->addVisa(Visa("JP", "tourist", DateRange(today(), daysAfter(200))));
    CHECK_NOTHROW(bookingFor(agency, 1, 5, 1));
}

TEST_CASE("Agency discount codes", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    const BookingPtr booking = bookingFor(agency, 1, 1, 1);
    agency.applyDiscountCode(booking->id(), "WELCOME10", today());
    CHECK(booking->total().cents() == 76500);
    CHECK_THROWS_AS(agency.applyDiscountCode(booking->id(), "UNKNOWN", today()), InvalidDiscountException);
}

TEST_CASE("Agency cancellation refunds and logs expense", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    const BookingPtr booking = bookingFor(agency, 1, 1, 1);
    agency.payBooking(booking->id(), usd(50000), PaymentMethod::Cash, today());
    const Money refund = agency.cancelBooking(booking->id(), today());
    CHECK(refund.cents() == 50000);
    CHECK(agency.ledger().balance().cents() == 0);
    const BookingPtr unpaid = bookingFor(agency, 2, 2, 1);
    CHECK(agency.cancelBooking(unpaid->id(), today()).cents() == 0);
    CHECK_THROWS_AS(agency.cancelBooking(unpaid->id(), today()), InvalidBookingStateException);
}

TEST_CASE("Agency reviews and complaints", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    const TourPtr tour = agency.catalog().findTour(1);
    CHECK(agency.averageRating(1) == 0.0);
    agency.addReview(Review(agency.findCustomer(1), tour, 5, "Great"));
    agency.addReview(Review(agency.findCustomer(2), tour, 4, "Good"));
    agency.addReview(Review(agency.findCustomer(2), agency.catalog().findTour(2), 1, "Bad"));
    CHECK(agency.averageRating(1) == Catch::Approx(4.5));
    const BookingPtr booking = bookingFor(agency);
    const int complaintId = agency.fileComplaint(1, booking->id(), "Late bus");
    CHECK(agency.openComplaintCount() == 1U);
    agency.resolveComplaint(complaintId, "Apology");
    CHECK(agency.openComplaintCount() == 0U);
    CHECK_THROWS_AS(agency.resolveComplaint(99, "x"), TravelException);
}

TEST_CASE("Agency offices are registered", "[agency]")
{
    TravelCompany agency = support::makeAgency();
    CHECK(agency.offices().size() == 1U);
    CHECK(agency.customers().size() == 2U);
    CHECK(agency.name() == "Horizon Travel");
}

TEST_CASE("SalesReport aggregates bookings", "[salesreport]")
{
    TravelCompany agency = support::makeAgency();
    const BookingPtr first = bookingFor(agency, 1, 1, 1);
    const BookingPtr second = bookingFor(agency, 2, 2, 1);
    agency.payBooking(first->id(), usd(85000), PaymentMethod::Card, today());
    agency.payBooking(second->id(), usd(10000), PaymentMethod::Card, today());
    agency.cancelBooking(second->id(), today());
    const SalesReport report = agency.generateReport("Q4", DateRange(daysAfter(-1), daysAfter(1)));
    CHECK(report.bookingCount() == 2U);
    CHECK(report.cancelledCount() == 1U);
    CHECK(report.totalRevenue().cents() == 85000);
    CHECK(report.averageBookingValue().cents() == 42500);
    CHECK(report.topTourName() == "Red Sea Relax");
    CHECK(report.title() == "Q4");
}

TEST_CASE("SalesReport empty and out-of-period reports", "[salesreport]")
{
    TravelCompany agency = support::makeAgency();
    SalesReport report("Empty", DateRange(daysAfter(-10), daysAfter(-5)));
    CHECK(report.averageBookingValue().cents() == 0);
    CHECK_THROWS_AS(report.topTourName(), TravelException);
    CHECK_FALSE(report.add(bookingFor(agency)));
}

TEST_CASE("SalesReport aggregates bookings with repeated tours", "[salesreport]")
{
    TravelCompany agency = support::makeAgency();
    // Два бронирования одного и того же тура (id 1)
    const BookingPtr first = bookingFor(agency, 1, 1, 1);
    const BookingPtr second = bookingFor(agency, 2, 1, 1);
    agency.payBooking(first->id(), usd(85000), PaymentMethod::Card, today());
    agency.payBooking(second->id(), usd(85000), PaymentMethod::Card, today());

    const SalesReport report = agency.generateReport("Repeated", DateRange(daysAfter(-1), daysAfter(1)));
    CHECK(report.bookingCount() == 2U);
    CHECK(report.topTourName() == "Red Sea Relax");
}

TEST_CASE("Manager without subordinates", "[service]")
{
    Manager soloManager(10, support::makePerson("Solo", "Boss", 1980), usd(300000), today(), "SoloDept", 20);
    CHECK(soloManager.teamSize() == 0U);
    CHECK(soloManager.teamPayroll().cents() == 300000);
}