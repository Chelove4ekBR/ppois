#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "AudioOutput.hpp"
#include "ByteSize.hpp"
#include "ClockFrequency.hpp"
#include "Display.hpp"
#include "Exceptions.hpp"
#include "Gpu.hpp"
#include "GraphicsCard.hpp"
#include "SoundCard.hpp"
#include "VideoPort.hpp"

TEST_CASE("Gpu rendering, compute and overclocking", "[gpu]") {
    Gpu gpu("GeForce RTX 4070 Ti", 7680, ClockFrequency::fromMHz(2310), 285);
    CHECK(gpu.model() == "GeForce RTX 4070 Ti");
    CHECK(gpu.cudaCores() == 7680);
    CHECK(gpu.tdpWatt() == 285);
    CHECK_FALSE(gpu.isOverheating());

    const double renderTimeMs = gpu.renderFrame(1000000);
    CHECK(renderTimeMs > 0.0);

    const double computeTimeMs = gpu.computeShader(5000);
    CHECK(computeTimeMs > 0.0);

    gpu.overclock(10.0);
    CHECK(gpu.currentClock().hertz() > gpu.baseClock().hertz());

    gpu.coolDown(5.0);
    CHECK(gpu.thermalZone().temperatureC() < 90.0);

    CHECK_THROWS_AS(Gpu("Bad", 0, ClockFrequency::fromMHz(1000)), InvalidConfigurationException);
}

TEST_CASE("VideoPort bandwidth and resolution validation", "[video_port]") {
    VideoPort dp(1, "DisplayPort", "1.4");
    CHECK(dp.portId() == 1);
    CHECK(dp.portType() == "DisplayPort");
    CHECK(dp.version() == "1.4");
    CHECK_FALSE(dp.isConnected());
    CHECK(dp.maxBandwidthGbps() > 30.0);

    CHECK(dp.supportsResolution(2560, 1440, 144));
    CHECK_FALSE(dp.supportsResolution(7680, 4320, 240));

    dp.connectCable();
    CHECK(dp.isConnected());
    CHECK_THROWS_AS(dp.connectCable(), PortOccupiedException);

    dp.disconnectCable();
    CHECK_FALSE(dp.isConnected());
    CHECK_FALSE(dp.describe().empty());

    VideoPort hdmi(2, "HDMI", "2.1");
    CHECK(hdmi.maxBandwidthGbps() == 48.0);
    CHECK_THROWS_AS(VideoPort(99, "", "1.0"), InvalidConfigurationException);
}

TEST_CASE("Display resolution, refresh rate and HDR", "[display]") {
    Display disp("Dell UltraSharp", 27.0, 3840, 2160, 60, true);
    CHECK(disp.model() == "Dell UltraSharp");
    CHECK(disp.diagonalInches() == 27.0);
    CHECK(disp.resolutionWidth() == 3840);
    CHECK(disp.resolutionHeight() == 2160);
    CHECK(disp.refreshRateHz() == 60);
    CHECK(disp.isHdrEnabled());
    CHECK_FALSE(disp.isPowerOn());

    disp.turnOn();
    CHECK(disp.isPowerOn());

    CHECK_NOTHROW(disp.displayFrame(100));

    disp.turnOff();
    CHECK_THROWS_AS(disp.displayFrame(100), DeviceNotConnectedException);

    disp.setResolution(1920, 1080);
    CHECK(disp.resolutionWidth() == 1920);
    CHECK_THROWS_AS(disp.setResolution(0, 1080), InvalidConfigurationException);

    disp.setRefreshRate(120);
    CHECK(disp.refreshRateHz() == 120);
    CHECK_THROWS_AS(disp.setRefreshRate(1000), InvalidConfigurationException);

    disp.toggleHdr();
    CHECK_FALSE(disp.isHdrEnabled());

    CHECK(disp.aspectRatio() > 1.7);
    CHECK_THROWS_AS(Display("", 0.0, 0, 0, 0), InvalidConfigurationException);
}

TEST_CASE("GraphicsCard display output and scene rendering", "[graphics_card]") {
    Gpu gpu("Radeon RX 7900 XTX", 6144, ClockFrequency::fromMHz(2500), 355);
    GraphicsCard card(gpu, ByteSize::fromGiB(24), 16);
    CHECK_THROWS_AS(GraphicsCard(gpu, ByteSize(0), 16), InvalidConfigurationException);
    CHECK(card.vramSize().toGiB() == 24.0);
    CHECK(card.pcieLanesNeeded() == 16);
    CHECK(card.totalPortsCount() == 2);
    CHECK(card.connectedDisplaysCount() == 0);

    card.addPort(VideoPort(3, "DisplayPort", "2.0"));
    CHECK(card.totalPortsCount() == 3);

    auto mon1 = std::make_shared<Display>("BenQ", 24.0, 1920, 1080, 144, false);
    mon1->turnOn();
    card.connectDisplay(1, mon1);
    CHECK(card.connectedDisplaysCount() == 1);

    const double timeMs = card.render3DScene(500000, 1);
    CHECK(timeMs > 0.0);

    card.coolDown(10.0);

    card.disconnectDisplay(1);
    CHECK(card.connectedDisplaysCount() == 0);

    CHECK_THROWS_AS(card.connectDisplay(99, mon1), ComponentNotFoundException);
    CHECK_THROWS_AS(card.connectDisplay(1, nullptr), ComponentNotFoundException);
    CHECK_THROWS_AS(card.disconnectDisplay(99), ComponentNotFoundException);
}

TEST_CASE("SoundCard sample rates, bit depth and processing", "[sound_card]") {
    SoundCard sc("Sound Blaster AE-9", 192, 32, 8, "ESS SABRE 9038");
    CHECK(sc.model() == "Sound Blaster AE-9");
    CHECK(sc.maxSampleRateKhz() == 192);
    CHECK(sc.bitDepth() == 32);
    CHECK(sc.channels() == 8);
    CHECK(sc.dacModel() == "ESS SABRE 9038");

    CHECK(sc.supportsHighResAudio());
    CHECK(sc.supportsSurround());
    CHECK(sc.calculateAudioBitrateKbps() > 40000.0);

    const double playSec = sc.processAudioBuffer(192000);
    CHECK(playSec == 1.0);

    sc.setSampleRate(96);
    CHECK(sc.maxSampleRateKhz() == 96);
    CHECK_THROWS_AS(sc.setSampleRate(500), InvalidConfigurationException);
    CHECK_THROWS_AS(SoundCard("Bad", 0, 16, 2), InvalidConfigurationException);
}

TEST_CASE("AudioOutput volume and mute controls", "[audio_output]") {
    AudioOutput out("Headphones", "Headphones", 70);
    CHECK(out.deviceName() == "Headphones");
    CHECK(out.deviceType() == "Headphones");
    CHECK(out.volumePercent() == 70);
    CHECK_FALSE(out.isMuted());
    CHECK(out.effectiveVolume() == 70);

    out.setVolume(85);
    CHECK(out.volumePercent() == 85);
    CHECK_THROWS_AS(out.setVolume(150), InvalidConfigurationException);

    out.mute();
    CHECK(out.isMuted());
    CHECK(out.effectiveVolume() == 0);

    out.unmute();
    CHECK_FALSE(out.isMuted());
    CHECK(out.effectiveVolume() == 85);

    out.toggleMute();
    CHECK(out.isMuted());

    CHECK_NOTHROW(out.playSound(500));
    CHECK_THROWS_AS(AudioOutput("Bad", "Speakers", 120), InvalidConfigurationException);

    AudioOutput zeroVol("Silent", "LineOut", 0);
    CHECK_NOTHROW(zeroVol.playSound(200));
    CHECK_THROWS_AS(out.playSound(0), InvalidConfigurationException);
}
