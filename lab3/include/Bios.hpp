#pragma once

#include <string>
#include <vector>

/**
 * @brief Представляет базовую систему ввода-вывода (BIOS/UEFI) материнской платы.
 */
class Bios {
public:
    /**
     * @brief Создаёт объект BIOS.
     * @param version Версия прошивки.
     * @param vendor Производитель (например, "AMI", "Award", "Phoenix").
     * @param uefiEnabled Режим UEFI.
     * @param secureBoot Состояние безопасной загрузки Secure Boot.
     */
    Bios(std::string version = "1.0.0", std::string vendor = "American Megatrends",
         bool uefiEnabled = true, bool secureBoot = true);

    /**
     * @brief Возвращает версию прошивки.
     * @return Строка версии.
     */
    const std::string& version() const { return version_; }

    /**
     * @brief Возвращает производителя BIOS.
     * @return Имя производителя.
     */
    const std::string& vendor() const { return vendor_; }

    /**
     * @brief Проверяет включён ли режим UEFI.
     * @return true, если активен UEFI.
     */
    bool isUefiEnabled() const { return uefiEnabled_; }

    /**
     * @brief Проверяет состояние Secure Boot.
     * @return true, если включена безопасная загрузка.
     */
    bool isSecureBootEnabled() const { return secureBoot_; }

    /**
     * @brief Возвращает порядок загрузочных устройств.
     * @return Список идентификаторов устройств.
     */
    const std::vector<std::string>& bootOrder() const { return bootOrder_; }

    /**
     * @brief Обновляет прошивку BIOS до новой версии.
     * @param newVersion Новая версия прошивки.
     */
    void updateFirmware(std::string newVersion);

    /**
     * @brief Включает или выключает Secure Boot.
     * @param enable Состояние.
     */
    void setSecureBoot(bool enable);

    /**
     * @brief Устанавливает приоритет загрузочных устройств.
     * @param order Список устройств.
     */
    void setBootOrder(std::vector<std::string> order);

    /**
     * @brief Возвращает первое по приоритету загрузочное устройство.
     * @return Название первого устройства.
     */
    const std::string& firstBootDevice() const;

    /**
     * @brief Выполняет процедуру самотестирования при включении (POST).
     * @param hasCpu Наличие установленного процессора.
     * @param hasRam Наличие оперативной памяти.
     * @return true, если POST пройден успешно.
     */
    bool performPost(bool hasCpu, bool hasRam) const;

private:
    std::string version_;
    std::string vendor_;
    bool uefiEnabled_;
    bool secureBoot_;
    std::vector<std::string> bootOrder_;
};
