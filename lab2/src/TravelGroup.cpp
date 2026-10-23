#include "TravelGroup.hpp"

#include <algorithm>
#include <utility>

#include "Exceptions.hpp"

namespace
{

    constexpr std::size_t kGroupDiscountSize = 5;

}

TravelGroup::TravelGroup(std::string name, std::shared_ptr<Customer> leader, std::size_t maxSize)
    : name_(std::move(name)), leader_(std::move(leader)), maxSize_(maxSize)
{
    members_.push_back(leader_);
}

void TravelGroup::addMember(const std::shared_ptr<Customer> &customer)
{
    if (isFull())
    {
        throw TourFullException("Group " + name_ + " is full");
    }
    if (!hasMember(customer->customerId()))
    {
        members_.push_back(customer);
    }
}

void TravelGroup::removeMember(int customerId)
{
    members_.erase(std::remove_if(members_.begin(), members_.end(),
                                  [customerId](const CustomerPtr &member)
                                  { return member->customerId() == customerId; }),
                   members_.end());
}

bool TravelGroup::hasMember(int customerId) const
{
    return std::any_of(members_.begin(), members_.end(),
                       [customerId](const CustomerPtr &member)
                       { return member->customerId() == customerId; });
}

bool TravelGroup::isFull() const
{
    return members_.size() >= maxSize_;
}

bool TravelGroup::isDiscountEligible() const
{
    return members_.size() >= kGroupDiscountSize;
}
