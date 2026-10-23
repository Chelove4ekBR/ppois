#pragma once

#include <stdexcept>
#include <string>

class TravelException : public std::runtime_error {
public:
    explicit TravelException(const std::string& message) : std::runtime_error(message) {}
};

class InvalidDateException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidMoneyException : public TravelException {
public:
    using TravelException::TravelException;
};

class CurrencyMismatchException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidContactException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidPassportException : public TravelException {
public:
    using TravelException::TravelException;
};

class ExpiredPassportException : public TravelException {
public:
    using TravelException::TravelException;
};

class VisaRequiredException : public TravelException {
public:
    using TravelException::TravelException;
};

class GuideUnavailableException : public TravelException {
public:
    using TravelException::TravelException;
};

class CustomerNotFoundException : public TravelException {
public:
    using TravelException::TravelException;
};

class TourNotFoundException : public TravelException {
public:
    using TravelException::TravelException;
};

class TourFullException : public TravelException {
public:
    using TravelException::TravelException;
};

class RoomUnavailableException : public TravelException {
public:
    using TravelException::TravelException;
};

class BookingNotFoundException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidBookingStateException : public TravelException {
public:
    using TravelException::TravelException;
};

class PaymentException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidDiscountException : public TravelException {
public:
    using TravelException::TravelException;
};

class InsuranceException : public TravelException {
public:
    using TravelException::TravelException;
};

class InvalidRatingException : public TravelException {
public:
    using TravelException::TravelException;
};
