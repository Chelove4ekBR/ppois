#include "GraphicsCard.hpp"

#include <utility>
#include "Exceptions.hpp"

GraphicsCard::GraphicsCard(Gpu gpu, ByteSize vramSize, uint32_t pcieLanesNeeded)
    : gpu_(std::move(gpu)), vramSize_(vramSize), pcieLanesNeeded_(pcieLanesNeeded) {
    if (vramSize_.bytes() == 0 || pcieLanesNeeded_ == 0) {
        throw InvalidConfigurationException("Graphics card must have VRAM and PCIe lane requirements");
    }
    ports_.emplace_back(1, "DisplayPort", "1.4");
    ports_.emplace_back(2, "HDMI", "2.1");
}

void GraphicsCard::addPort(const VideoPort& port) {
    ports_.push_back(port);
}

void GraphicsCard::connectDisplay(uint32_t portId, DisplayPtr display) {
    if (!display) {
        throw ComponentNotFoundException("Cannot connect null display");
    }
    for (auto& port : ports_) {
        if (port.portId() == portId) {
            port.connectCable();
            connectedDisplays_.push_back(std::move(display));
            return;
        }
    }
    throw ComponentNotFoundException("Video port #" + std::to_string(portId) + " not found on graphics card");
}

void GraphicsCard::disconnectDisplay(uint32_t portId) {
    for (auto& port : ports_) {
        if (port.portId() == portId) {
            port.disconnectCable();
            if (!connectedDisplays_.empty()) {
                connectedDisplays_.pop_back();
            }
            return;
        }
    }
    throw ComponentNotFoundException("Video port #" + std::to_string(portId) + " not found");
}

double GraphicsCard::render3DScene(uint32_t polygonCount, uint64_t frameId) {
    const double renderTimeMs = gpu_.renderFrame(polygonCount);
    for (auto& display : connectedDisplays_) {
        if (display && display->isPowerOn()) {
            display->displayFrame(frameId);
        }
    }
    return renderTimeMs;
}

uint32_t GraphicsCard::connectedDisplaysCount() const {
    return static_cast<uint32_t>(connectedDisplays_.size());
}

uint32_t GraphicsCard::totalPortsCount() const {
    return static_cast<uint32_t>(ports_.size());
}

void GraphicsCard::coolDown(double deltaC) {
    gpu_.coolDown(deltaC);
}
