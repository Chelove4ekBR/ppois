#include "TravelAgency.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

TravelAgency::TravelAgency(std::string name, CancellationPolicy policy, std::shared_ptr<Accountant> accountant)
    : name_(std::move(name)),
      policy_(policy),
      accountant_(std::move(accountant)),
      ledger_(name_ + " ledger"),
      nextBookingId_(0),
      nextPaymentId_(0) {}

void TravelAgency::registerCustomer(const CustomerPtr &customer)
{
    customers_.push_back(customer);
}

void TravelAgency::addOffice(const Office &office)
{
    offices_.push_back(office);
}

void TravelAgency::addDiscount(const Discount &discount)
{
    discounts_.push_back(discount);
}

CustomerPtr TravelAgency::findCustomer(int customerId) const
{
    for (const CustomerPtr &customer : customers_)
    {
        if (customer->customerId() == customerId)
        {
            return customer;
        }
    }
    throw CustomerNotFoundException("Customer " + std::to_string(customerId) + " not found");
}

BookingPtr TravelAgency::findBooking(int bookingId) const
{
    for (const BookingPtr &booking : bookings_)
    {
        if (booking->id() == bookingId)
        {
            return booking;
        }
    }
    throw BookingNotFoundException("Booking " + std::to_string(bookingId) + " not found");
}

void TravelAgency::notify(const CustomerPtr &customer, const std::string &message, const Date &date)
{
    notifications_.emplace_back(customer, message, date);
}

BookingPtr TravelAgency::createBooking(int customerId, int departureId, int participants, const Date &today)
{
    const CustomerPtr customer = findCustomer(customerId);
    const DeparturePtr departure = catalog_.findDeparture(departureId);
    const Country &country = departure->tour()->destination().city().country();
    customer->ensureCanEnter(country, departure->period());
    BookingPtr booking = std::make_shared<Booking>(++nextBookingId_, customer, departure, participants, today);
    booking->confirm();
    bookings_.push_back(booking);
    notify(customer, "Booking " + std::to_string(booking->id()) + " confirmed", today);
    return booking;
}

void TravelAgency::applyDiscountCode(int bookingId, const std::string &code, const Date &today)
{
    const BookingPtr booking = findBooking(bookingId);
    for (Discount &discount : discounts_)
    {
        if (discount.code() == code)
        {
            booking->applyDiscount(discount, today);
            return;
        }
    }
    throw InvalidDiscountException("Unknown discount code: " + code);
}

void TravelAgency::payBooking(int bookingId, const Money &amount, PaymentMethod method, const Date &today)
{
    const BookingPtr booking = findBooking(bookingId);
    const Payment payment(++nextPaymentId_, amount, method, today);
    booking->addPayment(payment);
    accountant_->recordPayment(ledger_, payment);
    if (booking->isFullyPaid())
    {
        booking->customer()->loyalty().earnPoints(booking->total());
        notify(booking->customer(), "Booking " + std::to_string(bookingId) + " is fully paid", today);
    }
}

Money TravelAgency::cancelBooking(int bookingId, const Date &today)
{
    const BookingPtr booking = findBooking(bookingId);
    const Money refund = booking->cancel(policy_, today);
    if (refund.cents() > 0)
    {
        accountant_->recordRefund(ledger_, refund, today);
    }
    notify(booking->customer(), "Booking " + std::to_string(bookingId) + " cancelled", today);
    return refund;
}

void TravelAgency::addReview(const Review &review)
{
    reviews_.push_back(review);
}

double TravelAgency::averageRating(int tourId) const
{
    int sum = 0;
    int count = 0;
    for (const Review &review : reviews_)
    {
        if (review.tour()->id() == tourId)
        {
            sum += review.rating();
            ++count;
        }
    }
    return count == 0 ? 0.0 : static_cast<double>(sum) / count;
}

int TravelAgency::fileComplaint(int customerId, int bookingId, const std::string &text)
{
    const CustomerPtr customer = findCustomer(customerId);
    const BookingPtr booking = findBooking(bookingId);
    const int complaintId = static_cast<int>(complaints_.size()) + 1;
    complaints_.emplace_back(complaintId, customer, booking, text);
    return complaintId;
}

void TravelAgency::resolveComplaint(int complaintId, const std::string &resolution)
{
    for (Complaint &complaint : complaints_)
    {
        if (complaint.id() == complaintId)
        {
            complaint.resolve(resolution);
            return;
        }
    }
    throw TravelException("Complaint " + std::to_string(complaintId) + " not found");
}

std::size_t TravelAgency::openComplaintCount() const
{
    return static_cast<std::size_t>(
        std::count_if(complaints_.begin(), complaints_.end(), [](const Complaint &complaint)
                      { return complaint.isOpen(); }));
}

std::size_t TravelAgency::unreadNotifications(int customerId) const
{
    return static_cast<std::size_t>(std::count_if(
        notifications_.begin(), notifications_.end(), [customerId](const Notification &notification)
        { return notification.isUnread() && notification.recipient()->customerId() == customerId; }));
}

SalesReport TravelAgency::generateReport(const std::string &title, const DateRange &period) const
{
    SalesReport report(title, period);
    for (const BookingPtr &booking : bookings_)
    {
        report.add(booking);
    }
    return report;
}
