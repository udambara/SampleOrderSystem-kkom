#pragma once

#include <string>
#include "third_party/nlohmann/json.hpp"

namespace dp {

enum class OrderStatus {
    RESERVED,
    CONFIRMED,
    PRODUCING,
    RELEASE,
    REJECTED
};

std::string ToString(OrderStatus status);
OrderStatus OrderStatusFromString(const std::string& text);

struct Order {
    std::string id;
    std::string orderNo;
    std::string sampleId;
    std::string customerName;
    int quantity = 0;
    OrderStatus status = OrderStatus::RESERVED;

    // Production line bookkeeping (only meaningful while status == PRODUCING).
    long long productionStartEpochSec = 0;  // 0 = still waiting in queue
    long long totalProductionSeconds = 0;
    int shortageQty = 0;                    // quantity to add to stock once production completes

    // Shipping bookkeeping (only meaningful once status == RELEASE).
    long long releasedAtEpochSec = 0;
    int releasedQty = 0;
};

void to_json(nlohmann::json& j, const Order& o);
void from_json(const nlohmann::json& j, Order& o);

}
