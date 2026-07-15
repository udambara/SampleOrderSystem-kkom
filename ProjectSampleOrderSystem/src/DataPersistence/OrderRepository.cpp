#include "DataPersistence/OrderRepository.h"
#include "third_party/nlohmann/json.hpp"

#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace dp {

OrderRepository::OrderRepository(std::string filePath) : m_filePath(std::move(filePath)) {
    if (!std::filesystem::exists(m_filePath)) {
        Save({});
    }
}

std::vector<Order> OrderRepository::Load() const {
    std::vector<Order> orders;
    std::ifstream in(m_filePath);
    if (!in.is_open()) {
        return orders;
    }
    nlohmann::json j;
    in >> j;
    for (const auto& item : j) {
        orders.push_back(item.get<Order>());
    }
    return orders;
}

void OrderRepository::Save(const std::vector<Order>& orders) const {
    std::filesystem::path path(m_filePath);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    nlohmann::json j = orders;
    std::ofstream out(m_filePath);
    out << j.dump(2);
}

bool OrderRepository::Add(const Order& order, std::string& errorMessage) {
    std::vector<Order> orders = Load();
    if (std::any_of(orders.begin(), orders.end(),
                     [&](const Order& o) { return o.id == order.id; })) {
        errorMessage = "이미 존재하는 주문ID입니다: " + order.id;
        return false;
    }
    orders.push_back(order);
    Save(orders);
    return true;
}

bool OrderRepository::Update(const Order& order) {
    std::vector<Order> orders = Load();
    auto it = std::find_if(orders.begin(), orders.end(),
                            [&](const Order& o) { return o.id == order.id; });
    if (it == orders.end()) {
        return false;
    }
    *it = order;
    Save(orders);
    return true;
}

std::vector<Order> OrderRepository::GetAll() const {
    return Load();
}

std::vector<Order> OrderRepository::GetByStatus(OrderStatus status) const {
    std::vector<Order> result;
    for (const auto& order : Load()) {
        if (order.status == status) {
            result.push_back(order);
        }
    }
    return result;
}

std::optional<Order> OrderRepository::FindById(const std::string& id) const {
    std::vector<Order> orders = Load();
    auto it = std::find_if(orders.begin(), orders.end(),
                            [&](const Order& o) { return o.id == id; });
    if (it == orders.end()) {
        return std::nullopt;
    }
    return *it;
}

std::map<OrderStatus, int> OrderRepository::CountByStatus() const {
    std::map<OrderStatus, int> counts{
        {OrderStatus::RESERVED, 0},
        {OrderStatus::CONFIRMED, 0},
        {OrderStatus::PRODUCING, 0},
        {OrderStatus::RELEASE, 0},
        {OrderStatus::REJECTED, 0}
    };
    for (const auto& order : Load()) {
        counts[order.status]++;
    }
    return counts;
}

std::string OrderRepository::GenerateOrderNo() const {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    std::ostringstream oss;
    oss << "ORD-" << std::put_time(&localTime, "%Y%m%d") << "-"
        << std::setw(4) << std::setfill('0') << (static_cast<int>(Load().size()) + 1);
    return oss.str();
}

}
