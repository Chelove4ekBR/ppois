#include <catch2/catch_test_macros.hpp>
#include "test_support.hpp"

#include "ByteSize.hpp"
#include "Exceptions.hpp"
#include "IpAddress.hpp"
#include "Keyboard.hpp"
#include "MacAddress.hpp"
#include "Mouse.hpp"
#include "NetworkAdapter.hpp"
#include "NetworkConnection.hpp"
#include "NetworkPacket.hpp"
#include "UsbHub.hpp"
#include "UsbPort.hpp"
#include "Webcam.hpp"
#include "WifiInterface.hpp"

TEST_CASE("MacAddress parsing, broadcast and multicast", "[mac_address]") {
    MacAddress mac({0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E});
    CHECK(mac.toString() == "00:1A:2B:3C:4D:5E");
    CHECK_FALSE(mac.isBroadcast());
    CHECK_FALSE(mac.isMulticast());

    MacAddress parsed = MacAddress::fromString("00:1A:2B:3C:4D:5E");
    CHECK(mac.matches(parsed));

    MacAddress bcast({0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});
    CHECK(bcast.isBroadcast());

    MacAddress mcast({0x01, 0x00, 0x5E, 0x00, 0x00, 0x01});
    CHECK(mcast.isMulticast());

    CHECK_THROWS_AS(MacAddress({0x00, 0x11}), InvalidAddressException);
    CHECK_THROWS_AS(MacAddress::fromString("invalid:mac:str"), InvalidAddressException);
    CHECK_THROWS_AS(MacAddress::fromString("00:11:22:33:44:GG"), InvalidAddressException);
    CHECK_THROWS_AS(MacAddress::fromString("00:11:22:33:44:"), InvalidAddressException);
    CHECK_THROWS_AS(MacAddress::fromString("00:11:22:33:44:55:66"), InvalidAddressException);
}

TEST_CASE("IpAddress subnets and RFC 1918 classification", "[ip_address]") {
    IpAddress ip1({192, 168, 1, 50}, 24);
    CHECK(ip1.toString() == "192.168.1.50");
    CHECK(ip1.toStringWithPrefix() == "192.168.1.50/24");
    CHECK(ip1.isPrivate());
    CHECK_FALSE(ip1.isLoopback());

    IpAddress ip2({192, 168, 1, 100}, 24);
    CHECK(ip1.isInSameSubnet(ip2));

    IpAddress diffPrefix({192, 168, 1, 50}, 16);
    CHECK_FALSE(ip1.isInSameSubnet(diffPrefix));

    IpAddress otherSub({192, 168, 2, 50}, 24);
    CHECK_FALSE(ip1.isInSameSubnet(otherSub));

    IpAddress loopback = IpAddress::fromString("127.0.0.1");
    CHECK(loopback.isLoopback());

    IpAddress publicIp = IpAddress::fromString("8.8.8.8/32");
    CHECK_FALSE(publicIp.isPrivate());

    IpAddress net10 = IpAddress::fromString("10.0.0.1/8");
    CHECK(net10.isPrivate());

    IpAddress net172 = IpAddress::fromString("172.20.0.1/16");
    CHECK(net172.isPrivate());

    CHECK_THROWS_AS(IpAddress::fromString("999.1.1.1"), InvalidAddressException);
    CHECK_THROWS_AS(IpAddress::fromString("1.1.1"), InvalidAddressException);
    CHECK_THROWS_AS(IpAddress::fromString("1.1.1.1/40"), InvalidAddressException);
    CHECK_THROWS_AS(IpAddress::fromString("192..1.1"), InvalidAddressException);
    CHECK_THROWS_AS(IpAddress::fromString("192.abc.1.1"), InvalidAddressException);
    CHECK_THROWS_AS(IpAddress::fromString("10.0.0.1/abc"), InvalidAddressException);
}

TEST_CASE("NetworkPacket checksum, TTL and expiration", "[network_packet]") {
    IpAddress src({10, 0, 0, 1}, 24);
    IpAddress dst({10, 0, 0, 2}, 24);
    NetworkPacket packet(src, dst, 1024, "TCP", 3);
    CHECK(packet.protocol() == "TCP");
    CHECK(packet.payloadBytes() == 1024);
    CHECK(packet.ttl() == 3);
    CHECK(packet.verifyChecksum());
    CHECK_FALSE(packet.isExpired());

    packet.decrementTtl();
    CHECK(packet.ttl() == 2);
    packet.decrementTtl();
    packet.decrementTtl();
    CHECK(packet.isExpired());

    CHECK_FALSE(packet.describe().empty());
    CHECK_THROWS_AS(NetworkPacket(src, dst, 100, ""), InvalidConfigurationException);
}

TEST_CASE("NetworkAdapter packet routing and throughput", "[network_adapter]") {
    NetworkAdapter nic("enp3s0", MacAddress(), IpAddress({192, 168, 1, 10}, 24), 2500);
    CHECK(nic.adapterName() == "enp3s0");
    CHECK(nic.maxSpeedMbps() == 2500);
    CHECK(nic.bandwidthGbps() == 2.5);
    CHECK(nic.isLinkUp());
    CHECK(nic.totalBytesTransferred() == 0);

    NetworkPacket p1(nic.ipAddress(), IpAddress({192, 168, 1, 1}, 24), 512, "UDP", 10);
    nic.sendPacket(p1);
    CHECK(nic.totalBytesTransferred() == 512);

    nic.receivePacket(p1);
    CHECK(nic.totalBytesTransferred() == 1024);

    nic.disconnectLink();
    CHECK_FALSE(nic.isLinkUp());
    CHECK_THROWS_AS(nic.sendPacket(p1), NetworkException);
    CHECK_THROWS_AS(nic.receivePacket(p1), NetworkException);

    nic.connectLink();
    NetworkPacket expired(nic.ipAddress(), IpAddress({192, 168, 1, 1}, 24), 512, "UDP", 0);
    CHECK_THROWS_AS(nic.sendPacket(expired), NetworkException);
    CHECK_THROWS_AS(NetworkAdapter("", MacAddress(), IpAddress(), 1000), InvalidConfigurationException);
    CHECK_THROWS_AS(NetworkAdapter("eth0", MacAddress(), IpAddress(), 0), InvalidConfigurationException);
}

TEST_CASE("WifiInterface connection and signal quality", "[wifi_interface]") {
    NetworkAdapter baseNic("wlan0", MacAddress(), IpAddress({192, 168, 0, 10}, 24), 1200);
    WifiInterface wifi(baseNic);
    CHECK_FALSE(wifi.isConnected());
    CHECK(wifi.signalQualityPercent() == 0.0);

    wifi.connectSsid("HomeNetwork_5G", "supersecret123");
    CHECK(wifi.isConnected());
    CHECK(wifi.currentSsid() == "HomeNetwork_5G");
    CHECK(wifi.signalQualityPercent() > 0.0);

    wifi.updateSignalStrength(-30);
    CHECK(wifi.signalQualityPercent() == 100.0);

    wifi.updateSignalStrength(-95);
    CHECK(wifi.signalQualityPercent() == 0.0);

    wifi.disconnectWifi();
    CHECK_FALSE(wifi.isConnected());

    CHECK_THROWS_AS(wifi.connectSsid("", "password123"), InvalidConfigurationException);
    CHECK_THROWS_AS(wifi.connectSsid("SSID", "short"), InvalidConfigurationException);
}

TEST_CASE("NetworkConnection data transmission and lifecycle", "[network_connection]") {
    NetworkConnection conn(1, IpAddress({192, 168, 1, 1}, 24), 443, "ESTABLISHED");
    CHECK(conn.connectionId() == 1);
    CHECK(conn.remotePort() == 443);
    CHECK(conn.isOpen());
    CHECK(conn.bytesTransferred() == 0);

    conn.transmitData(4096);
    CHECK(conn.bytesTransferred() == 4096);

    conn.close();
    CHECK_FALSE(conn.isOpen());
    CHECK_THROWS_AS(conn.transmitData(100), NetworkException);

    conn.reopen(80);
    CHECK(conn.isOpen());
    CHECK(conn.remotePort() == 80);

    CHECK_THROWS_AS(NetworkConnection(2, IpAddress(), 0), InvalidConfigurationException);
    CHECK_THROWS_AS(conn.reopen(0), InvalidConfigurationException);
    CHECK_FALSE(conn.describe().empty());
}

TEST_CASE("UsbPort power draw and speed", "[usb_port]") {
    UsbPort port1(1, "USB_3", 900);
    CHECK(port1.portNumber() == 1);
    CHECK(port1.version() == "USB_3");
    CHECK(port1.maxCurrentMa() == 900);
    CHECK(port1.speedMbps() == 5000.0);
    CHECK(port1.isAvailable());

    UsbPort port2(2, "USB_2", 500);
    CHECK(port2.speedMbps() == 480.0);
    UsbPort portC(3, "USB_C", 3000);
    CHECK(portC.speedMbps() == 10000.0);
    port2.plugDevice("FullDongle", 500);
    CHECK(port2.availableCurrentMa() == 0);
    CHECK_THROWS_AS(UsbPort(4, "USB_3", 0), InvalidConfigurationException);

    port1.plugDevice("Mouse", 100);
    CHECK(port1.isOccupied());
    CHECK_FALSE(port1.isAvailable());
    CHECK(port1.connectedDevice() == "Mouse");
    CHECK(port1.availableCurrentMa() == 800);

    CHECK_THROWS_AS(port1.plugDevice("Keyboard", 100), PortOccupiedException);

    port1.unplugDevice();
    CHECK(port1.isAvailable());
    CHECK_THROWS_AS(port1.unplugDevice(), DeviceNotConnectedException);

    CHECK_THROWS_AS(port1.plugDevice("HeatingPad", 2000), PowerSupplyOverloadException);
}

TEST_CASE("UsbHub device management and current load", "[usb_hub]") {
    UsbHub hub("FrontPanelHub", 4);
    CHECK(hub.ports().size() == 4);
    CHECK(hub.connectedDevicesCount() == 0);
    CHECK(hub.totalCurrentDrawMa() == 0);

    hub.plugDevice(1, "Flash Drive", 200);
    hub.plugDevice(2, "Wireless Dongle", 150);
    CHECK(hub.connectedDevicesCount() == 2);
    CHECK(hub.totalCurrentDrawMa() == 350);

    hub.unplugDevice(1);
    CHECK(hub.connectedDevicesCount() == 1);
    CHECK(hub.totalCurrentDrawMa() == 150);

    const UsbHub constHub("ConstHub", 2);
    CHECK(constHub.findPort(1).portNumber() == 1);
    CHECK_THROWS_AS(constHub.findPort(99), ComponentNotFoundException);
    CHECK_THROWS_AS(UsbHub(""), InvalidConfigurationException);

    UsbPort extraPort(5, "USB_C", 3000);
    hub.addPort(extraPort);
    CHECK(hub.ports().size() == 5);

    CHECK_THROWS_AS(hub.findPort(99), ComponentNotFoundException);
}

TEST_CASE("Keyboard, Mouse and Webcam inputs", "[peripherals]") {
    Keyboard kb("Keychron K2", "QWERTY", true, true);
    CHECK(kb.isMechanical());
    CHECK(kb.isBacklightActive());
    CHECK(kb.pressedKeysCount() == 0);

    kb.pressKey("Enter");
    kb.pressKey("A");
    CHECK(kb.pressedKeysCount() == 2);
    kb.resetKeys();
    CHECK(kb.pressedKeysCount() == 0);

    kb.switchLayout("JCUKEN");
    CHECK(kb.layout() == "JCUKEN");
    kb.toggleBacklight();
    CHECK_FALSE(kb.isBacklightActive());
    CHECK_FALSE(kb.describe().empty());

    CHECK_THROWS_AS(Keyboard("", "QWERTY"), InvalidConfigurationException);
    CHECK_THROWS_AS(Keyboard("Model", ""), InvalidConfigurationException);
    CHECK_THROWS_AS(kb.pressKey(""), InvalidConfigurationException);
    CHECK_THROWS_AS(kb.switchLayout(""), InvalidConfigurationException);

    Mouse mouse("Razer DeathAdder", 1600, 5, 1000);
    CHECK_THROWS_AS(Mouse("M", 50, 5, 1000), InvalidConfigurationException);
    CHECK_THROWS_AS(mouse.setDpi(50), InvalidConfigurationException);
    CHECK(mouse.currentDpi() == 1600);
    CHECK(mouse.clickCount() == 0);

    mouse.click(1);
    mouse.click(2);
    CHECK(mouse.clickCount() == 2);
    CHECK_THROWS_AS(mouse.click(10), ComponentNotFoundException);

    const double dist = mouse.move(30, 40);
    CHECK(dist == 50.0);

    mouse.setDpi(3200);
    CHECK(mouse.isHighPrecision());
    CHECK_FALSE(mouse.describe().empty());

    Webcam cam("Logitech Brio", 3840, 2160, 60);
    CHECK_FALSE(cam.isStreaming());
    cam.startStream();
    CHECK(cam.isStreaming());
    CHECK(cam.bandwidthRequiredMBs() > 100.0);
    cam.stopStream();
    CHECK_FALSE(cam.isStreaming());

    ByteSize photo = cam.capturePhoto();
    CHECK(photo.bytes() > 0);
    CHECK_FALSE(cam.describe().empty());
    CHECK_THROWS_AS(Webcam("Cam", 0, 1080, 30), InvalidConfigurationException);
}
