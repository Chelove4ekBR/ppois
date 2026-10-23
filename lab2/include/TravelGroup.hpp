#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Customer.hpp"

class TravelGroup {
public:
    TravelGroup(std::string name, std::shared_ptr<Customer> leader, std::size_t maxSize);
    const std::string& name() const { return name_; }
    const std::shared_ptr<Customer>& leader() const { return leader_; }
    const std::vector<std::shared_ptr<Customer>>& members() const { return members_; }
    std::size_t maxSize() const { return maxSize_; }
    void addMember(const std::shared_ptr<Customer>& customer);
    void removeMember(int customerId);
    bool hasMember(int customerId) const;
    bool isFull() const;
    bool isDiscountEligible() const;

private:
    std::string name_;
    std::shared_ptr<Customer> leader_;
    std::vector<std::shared_ptr<Customer>> members_;
    std::size_t maxSize_;
};
