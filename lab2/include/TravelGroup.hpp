#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Customer.hpp"

/**
 * @brief Представляет группу путешественников с руководителем, списком участников и ограничением размера.
 */
class TravelGroup {
public:
    /**
     * @brief Создаёт объект класса TravelGroup и инициализирует его исходные данные.
     * @param name название объекта.
     * @param leader руководитель группы.
     * @param maxSize максимальный размер группы.
     */
    TravelGroup(std::string name, std::shared_ptr<Customer> leader, std::size_t maxSize);
    /**
     * @brief Возвращает название или имя объекта.
     * @return Ссылка на сохранённый атрибут `name_`.
     */
    const std::string& name() const { return name_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Ссылка на сохранённый атрибут `leader_`.
     */
    const std::shared_ptr<Customer>& leader() const { return leader_; }
    /**
     * @brief Возвращает список участников группы.
     * @return Ссылка на сохранённый атрибут `members_`.
     */
    const std::vector<std::shared_ptr<Customer>>& members() const { return members_; }
    /**
     * @brief Возвращает значение, сохранённое в объекте.
     * @return Значение сохранённого атрибута `maxSize_`.
     */
    std::size_t maxSize() const { return maxSize_; }
    /**
     * @brief Добавляет клиента в группу.
     * @param customer клиент.
     */
    void addMember(const std::shared_ptr<Customer>& customer);
    /**
     * @brief Удаляет участника группы по идентификатору клиента.
     * @param customerId идентификатор клиента.
     */
    void removeMember(int customerId);
    /**
     * @brief Проверяет наличие участника с указанным идентификатором.
     * @param customerId идентификатор клиента.
     * @return Результат проверки условия.
     */
    bool hasMember(int customerId) const;
    /**
     * @brief Проверяет, достигнута ли максимальная вместимость.
     * @return Результат проверки условия.
     */
    bool isFull() const;
    /**
     * @brief Проверяет, удовлетворяет ли группа условию предоставления скидки.
     * @return Результат проверки условия.
     */
    bool isDiscountEligible() const;

private:
    std::string name_;
    std::shared_ptr<Customer> leader_;
    std::vector<std::shared_ptr<Customer>> members_;
    std::size_t maxSize_;
};
