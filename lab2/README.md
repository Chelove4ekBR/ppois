# Laboratory Work №2. Large-Scale OOP Project
**Variant:** Travel Company

Row format: `Class | number of fields | number of methods | related classes`.

| Class | Fields | Methods | Associations (Related classes) |
|---|---|---|---|
| Accountant | 3 | 4 | Booking, Date, Invoice, Ledger, Money, Payment, Person |
| Activity | 4 | 3 | Money |
| Address | 4 | 3 | — |
| Airport | 3 | 2 | City |
| Attraction | 4 | 3 | Money |
| BeachTour | 3 | 3 | Hotel, Money, Tour |
| Booking | 9 | 10 | CancellationPolicy, Customer, Date, Discount, InsurancePolicy, Money, Payment |
| Bus | 3 | 2 | Date, Driver, Money |
| CancellationPolicy | 4 | 3 | Date, Money |
| City | 3 | 3 | Country |
| Complaint | 6 | 3 | Booking, Customer |
| ContactInfo | 2 | 3 | — |
| Country | 4 | 2 | — |
| CruiseTour | 3 | 5 | City, Money, Ship, Tour |
| Customer | 4 | 3 | Country, DateRange, LoyaltyAccount, Passport, Person, Visa |
| Date | 3 | 7 | — |
| DateRange | 2 | 4 | Date |
| Destination | 4 | 5 | Attraction, City, Date, Money |
| Discount | 5 | 3 | Date, DateRange, Money |
| Driver | 3 | 5 | Date, DrivingLicense, Money, Person |
| DrivingLicense | 3 | 2 | Date |
| Employee | 4 | 4 | Date, Money, Person |
| ExcursionTour | 3 | 4 | Money, Tour, TourGuide |
| Flight | 5 | 4 | Airport, Date, Money |
| Hotel | 4 | 5 | Address, DateRange, Money, Room |
| HotelReservation | 7 | 3 | Customer, DateRange, Hotel, MealPlan, Money, Room |
| InsuranceClaim | 5 | 3 | Date, InsurancePolicy, Money |
| InsurancePolicy | 6 | 4 | Customer, Date, DateRange, Money |
| Invoice | 4 | 4 | Booking, Date, Money |
| Itinerary | 3 | 4 | City, ItineraryDay, Money |
| ItineraryDay | 3 | 4 | Activity, Money |
| Ledger | 2 | 5 | Date, LedgerEntry, Money |
| LedgerEntry | 4 | 2 | Date, Money |
| LoyaltyAccount | 2 | 4 | Money |
| Manager | 3 | 5 | Date, Employee, Money, Person |
| MealPlan | 3 | 3 | Money |
| Money | 2 | 9 | — |
| Notification | 4 | 3 | Customer, Date |
| Office | 4 | 4 | Address, Employee, Manager, Money |
| Passport | 4 | 3 | Date, DateRange |
| Payment | 4 | 4 | Date, Money |
| Person | 5 | 3 | Address, ContactInfo, Date |
| Review | 4 | 3 | Customer, Tour |
| Room | 4 | 5 | DateRange, Money |
| SalesReport | 3 | 6 | Booking, DateRange, Money |
| Ship | 4 | 3 | — |
| SkiTour | 3 | 4 | Money, Tour |
| Tour | 7 | 5 | Date, Destination, Itinerary, Money |
| TourCatalog | 2 | 7 | Date, DateRange, Money, Tour |
| TourDeparture | 4 | 6 | Date, DateRange, Tour |
| TourGuide | 3 | 6 | Date, DateRange, Money, Person |
| Train | 3 | 3 | Money |
| Transfer | 5 | 3 | Address, Bus, Date, Money |
| Transport | 5 | 6 | Money |
| TransportTicket | 4 | 3 | Customer, Money, Transport |
| TravelCompany | 14 | 17 | Accountant, Booking, CancellationPolicy, Complaint, Customer, Date, DateRange, Discount, Ledger, Money, Notification, Office, Review, SalesReport, TourCatalog |
| TravelGroup | 4 | 5 | Customer |
| Visa | 3 | 2 | Date, DateRange |

### Exceptions (19)

- TravelException
- InvalidDateException
- InvalidMoneyException
- CurrencyMismatchException
- InvalidContactException
- InvalidPassportException
- ExpiredPassportException
- VisaRequiredException
- GuideUnavailableException
- CustomerNotFoundException
- TourNotFoundException
- TourFullException
- RoomUnavailableException
- BookingNotFoundException
- InvalidBookingStateException
- PaymentException
- InvalidDiscountException
- InsuranceException
- InvalidRatingException

### Final Statistics

| Indicator | Value |
|---|---|
| Fields | 232 |
| Behaviors | 246 |
| Associations | 160 |
| Exceptions | 19 |
