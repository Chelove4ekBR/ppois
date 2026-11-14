#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Базовое исключение для ошибок предметной области туристической компании.
 */
class TravelException : public std::runtime_error
{
public:
    /**
     * @brief Создаёт объект класса TravelException и инициализирует его исходные данные.
     * @param message текст сообщения.
     */
    explicit TravelException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief Исключение, сообщающее об ошибке или недопустимом значении даты.
 */
class InvalidDateException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недопустимом денежном значении.
 */
class InvalidMoneyException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о попытке выполнить денежную операцию с разными валютами.
 */
class CurrencyMismatchException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о некорректных контактных данных.
 */
class InvalidContactException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о некорректных данных паспорта.
 */
class InvalidPassportException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее об истёкшем сроке действия паспорта.
 */
class ExpiredPassportException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее об отсутствии визы, необходимой для поездки.
 */
class VisaRequiredException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недоступности гида в заданный период.
 */
class GuideUnavailableException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее, что клиент с указанным идентификатором не найден.
 */
class CustomerNotFoundException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее, что тур с указанным идентификатором не найден.
 */
class TourNotFoundException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее об отсутствии свободных мест в туре.
 */
class TourFullException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недоступности номера в указанный период.
 */
class RoomUnavailableException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее, что бронирование с указанным идентификатором не найдено.
 */
class BookingNotFoundException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недопустимой операции для текущего состояния бронирования.
 */
class InvalidBookingStateException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, связанное с ошибкой при обработке платежа.
 */
class PaymentException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недействительной или неприменимой скидке.
 */
class InvalidDiscountException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, связанное с ошибкой обработки страхового полиса или требования.
 */
class InsuranceException : public TravelException
{
public:
    using TravelException::TravelException;
};

/**
 * @brief Исключение, сообщающее о недопустимой оценке.
 */
class InvalidRatingException : public TravelException
{
public:
    using TravelException::TravelException;
};
