#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "City.hpp"
#include "Money.hpp"
#include "Ship.hpp"
#include "Tour.hpp"

class CruiseTour : public Tour {
public:
    CruiseTour(Tour base, ShipPtr ship, std::string cabinCategory, std::vector<City> ports);
    const ShipPtr& ship() const { return ship_; }
    const std::string& cabinCategory() const { return cabinCategory_; }
    const std::vector<City>& ports() const { return ports_; }
    int cabinMultiplierPercent() const;
    std::size_t portCount() const;
    bool visitsCity(const std::string& cityName) const;
    Money pricePerPerson() const override;
    std::string describe() const override;

private:
    ShipPtr ship_;
    std::string cabinCategory_;
    std::vector<City> ports_;
};
