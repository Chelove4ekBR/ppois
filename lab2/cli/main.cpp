#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "staff/Accountant.hpp"
#include "tours/Activity.hpp"
#include "core/Address.hpp"
#include "geo/Attraction.hpp"
#include "tours/BeachTour.hpp"
#include "booking/Booking.hpp"
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
#include "staff/Employee.hpp"
#include "core/Exceptions.hpp"
#include "tours/ExcursionTour.hpp"
#include "lodging/Hotel.hpp"
#include "service/InsurancePolicy.hpp"
#include "booking/Invoice.hpp"
#include "tours/Itinerary.hpp"
#include "tours/ItineraryDay.hpp"
#include "booking/Ledger.hpp"
#include "booking/LedgerEntry.hpp"
#include "customer/LoyaltyAccount.hpp"
#include "staff/Manager.hpp"
#include "core/Money.hpp"
#include "staff/Office.hpp"
#include "customer/Passport.hpp"
#include "booking/Payment.hpp"
#include "customer/Person.hpp"
#include "service/Review.hpp"
#include "booking/SalesReport.hpp"
#include "transport/Ship.hpp"
#include "tours/SkiTour.hpp"
#include "tours/Tour.hpp"
#include "tours/TourCatalog.hpp"
#include "tours/TourDeparture.hpp"
#include "staff/TourGuide.hpp"
#include "company/TravelCompany.hpp"
#include "customer/Visa.hpp"

namespace
{

    struct AppState
    {
        TravelCompany agency;
        std::shared_ptr<Accountant> accountant;
        Date today;
    };

    Person makePerson(const std::string &first, const std::string &last, int birthYear,
                      const std::string &email = "", const std::string &phone = "")
    {
        std::string e = email.empty() ? (first + "." + last + "@example.com") : email;
        std::string p = phone.empty() ? "+7 900 123 45 67" : phone;
        return Person(first, last, Date(birthYear, 1, 1),
                      ContactInfo(e, p),
                      Address("Россия", "Москва", "ул. Тверская, д. 1", "125009"));
    }

    CustomerPtr makeCustomer(int id, const std::string &first, const std::string &last,
                             int birthYear, const std::string &email, const std::string &phone,
                             const std::string &passportNumber, const Date &today)
    {
        Passport passport(passportNumber.empty() ? ("P" + std::to_string(id) + "789012") : passportNumber,
                          "RU", first + " " + last, today.addDays(3650));
        return std::make_shared<Customer>(id, makePerson(first, last, birthYear, email, phone), passport);
    }

    Tour makeBaseTour(int id, const std::string &name, int days, long long priceCents, int capacity,
                      const City &city, const std::string &destDesc, const std::string &attrName,
                      long long attrPriceCents, int attrMinutes)
    {
        Destination dest(city, destDesc, {5, 6, 7, 8, 9, 10});
        dest.addAttraction(Attraction(attrName, Money(attrPriceCents), attrMinutes, false));

        Itinerary itinerary(name + " — программа тура", city);
        ItineraryDay day1(1, "День 1: Прибытие, трансфер и приветственный ужин");
        day1.addActivity(Activity("Обзорная пешая прогулка", 90, Money(0), true));
        day1.addActivity(Activity("Приветственный гастрономический ужин", 120, Money(3500), false));
        itinerary.addDay(day1);

        ItineraryDay day2(2, "День 2: Основная экскурсионная программа");
        day2.addActivity(Activity("Посещение главных достопримечательностей", 180, Money(2500), true));
        itinerary.addDay(day2);

        return Tour(id, name, dest, days, Money(priceCents), capacity, itinerary);
    }

    AppState buildDemoAgency(const Date &today)
    {
        CancellationPolicy policy(30, 20, 50, 7);
        auto accountant = std::make_shared<Accountant>(2, makePerson("Ирина", "Книгина", 1985), Money(250000),
                                                       today.addDays(-365), "ACC-2024-RUS-17");
        TravelCompany agency("Travel Company Deluxe", policy, accountant);

        auto guideElena = std::make_shared<TourGuide>(10, makePerson("Елена", "Гидман", 1988), Money(300000),
                                                      today.addDays(-365), std::vector<std::string>{"English", "Russian", "Turkish"}, Money(8000));

        // 1. Пляжный тур (Хургада, Египет)
        Country egypt("Египет", "EG", "EGP", false);
        auto hotel = std::make_shared<Hotel>("Sea Breeze Resort & Spa", 5,
                                             Address("Египет", "Хургада", "Корниш Роуд 10", "84511"));
        hotel->addRoom(101, 2, Money(9000));
        hotel->addRoom(102, 3, Money(12000));
        agency.catalog().addTour(std::make_shared<BeachTour>(
            makeBaseTour(1, "Red Sea Relax", 7, 80000, 20, City("Hurghada", egypt, 2),
                         "Курортная зона Макади Бэй на побережье Красного моря",
                         "Морской заповедник коралловых рифов", 4000, 180),
            "Макади Бэй", hotel, true));

        // 2. Экскурсионный тур (Стамбул, Турция)
        Country turkey("Турция", "TR", "TRY", false);
        agency.catalog().addTour(std::make_shared<ExcursionTour>(
            makeBaseTour(2, "Istanbul Classic", 3, 30000, 15, City("Istanbul", turkey, 3),
                         "Исторический центр Стамбула (Султанахмет)",
                         "Собор Святой Софии и Голубая мечеть", 3000, 120),
            guideElena, "Russian", 5));

        // 3. Круизный тур (Средиземное море, Италия)
        Country italy("Италия", "IT", "EUR", false);
        auto ship = std::make_shared<Ship>("Mediterranean Star", 250, 8, 500);
        agency.catalog().addTour(std::make_shared<CruiseTour>(
            makeBaseTour(3, "Mediterranean Cruise", 10, 150000, 30, City("Rome", italy, 1),
                         "Круизный маршрут по жемчужинам Средиземноморья",
                         "Колизей и Римский форум", 5000, 180),
            ship, "Ocean View Balcony", std::vector<City>{City("Naples", italy, 1), City("Palermo", italy, 1)}));

        // 4. Горнолыжный тур (Инсбрук, Австрия)
        Country austria("Австрия", "AT", "EUR", false);
        agency.catalog().addTour(std::make_shared<SkiTour>(
            makeBaseTour(4, "Tyrol Ski Week", 6, 70000, 12, City("Innsbruck", austria, 1),
                         "Альпийский горнолыжный регион Тироль",
                         "Смотровой комплекс Нордкетте", 4500, 90),
            "Axamer Lizum", Money(4000), 2));

        // 5. Экскурсионный тур (Киото, Япония)
        Country japan("Япония", "JP", "JPY", true);
        agency.catalog().addTour(std::make_shared<ExcursionTour>(
            makeBaseTour(5, "Kyoto Temples", 5, 120000, 10, City("Kyoto", japan, 9),
                         "Древняя столица Японии с тысячью храмов",
                         "Храмовый ансамбль Кинкаку-дзи", 2000, 150),
            guideElena, "English", 8));

        // Отправления по турам
        int offset = 15;
        for (const auto &tour : agency.catalog().tours())
        {
            Date start = today.addDays(offset);
            agency.catalog().addDeparture(tour, DateRange(start, start.addDays(tour->durationDays())));
            offset += 10;
        }

        // Офис компании и сотрудники
        auto manager = std::make_shared<Manager>(1, makePerson("Ольга", "Руководитель", 1980), Money(500000),
                                                 today.addDays(-730), "Отдел продаж и обслуживания", 25);
        Office office("Центральный офис в Москве", Address("Россия", "Москва", "ул. Тверская, д. 1", "125009"), manager);
        office.hire(guideElena);
        office.hire(accountant);
        agency.addOffice(office);

        // Промокоды
        agency.addDiscount(Discount("WELCOME10", 10, DateRange(today.addDays(-30), today.addDays(365)), 100));
        agency.addDiscount(Discount("SUMMER2026", 15, DateRange(today.addDays(-30), today.addDays(365)), 50));
        agency.addDiscount(Discount("VIP20", 20, DateRange(today.addDays(-30), today.addDays(365)), 20));

        // Клиенты
        auto c1 = makeCustomer(1, "Иван", "Петров", 1990, "ivan.petrov@mail.ru", "+7 916 111 22 33", "P1778899", today);
        auto c2 = makeCustomer(2, "Анна", "Смирнова", 1993, "anna.smirnova@yandex.ru", "+7 926 444 55 66", "P2889900", today);
        auto c3 = makeCustomer(3, "Сергей", "Иванов", 1985, "sergey.ivanov@gmail.com", "+7 903 777 88 99", "P3990011", today);

        // Бонусы и виза для Анны
        c2->loyalty().earnPoints(Money(60000)); // серебряный уровень
        c2->addVisa(Visa("IT", "Schengen Type C", DateRange(today.addDays(-30), today.addDays(150))));

        agency.registerCustomer(c1);
        agency.registerCustomer(c2);
        agency.registerCustomer(c3);

        // Демо-бронирование
        auto demoBooking = agency.createBooking(1, 2, 1, today.addDays(-5));
        agency.payBooking(demoBooking->id(), Money(15000), PaymentMethod::Card, today.addDays(-3));

        // Демо-отзывы
        agency.addReview(Review(c2, agency.catalog().findTour(1), 5,
                                "Прекрасный отель Sea Breeze и великолепный снорклинг! Всё организовано безупречно."));
        agency.addReview(Review(c1, agency.catalog().findTour(2), 4,
                                "Очень насыщенный маршрут по Стамбулу. Гид Елена — настоящий профессионал."));

        // Демо-жалоба
        agency.fileComplaint(1, demoBooking->id(), "Хотелось бы иметь чуть больше свободного времени во второй день тура.");

        return {agency, accountant, today};
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
        // trim leading/trailing whitespace
        const auto strBegin = line.find_first_not_of(" \t\r\n");
        if (strBegin == std::string::npos)
        {
            return "";
        }
        const auto strEnd = line.find_last_not_of(" \t\r\n");
        return line.substr(strBegin, strEnd - strBegin + 1);
    }

    int promptInt(const std::string &msg, int defaultVal = -1)
    {
        std::string hint = msg;
        if (defaultVal != -1)
        {
            hint += " [по умолчанию " + std::to_string(defaultVal) + "]";
        }
        std::string text = prompt(hint);
        if (text.empty() && defaultVal != -1)
        {
            return defaultVal;
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

    Date currentDate()
    {
        std::time_t now = std::time(nullptr);
        std::tm *local = std::localtime(&now);
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
        std::cout << "\n--- " << title << " "
                  << std::string(std::max(0, 64 - static_cast<int>(title.size())), '-') << "\n";
    }

    // Меню и операции

    void showMainMenu(const TravelCompany &agency, const Date &today)
    {
        std::cout << "        Текущая системная дата: " << today.toString() << "\n"
                  << "  1. Список всех туров и доступных отправлений\n"
                  << "  2. Поиск туров по городу назначения\n"
                  << "  3. Подбор туров по максимальному бюджету\n"
                  << "  4. Детальная программа тура (маршрут, отель, гид, лайнер)\n"
                  << "  5. Список зарегистрированных клиентов\n"
                  << "  6. Карточка клиента (паспорт, баланс лояльности, визы, сообщения)\n"
                  << "  7. Регистрация нового клиента\n"
                  << "  8. Оформление / привязка визы клиенту\n"
                  << "  9. Создать новое бронирование тура\n"
                  << " 10. Детальная информация о бронировании и счёт-фактура (Invoice)\n"
                  << " 11. Оплатить бронирование (карта / наличные / перевод / ваучер)\n"
                  << " 12. Оформить и прикрепить страховой полис к бронированию\n"
                  << " 13. Применить промокод на скидку\n"
                  << " 14. Отменить бронирование с расчётом возврата средств\n"
                  << " 15. Оставить отзыв о туре и посмотреть средний рейтинг\n"
                  << " 16. Подать официальную жалобу по бронированию\n"
                  << " 17. Рассмотреть жалобу (зафиксировать решение менеджера)\n"
                  << " 18. Статус обращений и количество открытых жалоб\n"
                  << " 19. Отчёт по продажам за период (SalesReport)\n"
                  << " 20. Финансовая книга компании Ledger (баланс, доходы, расходы)\n"
                  << " 21. Офисы компании, руководство и штат сотрудников\n"
                  << "  0. Выход из программы\n";
    }

    void handleListDepartures(const TravelCompany &agency)
    {
        printSectionHeader("КАТАЛОГ ТУРОВ И ОТПРАВЛЕНИЙ");
        const auto &departures = agency.catalog().departures();
        if (departures.empty())
        {
            std::cout << "В каталоге пока нет доступных отправлений.\n";
            return;
        }

        std::cout << std::left
                  << std::setw(6) << "ID отпр."
                  << std::setw(6) << "ID тура"
                  << std::setw(24) << "Название тура"
                  << std::setw(15) << "Город"
                  << std::setw(26) << "Даты поездки"
                  << std::setw(14) << "Цена / чел."
                  << std::setw(12) << "Своб. мест"
                  << "\n"
                  << std::string(103, '-') << "\n";

        for (const auto &dep : departures)
        {
            const auto tour = dep->tour();
            std::cout << std::left
                      << std::setw(6) << dep->id()
                      << std::setw(6) << tour->id()
                      << std::setw(24) << tour->name()
                      << std::setw(15) << tour->destination().city().name()
                      << std::setw(26) << dep->period().toString()
                      << std::setw(14) << tour->pricePerPerson().toString()
                      << std::setw(4) << dep->freeSeats() << " из " << tour->capacity()
                      << (dep->isSoldOut() ? " [МЕСТ НЕТ]" : "")
                      << "\n";
        }
    }

    void handleSearchByCity(const TravelCompany &agency)
    {
        printSectionHeader("ПОИСК ТУРОВ ПО ГОРОДУ");
        std::string city = prompt("Введите город назначения (напр., Rome, Hurghada, Istanbul, Kyoto, Innsbruck)");
        if (city.empty())
        {
            std::cout << "Город не указан.\n";
            return;
        }

        auto results = agency.catalog().findByCity(city);
        if (results.empty())
        {
            std::cout << "Туров в город «" << city << "» не найдено.\n";
            return;
        }

        std::cout << "Найдено туров: " << results.size() << "\n";
        for (const auto &tour : results)
        {
            std::cout << " • [# " << tour->id() << "] " << tour->name()
                      << " (" << tour->durationDays() << " дн., "
                      << tour->pricePerPerson().toString() << "/чел., вместимость: "
                      << tour->capacity() << " чел.)\n";
        }
    }

    void handleSearchByBudget(const TravelCompany &agency)
    {
        printSectionHeader("ПОДБОР ТУРОВ ПО БЮДЖЕТУ");
        int maxUsd = promptInt("Введите максимальный бюджет на человека в USD (целое число)");
        Money maxMoney(static_cast<long long>(maxUsd) * 100);

        auto results = agency.catalog().findAffordable(maxMoney);
        if (results.empty())
        {
            std::cout << "В пределах бюджета " << maxMoney.toString() << " туров не найдено.\n";
            return;
        }

        std::cout << "Туры с ценой не более " << maxMoney.toString() << " (найдено: " << results.size() << "):\n";
        for (const auto &tour : results)
        {
            std::cout << " • [# " << tour->id() << "] " << tour->name()
                      << " — " << tour->pricePerPerson().toString()
                      << " (" << tour->durationDays() << " дн., направление: "
                      << tour->destination().city().name() << ")\n";
        }
    }

    void handleTourDetails(const TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ДЕТАЛЬНАЯ ПРОГРАММА И ОПИСАНИЕ ТУРА");
        int tourId = promptInt("Введите ID интересующего тура (1..5)");
        auto tour = agency.catalog().findTour(tourId);

        std::cout << "\n=== ТУР #" << tour->id() << ": " << tour->name() << " ===\n"
                  << "Направление:        " << tour->destination().city().name() << ", "
                  << tour->destination().city().country().name() << "\n"
                  << "Описание местности: " << tour->destination().description() << "\n"
                  << "Продолжительность:  " << tour->durationDays() << " дн.\n"
                  << "Базовая цена:       " << tour->pricePerPerson().toString() << " на 1 человека\n"
                  << "Макс. группа:       " << tour->capacity() << " человек\n"
                  << "Сезонность на дату: " << (tour->isSuitableForSeason(today) ? "Да, благоприятный сезон" : "Не является основным сезоном") << "\n";

        // Специфика подтипа тура
        if (auto beach = std::dynamic_pointer_cast<BeachTour>(tour))
        {
            std::cout << "\n[ПЛЯЖНЫЙ ТУР]\n"
                      << "Пляж:        " << beach->beachName() << "\n"
                      << "Отель:       " << beach->hotel()->name() << " (" << beach->hotel()->stars() << " звёзд)\n"
                      << "Адрес отеля: " << beach->hotel()->address().fullText() << "\n"
                      << "Снорклинг:   " << (beach->snorkelingIncluded() ? "Включён в стоимость пакета" : "Не включён") << "\n";
        }
        else if (auto exc = std::dynamic_pointer_cast<ExcursionTour>(tour))
        {
            std::cout << "\n[ЭКСКУРСИОННЫЙ ТУР]\n"
                      << "Гид:            " << exc->guide()->fullName() << "\n"
                      << "Язык тура:      " << exc->language() << "\n"
                      << "Пеший маршрут:  " << exc->walkingKm() << " км "
                      << (exc->isEasy() ? "(лёгкий уровень сложности)" : "(насыщенный пеший маршрут)") << "\n";
        }
        else if (auto cruise = std::dynamic_pointer_cast<CruiseTour>(tour))
        {
            std::cout << "\n[КРУИЗНЫЙ ТУР]\n"
                      << "Лайнер:         " << cruise->ship()->name()
                      << " (вместимость " << cruise->ship()->passengerCapacity() << " пасс., "
                      << cruise->ship()->cabins() << " кают, "
                      << cruise->ship()->decks() << " палуб)\n"
                      << "Класс каюты:    " << cruise->cabinCategory() << "\n"
                      << "Порты захода:   ";
            for (std::size_t i = 0; i < cruise->ports().size(); ++i)
            {
                std::cout << (i > 0 ? ", " : "") << cruise->ports()[i].name();
            }
            std::cout << "\n";
        }
        else if (auto ski = std::dynamic_pointer_cast<SkiTour>(tour))
        {
            std::cout << "\n[ГОРНОЛЫЖНЫЙ ТУР]\n"
                      << "Курорт:         " << ski->resortName() << "\n"
                      << "Сложность:      Уровень " << ski->difficulty() << " из 5 "
                      << (ski->isForBeginners() ? "(подходит новичкам)" : "(для опытных лыжников)") << "\n"
                      << "Ски-пасс/день:  " << ski->skiPassPricePerDay().toString() << "\n"
                      << "Ски-пасс на тур:" << ski->skiPassCost().toString() << "\n";
        }

        // Программа по дням (Itinerary)
        std::cout << "\n[ПРОГРАММА ПО ДНЯМ (" << tour->itinerary().title() << ")]:\n";
        for (const auto &day : tour->itinerary().days())
        {
            std::cout << "  • День " << day.dayNumber() << ": " << day.notes()
                      << " (занятость: " << day.totalMinutes() << " мин., сумма доп. затрат: "
                      << day.totalCost().toString() << ")\n";
            for (const auto &act : day.activities())
            {
                std::cout << "      - " << act.describe()
                          << (act.outdoor() ? " [на свежем воздухе]" : " [в помещении]") << "\n";
            }
        }

        // Достопримечательности
        const auto &attrs = tour->destination().attractions();
        if (!attrs.empty())
        {
            std::cout << "\n[ДОСТОПРИМЕЧАТЕЛЬНОСТИ В НАПРАВЛЕНИИ]:\n";
            for (const auto &attr : attrs)
            {
                std::cout << "  • " << attr.name() << " (билет: " << attr.ticketPrice().toString()
                          << ", рекомендуемое время: " << attr.visitMinutes() << " мин.)\n";
            }
        }

        double rating = agency.averageRating(tourId);
        std::cout << "\nСредний рейтинг тура по отзывам: "
                  << (rating > 0.0 ? (std::to_string(rating).substr(0, 4) + " / 5.0") : "Пока нет отзывов") << "\n";
    }

    void handleListCustomers(const TravelCompany &agency)
    {
        printSectionHeader("СПИСОК ЗАРЕГИСТРИРОВАННЫХ КЛИЕНТОВ");
        const auto &customers = agency.customers();
        if (customers.empty())
        {
            std::cout << "Нет зарегистрированных клиентов.\n";
            return;
        }

        std::cout << std::left
                  << std::setw(5) << "ID"
                  << std::setw(22) << "ФИО Клиента"
                  << std::setw(26) << "Электронная почта"
                  << std::setw(12) << "Паспорт"
                  << std::setw(10) << "Уровень"
                  << std::setw(8) << "Баллы"
                  << std::setw(10) << "Скидка"
                  << "\n"
                  << std::string(93, '-') << "\n";

        for (const auto &c : customers)
        {
            std::cout << std::left
                      << std::setw(5) << c->customerId()
                      << std::setw(22) << c->fullName()
                      << std::setw(26) << c->contact().email()
                      << std::setw(12) << c->passport().number()
                      << std::setw(10) << c->loyalty().tier()
                      << std::setw(8) << c->loyalty().points()
                      << std::setw(10) << (std::to_string(c->loyalty().discountPercent()) + "%")
                      << "\n";
        }
    }

    void handleCustomerCard(const TravelCompany &agency, const Date &today)
    {
        printSectionHeader("КАРТОЧКА КЛИЕНТА");
        int customerId = promptInt("Введите ID клиента");
        auto c = agency.findCustomer(customerId);

        std::cout << "\n=== КЛИЕНТ #" << c->customerId() << ": " << c->fullName() << " ===\n"
                  << "Дата рождения:       " << c->birthDate().toString() << " (" << c->ageOn(today) << " лет)\n"
                  << "Email:               " << c->contact().email() << "\n"
                  << "Телефон:             " << c->contact().phone() << "\n"
                  << "Адрес проживания:    " << c->address().fullText() << "\n"
                  << "\n[ПАСПОРТНЫЕ ДАННЫЕ]:\n"
                  << "Номер:               " << c->passport().number() << "\n"
                  << "Страна выдачи:       " << c->passport().issuingCountry() << "\n"
                  << "ФИО в паспорте:      " << c->passport().holderName() << "\n"
                  << "Срок действия до:    " << c->passport().expiry().toString()
                  << " (" << (!c->passport().isExpiredOn(today) ? "Действителен" : "ПРОСРОЧЕН") << ")\n"
                  << "\n[ПРОГРАММА ЛОЯЛЬНОСТИ]:\n"
                  << "Уровень карты:       " << c->loyalty().tier() << "\n"
                  << "Накопленные баллы:   " << c->loyalty().points() << " pts\n"
                  << "Сумма покупок:       " << c->loyalty().lifetimeSpent().toString() << "\n"
                  << "Постоянная скидка:   " << c->loyalty().discountPercent() << "%\n";

        // Визы
        std::cout << "\n[ДЕЙСТВУЮЩИЕ ВИЗЫ]:\n";
        if (c->visas().empty())
        {
            std::cout << "  Визы не оформлены.\n";
        }
        else
        {
            for (const auto &v : c->visas())
            {
                std::cout << "  • Страна: " << v.country() << ", тип: " << v.type()
                          << ", период: " << v.validity().toString()
                          << " (" << (v.isActiveOn(today) ? "Активна" : "Неактивна") << ")\n";
            }
        }

        // Непрочитанные уведомления
        std::size_t unread = agency.unreadNotifications(customerId);
        std::cout << "\nНепрочитанных системных уведомлений: " << unread << "\n";

        // Бронирования клиента
        std::cout << "\n[ИСТОРИЯ БРОНИРОВАНИЙ КЛИЕНТА]:\n";
        bool hasBookings = false;
        for (const auto &b : agency.bookings())
        {
            if (b->customer()->customerId() == customerId)
            {
                hasBookings = true;
                std::cout << "  • Бронь #" << b->id() << ": " << b->departure()->tour()->name()
                          << " | Даты: " << b->departure()->period().toString()
                          << " | Сумма: " << b->total().toString()
                          << " | Статус: " << bookingStatusToStringRu(b->status()) << "\n";
            }
        }
        if (!hasBookings)
        {
            std::cout << "  Бронирований пока нет.\n";
        }
    }

    void handleRegisterCustomer(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("РЕГИСТРАЦИЯ НОВОГО КЛИЕНТА");
        int nextId = static_cast<int>(agency.customers().size()) + 1;
        std::string first = prompt("Имя (First name)");
        std::string last = prompt("Фамилия (Last name)");
        int birthYear = promptInt("Год рождения", 1995);
        std::string email = prompt("Email (напр., user@example.com)");
        std::string phone = prompt("Телефон (напр., +7 999 123 45 67)");
        std::string passNum = prompt("Номер паспорта (напр., P1234567)");

        auto newCustomer = makeCustomer(nextId, first, last, birthYear, email, phone, passNum, today);
        agency.registerCustomer(newCustomer);
        std::cout << "\n[OK] Клиент успешно зарегистрирован под ID: " << nextId
                  << " (" << newCustomer->fullName() << ")!\n";
    }

    void handleAddVisa(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ВЫДАЧА И ДОБАВЛЕНИЕ ВИЗЫ");
        int customerId = promptInt("Введите ID клиента");
        auto c = agency.findCustomer(customerId);

        std::string countryCode = prompt("Код страны назначения (напр., IT, JP, AT, EG)");
        std::string visaType = prompt("Тип/категория визы (напр., Tourist Type C)");
        int durationDays = promptInt("Срок действия визы в днях", 90);

        DateRange validity(today, today.addDays(durationDays));
        c->addVisa(Visa(countryCode, visaType, validity));
        std::cout << "\n[OK] Виза в страну «" << countryCode << "» на срок "
                  << validity.toString() << " успешно привязана к клиенту "
                  << c->fullName() << "!\n";
    }

    void handleCreateBooking(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("СОЗДАНИЕ НОВОГО БРОНИРОВАНИЯ");
        int customerId = promptInt("Введите ID клиента");
        int departureId = promptInt("Введите ID отправления тура");
        int participants = promptInt("Количество участников поездки", 1);

        auto booking = agency.createBooking(customerId, departureId, participants, today);
        std::cout << "\n[OK] Бронирование #" << booking->id() << " успешно создано!\n"
                  << "Тур:          " << booking->departure()->tour()->name() << "\n"
                  << "Даты поездки: " << booking->departure()->period().toString() << "\n"
                  << "Участников:   " << booking->participants() << " чел.\n"
                  << "К оплате:     " << booking->total().toString() << "\n"
                  << "Статус:       " << bookingStatusToStringRu(booking->status()) << "\n";
    }

    void handleBookingDetails(const TravelCompany &agency, Accountant &accountant, const Date &today)
    {
        printSectionHeader("ИНФОРМАЦИЯ О БРОНИРОВАНИИ И СЧЁТ");
        int bookingId = promptInt("Введите ID бронирования");
        auto b = agency.findBooking(bookingId);

        std::cout << "\n=== БРОНИРОВАНИЕ #" << b->id() << " ===\n"
                  << "Дата оформления:  " << b->createdOn().toString() << "\n"
                  << "Клиент:           #" << b->customer()->customerId() << " " << b->customer()->fullName()
                  << " (" << b->customer()->contact().email() << ")\n"
                  << "Тур:              " << b->departure()->tour()->name() << "\n"
                  << "Направление:      " << b->departure()->tour()->destination().city().name() << "\n"
                  << "Даты отправления: " << b->departure()->period().toString() << " ("
                  << b->departure()->tour()->durationDays() << " дн.)\n"
                  << "Количество мест:  " << b->participants() << " чел.\n"
                  << "Текущий статус:   " << bookingStatusToStringRu(b->status()) << "\n"
                  << "\n[ФИНАНСОВЫЙ БАЛАНС]:\n"
                  << "Итоговая сумма:   " << b->total().toString() << "\n"
                  << "Оплачено:         " << b->totalPaid().toString() << "\n"
                  << "Остаток к оплате: " << b->amountDue().toString() << "\n"
                  << "Статус оплаты:    " << (b->isFullyPaid() ? "ПОЛНОСТЬЮ ОПЛАЧЕНО" : "ТРЕБУЕТСЯ ДОПЛАТА") << "\n";

        // История оплат
        std::cout << "\n[ИСТОРИЯ ПЛАТЕЖЕЙ]:\n";
        if (b->payments().empty())
        {
            std::cout << "  Платежи пока не поступали.\n";
        }
        else
        {
            for (const auto &p : b->payments())
            {
                std::cout << "  • Платёж #" << p.id() << " от " << p.date().toString()
                          << " на сумму " << p.amount().toString()
                          << " (способ: " << methodToString(p.method()) << ")\n";
            }
        }

        // Страховой полис
        std::cout << "\n[СТРАХОВОЙ ПОЛИС]:\n";
        if (b->insurance())
        {
            const auto &ins = b->insurance();
            std::cout << "  Номер полиса:   " << ins->policyNumber() << "\n"
                      << "  Застрахованный: " << ins->insured()->fullName() << "\n"
                      << "  Срок действия:  " << ins->period().toString() << "\n"
                      << "  Лимит покрытия: " << ins->coverage().toString() << "\n"
                      << "  Остаток лимита: " << ins->remainingCoverage().toString() << "\n"
                      << "  Премия:         " << ins->premium().toString() << "\n";
        }
        else
        {
            std::cout << "  Страховой полис не оформлен.\n";
        }

        // Официальный счёт (Invoice), сформированный бухгалтером
        Invoice invoice = accountant.issueInvoice(b, today);
        std::cout << "\n[ОФИЦИАЛЬНЫЙ СЧЁТ БУХГАЛТЕРИИ (INVOICE)]:\n"
                  << "  Номер счёта:     № " << invoice.number() << "\n"
                  << "  Дата выставления:" << invoice.issueDate().toString() << "\n"
                  << "  Оплатить до:     " << invoice.dueDate().toString()
                  << " (" << (invoice.isOverdue(today) ? "ПРОСРОЧЕН" : "Действителен") << ")\n"
                  << "  Сумма к оплате:  " << invoice.amountDue().toString() << "\n";
    }

    void handlePayBooking(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ОПЛАТА БРОНИРОВАНИЯ");
        int bookingId = promptInt("Введите ID бронирования");
        auto b = agency.findBooking(bookingId);

        std::cout << "Текущий остаток к оплате по бронированию: " << b->amountDue().toString() << "\n";
        if (b->isFullyPaid())
        {
            std::cout << "Бронирование уже полностью оплачено!\n";
            return;
        }

        int defaultUsd = static_cast<int>(b->amountDue().cents() / 100);
        int payUsd = promptInt("Сумма платежа в USD (целое число)", defaultUsd);
        Money amount(static_cast<long long>(payUsd) * 100);

        std::cout << "Выберите способ оплаты:\n"
                  << "  1. Банковская карта (Card)\n"
                  << "  2. Наличный расчёт в кассе (Cash)\n"
                  << "  3. Банковский перевод по реквизитам (BankTransfer)\n"
                  << "  4. Подарочный / туристический ваучер (Voucher)\n";
        int methodChoice = promptInt("Способ (1..4)", 1);
        PaymentMethod method = methodFromChoice(methodChoice);

        agency.payBooking(bookingId, amount, method, today);

        std::cout << "\n[OK] Платёж на сумму " << amount.toString()
                  << " способом «" << methodToString(method) << "» успешно проведён!\n"
                  << "Оставшаяся задолженность: " << b->amountDue().toString() << "\n"
                  << "Текущий статус: " << bookingStatusToStringRu(b->status()) << "\n";

        if (b->isFullyPaid())
        {
            std::cout << "\n>>> Поздравляем! Заказ полностью оплачен. Клиенту "
                      << b->customer()->fullName() << " начислены баллы лояльности (всего: "
                      << b->customer()->loyalty().points() << " pts, уровень: "
                      << b->customer()->loyalty().tier() << ")!\n";
        }
    }

    void handleAttachInsurance(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ОФОРМЛЕНИЕ СТРАХОВОГО ПОЛИСА");
        int bookingId = promptInt("Введите ID бронирования");
        auto b = agency.findBooking(bookingId);

        if (b->insurance())
        {
            std::cout << "К данному бронированию уже прикреплён полис: "
                      << b->insurance()->policyNumber() << "\n";
            return;
        }

        int coverageUsd = promptInt("Желаемый лимит страхового покрытия в USD", 50000);
        Money coverage(static_cast<long long>(coverageUsd) * 100);

        // Рассчитываем премию исходя из стоимости тура и возраста
        int age = b->customer()->ageOn(today);
        Money premium = InsurancePolicy::premiumFor(b->total(), age);

        std::string policyNumber = "POL-" + std::to_string(b->id()) + "-" + std::to_string(today.year());
        DateRange period = b->departure()->period();

        auto policy = std::make_shared<InsurancePolicy>(policyNumber, b->customer(), coverage, period, premium);
        b->attachInsurance(policy);

        std::cout << "\n[OK] Страховой полис успешно оформлен и привязан к бронированию!\n"
                  << "Номер полиса:    " << policyNumber << "\n"
                  << "Застрахован:     " << b->customer()->fullName() << " (" << age << " лет)\n"
                  << "Срок действия:   " << period.toString() << "\n"
                  << "Сумма покрытия:  " << coverage.toString() << "\n"
                  << "Страховая премия:" << premium.toString() << "\n";
    }

    void handleApplyDiscount(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ПРИМЕНЕНИЕ ПРОМОКОДА НА СКИДКУ");
        int bookingId = promptInt("Введите ID бронирования");
        auto b = agency.findBooking(bookingId);
        std::cout << "Текущая сумма бронирования: " << b->total().toString() << "\n";

        std::string code = prompt("Введите промокод (напр., WELCOME10, SUMMER2026, VIP20)");
        agency.applyDiscountCode(bookingId, code, today);

        std::cout << "\n[OK] Промокод «" << code << "» успешно применён!\n"
                  << "Новая сумма к оплате: " << b->total().toString() << "\n"
                  << "Остаток к оплате:     " << b->amountDue().toString() << "\n";
    }

    void handleCancelBooking(TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ОТМЕНА БРОНИРОВАНИЯ");
        int bookingId = promptInt("Введите ID отменяемого бронирования");
        auto b = agency.findBooking(bookingId);

        std::cout << "Отмена брони #" << b->id() << " для клиента " << b->customer()->fullName() << "\n"
                  << "Тур: " << b->departure()->tour()->name() << " (" << b->departure()->period().toString() << ")\n"
                  << "Оплаченная сумма: " << b->totalPaid().toString() << "\n";

        std::string confirm = prompt("Вы уверены, что хотите аннулировать заказ? (y/n)");
        if (confirm != "y" && confirm != "Y" && confirm != "да" && confirm != "ДА")
        {
            std::cout << "Отмена действия. Заказ остался без изменений.\n";
            return;
        }

        Money refund = agency.cancelBooking(bookingId, today);
        std::cout << "\n[OK] Бронирование аннулировано.\n"
                  << "Рассчитанная сумма возврата клиенту по правилам отмены: " << refund.toString() << "\n"
                  << "Статус бронирования: " << bookingStatusToStringRu(b->status()) << "\n";
    }

    void handleReviews(TravelCompany &agency)
    {
        printSectionHeader("ОТЗЫВЫ И РЕЙТИНГ ТУРОВ");
        int customerId = promptInt("Введите ID клиента, оставляющего отзыв");
        auto customer = agency.findCustomer(customerId);
        int tourId = promptInt("Введите ID тура (1..5)");
        auto tour = agency.catalog().findTour(tourId);

        int rating = promptInt("Оценка по шкале от 1 до 5", 5);
        if (rating < 1 || rating > 5)
        {
            std::cout << "Оценка должна быть в диапазоне от 1 до 5.\n";
            return;
        }

        std::string comment = prompt("Текст отзыва / впечатления о поездке");
        agency.addReview(Review(customer, tour, rating, comment));

        double avgRating = agency.averageRating(tourId);
        std::cout << "\n[OK] Благодарим за отзыв о туре «" << tour->name() << "»!\n"
                  << "Ваша оценка: " << rating << " / 5. Текст: \"" << comment << "\"\n"
                  << "Обновлённый средний рейтинг тура: "
                  << std::to_string(avgRating).substr(0, 4) << " / 5.0\n";
    }

    void handleFileComplaint(TravelCompany &agency)
    {
        printSectionHeader("РЕГИСТРАЦИЯ ЖАЛОБЫ КЛИЕНТА");
        int customerId = promptInt("Введите ID клиента");
        int bookingId = promptInt("Введите ID бронирования");
        std::string text = prompt("Опишите суть претензии или проблемы");

        int complaintId = agency.fileComplaint(customerId, bookingId, text);
        std::cout << "\n[OK] Жалоба зарегистрирована под номером #" << complaintId << "!\n"
                  << "Обращение передано в отдел контроля качества на рассмотрение руководителю.\n";
    }

    void handleResolveComplaint(TravelCompany &agency)
    {
        printSectionHeader("РАССМОТРЕНИЕ И УРЕГУЛИРОВАНИЕ ЖАЛОБЫ");
        std::cout << "Открытых нерассмотренных обращений: " << agency.openComplaintCount() << "\n";
        int complaintId = promptInt("Введите ID рассматриваемой жалобы");
        std::string resolution = prompt("Введите официальное решение / принятые меры по жалобе");

        agency.resolveComplaint(complaintId, resolution);
        std::cout << "\n[OK] Жалоба #" << complaintId << " успешно урегулирована и закрыта!\n"
                  << "Текст принятого решения зафиксирован в системе.\n";
    }

    void handleComplaintStatus(const TravelCompany &agency)
    {
        printSectionHeader("СТАТУС ОБРАЩЕНИЙ И ЖАЛОБ КЛИЕНТОВ");
        std::size_t openCount = agency.openComplaintCount();
        std::cout << "Количество активных (нерешённых) жалоб: " << openCount << "\n";
        if (openCount == 0)
        {
            std::cout << "Все поступившие обращения клиентов успешно обработаны и закрыты.\n";
        }
        else
        {
            std::cout << "Внимание: требуется урегулировать " << openCount << " жалоб(ы) через пункт меню 17.\n";
        }
    }

    void handleSalesReport(const TravelCompany &agency, const Date &today)
    {
        printSectionHeader("ФИНАНСОВЫЙ ОТЧЁТ О ПРОДАЖАХ (SALES REPORT)");
        std::cout << "Выберите временной период отчёта:\n"
                  << "  1. За последний месяц (30 дней)\n"
                  << "  2. За последние полгода (180 дней)\n"
                  << "  3. За последний год (365 дней)\n"
                  << "  4. За весь доступный диапазон (±1 год от текущей даты)\n";
        int periodChoice = promptInt("Период (1..4)", 4);

        Date startDate = today.addDays(-365);
        Date endDate = today.addDays(365);
        std::string periodName = "Годовой отчёт";

        switch (periodChoice)
        {
        case 1:
            startDate = today.addDays(-30);
            endDate = today.addDays(30);
            periodName = "Месячный отчёт";
            break;
        case 2:
            startDate = today.addDays(-180);
            endDate = today.addDays(180);
            periodName = "Полугодовой отчёт";
            break;
        case 3:
            startDate = today.addDays(-365);
            endDate = today;
            periodName = "Отчёт за прошедший год";
            break;
        default:
            break;
        }

        SalesReport report = agency.generateReport(periodName, DateRange(startDate, endDate));

        std::cout << "\n============================================================\n"
                  << "  " << report.title() << "\n"
                  << "  Диапазон дат:       " << report.period().toString() << "\n"
                  << "============================================================\n"
                  << "Всего бронирований:   " << report.bookingCount() << "\n"
                  << "Аннулировано заказов: " << report.cancelledCount() << "\n"
                  << "Суммарная выручка:    " << report.totalRevenue().toString() << "\n"
                  << "Средний чек (чек/тур):" << report.averageBookingValue().toString() << "\n"
                  << "Самый популярный тур: " << (report.topTourName().empty() ? "Нет данных" : report.topTourName()) << "\n"
                  << "============================================================\n";
    }

    void handleLedger(const TravelCompany &agency, const Date &today)
    {
        printSectionHeader("БУХГАЛТЕРСКАЯ КНИГА КОМПАНИИ (LEDGER)");
        const auto &ledger = agency.ledger();

        std::cout << "Финансовый журнал:    " << ledger.name() << "\n"
                  << "Текущий нетто-баланс: " << ledger.balance().toString() << "\n"
                  << "Общий доход (Income): " << ledger.totalIncome().toString() << "\n"
                  << "Общие расходы/возврат:" << ledger.totalExpenses().toString() << "\n"
                  << "Всего проводок:       " << ledger.entries().size() << "\n"
                  << "Проводок за сегодня:  " << ledger.entriesOn(today) << "\n"
                  << "\n[ПОСЛЕДНИЕ ФИНАНСОВЫЕ ЗАПИСИ]:\n";

        if (ledger.entries().empty())
        {
            std::cout << "  В журнале пока нет проводок.\n";
        }
        else
        {
            for (const auto &entry : ledger.entries())
            {
                std::cout << "  • " << entry.describe()
                          << (entry.income() ? "  [ДОХОД]" : "  [РАСХОД]") << "\n";
            }
        }
    }

    void handleOffices(const TravelCompany &agency)
    {
        printSectionHeader("ОФИСЫ КОМПАНИИ И ШТАТ СОТРУДНИКОВ");
        const auto &offices = agency.offices();

        for (std::size_t i = 0; i < offices.size(); ++i)
        {
            const auto &off = offices[i];
            std::cout << "=== ОФИС #" << (i + 1) << ": " << off.name() << " ===\n"
                      << "Адрес:               " << off.address().fullText() << "\n"
                      << "Общий ФОТ офиса:     " << off.payroll().toString() << " в месяц\n"
                      << "Штат сотрудников:    " << off.staffCount() << " чел.\n";

            if (off.manager())
            {
                const auto &mgr = off.manager();
                std::cout << "\n  [РУКОВОДИТЕЛЬ ОФИСА]:\n"
                          << "  ФИО:               " << mgr->fullName() << "\n"
                          << "  Роль:              " << mgr->describeRole() << "\n"
                          << "  Оклад:             " << mgr->salary().toString() << "\n"
                          << "  Лимит скидки:      до " << mgr->discountLimitPercent() << "%\n"
                          << "  Размер команды:    " << mgr->teamSize() << " чел.\n"
                          << "  Зарплата команды:  " << mgr->teamPayroll().toString() << "\n";
            }

            std::cout << "\n  [СОТРУДНИКИ ОФИСА]:\n";
            for (const auto &emp : off.staff())
            {
                std::cout << "  • [#" << emp->employeeId() << "] " << emp->fullName()
                          << " — " << emp->describeRole()
                          << ", оклад: " << emp->salary().toString() << "\n";
            }
            std::cout << "\n";
        }
    }

} // namespace

int main()
{
    const Date today = currentDate();
    AppState state = buildDemoAgency(today);

    while (std::cin)
    {
        showMainMenu(state.agency, state.today);

        int choice = -1;
        try
        {
            choice = promptInt("Выберите пункт меню (0..21)");
        }
        catch (const TravelException &)
        {
            std::cout << "\n[!] Пожалуйста, введите корректное число от 0 до 21.\n";
            continue;
        }

        if (choice == 0 || !std::cin)
        {
            std::cout << "\nЗавершение работы программы. До свидания!\n";
            break;
        }

        try
        {
            switch (choice)
            {
            case 1:
                handleListDepartures(state.agency);
                break;
            case 2:
                handleSearchByCity(state.agency);
                break;
            case 3:
                handleSearchByBudget(state.agency);
                break;
            case 4:
                handleTourDetails(state.agency, state.today);
                break;
            case 5:
                handleListCustomers(state.agency);
                break;
            case 6:
                handleCustomerCard(state.agency, state.today);
                break;
            case 7:
                handleRegisterCustomer(state.agency, state.today);
                break;
            case 8:
                handleAddVisa(state.agency, state.today);
                break;
            case 9:
                handleCreateBooking(state.agency, state.today);
                break;
            case 10:
                handleBookingDetails(state.agency, *state.accountant, state.today);
                break;
            case 11:
                handlePayBooking(state.agency, state.today);
                break;
            case 12:
                handleAttachInsurance(state.agency, state.today);
                break;
            case 13:
                handleApplyDiscount(state.agency, state.today);
                break;
            case 14:
                handleCancelBooking(state.agency, state.today);
                break;
            case 15:
                handleReviews(state.agency);
                break;
            case 16:
                handleFileComplaint(state.agency);
                break;
            case 17:
                handleResolveComplaint(state.agency);
                break;
            case 18:
                handleComplaintStatus(state.agency);
                break;
            case 19:
                handleSalesReport(state.agency, state.today);
                break;
            case 20:
                handleLedger(state.agency, state.today);
                break;
            case 21:
                handleOffices(state.agency);
                break;
            default:
                std::cout << "\n[!] Неизвестная команда '" << choice << "'. Выберите число от 0 до 21.\n";
                break;
            }
        }
        catch (const TravelException &err)
        {
            std::cout << "\n[ОШИБКА ОПЕРАЦИИ]: " << err.what() << "\n";
        }
        catch (const std::exception &err)
        {
            std::cout << "\n[СИСТЕМНАЯ ОШИБКА]: " << err.what() << "\n";
        }

        std::cout << "\nНажмите Enter, чтобы вернуться в главное меню...";
        std::string pause;
        std::getline(std::cin, pause);
    }

    return 0;
}
