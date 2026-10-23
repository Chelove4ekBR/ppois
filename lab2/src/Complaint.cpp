#include "Complaint.hpp"

#include <utility>

Complaint::Complaint(int id, CustomerPtr customer, BookingPtr booking, std::string text)
    : id_(id),
      customer_(std::move(customer)),
      booking_(std::move(booking)),
      text_(std::move(text)),
      resolved_(false),
      resolution_() {}

bool Complaint::isOpen() const {
    return !resolved_;
}

void Complaint::resolve(const std::string& resolutionText) {
    resolved_ = true;
    resolution_ = resolutionText;
}

std::string Complaint::describe() const {
    const std::string state = resolved_ ? "resolved: " + resolution_ : "open";
    return "Complaint #" + std::to_string(id_) + " about booking " + std::to_string(booking_->id()) + " (" + state + ")";
}
