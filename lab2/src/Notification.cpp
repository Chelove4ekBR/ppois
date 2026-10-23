#include "Notification.hpp"

#include <utility>

namespace {

const char* const kEllipsis = "...";

}

Notification::Notification(CustomerPtr recipient, std::string message, Date sentOn)
    : recipient_(std::move(recipient)), message_(std::move(message)), sentOn_(sentOn), read_(false) {}

bool Notification::isUnread() const {
    return !read_;
}

void Notification::markRead() {
    read_ = true;
}

std::string Notification::preview(std::size_t maxLength) const {
    if (message_.size() <= maxLength) {
        return message_;
    }
    return message_.substr(0, maxLength) + kEllipsis;
}
