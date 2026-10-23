#include "Review.hpp"

#include <utility>

#include "Exceptions.hpp"

namespace {

constexpr int kMinRating = 1;
constexpr int kMaxRating = 5;
constexpr int kPositiveRating = 4;

}

Review::Review(CustomerPtr author, TourPtr tour, int rating, std::string text)
    : author_(std::move(author)), tour_(std::move(tour)), rating_(rating), text_(std::move(text)) {
    if (rating_ < kMinRating || rating_ > kMaxRating) {
        throw InvalidRatingException("Rating must be between 1 and 5, got " + std::to_string(rating_));
    }
}

bool Review::isPositive() const {
    return rating_ >= kPositiveRating;
}

bool Review::isByCustomer(int customerId) const {
    return author_->customerId() == customerId;
}

std::string Review::summary() const {
    return tour_->name() + ": " + std::to_string(rating_) + "/5 by " + author_->fullName();
}
