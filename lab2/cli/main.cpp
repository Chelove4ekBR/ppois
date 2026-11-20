#include <algorithm>
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "booking/Booking.hpp"
#include "booking/CancellationPolicy.hpp"
#include "booking/Discount.hpp"
#include "booking/Invoice.hpp"
#include "booking/Ledger.hpp"
#include "booking/LedgerEntry.hpp"
#include "booking/Payment.hpp"
#include "booking/SalesReport.hpp"
#include "company/TravelCompany.hpp"
#include "core/Address.hpp"
#include "core/ContactInfo.hpp"
#include "core/Date.hpp"
#include "core/DateRange.hpp"
#include "core/Exceptions.hpp"
#include "core/Money.hpp"
#include "customer/Customer.hpp"
#include "customer/Passport.hpp"
#include "customer/Person.hpp"
#include "customer/Visa.hpp"
#include "geo/Attraction.hpp"
#include "geo/City.hpp"
#include "geo/Country.hpp"
#include "geo/Destination.hpp"
#include "lodging/Hotel.hpp"
#include "service/Complaint.hpp"
#include "service/InsurancePolicy.hpp"
#include "service/Review.hpp"
#include "staff/Accountant.hpp"
#include "staff/Employee.hpp"
#include "staff/Manager.hpp"
#include "staff/Office.hpp"
#include "staff/TourGuide.hpp"
#include "tours/Activity.hpp"
#include "tours/BeachTour.hpp"
#include "tours/CruiseTour.hpp"
#include "tours/ExcursionTour.hpp"
#include "tours/Itinerary.hpp"
#include "tours/ItineraryDay.hpp"
#include "tours/SkiTour.hpp"
#include "tours/Tour.hpp"
#include "tours/TourCatalog.hpp"
#include "tours/TourDeparture.hpp"
#include "transport/Ship.hpp"

namespace
{

    constexpr int kNoDefault = -1;
    constexpr int kCentsPerUsd = 100;

    constexpr int kExitChoice = 0;
    constexpr int kCompanyChoice = 1;
    constexpr int kMaxSetupChoice = 1;
    constexpr int kMaxMainChoice = 26;
    constexpr int kInvalidChoice = -1;

    constexpr int kDefaultBirthYear = 1990;
    constexpr int kDefaultAccountantSalaryUsd = 2500;
    constexpr int kDefaultManagerSalaryUsd = 3000;
    constexpr int kDefaultManagerDiscountLimit = 20;
    constexpr int kDefaultGuideSalaryUsd = 1000;
    constexpr int kDefaultGuideDailyRateUsd = 100;
    constexpr int kPassportValidityYears = 10;

    constexpr int kDefaultTourDays = 7;
    constexpr long long kDefaultTourPriceUsd = 1000;
    constexpr int kDefaultCapacity = 10;
    constexpr int kDefaultHotelStars = 4;
    constexpr int kDefaultHotelRoomNumber = 101;
    constexpr int kDefaultWalkingKm = 5;
    constexpr int kDefaultSkiDifficulty = 2;
    constexpr long long kDefaultSkiPassUsd = 50;
    constexpr int kMinDifficulty = 1;
    constexpr int kMaxDifficulty = 5;
    constexpr int kDefaultDepartureOffsetDays = 30;

    constexpr int kDefaultFreeDays = 30;
    constexpr int kDefaultEarlyPenalty = 20;
    constexpr int kDefaultLatePenalty = 50;
    constexpr int kDefaultLateThreshold = 7;
    constexpr int kDefaultDiscountPercent = 10;
    constexpr int kDefaultDiscountUses = 100;
    constexpr int kDefaultReportDays = 365;

    struct AppState
    {
        explicit AppState(const Date &currentDate) : today(currentDate) {}

        bool hasCompany() const { return agency != nullptr; }

        std::unique_ptr<TravelCompany> agency;
        std::shared_ptr<Accountant> accountant;
        std::vector<TourGuidePtr> guides;
        Date today;
        int nextEmployeeId = 1;
    };

    std::string prompt(const std::string &message, const std::string &defaultValue = "")
    {
        std::cout << message;
        if (!defaultValue.empty())
        {
            std::cout << " [" << defaultValue << "]";
        }
        std::cout << ": ";

        std::string line;
        std::getline(std::cin, line);
        const std::size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
        {
            return defaultValue;
        }
        const std::size_t last = line.find_last_not_of(" \t\r\n");
        return line.substr(first, last - first + 1);
    }

    int promptInt(const std::string &message, int defaultValue = kNoDefault)
    {
        std::string hint = message;
        if (defaultValue != kNoDefault)
        {
            hint += " (по умолчанию " + std::to_string(defaultValue) + ")";
        }

        const std::string text = prompt(hint);
        if (text.empty())
        {
            if (defaultValue != kNoDefault)
            {
                return defaultValue;
            }
            throw TravelException("Ожидалось целое число, но ввод пуст");
        }

        try
        {
            return std::stoi(text);
        }
        catch (const std::exception &)
        {
            throw TravelException("Ожидалось целое число, получено: '" + text + "'");
        }
    }

    long long promptUsdCents(const std::string &message, long long defaultUsd)
    {
        const int amountUsd = promptInt(message + ", USD", static_cast<int>(defaultUsd));
        if (amountUsd < 0)
        {
            throw TravelException("Сумма не может быть отрицательной");
        }
        return static_cast<long long>(amountUsd) * kCentsPerUsd;
    }

    Date promptDate(const std::string &message, const Date &defaultDate)
    {
        const std::string text = prompt(message + " (ГГГГ-ММ-ДД)", defaultDate.toString());

        std::istringstream stream(text);
        int year = 0;
        int month = 0;
        int day = 0;
        char firstDash = 0;
        char secondDash = 0;
        if (!(stream >> year >> firstDash >> month >> secondDash >> day) || firstDash != '-' || secondDash != '-' ||
            month < 1 || month > 12 || day < 1 || day > 31)
        {
            throw TravelException("Ожидалась дата в формате ГГГГ-ММ-ДД, получено: '" + text + "'");
        }
        return Date(year, static_cast<unsigned>(month), static_cast<unsigned>(day));
    }

    bool promptYesNo(const std::string &message, bool defaultValue)
    {
        const std::string text = prompt(message + (defaultValue ? " [Y/n]" : " [y/N]"));
        if (text.empty())
        {
            return defaultValue;
        }
        return text == "y" || text == "Y" || text == "yes" || text == "Yes" ||
               text == "д" || text == "Д" || text == "да" || text == "Да";
    }

    std::vector<std::string> splitByComma(const std::string &text)
    {
        std::vector<std::string> parts;
        std::istringstream stream(text);
        std::string part;
        while (std::getline(stream, part, ','))
        {
            const std::size_t first = part.find_first_not_of(" \t");
            if (first == std::string::npos)
            {
                continue;
            }
            const std::size_t last = part.find_last_not_of(" \t");
            parts.push_back(part.substr(first, last - first + 1));
        }
        return parts;
    }

    Date currentDate()
    {
        const std::time_t now = std::time(nullptr);
        const std::tm *local = std::localtime(&now);
        return Date(local->tm_year + 1900,
                    static_cast<unsigned>(local->tm_mon + 1),
                    static_cast<unsigned>(local->tm_mday));
    }

    void printBanner(const std::string &title)
    {
        std::cout << "\n======================================================================\n"
                  << "  " << title << "\n"
                  << "======================================================================\n";
    }

    void printSectionHeader(const std::string &title)
    {
        constexpr int kLineWidth = 64;
        const int dashCount = std::max(0, kLineWidth - static_cast<int>(title.size()));
        std::cout << "\n--- " << title << " " << std::string(static_cast<std::size_t>(dashCount), '-') << "\n";
    }

    void pause()
    {
        std::cout << "\nНажмите Enter, чтобы вернуться в главное меню...";
        std::string line;
        std::getline(std::cin, line);
    }

    std::string methodToString(PaymentMethod method)
    {
        switch (method)
        {
        case PaymentMethod::Card:
            return "Банковская карта";
        case PaymentMethod::Cash:
            return "Наличные";
        case PaymentMethod::BankTransfer:
            return "Банковский перевод";
        case PaymentMethod::Voucher:
            return "Туристический ваучер";
        }
        return "Неизвестно";
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

    std::string bookingStatusToStringRu(BookingStatus status)
    {
        switch (status)
        {
        case BookingStatus::Pending:
            return "Ожидает подтверждения (Pending)";
        case BookingStatus::Confirmed:
            return "Подтверждено (Confirmed)";
        case BookingStatus::Paid:
            return "Полностью оплачено (Paid)";
        case BookingStatus::Cancelled:
            return "Отменено (Cancelled)";
        case BookingStatus::Completed:
            return "Завершено (Completed)";
        }
        return "Неизвестно";
    }

    std::string join(const std::vector<std::string> &values)
    {
        std::string result;
        for (std::size_t i = 0; i < values.size(); ++i)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += values[i];
        }
        return result;
    }

    Person makeEmployeePerson(const std::string &role)
    {
        const std::string firstName = prompt(role + ": имя", "Сотрудник");
        const std::string lastName = prompt(role + ": фамилия", "Компании");
        const int birthYear = promptInt(role + ": год рождения", kDefaultBirthYear);
        return Person(firstName, lastName, Date(birthYear, 1, 1),
                      ContactInfo("employee@company.example", "+7 900 000 00 00"),
                      Address("Россия", "Москва", "ул. Тверская, д. 1", "125009"));
    }

    std::shared_ptr<Accountant> createAccountant(int employeeId, const Date &today)
    {
        const Person person = makeEmployeePerson("Бухгалтер");
        const int salaryUsd = promptInt("Оклад бухгалтера, USD", kDefaultAccountantSalaryUsd);
        const std::string certificate = prompt("Номер сертификата бухгалтера",
                                               "ACC-" + std::to_string(today.year()) + "-" + std::to_string(employeeId));
        return std::make_shared<Accountant>(employeeId, person,
                                            Money(static_cast<long long>(salaryUsd) * kCentsPerUsd),
                                            today, certificate);
    }

    std::shared_ptr<Manager> createManager(int employeeId, const Date &today)
    {
        const Person person = makeEmployeePerson("Руководитель");
        const int salaryUsd = promptInt("Оклад руководителя, USD", kDefaultManagerSalaryUsd);
        const std::string department = prompt("Отдел руководителя", "Продажи");
        const int discountLimit = promptInt("Лимит скидки руководителя, %", kDefaultManagerDiscountLimit);
        return std::make_shared<Manager>(employeeId, person,
                                         Money(static_cast<long long>(salaryUsd) * kCentsPerUsd),
                                         today, department, discountLimit);
    }

    TourGuidePtr createGuide(int employeeId, const Date &today)
    {
        const Person person = makeEmployeePerson("Гид");
        const int salaryUsd = promptInt("Оклад гида, USD", kDefaultGuideSalaryUsd);
        const std::string languagesText = prompt("Языки гида через запятую", "Русский, English");
        const int dailyRateUsd = promptInt("Ставка гида за день, USD", kDefaultGuideDailyRateUsd);
        return std::make_shared<TourGuide>(employeeId, person,
                                           Money(static_cast<long long>(salaryUsd) * kCentsPerUsd),
                                           today, splitByComma(languagesText),
                                           Money(static_cast<long long>(dailyRateUsd) * kCentsPerUsd));
    }

    CustomerPtr createCustomer(int customerId, const Date &today)
    {
        const std::string firstName = prompt("Имя клиента", "Клиент");
        const std::string lastName = prompt("Фамилия клиента", std::to_string(customerId));
        const int birthYear = promptInt("Год рождения клиента", kDefaultBirthYear);
        const std::string email = prompt("Email клиента", firstName + "." + lastName + "@example.com");
        const std::string phone = prompt("Телефон клиента", "+7 900 000 00 00");
        const std::string country = prompt("Страна клиента", "Россия");
        const std::string city = prompt("Город клиента", "Москва");
        const std::string street = prompt("Улица и дом клиента", "ул. Тверская, д. 1");
        const std::string passportNumber = prompt("Номер паспорта", "P" + std::to_string(customerId) + "000001");
        const std::string passportCountry = prompt("Страна выдачи паспорта", "RU");

        const Person person(firstName, lastName, Date(birthYear, 1, 1), ContactInfo(email, phone),
                            Address(country, city, street, "125009"));
        const Passport passport(passportNumber, passportCountry, firstName + " " + lastName,
                                today.addDays(kPassportValidityYears * 365));
        return std::make_shared<Customer>(customerId, person, passport);
    }

    TourGuidePtr findGuide(const std::vector<TourGuidePtr> &guides, int employeeId)
    {
        for (const auto &guide : guides)
        {
            if (guide->employeeId() == employeeId)
            {
                return guide;
            }
        }
        throw TravelException("Гид с ID " + std::to_string(employeeId) + " не найден");
    }

    std::vector<unsigned> promptBestMonths()
    {
        const std::string text = prompt("Лучшие месяцы (1-12 через запятую)", "1,2,3,4,5,6,7,8,9,10,11,12");
        std::vector<unsigned> months;
        for (const std::string &part : splitByComma(text))
        {
            std::istringstream stream(part);
            int month = 0;
            if (!(stream >> month) || month < 1 || month > 12)
            {
                throw TravelException("Месяц должен быть от 1 до 12: '" + part + "'");
            }
            months.push_back(static_cast<unsigned>(month));
        }
        if (months.empty())
        {
            for (unsigned month = 1; month <= 12; ++month)
            {
                months.push_back(month);
            }
        }
        return months;
    }

    Attraction promptAttraction()
    {
        const std::string name = prompt("Название достопримечательности", "Главная достопримечательность");
        const long long ticketCents = promptUsdCents("Цена билета", 10);
        const int visitMinutes = promptInt("Рекомендуемое время посещения, минут", 60);
        const bool indoor = promptYesNo("Достопримечательность в помещении", false);
        return Attraction(name, Money(ticketCents), visitMinutes, indoor);
    }

    Tour buildBaseTour(int tourId)
    {
        const std::string name = prompt("Название тура", "Новый тур");
        const std::string countryName = prompt("Страна назначения", "Россия");
        const std::string countryCode = prompt("Код страны (ISO)", "RU");
        const std::string countryCurrency = prompt("Валюта страны", "USD");
        const bool visaRequired = promptYesNo("Нужна ли виза для въезда", false);
        const Country country(countryName, countryCode, countryCurrency, visaRequired);

        const std::string cityName = prompt("Город назначения", "Москва");
        const int utcOffset = promptInt("Часовой пояс города (UTC, часы)", 3);
        const City city(cityName, country, utcOffset);

        const std::string description = prompt("Описание направления", "Туристическое направление");
        Destination destination(city, description, promptBestMonths());
        if (promptYesNo("Добавить достопримечательность", false))
        {
            destination.addAttraction(promptAttraction());
        }

        const int durationDays = promptInt("Длительность тура, дней", kDefaultTourDays);
        const long long priceCents = promptUsdCents("Цена на одного человека", kDefaultTourPriceUsd);
        const int capacity = promptInt("Максимальный размер группы", kDefaultCapacity);

        Itinerary itinerary(name + " — программа тура", city);
        const std::string program = prompt("Краткое описание программы (можно пропустить)", "");
        if (!program.empty())
        {
            itinerary.addDay(ItineraryDay(1, program));
        }

        return Tour(tourId, name, destination, durationDays, Money(priceCents), capacity, itinerary);
    }

    TourPtr buildBeachTour(Tour base)
    {
        const City city = base.destination().city();
        const std::string beachName = prompt("Название пляжа", "Городской пляж");
        const std::string hotelName = prompt("Название отеля", "Курортный отель");
        const int stars = promptInt("Количество звёзд отеля", kDefaultHotelStars);
        const std::string street = prompt("Адрес отеля", "Набережная, д. 1");

        const auto hotel = std::make_shared<Hotel>(
            hotelName, stars, Address(city.country().name(), city.name(), street, "000000"));
        const long long defaultRoomUsd =
            base.basePrice().cents() / (kCentsPerUsd * std::max(1, base.durationDays()));
        const long long roomPriceCents = promptUsdCents("Цена номера за ночь", defaultRoomUsd);
        hotel->addRoom(kDefaultHotelRoomNumber, base.capacity(), Money(roomPriceCents));

        const bool snorkeling = promptYesNo("Включить снорклинг в стоимость", false);
        return std::make_shared<BeachTour>(std::move(base), beachName, hotel, snorkeling);
    }

    std::string cabinCategoryFromChoice(int choice)
    {
        switch (choice)
        {
        case 1:
            return "Inside";
        case 2:
            return "Ocean";
        case 3:
            return "Suite";
        default:
            throw TravelException("Категория каюты должна быть 1, 2 или 3");
        }
    }

    TourPtr buildCruiseTour(Tour base)
    {
        const City destinationCity = base.destination().city();
        const std::string shipName = prompt("Название лайнера", "Ocean Star");
        const int cabins = promptInt("Количество кают", 100);
        const int decks = promptInt("Количество палуб", 5);
        const int shipCapacity = promptInt("Вместимость лайнера, пассажиров", base.capacity());
        if (cabins <= 0 || decks <= 0 || shipCapacity <= 0)
        {
            throw TravelException("Количество кают, палуб и мест должно быть положительным");
        }

        const int categoryChoice = promptInt("Категория каюты (1 — Inside, 2 — Ocean, 3 — Suite)", 2);
        const std::string category = cabinCategoryFromChoice(categoryChoice);
        const auto ship = std::make_shared<Ship>(shipName, cabins, decks, shipCapacity);

        std::vector<City> ports;
        const std::string portsText = prompt("Порты захода через запятую (можно пропустить)", "");
        for (const std::string &portName : splitByComma(portsText))
        {
            ports.emplace_back(portName, destinationCity.country(), 0);
        }
        return std::make_shared<CruiseTour>(std::move(base), ship, category, ports);
    }

    TourPtr buildSkiTour(Tour base)
    {
        const City city = base.destination().city();
        const std::string resort = prompt("Название горнолыжного курорта", city.name());
        const long long skiPassCents = promptUsdCents("Ски-пасс за день", kDefaultSkiPassUsd);
        const int difficulty = promptInt("Сложность трасс (1-5)", kDefaultSkiDifficulty);
        if (difficulty < kMinDifficulty || difficulty > kMaxDifficulty)
        {
            throw TravelException("Сложность должна быть от 1 до 5");
        }
        return std::make_shared<SkiTour>(std::move(base), resort, Money(skiPassCents), difficulty);
    }

    TourPtr buildExcursionTour(AppState &state, Tour base)
    {
        if (state.guides.empty())
        {
            throw TravelException("Сначала добавьте гида (пункт 24 главного меню)");
        }
        std::cout << "\nДоступные гиды:\n";
        for (const auto &guide : state.guides)
        {
            std::cout << "  #" << guide->employeeId() << " " << guide->fullName()
                      << " (языки: " << join(guide->languages()) << ")\n";
        }

        const int guideId = promptInt("ID гида");
        const TourGuidePtr guide = findGuide(state.guides, guideId);
        const std::string language = prompt("Язык экскурсии", "Русский");
        const int walkingKm = promptInt("Пешая часть экскурсии, км", kDefaultWalkingKm);
        return std::make_shared<ExcursionTour>(std::move(base), guide, language, walkingKm);
    }

    void printTourList(const std::vector<TourPtr> &tours)
    {
        std::cout << std::left
                  << std::setw(5) << "ID"
                  << std::setw(30) << "Название"
                  << std::setw(20) << "Город"
                  << std::setw(12) << "Дней"
                  << std::setw(18) << "Цена / чел."
                  << "\n"
                  << std::string(85, '-') << "\n";
        for (const auto &tour : tours)
        {
            std::cout << std::left
                      << std::setw(5) << tour->id()
                      << std::setw(30) << tour->name()
                      << std::setw(20) << tour->destination().city().name()
                      << std::setw(12) << tour->durationDays()
                      << std::setw(18) << tour->pricePerPerson().toString()
                      << "\n";
        }
    }

    void showDepartures(const std::vector<DeparturePtr> &departures)
    {
        if (departures.empty())
        {
            std::cout << "\nОтправлений пока нет. Добавьте их через пункт 23 главного меню.\n";
            return;
        }

        std::cout << "\n[ОТПРАВЛЕНИЯ]:\n"
                  << std::left
                  << std::setw(8) << "ID"
                  << std::setw(8) << "Тур"
                  << std::setw(30) << "Название"
                  << std::setw(26) << "Даты поездки"
                  << std::setw(18) << "Свободно"
                  << "\n"
                  << std::string(90, '-') << "\n";
        for (const auto &departure : departures)
        {
            std::cout << std::left
                      << std::setw(8) << departure->id()
                      << std::setw(8) << departure->tour()->id()
                      << std::setw(30) << departure->tour()->name()
                      << std::setw(26) << departure->period().toString()
                      << std::setw(4) << departure->freeSeats() << " из " << departure->tour()->capacity()
                      << (departure->isSoldOut() ? " [МЕСТ НЕТ]" : "")
                      << "\n";
        }
    }

    void printItinerary(const Itinerary &itinerary)
    {
        std::cout << "\n[ПРОГРАММА ПО ДНЯМ (" << itinerary.title() << ")]:\n";
        if (itinerary.days().empty())
        {
            std::cout << "  Программа пока не заполнена.\n";
            return;
        }

        for (const auto &day : itinerary.days())
        {
            std::cout << "  • День " << day.dayNumber() << ": " << day.notes()
                      << " (занятость: " << day.totalMinutes() << " мин., сумма доп. затрат: "
                      << day.totalCost().toString() << ")\n";
            for (const auto &activity : day.activities())
            {
                std::cout << "      - " << activity.describe()
                          << (activity.outdoor() ? " [на свежем воздухе]" : " [в помещении]") << "\n";
            }
        }
    }

    void printAttractions(const Destination &destination)
    {
        if (destination.attractions().empty())
        {
            return;
        }

        std::cout << "\n[ДОСТОПРИМЕЧАТЕЛЬНОСТИ В НАПРАВЛЕНИИ]:\n";
        for (const auto &attraction : destination.attractions())
        {
            std::cout << "  • " << attraction.name() << " (билет: " << attraction.ticketPrice().toString()
                      << ", рекомендуемое время: " << attraction.visitMinutes() << " мин.)\n";
        }
    }

    void printTourSpecifics(const TourPtr &tour)
    {
        if (const auto beach = std::dynamic_pointer_cast<BeachTour>(tour))
        {
            std::cout << "\n[ПЛЯЖНЫЙ ТУР]\n"
                      << "Пляж:        " << beach->beachName() << "\n"
                      << "Отель:       " << beach->hotel()->name() << " (" << beach->hotel()->stars() << " звёзд)\n"
                      << "Адрес отеля: " << beach->hotel()->address().fullText() << "\n"
                      << "Снорклинг:   " << (beach->snorkelingIncluded() ? "Включён в стоимость" : "Не включён") << "\n";
            return;
        }

        if (const auto excursion = std::dynamic_pointer_cast<ExcursionTour>(tour))
        {
            std::cout << "\n[ЭКСКУРСИОННЫЙ ТУР]\n"
                      << "Гид:           " << excursion->guide()->fullName() << "\n"
                      << "Язык тура:     " << excursion->language() << "\n"
                      << "Пеший маршрут: " << excursion->walkingKm() << " км "
                      << (excursion->isEasy() ? "(лёгкий уровень)" : "(насыщенный маршрут)") << "\n";
            return;
        }

        if (const auto cruise = std::dynamic_pointer_cast<CruiseTour>(tour))
        {
            std::cout << "\n[КРУИЗНЫЙ ТУР]\n"
                      << "Лайнер:      " << cruise->ship()->name()
                      << " (вместимость " << cruise->ship()->passengerCapacity() << " пасс., "
                      << cruise->ship()->cabins() << " кают, " << cruise->ship()->decks() << " палуб)\n"
                      << "Класс каюты: " << cruise->cabinCategory() << "\n"
                      << "Порты захода: ";
            for (std::size_t i = 0; i < cruise->ports().size(); ++i)
            {
                std::cout << (i > 0 ? ", " : "") << cruise->ports()[i].name();
            }
            std::cout << "\n";
            return;
        }

        if (const auto ski = std::dynamic_pointer_cast<SkiTour>(tour))
        {
            std::cout << "\n[ГОРНОЛЫЖНЫЙ ТУР]\n"
                      << "Курорт:          " << ski->resortName() << "\n"
                      << "Сложность:       Уровень " << ski->difficulty() << " из 5 "
                      << (ski->isForBeginners() ? "(подходит новичкам)" : "(для опытных лыжников)") << "\n"
                      << "Ски-пасс/день:   " << ski->skiPassPricePerDay().toString() << "\n"
                      << "Ски-пасс на тур: " << ski->skiPassCost().toString() << "\n";
        }
    }

    void handleCreateCompany(AppState &state)
    {
        printSectionHeader("СОЗДАНИЕ ТУРИСТИЧЕСКОЙ КОМПАНИИ");
        const std::string name = prompt("Название компании", "Моя туристическая компания");
        const int freeDays = promptInt("Дней до поездки для бесплатной отмены", kDefaultFreeDays);
        const int earlyPenalty = promptInt("Штраф при ранней отмене, %", kDefaultEarlyPenalty);
        const int latePenalty = promptInt("Штраф при поздней отмене, %", kDefaultLatePenalty);
        const int lateThreshold = promptInt("Порог поздней отмены, дней до поездки", kDefaultLateThreshold);
        if (freeDays < 0 || earlyPenalty < 0 || earlyPenalty > 100 ||
            latePenalty < 0 || latePenalty > 100 || lateThreshold < 0)
        {
            throw TravelException("Параметры политики отмены должны быть в диапазоне 0..100");
        }

        const std::shared_ptr<Accountant> accountant = createAccountant(state.nextEmployeeId, state.today);
        ++state.nextEmployeeId;

        const CancellationPolicy policy(freeDays, earlyPenalty, latePenalty, lateThreshold);
        state.accountant = accountant;
        state.agency = std::make_unique<TravelCompany>(name, policy, accountant);

        std::cout << "\n[OK] Компания «" << name << "» создана. Бухгалтер: " << accountant->fullName() << ".\n"
                  << "Теперь добавьте тур (пункт 22), отправление (пункт 23) и клиента (пункт 7).\n";
    }

    void handleAddTour(AppState &state)
    {
        printSectionHeader("ДОБАВЛЕНИЕ ТУРА");
        std::cout << "  1. Пляжный\n"
                  << "  2. Экскурсионный\n"
                  << "  3. Круизный\n"
                  << "  4. Горнолыжный\n";
        const int type = promptInt("Тип тура (1..4)", 1);
        const int tourId = static_cast<int>(state.agency->catalog().tours().size()) + 1;

        Tour base = buildBaseTour(tourId);
        TourPtr tour;
        switch (type)
        {
        case 1:
            tour = buildBeachTour(std::move(base));
            break;
        case 2:
            tour = buildExcursionTour(state, std::move(base));
            break;
        case 3:
            tour = buildCruiseTour(std::move(base));
            break;
        case 4:
            tour = buildSkiTour(std::move(base));
            break;
        default:
            throw TravelException("Тип тура должен быть от 1 до 4");
        }

        state.agency->catalog().addTour(tour);
        std::cout << "\n[OK] Тур #" << tour->id() << " «" << tour->name() << "» добавлен в каталог.\n"
                  << "Теперь добавьте отправление (пункт 23 главного меню).\n";
    }

    void handleAddDeparture(AppState &state)
    {
        printSectionHeader("ДОБАВЛЕНИЕ ОТПРАВЛЕНИЯ");
        const auto &tours = state.agency->catalog().tours();
        if (tours.empty())
        {
            std::cout << "Сначала добавьте тур (пункт 22 главного меню).\n";
            return;
        }

        printTourList(tours);
        const int tourId = promptInt("ID тура");
        const TourPtr tour = state.agency->catalog().findTour(tourId);
        const Date start = promptDate("Дата начала поездки", state.today.addDays(kDefaultDepartureOffsetDays));
        const DateRange period(start, start.addDays(tour->durationDays()));
        const DeparturePtr departure = state.agency->catalog().addDeparture(tour, period);

        std::cout << "\n[OK] Отправление #" << departure->id() << " для тура «" << tour->name() << "»: "
                  << period.toString() << ", свободно мест: " << departure->freeSeats() << ".\n";
    }

    void handleAddGuide(AppState &state)
    {
        printSectionHeader("ДОБАВЛЕНИЕ ГИДА");
        const TourGuidePtr guide = createGuide(state.nextEmployeeId, state.today);
        ++state.nextEmployeeId;
        state.guides.push_back(guide);
        std::cout << "\n[OK] Гид #" << guide->employeeId() << " " << guide->fullName()
                  << " добавлен (языки: " << join(guide->languages()) << ").\n";
    }

    void handleAddOffice(AppState &state)
    {
        printSectionHeader("ДОБАВЛЕНИЕ ОФИСА");
        const std::string name = prompt("Название офиса", "Центральный офис");
        const std::string country = prompt("Страна офиса", "Россия");
        const std::string city = prompt("Город офиса", "Москва");
        const std::string street = prompt("Улица и дом офиса", "ул. Тверская, д. 1");
        const std::string postalCode = prompt("Индекс офиса", "125009");

        const std::shared_ptr<Manager> manager = createManager(state.nextEmployeeId, state.today);
        ++state.nextEmployeeId;

        Office office(name, Address(country, city, street, postalCode), manager);
        office.hire(state.accountant);
        for (const auto &guide : state.guides)
        {
            office.hire(guide);
        }
        state.agency->addOffice(office);

        std::cout << "\n[OK] Офис «" << name << "» добавлен. Руководитель: " << manager->fullName()
                  << ", сотрудников в офисе: " << office.staffCount() << ".\n";
    }

    void handleAddDiscount(AppState &state)
    {
        printSectionHeader("ДОБАВЛЕНИЕ ПРОМОКОДА");
        const std::string code = prompt("Промокод", "PROMO10");
        const int percent = promptInt("Размер скидки, %", kDefaultDiscountPercent);
        const Date start = promptDate("Действует с", state.today);
        const Date end = promptDate("Действует до", state.today.addDays(kDefaultReportDays));
        const int usageLimit = promptInt("Лимит использований", kDefaultDiscountUses);

        state.agency->addDiscount(Discount(code, percent, DateRange(start, end), usageLimit));
        std::cout << "\n[OK] Промокод «" << code << "» на " << percent << "% добавлен.\n";
    }

    void handleShowCatalog(const AppState &state)
    {
        printSectionHeader("КАТАЛОГ ТУРОВ И ОТПРАВЛЕНИЙ");
        const TourCatalog &catalog = state.agency->catalog();
        if (catalog.tours().empty())
        {
            std::cout << "Каталог пуст. Добавьте тур через пункт 22 главного меню.\n";
            return;
        }
        printTourList(catalog.tours());
        showDepartures(catalog.departures());
    }

    void handleSearchByCity(const AppState &state)
    {
        printSectionHeader("ПОИСК ТУРОВ ПО ГОРОДУ");
        const std::string city = prompt("Введите город назначения");
        if (city.empty())
        {
            std::cout << "Город не указан.\n";
            return;
        }

        const auto results = state.agency->catalog().findByCity(city);
        if (results.empty())
        {
            std::cout << "Туров в город «" << city << "» не найдено.\n";
            return;
        }

        std::cout << "Найдено туров: " << results.size() << "\n";
        for (const auto &tour : results)
        {
            std::cout << " • [#" << tour->id() << "] " << tour->name()
                      << " (" << tour->durationDays() << " дн., "
                      << tour->pricePerPerson().toString() << "/чел., мест: "
                      << tour->capacity() << ")\n";
        }
    }

    void handleSearchByBudget(const AppState &state)
    {
        printSectionHeader("ПОДБОР ТУРОВ ПО БЮДЖЕТУ");
        const int maxUsd = promptInt("Введите максимальный бюджет на человека, USD");
        const Money maxMoney(static_cast<long long>(maxUsd) * kCentsPerUsd);

        const auto results = state.agency->catalog().findAffordable(maxMoney);
        if (results.empty())
        {
            std::cout << "В пределах бюджета " << maxMoney.toString() << " туров не найдено.\n";
            return;
        }

        std::cout << "Туры с ценой не более " << maxMoney.toString() << " (найдено: " << results.size() << "):\n";
        for (const auto &tour : results)
        {
            std::cout << " • [#" << tour->id() << "] " << tour->name()
                      << " — " << tour->pricePerPerson().toString()
                      << " (" << tour->durationDays() << " дн., направление: "
                      << tour->destination().city().name() << ")\n";
        }
    }

    void handleTourDetails(const AppState &state)
    {
        printSectionHeader("ДЕТАЛЬНАЯ ПРОГРАММА И ОПИСАНИЕ ТУРА");
        if (state.agency->catalog().tours().empty())
        {
            std::cout << "Каталог пуст. Добавьте тур через пункт 22 главного меню.\n";
            return;
        }

        const int tourId = promptInt("Введите ID тура");
        const TourPtr tour = state.agency->catalog().findTour(tourId);

        std::cout << "\n=== ТУР #" << tour->id() << ": " << tour->name() << " ===\n"
                  << "Направление:        " << tour->destination().city().name() << ", "
                  << tour->destination().city().country().name() << "\n"
                  << "Описание местности: " << tour->destination().description() << "\n"
                  << "Продолжительность:  " << tour->durationDays() << " дн.\n"
                  << "Базовая цена:       " << tour->pricePerPerson().toString() << " на человека\n"
                  << "Макс. группа:       " << tour->capacity() << " человек\n"
                  << "Сезонность на дату: "
                  << (tour->isSuitableForSeason(state.today) ? "благоприятный сезон" : "не основной сезон") << "\n";

        printTourSpecifics(tour);
        printItinerary(tour->itinerary());
        printAttractions(tour->destination());

        const double rating = state.agency->averageRating(tourId);
        std::cout << "\nСредний рейтинг тура: ";
        if (rating > 0.0)
        {
            std::cout << std::fixed << std::setprecision(1) << rating << " / 5.0\n"
                      << std::defaultfloat;
        }
        else
        {
            std::cout << "Пока нет отзывов\n";
        }
    }

    void handleListCustomers(const AppState &state)
    {
        printSectionHeader("СПИСОК ЗАРЕГИСТРИРОВАННЫХ КЛИЕНТОВ");
        const auto &customers = state.agency->customers();
        if (customers.empty())
        {
            std::cout << "Нет зарегистрированных клиентов. Добавьте клиента через пункт 7.\n";
            return;
        }

        std::cout << std::left
                  << std::setw(5) << "ID"
                  << std::setw(26) << "ФИО клиента"
                  << std::setw(36) << "Электронная почта"
                  << std::setw(16) << "Паспорт"
                  << std::setw(12) << "Уровень"
                  << std::setw(10) << "Баллы"
                  << std::setw(12) << "Скидка"
                  << "\n"
                  << std::string(117, '-') << "\n";

        for (const auto &customer : customers)
        {
            std::cout << std::left
                      << std::setw(5) << customer->customerId()
                      << std::setw(26) << customer->fullName()
                      << std::setw(36) << customer->contact().email()
                      << std::setw(16) << customer->passport().number()
                      << std::setw(12) << customer->loyalty().tier()
                      << std::setw(10) << customer->loyalty().points()
                      << std::setw(12) << (std::to_string(customer->loyalty().discountPercent()) + "%")
                      << "\n";
        }
    }

    void handleCustomerCard(const AppState &state)
    {
        printSectionHeader("КАРТОЧКА КЛИЕНТА");
        const int customerId = promptInt("Введите ID клиента");
        const CustomerPtr customer = state.agency->findCustomer(customerId);

        std::cout << "\n=== КЛИЕНТ #" << customer->customerId() << ": " << customer->fullName() << " ===\n"
                  << "Дата рождения:       " << customer->birthDate().toString()
                  << " (" << customer->ageOn(state.today) << " лет)\n"
                  << "Email:               " << customer->contact().email() << "\n"
                  << "Телефон:             " << customer->contact().phone() << "\n"
                  << "Адрес проживания:    " << customer->address().fullText() << "\n"
                  << "\n[ПАСПОРТНЫЕ ДАННЫЕ]:\n"
                  << "Номер:               " << customer->passport().number() << "\n"
                  << "Страна выдачи:       " << customer->passport().issuingCountry() << "\n"
                  << "ФИО в паспорте:      " << customer->passport().holderName() << "\n"
                  << "Срок действия до:    " << customer->passport().expiry().toString()
                  << " (" << (!customer->passport().isExpiredOn(state.today) ? "Действителен" : "ПРОСРОЧЕН") << ")\n"
                  << "\n[ПРОГРАММА ЛОЯЛЬНОСТИ]:\n"
                  << "Уровень карты:       " << customer->loyalty().tier() << "\n"
                  << "Накопленные баллы:   " << customer->loyalty().points() << " pts\n"
                  << "Сумма покупок:       " << customer->loyalty().lifetimeSpent().toString() << "\n"
                  << "Постоянная скидка:   " << customer->loyalty().discountPercent() << "%\n";

        std::cout << "\n[ДЕЙСТВУЮЩИЕ ВИЗЫ]:\n";
        if (customer->visas().empty())
        {
            std::cout << "  Визы не оформлены.\n";
        }
        else
        {
            for (const auto &visa : customer->visas())
            {
                std::cout << "  • Страна: " << visa.country() << ", тип: " << visa.type()
                          << ", период: " << visa.validity().toString()
                          << " (" << (visa.isActiveOn(state.today) ? "Активна" : "Неактивна") << ")\n";
            }
        }

        std::cout << "\nНепрочитанных системных уведомлений: "
                  << state.agency->unreadNotifications(customerId) << "\n";

        std::cout << "\n[ИСТОРИЯ БРОНИРОВАНИЙ КЛИЕНТА]:\n";
        bool hasBookings = false;
        for (const auto &booking : state.agency->bookings())
        {
            if (booking->customer()->customerId() != customerId)
            {
                continue;
            }
            hasBookings = true;
            std::cout << "  • Бронь #" << booking->id() << ": " << booking->departure()->tour()->name()
                      << " | Даты: " << booking->departure()->period().toString()
                      << " | Сумма: " << booking->total().toString()
                      << " | Статус: " << bookingStatusToStringRu(booking->status()) << "\n";
        }
        if (!hasBookings)
        {
            std::cout << "  Бронирований пока нет.\n";
        }
    }

    void handleRegisterCustomer(AppState &state)
    {
        printSectionHeader("РЕГИСТРАЦИЯ НОВОГО КЛИЕНТА");
        const int customerId = static_cast<int>(state.agency->customers().size()) + 1;
        const CustomerPtr customer = createCustomer(customerId, state.today);
        state.agency->registerCustomer(customer);
        std::cout << "\n[OK] Клиент зарегистрирован под ID: " << customerId
                  << " (" << customer->fullName() << ").\n";
    }

    void handleAddVisa(AppState &state)
    {
        printSectionHeader("ВЫДАЧА И ДОБАВЛЕНИЕ ВИЗЫ");
        const int customerId = promptInt("Введите ID клиента");
        const CustomerPtr customer = state.agency->findCustomer(customerId);

        const std::string countryCode = prompt("Код страны назначения (напр., IT, JP, AT, EG)");
        const std::string visaType = prompt("Тип/категория визы (напр., Tourist Type C)");
        const int durationDays = promptInt("Срок действия визы в днях", 90);

        const DateRange validity(state.today, state.today.addDays(durationDays));
        customer->addVisa(Visa(countryCode, visaType, validity));
        std::cout << "\n[OK] Виза в страну «" << countryCode << "» на срок "
                  << validity.toString() << " привязана к клиенту " << customer->fullName() << ".\n";
    }

    void printCustomerChoices(const AppState &state)
    {
        std::cout << "\nКлиенты:\n";
        for (const auto &customer : state.agency->customers())
        {
            std::cout << "  #" << customer->customerId() << " " << customer->fullName() << "\n";
        }
    }

    void printDepartureChoices(const AppState &state)
    {
        std::cout << "\nОтправления:\n";
        for (const auto &departure : state.agency->catalog().departures())
        {
            std::cout << "  #" << departure->id() << " " << departure->tour()->name()
                      << " (" << departure->period().toString()
                      << ", свободно мест: " << departure->freeSeats() << ")\n";
        }
    }

    void handleCreateBooking(AppState &state)
    {
        printSectionHeader("СОЗДАНИЕ НОВОГО БРОНИРОВАНИЯ");
        if (state.agency->customers().empty())
        {
            std::cout << "Сначала зарегистрируйте клиента (пункт 7 главного меню).\n";
            return;
        }
        if (state.agency->catalog().departures().empty())
        {
            std::cout << "Сначала добавьте тур и отправление (пункты 22 и 23 главного меню).\n";
            return;
        }

        printCustomerChoices(state);
        printDepartureChoices(state);
        const int customerId = promptInt("Введите ID клиента");
        const int departureId = promptInt("Введите ID отправления");
        const int participants = promptInt("Количество участников поездки", 1);

        const BookingPtr booking = state.agency->createBooking(customerId, departureId, participants, state.today);
        std::cout << "\n[OK] Бронирование #" << booking->id() << " создано.\n"
                  << "Тур:          " << booking->departure()->tour()->name() << "\n"
                  << "Даты поездки: " << booking->departure()->period().toString() << "\n"
                  << "Участников:   " << booking->participants() << " чел.\n"
                  << "К оплате:     " << booking->total().toString() << "\n"
                  << "Статус:       " << bookingStatusToStringRu(booking->status()) << "\n";
    }

    void handleBookingDetails(const AppState &state)
    {
        printSectionHeader("ИНФОРМАЦИЯ О БРОНИРОВАНИИ И СЧЁТ");
        const int bookingId = promptInt("Введите ID бронирования");
        const BookingPtr booking = state.agency->findBooking(bookingId);

        std::cout << "\n=== БРОНИРОВАНИЕ #" << booking->id() << " ===\n"
                  << "Дата оформления:  " << booking->createdOn().toString() << "\n"
                  << "Клиент:           #" << booking->customer()->customerId() << " "
                  << booking->customer()->fullName() << " (" << booking->customer()->contact().email() << ")\n"
                  << "Тур:              " << booking->departure()->tour()->name() << "\n"
                  << "Направление:      " << booking->departure()->tour()->destination().city().name() << "\n"
                  << "Даты отправления: " << booking->departure()->period().toString() << " ("
                  << booking->departure()->tour()->durationDays() << " дн.)\n"
                  << "Количество мест:  " << booking->participants() << " чел.\n"
                  << "Текущий статус:   " << bookingStatusToStringRu(booking->status()) << "\n"
                  << "\n[ФИНАНСОВЫЙ БАЛАНС]:\n"
                  << "Итоговая сумма:   " << booking->total().toString() << "\n"
                  << "Оплачено:         " << booking->totalPaid().toString() << "\n"
                  << "Остаток к оплате: " << booking->amountDue().toString() << "\n"
                  << "Статус оплаты:    " << (booking->isFullyPaid() ? "ПОЛНОСТЬЮ ОПЛАЧЕНО" : "ТРЕБУЕТСЯ ДОПЛАТА") << "\n";

        std::cout << "\n[ИСТОРИЯ ПЛАТЕЖЕЙ]:\n";
        if (booking->payments().empty())
        {
            std::cout << "  Платежи пока не поступали.\n";
        }
        else
        {
            for (const auto &payment : booking->payments())
            {
                std::cout << "  • Платёж #" << payment.id() << " от " << payment.date().toString()
                          << " на сумму " << payment.amount().toString()
                          << " (способ: " << methodToString(payment.method()) << ")\n";
            }
        }

        std::cout << "\n[СТРАХОВОЙ ПОЛИС]:\n";
        if (booking->insurance())
        {
            const auto &insurance = booking->insurance();
            std::cout << "  Номер полиса:   " << insurance->policyNumber() << "\n"
                      << "  Застрахованный: " << insurance->insured()->fullName() << "\n"
                      << "  Срок действия:  " << insurance->period().toString() << "\n"
                      << "  Лимит покрытия: " << insurance->coverage().toString() << "\n"
                      << "  Остаток лимита: " << insurance->remainingCoverage().toString() << "\n"
                      << "  Премия:         " << insurance->premium().toString() << "\n";
        }
        else
        {
            std::cout << "  Страховой полис не оформлен.\n";
        }

        const Invoice invoice = state.accountant->issueInvoice(booking, state.today);
        std::cout << "\n[ОФИЦИАЛЬНЫЙ СЧЁТ БУХГАЛТЕРИИ (INVOICE)]:\n"
                  << "  Номер счёта:      № " << invoice.number() << "\n"
                  << "  Дата выставления: " << invoice.issueDate().toString() << "\n"
                  << "  Оплатить до:      " << invoice.dueDate().toString()
                  << " (" << (invoice.isOverdue(state.today) ? "ПРОСРОЧЕН" : "Действителен") << ")\n"
                  << "  Сумма к оплате:   " << invoice.amountDue().toString() << "\n";
    }

    void handlePayBooking(AppState &state)
    {
        printSectionHeader("ОПЛАТА БРОНИРОВАНИЯ");
        const int bookingId = promptInt("Введите ID бронирования");
        const BookingPtr booking = state.agency->findBooking(bookingId);

        std::cout << "Текущий остаток к оплате: " << booking->amountDue().toString() << "\n";
        if (booking->isFullyPaid())
        {
            std::cout << "Бронирование уже полностью оплачено.\n";
            return;
        }

        const int defaultUsd = static_cast<int>(booking->amountDue().cents() / kCentsPerUsd);
        const int payUsd = promptInt("Сумма платежа в USD (целое число)", defaultUsd);
        const Money amount(static_cast<long long>(payUsd) * kCentsPerUsd);

        std::cout << "Выберите способ оплаты:\n"
                  << "  1. Банковская карта (Card)\n"
                  << "  2. Наличный расчёт в кассе (Cash)\n"
                  << "  3. Банковский перевод по реквизитам (BankTransfer)\n"
                  << "  4. Подарочный / туристический ваучер (Voucher)\n";
        const int methodChoice = promptInt("Способ (1..4)", 1);
        const PaymentMethod method = methodFromChoice(methodChoice);

        state.agency->payBooking(bookingId, amount, method, state.today);

        std::cout << "\n[OK] Платёж на сумму " << amount.toString()
                  << " способом «" << methodToString(method) << "» проведён.\n"
                  << "Оставшаяся задолженность: " << booking->amountDue().toString() << "\n"
                  << "Текущий статус: " << bookingStatusToStringRu(booking->status()) << "\n";

        if (booking->isFullyPaid())
        {
            std::cout << "\n>>> Заказ полностью оплачен. Клиенту " << booking->customer()->fullName()
                      << " начислены баллы лояльности (всего: " << booking->customer()->loyalty().points()
                      << " pts, уровень: " << booking->customer()->loyalty().tier() << ").\n";
        }
    }

    void handleAttachInsurance(AppState &state)
    {
        printSectionHeader("ОФОРМЛЕНИЕ СТРАХОВОГО ПОЛИСА");
        const int bookingId = promptInt("Введите ID бронирования");
        const BookingPtr booking = state.agency->findBooking(bookingId);

        if (booking->insurance())
        {
            std::cout << "К бронированию уже прикреплён полис: " << booking->insurance()->policyNumber() << "\n";
            return;
        }

        const int coverageUsd = promptInt("Желаемый лимит страхового покрытия, USD", 50000);
        const Money coverage(static_cast<long long>(coverageUsd) * kCentsPerUsd);
        const int age = booking->customer()->ageOn(state.today);
        const Money premium = InsurancePolicy::premiumFor(booking->total(), age);

        const std::string policyNumber = "POL-" + std::to_string(booking->id()) + "-" +
                                         std::to_string(state.today.year());
        const DateRange period = booking->departure()->period();
        const auto policy = std::make_shared<InsurancePolicy>(policyNumber, booking->customer(),
                                                              coverage, period, premium);
        booking->attachInsurance(policy);

        std::cout << "\n[OK] Страховой полис оформлен и привязан к бронированию.\n"
                  << "Номер полиса:     " << policyNumber << "\n"
                  << "Застрахован:      " << booking->customer()->fullName() << " (" << age << " лет)\n"
                  << "Срок действия:    " << period.toString() << "\n"
                  << "Сумма покрытия:   " << coverage.toString() << "\n"
                  << "Страховая премия: " << premium.toString() << "\n";
    }

    void handleApplyDiscount(AppState &state)
    {
        printSectionHeader("ПРИМЕНЕНИЕ ПРОМОКОДА НА СКИДКУ");
        const int bookingId = promptInt("Введите ID бронирования");
        const BookingPtr booking = state.agency->findBooking(bookingId);
        std::cout << "Текущая сумма бронирования: " << booking->total().toString() << "\n";

        const std::string code = prompt("Введите промокод");
        state.agency->applyDiscountCode(bookingId, code, state.today);

        std::cout << "\n[OK] Промокод «" << code << "» применён.\n"
                  << "Новая сумма к оплате: " << booking->total().toString() << "\n"
                  << "Остаток к оплате:     " << booking->amountDue().toString() << "\n";
    }

    void handleCancelBooking(AppState &state)
    {
        printSectionHeader("ОТМЕНА БРОНИРОВАНИЯ");
        const int bookingId = promptInt("Введите ID отменяемого бронирования");
        const BookingPtr booking = state.agency->findBooking(bookingId);

        std::cout << "Отмена брони #" << booking->id() << " для клиента " << booking->customer()->fullName() << "\n"
                  << "Тур: " << booking->departure()->tour()->name()
                  << " (" << booking->departure()->period().toString() << ")\n"
                  << "Оплаченная сумма: " << booking->totalPaid().toString() << "\n";

        const std::string confirmation = prompt("Вы уверены, что хотите аннулировать заказ? (y/n)");
        if (confirmation != "y" && confirmation != "Y" && confirmation != "да" && confirmation != "ДА")
        {
            std::cout << "Отмена действия. Заказ остался без изменений.\n";
            return;
        }

        const Money refund = state.agency->cancelBooking(bookingId, state.today);
        std::cout << "\n[OK] Бронирование аннулировано.\n"
                  << "Сумма возврата клиенту: " << refund.toString() << "\n"
                  << "Статус бронирования:   " << bookingStatusToStringRu(booking->status()) << "\n";
    }

    void handleAddReview(AppState &state)
    {
        printSectionHeader("ОТЗЫВЫ И РЕЙТИНГ ТУРОВ");
        const int customerId = promptInt("Введите ID клиента, оставляющего отзыв");
        const CustomerPtr customer = state.agency->findCustomer(customerId);
        const int tourId = promptInt("Введите ID тура");
        const TourPtr tour = state.agency->catalog().findTour(tourId);

        const int rating = promptInt("Оценка по шкале от 1 до 5", 5);
        if (rating < 1 || rating > 5)
        {
            std::cout << "Оценка должна быть в диапазоне от 1 до 5.\n";
            return;
        }

        const std::string comment = prompt("Текст отзыва / впечатления о поездке");
        state.agency->addReview(Review(customer, tour, rating, comment));

        const double averageRating = state.agency->averageRating(tourId);
        std::cout << "\n[OK] Отзыв о туре «" << tour->name() << "» добавлен.\n"
                  << "Оценка: " << rating << " / 5. Текст: \"" << comment << "\"\n"
                  << "Обновлённый средний рейтинг: "
                  << std::fixed << std::setprecision(1) << averageRating << " / 5.0\n"
                  << std::defaultfloat;
    }

    void handleFileComplaint(const AppState &state)
    {
        printSectionHeader("РЕГИСТРАЦИЯ ЖАЛОБЫ КЛИЕНТА");
        const int customerId = promptInt("Введите ID клиента");
        const int bookingId = promptInt("Введите ID бронирования");
        const std::string text = prompt("Опишите суть претензии или проблемы");

        const int complaintId = state.agency->fileComplaint(customerId, bookingId, text);
        std::cout << "\n[OK] Жалоба зарегистрирована под номером #" << complaintId << ".\n"
                  << "Обращение передано в отдел контроля качества.\n";
    }

    void handleResolveComplaint(const AppState &state)
    {
        printSectionHeader("РАССМОТРЕНИЕ И УРЕГУЛИРОВАНИЕ ЖАЛОБЫ");
        std::cout << "Открытых нерассмотренных обращений: " << state.agency->openComplaintCount() << "\n";
        const int complaintId = promptInt("Введите ID рассматриваемой жалобы");
        const std::string resolution = prompt("Введите официальное решение / принятые меры по жалобе");

        state.agency->resolveComplaint(complaintId, resolution);
        std::cout << "\n[OK] Жалоба #" << complaintId << " урегулирована и закрыта.\n";
    }

    void handleComplaintStatus(const AppState &state)
    {
        printSectionHeader("СТАТУС ОБРАЩЕНИЙ И ЖАЛОБ КЛИЕНТОВ");
        const std::size_t openCount = state.agency->openComplaintCount();
        std::cout << "Количество активных (нерешённых) жалоб: " << openCount << "\n";
        if (openCount == 0)
        {
            std::cout << "Все поступившие обращения клиентов обработаны и закрыты.\n";
        }
        else
        {
            std::cout << "Требуется урегулировать жалоб через пункт 17: " << openCount << ".\n";
        }
    }

    void handleSalesReport(const AppState &state)
    {
        printSectionHeader("ФИНАНСОВЫЙ ОТЧЁТ О ПРОДАЖАХ (SALES REPORT)");
        std::cout << "Выберите временной период отчёта:\n"
                  << "  1. За последний месяц (30 дней)\n"
                  << "  2. За последние полгода (180 дней)\n"
                  << "  3. За последний год (365 дней)\n"
                  << "  4. За весь доступный диапазон (±1 год от текущей даты)\n";
        const int periodChoice = promptInt("Период (1..4)", 4);

        Date startDate = state.today.addDays(-kDefaultReportDays);
        Date endDate = state.today.addDays(kDefaultReportDays);
        std::string periodName = "Годовой отчёт";

        switch (periodChoice)
        {
        case 1:
            startDate = state.today.addDays(-30);
            endDate = state.today.addDays(30);
            periodName = "Месячный отчёт";
            break;
        case 2:
            startDate = state.today.addDays(-180);
            endDate = state.today.addDays(180);
            periodName = "Полугодовой отчёт";
            break;
        case 3:
            startDate = state.today.addDays(-kDefaultReportDays);
            endDate = state.today;
            periodName = "Отчёт за прошедший год";
            break;
        default:
            break;
        }

        const SalesReport report = state.agency->generateReport(periodName, DateRange(startDate, endDate));
        const std::string topTour = report.bookingCount() == 0 ? "" : report.topTourName();
        std::cout << "\n============================================================\n"
                  << "  " << report.title() << "\n"
                  << "  Диапазон дат:        " << report.period().toString() << "\n"
                  << "============================================================\n"
                  << "Всего бронирований:    " << report.bookingCount() << "\n"
                  << "Аннулировано заказов:  " << report.cancelledCount() << "\n"
                  << "Суммарная выручка:     " << report.totalRevenue().toString() << "\n"
                  << "Средний чек:           " << report.averageBookingValue().toString() << "\n"
                  << "Самый популярный тур:  " << (topTour.empty() ? "Нет данных" : topTour) << "\n"
                  << "============================================================\n";
    }

    void handleLedger(const AppState &state)
    {
        printSectionHeader("БУХГАЛТЕРСКАЯ КНИГА КОМПАНИИ (LEDGER)");
        const Ledger &ledger = state.agency->ledger();

        std::cout << "Финансовый журнал:     " << ledger.name() << "\n"
                  << "Текущий нетто-баланс:  " << ledger.balance().toString() << "\n"
                  << "Общий доход (Income):  " << ledger.totalIncome().toString() << "\n"
                  << "Общие расходы/возврат: " << ledger.totalExpenses().toString() << "\n"
                  << "Всего проводок:        " << ledger.entries().size() << "\n"
                  << "Проводок за сегодня:   " << ledger.entriesOn(state.today) << "\n"
                  << "\n[ПОСЛЕДНИЕ ФИНАНСОВЫЕ ЗАПИСИ]:\n";

        if (ledger.entries().empty())
        {
            std::cout << "  В журнале пока нет проводок.\n";
            return;
        }

        for (const auto &entry : ledger.entries())
        {
            std::cout << "  • " << entry.describe()
                      << (entry.income() ? "  [ДОХОД]" : "  [РАСХОД]") << "\n";
        }
    }

    void handleOffices(const AppState &state)
    {
        printSectionHeader("ОФИСЫ КОМПАНИИ И ШТАТ СОТРУДНИКОВ");
        const auto &offices = state.agency->offices();
        if (offices.empty())
        {
            std::cout << "Офисы пока не добавлены. Добавьте офис через пункт 25 главного меню.\n";
            return;
        }

        for (std::size_t i = 0; i < offices.size(); ++i)
        {
            const Office &office = offices[i];
            std::cout << "=== ОФИС #" << (i + 1) << ": " << office.name() << " ===\n"
                      << "Адрес:            " << office.address().fullText() << "\n"
                      << "Общий ФОТ офиса:  " << office.payroll().toString() << " в месяц\n"
                      << "Штат сотрудников: " << office.staffCount() << " чел.\n";

            if (office.manager())
            {
                const auto &manager = office.manager();
                std::cout << "\n  [РУКОВОДИТЕЛЬ ОФИСА]:\n"
                          << "  ФИО:              " << manager->fullName() << "\n"
                          << "  Роль:             " << manager->describeRole() << "\n"
                          << "  Оклад:            " << manager->salary().toString() << "\n"
                          << "  Лимит скидки:     до " << manager->discountLimitPercent() << "%\n"
                          << "  Размер команды:   " << manager->teamSize() << " чел.\n"
                          << "  Зарплата команды: " << manager->teamPayroll().toString() << "\n";
            }

            std::cout << "\n  [СОТРУДНИКИ ОФИСА]:\n";
            for (const auto &employee : office.staff())
            {
                std::cout << "  • [#" << employee->employeeId() << "] " << employee->fullName()
                          << " — " << employee->describeRole()
                          << ", оклад: " << employee->salary().toString() << "\n";
            }
            std::cout << "\n";
        }
    }

    void showSetupMenu()
    {
        printBanner("СОЗДАНИЕ ТУРИСТИЧЕСКОЙ КОМПАНИИ");
        std::cout << "  1. Создать компанию (название, правила отмены, бухгалтер)\n"
                  << "  0. Выход из программы\n";
    }

    void showMainMenu(const AppState &state)
    {
        const TravelCompany &company = *state.agency;
        std::cout << "\nКомпания: " << company.name()
                  << " | Дата: " << state.today.toString() << "\n"
                  << "Туров: " << company.catalog().tours().size()
                  << " | Отправлений: " << company.catalog().departures().size()
                  << " | Клиентов: " << company.customers().size()
                  << " | Бронирований: " << company.bookings().size() << "\n\n"
                  << "   1. Список всех туров и доступных отправлений\n"
                  << "   2. Поиск туров по городу назначения\n"
                  << "   3. Подбор туров по максимальному бюджету\n"
                  << "   4. Детальная программа тура\n"
                  << "   5. Список зарегистрированных клиентов\n"
                  << "   6. Карточка клиента\n"
                  << "   7. Регистрация нового клиента\n"
                  << "   8. Оформление / привязка визы клиенту\n"
                  << "   9. Создать новое бронирование тура\n"
                  << "  10. Детальная информация о бронировании и счёт-фактура\n"
                  << "  11. Оплатить бронирование\n"
                  << "  12. Оформить и прикрепить страховой полис\n"
                  << "  13. Применить промокод на скидку\n"
                  << "  14. Отменить бронирование с расчётом возврата\n"
                  << "  15. Оставить отзыв о туре и посмотреть средний рейтинг\n"
                  << "  16. Подать официальную жалобу по бронированию\n"
                  << "  17. Рассмотреть жалобу\n"
                  << "  18. Статус обращений и количество открытых жалоб\n"
                  << "  19. Отчёт по продажам за период\n"
                  << "  20. Финансовая книга компании Ledger\n"
                  << "  21. Офисы компании, руководство и штат сотрудников\n"
                  << "  22. Добавить тур\n"
                  << "  23. Добавить отправление тура\n"
                  << "  24. Добавить гида\n"
                  << "  25. Добавить офис\n"
                  << "  26. Добавить промокод на скидку\n"
                  << "   0. Выход из программы\n";
    }

    int readChoice(int maxChoice)
    {
        try
        {
            const int choice = promptInt("Выберите пункт меню (0.." + std::to_string(maxChoice) + ")");
            if (choice < 0 || choice > maxChoice)
            {
                std::cout << "\n[!] Пожалуйста, введите число от 0 до " << maxChoice << ".\n";
                return kInvalidChoice;
            }
            return choice;
        }
        catch (const TravelException &)
        {
            std::cout << "\n[!] Пожалуйста, введите корректное число.\n";
            return kInvalidChoice;
        }
    }

    template <typename Action>
    void executeSafely(Action action)
    {
        try
        {
            action();
        }
        catch (const TravelException &error)
        {
            std::cout << "\n[ОШИБКА ОПЕРАЦИИ]: " << error.what() << "\n";
        }
        catch (const std::exception &error)
        {
            std::cout << "\n[СИСТЕМНАЯ ОШИБКА]: " << error.what() << "\n";
        }
    }

    void executeMainChoice(AppState &state, int choice)
    {
        switch (choice)
        {
        case 1:
            handleShowCatalog(state);
            break;
        case 2:
            handleSearchByCity(state);
            break;
        case 3:
            handleSearchByBudget(state);
            break;
        case 4:
            handleTourDetails(state);
            break;
        case 5:
            handleListCustomers(state);
            break;
        case 6:
            handleCustomerCard(state);
            break;
        case 7:
            handleRegisterCustomer(state);
            break;
        case 8:
            handleAddVisa(state);
            break;
        case 9:
            handleCreateBooking(state);
            break;
        case 10:
            handleBookingDetails(state);
            break;
        case 11:
            handlePayBooking(state);
            break;
        case 12:
            handleAttachInsurance(state);
            break;
        case 13:
            handleApplyDiscount(state);
            break;
        case 14:
            handleCancelBooking(state);
            break;
        case 15:
            handleAddReview(state);
            break;
        case 16:
            handleFileComplaint(state);
            break;
        case 17:
            handleResolveComplaint(state);
            break;
        case 18:
            handleComplaintStatus(state);
            break;
        case 19:
            handleSalesReport(state);
            break;
        case 20:
            handleLedger(state);
            break;
        case 21:
            handleOffices(state);
            break;
        case 22:
            handleAddTour(state);
            break;
        case 23:
            handleAddDeparture(state);
            break;
        case 24:
            handleAddGuide(state);
            break;
        case 25:
            handleAddOffice(state);
            break;
        case 26:
            handleAddDiscount(state);
            break;
        default:
            std::cout << "\n[!] Неизвестная команда '" << choice << "'.\n";
            break;
        }
    }

    void runApplication()
    {
        AppState state(currentDate());
        printBanner("ТУРИСТИЧЕСКАЯ КОМПАНИЯ — КОНСОЛЬНАЯ СИСТЕМА");

        while (true)
        {
            if (!state.hasCompany())
            {
                showSetupMenu();
                const int choice = readChoice(kMaxSetupChoice);
                if (!std::cin || choice == kExitChoice)
                {
                    break;
                }
                if (choice == kCompanyChoice)
                {
                    executeSafely([&state]()
                                  { handleCreateCompany(state); });
                }
                else if (choice != kInvalidChoice)
                {
                    std::cout << "\n[!] Неизвестная команда. Выберите 0 или 1.\n";
                }
                pause();
                continue;
            }

            showMainMenu(state);
            const int choice = readChoice(kMaxMainChoice);
            if (!std::cin || choice == kExitChoice)
            {
                break;
            }
            if (choice != kInvalidChoice)
            {
                executeSafely([&state, choice]()
                              { executeMainChoice(state, choice); });
            }
            pause();
        }

        std::cout << "\nЗавершение работы программы. До свидания!\n";
    }

}

int main()
{
    runApplication();
    return 0;
}
