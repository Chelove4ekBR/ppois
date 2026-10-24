#include <catch2/catch_test_macros.hpp>

#include "test_support.hpp"

using support::daysAfter;
using support::today;
using support::usd;

static const DateRange kWeek(daysAfter(10), daysAfter(17));

TEST_CASE("MealPlan cost and description", "[lodging]") {
    const MealPlan halfBoard("Half board", usd(2000), 2);
    CHECK(halfBoard.costFor(2, 7).cents() == 28000);
    CHECK_FALSE(halfBoard.isFullBoard());
    CHECK(MealPlan("Full", usd(3000), 3).isFullBoard());
    CHECK(halfBoard.describe() == "Half board: 2 meals/day, 20.00 USD");
}

TEST_CASE("Room reservation lifecycle", "[lodging]") {
    Room room(101, 2, usd(9000));
    CHECK(room.fits(2));
    CHECK_FALSE(room.fits(3));
    CHECK(room.costFor(kWeek).cents() == 63000);
    room.reserve(kWeek);
    CHECK_FALSE(room.isAvailable(DateRange(daysAfter(15), daysAfter(20))));
    CHECK(room.isAvailable(DateRange(daysAfter(17), daysAfter(20))));
    CHECK_THROWS_AS(room.reserve(DateRange(daysAfter(15), daysAfter(20))), RoomUnavailableException);
    room.cancelReservation(kWeek);
    CHECK(room.isAvailable(kWeek));
    CHECK(room.bookedPeriods().empty());
}

TEST_CASE("Hotel finds rooms and aggregates", "[lodging]") {
    Hotel hotel("Sea Breeze", 5, support::sampleAddress());
    CHECK(hotel.averagePrice().cents() == 0);
    const RoomPtr small = hotel.addRoom(101, 2, usd(9000));
    hotel.addRoom(102, 4, usd(12000));
    CHECK(hotel.averagePrice().cents() == 10500);
    CHECK(hotel.isLuxury());
    CHECK(hotel.findAvailableRoom(kWeek, 2)->number() == 101);
    small->reserve(kWeek);
    CHECK(hotel.findAvailableRoom(kWeek, 2)->number() == 102);
    CHECK(hotel.countAvailable(kWeek) == 1U);
    CHECK_THROWS_AS(hotel.findAvailableRoom(kWeek, 6), RoomUnavailableException);
    CHECK_FALSE(Hotel("Motel", 2, support::sampleAddress()).isLuxury());
}

TEST_CASE("HotelReservation reserves and cancels", "[lodging]") {
    const auto hotel = std::make_shared<Hotel>("Sea Breeze", 4, support::sampleAddress());
    const RoomPtr room = hotel->addRoom(101, 2, usd(9000));
    HotelReservation reservation(1, hotel, room, support::makeCustomer(), kWeek, MealPlan("Half", usd(2000), 2), 2);
    CHECK(reservation.nights() == 7);
    CHECK(reservation.totalCost().cents() == 63000 + 28000);
    CHECK_FALSE(room->isAvailable(kWeek));
    CHECK_THROWS_AS(HotelReservation(2, hotel, room, support::makeCustomer(), kWeek, MealPlan("Half", usd(2000), 2), 2),
                    RoomUnavailableException);
    reservation.cancel();
    CHECK(room->isAvailable(kWeek));
}

TEST_CASE("Transport seat sales and refunds", "[transport]") {
    Transport transport(1, "AirOne", 2, usd(10000));
    CHECK(transport.sellSeat() == 1);
    CHECK(transport.sellSeat() == 2);
    CHECK(transport.isFull());
    CHECK_THROWS_AS(transport.sellSeat(), TourFullException);
    transport.refundSeat();
    CHECK(transport.freeSeats() == 1);
    CHECK(transport.ticketPrice().cents() == 10000);
    CHECK(transport.describe() == "AirOne #1, free seats 1");
    Transport empty(2, "None", 1, usd(1));
    empty.refundSeat();
    CHECK(empty.soldSeats() == 0);
}

TEST_CASE("Flight international flights include taxes", "[transport]") {
    const Airport cairo = support::airportIn("CAI", support::visaFreeCountry());
    const Airport hurghada = support::airportIn("HRG", support::visaFreeCountry());
    const Airport osaka = support::airportIn("KIX", support::visaCountry());
    const Flight domestic(1, "AirOne", 100, usd(10000), "AO1", cairo, hurghada, daysAfter(5), 20);
    const Flight international(2, "AirOne", 100, usd(10000), "AO2", cairo, osaka, daysAfter(5), 20);
    CHECK_FALSE(domestic.isInternational());
    CHECK(domestic.ticketPrice().cents() == 10000);
    CHECK(international.isInternational());
    CHECK(international.ticketPrice().cents() == 11200);
    CHECK(international.luggageFee(25).cents() == 7500);
    CHECK(international.luggageFee(10).cents() == 0);
    CHECK(international.describe() == "Flight AO2 CAI->KIX on 2026-10-26");
}

TEST_CASE("Train sleeper surcharge", "[transport]") {
    const Train seated(1, "RZD", 400, usd(8000), "T7", 8, false);
    const Train sleeper(2, "RZD", 400, usd(8000), "T8", 8, true);
    CHECK(seated.ticketPrice().cents() == 8000);
    CHECK(sleeper.ticketPrice().cents() == 10000);
    CHECK(seated.seatsPerCarriage() == 50);
    CHECK(seated.describe() == "Train T7, 8 carriages");
}

TEST_CASE("Bus driver qualification", "[transport]") {
    const BusPtr bus = support::makeBus();
    CHECK(bus->isDriverQualified(support::today()));
    const Bus unqualified(6, "CityBus", 20, usd(1000), "B777BB", support::makeDriver("B"), false);
    CHECK_FALSE(unqualified.isDriverQualified(support::today()));
    CHECK(bus->describe() == "Bus A123BC driven by Boris Wheel");
}

TEST_CASE("Ship capacity math", "[transport]") {
    const Ship ship("Star", 200, 8, 400);
    CHECK(ship.cabinsPerDeck() == 25);
    CHECK(ship.canHost(400));
    CHECK_FALSE(ship.canHost(401));
    CHECK(ship.describe() == "Ship Star, 200 cabins on 8 decks");
}

TEST_CASE("TransportTicket assigns seat and cancels", "[transport]") {
    const TransportPtr bus = support::makeBus(2);
    TransportTicket ticket(bus, support::makeCustomer(3));
    CHECK(ticket.seatNumber() == 1);
    CHECK(ticket.price().cents() == 1000);
    CHECK(ticket.isForPassenger(3));
    CHECK_FALSE(ticket.isForPassenger(4));
    CHECK(ticket.describe() == "Ivan Petrov, seat 1, 10.00 USD");
    ticket.cancel();
    CHECK(bus->freeSeats() == 2);
}

TEST_CASE("Transfer pricing and capacity", "[transport]") {
    const Transfer transfer(support::sampleAddress(), Address("Russia", "Moscow", "Airport 1", "141400"),
                            support::makeBus(3), daysAfter(3), usd(2500));
    CHECK(transfer.isIntraCity());
    CHECK(transfer.priceFor(2).cents() == 5000);
    CHECK_THROWS_AS(transfer.priceFor(4), TourFullException);
    CHECK(transfer.describe() == "Transfer Moscow -> Moscow on 2026-10-24");
    const Transfer intercity(support::sampleAddress(), Address("Russia", "Kazan", "Station 1", "420000"),
                             support::makeBus(), daysAfter(3), usd(9000));
    CHECK_FALSE(intercity.isIntraCity());
}
