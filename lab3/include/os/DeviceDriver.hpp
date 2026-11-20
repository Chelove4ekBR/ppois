#pragma once

#include <cstdint>
#include <string>

/**
 * @brief Представляет драйвер аппаратного устройства в операционной системе.
 */
class DeviceDriver {
public:
    /**
     * @brief Создаёт драйвер устройства.
     * @param driverName Имя драйвера (например, "nvidia.ko" или "e1000e").
     * @param deviceCategory Категория устройства ("GPU", "NIC", "Audio", "Storage").
     * @param version Версия драйвера.
     */
    DeviceDriver(std::string driverName, std::string deviceCategory, std::string version);

    /**
     * @brief Возвращает имя драйвера.
     * @return Название модуля.
     */
    const std::string& driverName() const { return driverName_; }

    /**
     * @brief Возвращает категорию обслуживаемого оборудования.
     * @return Категория устройства.
     */
    const std::string& deviceCategory() const { return deviceCategory_; }

    /**
     * @brief Возвращает версию драйвера.
     * @return Строка версии.
     */
    const std::string& version() const { return version_; }

    /**
     * @brief Проверяет факт загрузки драйвера в ядро ОС.
     * @return true, если модуль загружен.
     */
    bool isLoaded() const { return loaded_; }

    /**
     * @brief Загружает драйвер в пространство ядра.
     */
    void load();

    /**
     * @brief Выгружает драйвер из памяти.
     */
    void unload();

    /**
     * @brief Отправляет управляющий вызов ioctl аппаратному устройству.
     * @param commandCode Код управляющей команды.
     * @return true, если команда успешно исполнена устройством.
     */
    bool sendIoctl(uint32_t commandCode);

    /**
     * @brief Проверяет соответствие версии минимальным системным требованиям.
     * @param minVersion Минимально необходимая версия.
     * @return true, если текущая версия удовлетворяет требованиям.
     */
    bool isSupportedVersion(const std::string& minVersion) const;

    /**
     * @brief Формирует текстовое описание драйвера.
     * @return Строка описания.
     */
    std::string describe() const;

private:
    std::string driverName_;
    std::string deviceCategory_;
    std::string version_;
    bool loaded_;
};
