#include <catch2/catch_test_macros.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::today;
using support::usd;

static TravelCompany makeFixtureAgency() { return support::makeAgency(); }

TEST_CASE("Booking enum toString - status and payment method names", "[booking][enum]")
{
    CHECK(toString(BookingStatus::Pending) == "Pending");
    CHECK(toString(BookingStatus::Confirmed) == "Confirmed");
    CHECK(toString(BookingStatus::Paid) == "Paid");
    CHECK(toString(BookingStatus::Cancelled) == "Cancelled");
    CHECK(toString(BookingStatus::Completed) == "Completed");
    CHECK(toString(static_cast<BookingStatus>(999)) == "Unknown"); // fallback

    CHECK(toString(PaymentMethod::Card) == "Card");
    CHECK(toString(PaymentMethod::Cash) == "Cash");
    CHECK(toString(PaymentMethod::BankTransfer) == "BankTransfer");
    CHECK(toString(PaymentMethod::Voucher) == "Voucher");
    CHECK(toString(static_cast<PaymentMethod>(999)) == "Unknown"); // fallback
}

TEST_CASE("Payment properties", "[booking][payment]")
{
    const Payment payment(7, usd(10000), PaymentMethod::Cash, today());
    CHECK(payment.isCash());
    CHECK(payment.isOnOrBefore(daysAfter(1)));
    CHECK_FALSE(payment.isOnOrBefore(daysAfter(-1)));
    CHECK(payment.refundPortion(80).cents() == 8000);
    CHECK(payment.describe() == "Payment #7 100.00 USD by Cash on 2026-10-21");
    CHECK_FALSE(Payment(8, usd(1), PaymentMethod::Card, today()).isCash());
}

TEST_CASE("Ledger balance and totals", "[booking][ledger]")
{
    Ledger ledger("Main");
    ledger.record(LedgerEntry("Sale", usd(10000), today(), true));
    ledger.record(LedgerEntry("Refund", usd(2500), today(), false));
    ledger.record(LedgerEntry("Sale 2", usd(500), daysAfter(1), true));
    CHECK(ledger.balance().cents() == 8000);
    CHECK(ledger.totalIncome().cents() == 10500);
    CHECK(ledger.totalExpenses().cents() == 2500);
    CHECK(ledger.entriesOn(today()) == 2U);
    CHECK(ledger.entries().front().describe() == "2026-10-21 Sale: 100.00 USD");
    CHECK(ledger.entries()[1].signedAmount().cents() == -2500);
}

TEST_CASE("Discount validity and usage", "[booking][discount]")
{
    Discount discount("PROMO", 10, DateRange(today(), daysAfter(30)), 1);
    CHECK(discount.isValidOn(daysAfter(5)));
    CHECK_FALSE(discount.isValidOn(daysAfter(40)));
    CHECK(discount.apply(usd(10000), today()).cents() == 9000);
    CHECK(discount.remainingUses() == 0);
    CHECK_THROWS_AS(discount.apply(usd(10000), today()), InvalidDiscountException);
    CHECK_THROWS_AS(Discount("BAD", 0, DateRange(today(), daysAfter(1)), 1), InvalidDiscountException);
    CHECK_THROWS_AS(Discount("BAD", 80, DateRange(today(), daysAfter(1)), 1), InvalidDiscountException);
}

TEST_CASE("CancellationPolicy penalty tiers", "[booking][cancellation]")
{
    const CancellationPolicy policy(30, 20, 50, 7);
    const Date start = daysAfter(40);
    CHECK(policy.penaltyPercentFor(today(), start) == 0);
    CHECK(policy.isFreeCancellation(today(), start));
    CHECK(policy.penaltyPercentFor(daysAfter(20), start) == 20);
    CHECK(policy.penaltyPercentFor(daysAfter(37), start) == 50);
    CHECK(policy.penaltyFor(usd(10000), daysAfter(37), start).cents() == 5000);
    CHECK(policy.freeDays() == 30);
}

TEST_CASE("InsurancePolicy claims", "[booking][insurance]")
{
    TravelCompany agency = makeFixtureAgency();
    CustomerPtr customer = support::makeCustomer();
    InsurancePolicy policy("POL-1", customer, usd(100000), DateRange(today(), daysAfter(60)), usd(5000));
    CHECK(InsurancePolicy::premiumFor(usd(100000), 30).cents() == 5000);
    CHECK(InsurancePolicy::premiumFor(usd(100000), 70).cents() == 8000);
    CHECK(policy.isActiveOn(daysAfter(10)));
    policy.registerClaim(usd(30000), daysAfter(10));
    CHECK(policy.remainingCoverage().cents() == 70000);
    CHECK_THROWS_AS(policy.registerClaim(usd(80000), daysAfter(10)), InsuranceException);
    CHECK_THROWS_AS(policy.registerClaim(usd(100), daysAfter(90)), InsuranceException);
}

TEST_CASE("InsuranceClaim processing", "[booking][insurance]")
{
    CustomerPtr customer = support::makeCustomer();
    const auto policy = std::make_shared<InsurancePolicy>("POL-1", customer, usd(100000), DateRange(today(), daysAfter(60)), usd(5000));
    InsuranceClaim approved(1, policy, usd(20000), "Lost luggage");
    CHECK(approved.isPending());
    approved.approve(today());
    CHECK(approved.status() == ClaimStatus::Approved);
    CHECK(policy->claimsTotal().cents() == 20000);
    CHECK_THROWS_AS(approved.approve(today()), InsuranceException);
    InsuranceClaim rejected(2, policy, usd(100), "Dubious");
    rejected.reject();
    CHECK(rejected.status() == ClaimStatus::Rejected);
    CHECK_THROWS_AS(rejected.reject(), InsuranceException);
}

TEST_CASE("Booking construction reserves seats", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 3, today());
    CHECK(departure->bookedSeats() == 3);
    CHECK(booking->total().cents() == 255000);
    CHECK(booking->status() == BookingStatus::Pending);
    CHECK_THROWS_AS(Booking(2, customer, departure, 0, today()), TravelException);
    CHECK_THROWS_AS(Booking(2, customer, departure, 99, today()), TourFullException);
}

TEST_CASE("Booking payment lifecycle", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    const Payment early(1, usd(1000), PaymentMethod::Card, today());
    CHECK_THROWS_AS(booking->addPayment(early), InvalidBookingStateException);
    booking->confirm();
    CHECK_THROWS_AS(booking->confirm(), InvalidBookingStateException);
    booking->addPayment(early);
    CHECK(booking->amountDue().cents() == 84000);
    CHECK_THROWS_AS(booking->addPayment(Payment(2, usd(90000), PaymentMethod::Card, today())), PaymentException);
    booking->addPayment(Payment(3, usd(84000), PaymentMethod::Card, today()));
    CHECK(booking->isFullyPaid());
    CHECK(booking->status() == BookingStatus::Paid);
    CHECK(booking->payments().size() == 2U);
    CHECK_THROWS_AS(booking->addPayment(Payment(4, usd(1), PaymentMethod::Card, today())), PaymentException);
}

TEST_CASE("Booking discount rules", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    Discount discount("PROMO", 10, DateRange(today(), daysAfter(30)), 5);
    booking->applyDiscount(discount, today());
    CHECK(booking->total().cents() == 76500);
    booking->confirm();
    booking->addPayment(Payment(1, usd(100), PaymentMethod::Card, today()));
    CHECK_THROWS_AS(booking->applyDiscount(discount, today()), InvalidBookingStateException);
}

TEST_CASE("Cancelled booking rejects discount", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    CancellationPolicy policy = agency.policy();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    booking->cancel(policy, today());
    Discount discount("PROMO", 10, DateRange(today(), daysAfter(30)), 5);
    CHECK_THROWS_AS(booking->applyDiscount(discount, today()), InvalidBookingStateException);
}

TEST_CASE("Booking insurance must belong to customer", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    const auto own = std::make_shared<InsurancePolicy>("P1", customer, usd(1), DateRange(today(), daysAfter(1)), usd(1));
    const auto foreign = std::make_shared<InsurancePolicy>("P2", support::makeCustomer(2), usd(1), DateRange(today(), daysAfter(1)), usd(1));
    booking->attachInsurance(own);
    CHECK(booking->insurance()->policyNumber() == "P1");
    CHECK_THROWS_AS(booking->attachInsurance(foreign), InsuranceException);
}

TEST_CASE("Booking cancellation refunds according to policy", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    CancellationPolicy policy = agency.policy();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    booking->confirm();
    booking->addPayment(Payment(1, usd(40000), PaymentMethod::Card, today()));
    const Money refund = booking->cancel(policy, daysAfter(15));
    CHECK(refund.cents() == 32000);
    CHECK(booking->status() == BookingStatus::Cancelled);
    CHECK(departure->bookedSeats() == 0);
    CHECK_THROWS_AS(booking->cancel(policy, today()), InvalidBookingStateException);
}

TEST_CASE("Booking completion requires finished paid trip", "[booking]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    CancellationPolicy policy = agency.policy();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    CHECK_THROWS_AS(booking->complete(today()), InvalidBookingStateException);
    booking->confirm();
    booking->addPayment(Payment(1, booking->total(), PaymentMethod::Card, today()));
    CHECK_THROWS_AS(booking->complete(today()), InvalidBookingStateException);
    booking->complete(daysAfter(100));
    CHECK(booking->status() == BookingStatus::Completed);
    CHECK_THROWS_AS(booking->cancel(policy, daysAfter(101)), InvalidBookingStateException);
}

TEST_CASE("Invoice tracks overdue amounts", "[booking][invoice]")
{
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    booking->confirm();
    const Invoice invoice(1, booking, today(), daysAfter(14));
    CHECK(invoice.amountDue().cents() == 85000);
    CHECK_FALSE(invoice.isOverdue(daysAfter(14)));
    CHECK(invoice.isOverdue(daysAfter(20)));
    CHECK(invoice.daysOverdue(daysAfter(20)) == 6);
    CHECK(invoice.daysOverdue(today()) == 0);
    CHECK(invoice.describe() == "Invoice #1 for booking 1, due 2026-11-04, amount 850.00 USD");
    booking->addPayment(Payment(1, booking->total(), PaymentMethod::Card, today()));
    CHECK_FALSE(invoice.isOverdue(daysAfter(20)));
}

TEST_CASE("Accountant records financial events", "[booking][accountant]")
{
    Accountant accountant(2, support::makePerson("Irina", "Books", 1985), usd(250000), daysAfter(-400), "ACC-1");
    Ledger ledger("Books");
    TravelCompany agency = makeFixtureAgency();
    DeparturePtr departure = agency.catalog().findDeparture(1);
    CustomerPtr customer = support::makeCustomer();
    const BookingPtr booking = std::make_shared<Booking>(1, customer, departure, 1, today());
    const Invoice first = accountant.issueInvoice(booking, today());
    const Invoice second = accountant.issueInvoice(booking, today());
    CHECK(first.number() == 1);
    CHECK(second.number() == 2);
    CHECK(second.dueDate().toString() == "2026-11-04");
    accountant.recordPayment(ledger, Payment(1, usd(5000), PaymentMethod::Card, today()));
    accountant.recordRefund(ledger, usd(1000), today());
    CHECK(ledger.balance().cents() == 4000);
    CHECK(accountant.processedPayments() == 1);
    CHECK(accountant.describeRole() == "Accountant Irina Books (certificate ACC-1)");
}

TEST_CASE("Ledger empty handling", "[booking][ledger]")
{
    Ledger emptyLedger("Empty");
    CHECK(emptyLedger.balance().cents() == 0);
    CHECK(emptyLedger.totalIncome().cents() == 0);
    CHECK(emptyLedger.totalExpenses().cents() == 0);
    CHECK(emptyLedger.entriesOn(today()) == 0U);
}