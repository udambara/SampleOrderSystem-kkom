#pragma once

#include <optional>
#include <string>
#include <vector>
#include "DataPersistence/Order.h"

namespace dp {

class OrderRepository {
public:
    explicit OrderRepository(std::string filePath = "data/orders.json");

    bool Add(const Order& order, std::string& errorMessage);

    // Replaces the stored order with the same id. Returns false if not found.
    bool Update(const Order& order);

    std::vector<Order> GetAll() const;

    std::optional<Order> FindById(const std::string& id) const;

    // Formats as ORD-YYYYMMDD-NNNN using the current date and existing order count + 1.
    std::string GenerateOrderNo() const;

private:
    std::string m_filePath;

    std::vector<Order> Load() const;
    void Save(const std::vector<Order>& orders) const;
};

}
