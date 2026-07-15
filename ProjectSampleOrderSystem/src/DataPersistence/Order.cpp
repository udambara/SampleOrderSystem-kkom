#include "DataPersistence/Order.h"
#include <stdexcept>

namespace dp {

std::string ToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::RESERVED: return "RESERVED";
        case OrderStatus::CONFIRMED: return "CONFIRMED";
        case OrderStatus::PRODUCING: return "PRODUCING";
        case OrderStatus::RELEASE: return "RELEASE";
        case OrderStatus::REJECTED: return "REJECTED";
    }
    throw std::invalid_argument("Unknown OrderStatus");
}

OrderStatus OrderStatusFromString(const std::string& text) {
    if (text == "RESERVED") return OrderStatus::RESERVED;
    if (text == "CONFIRMED") return OrderStatus::CONFIRMED;
    if (text == "PRODUCING") return OrderStatus::PRODUCING;
    if (text == "RELEASE") return OrderStatus::RELEASE;
    if (text == "REJECTED") return OrderStatus::REJECTED;
    throw std::invalid_argument("Unknown order status text: " + text);
}

void to_json(nlohmann::json& j, const Order& o) {
    j = nlohmann::json{
        {"id", o.id},
        {"orderNo", o.orderNo},
        {"sampleId", o.sampleId},
        {"customerName", o.customerName},
        {"quantity", o.quantity},
        {"status", ToString(o.status)},
        {"productionStartEpochSec", o.productionStartEpochSec},
        {"totalProductionSeconds", o.totalProductionSeconds},
        {"shortageQty", o.shortageQty},
        {"releasedAtEpochSec", o.releasedAtEpochSec},
        {"releasedQty", o.releasedQty}
    };
}

void from_json(const nlohmann::json& j, Order& o) {
    j.at("id").get_to(o.id);
    o.orderNo = j.value("orderNo", std::string());
    j.at("sampleId").get_to(o.sampleId);
    j.at("customerName").get_to(o.customerName);
    j.at("quantity").get_to(o.quantity);
    o.status = OrderStatusFromString(j.at("status").get<std::string>());
    o.productionStartEpochSec = j.value("productionStartEpochSec", 0LL);
    o.totalProductionSeconds = j.value("totalProductionSeconds", 0LL);
    o.shortageQty = j.value("shortageQty", 0);
    o.releasedAtEpochSec = j.value("releasedAtEpochSec", 0LL);
    o.releasedQty = j.value("releasedQty", 0);
}

}
