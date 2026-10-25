#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Cpu.hpp"
#include "HardwareSensor.hpp"
#include "Motherboard.hpp"
#include "PowerSupplyUnit.hpp"

/**
 * @brief Проводит комплексную аппаратную диагностику компонентов компьютера (POST/Health-check).
 */
class HardwareDiagnostic {
public:
    /**
     * @brief Создаёт модуль аппаратной диагностики.
     */
    HardwareDiagnostic();

    /**
     * @brief Возвращает список датчиков.
     * @return Вектор HardwareSensor.
     */
    const std::vector<HardwareSensor>& sensors() const { return sensors_; }

    /**
     * @brief Возвращает список обнаруженных проблем.
     * @return Вектор сообщений о неполадках.
     */
    const std::vector<std::string>& detectedIssues() const { return detectedIssues_; }

    /**
     * @brief Проверяет факт успешного прохождения проверки.
     * @return true, если все тесты пройдены без ошибок.
     */
    bool allPassed() const { return allPassed_; }

    /**
     * @brief Регистрирует телеметрический датчик для мониторинга.
     * @param sensor Датчик HardwareSensor.
     */
    void addSensor(const HardwareSensor& sensor);

    /**
     * @brief Выполняет комплексный опрос датчиков и узлов системы.
     * @param cpu Процессор.
     * @param mobo Материнская плата.
     * @param psu Блок питания.
     */
    void runDiagnostic(const Cpu& cpu, const Motherboard& mobo, const PowerSupplyUnit& psu);

    /**
     * @brief Возвращает количество выявленных аппаратных проблем.
     * @return Число неполадок.
     */
    std::size_t issuesCount() const;

    /**
     * @brief Проверяет наличие критических проблем в оборудовании.
     * @return true, если есть аварийные датчики или перегрев.
     */
    bool hasCriticalIssues() const;

    /**
     * @brief Формирует подробный отчёт диагностики.
     * @return Текст отчёта.
     */
    std::string generateReport() const;

private:
    std::vector<HardwareSensor> sensors_;
    std::vector<std::string> detectedIssues_;
    bool allPassed_;
};
