#include "DataPersistence/OrderWorkflow.h"

#include <algorithm>
#include <cmath>
#include <ctime>

namespace dp {

namespace {

long long NowEpochSec() {
    return static_cast<long long>(std::time(nullptr));
}

}

OrderWorkflow::OrderWorkflow(SampleRepository& sampleRepo, OrderRepository& orderRepo)
    : m_sampleRepo(sampleRepo), m_orderRepo(orderRepo) {}

bool OrderWorkflow::IsLineBusy(const std::vector<Order>& orders) const {
    return std::any_of(orders.begin(), orders.end(), [](const Order& o) {
        return o.status == OrderStatus::PRODUCING && o.productionStartEpochSec > 0;
    });
}

Order OrderWorkflow::Approve(Order order) {
    std::optional<Sample> sample = m_sampleRepo.FindById(order.sampleId);
    if (!sample.has_value()) {
        return order;
    }

    if (sample->stock >= order.quantity) {
        std::string errorMessage;
        m_sampleRepo.AdjustStock(order.sampleId, -order.quantity, errorMessage);
        order.status = OrderStatus::CONFIRMED;
        order.stockDeductedAtApproval = true;
        m_orderRepo.Update(order);
        return order;
    }

    int shortage = order.quantity - sample->stock;
    int actualProductionQty = static_cast<int>(std::ceil(shortage / sample->yield));

    order.status = OrderStatus::PRODUCING;
    order.shortageQty = shortage;
    order.actualProductionQty = actualProductionQty;
    order.totalProductionSeconds =
        static_cast<long long>(sample->avgProductionTimeMinutes * actualProductionQty);

    bool lineBusy = IsLineBusy(m_orderRepo.GetAll());
    order.productionStartEpochSec = lineBusy ? 0 : NowEpochSec();

    m_orderRepo.Update(order);
    return order;
}

Order OrderWorkflow::EnqueueTestOrder(Order order) {
    bool lineBusy = IsLineBusy(m_orderRepo.GetAll());
    order.productionStartEpochSec = lineBusy ? 0 : NowEpochSec();

    std::string errorMessage;
    m_orderRepo.Add(order, errorMessage);
    return order;
}

Order OrderWorkflow::Reject(Order order) {
    order.status = OrderStatus::REJECTED;
    m_orderRepo.Update(order);
    return order;
}

std::optional<Order> OrderWorkflow::Tick() {
    std::vector<Order> orders = m_orderRepo.GetAll();

    auto currentIt = std::find_if(orders.begin(), orders.end(), [](const Order& o) {
        return o.status == OrderStatus::PRODUCING && o.productionStartEpochSec > 0;
    });

    std::optional<Order> completed;
    if (currentIt != orders.end()) {
        long long elapsed = NowEpochSec() - currentIt->productionStartEpochSec;
        if (elapsed >= currentIt->totalProductionSeconds) {
            std::string errorMessage;
            m_sampleRepo.AdjustStock(currentIt->sampleId, currentIt->actualProductionQty, errorMessage);
            currentIt->status = OrderStatus::CONFIRMED;
            m_orderRepo.Update(*currentIt);
            completed = *currentIt;
            orders = m_orderRepo.GetAll();
        }
    }

    if (!IsLineBusy(orders)) {
        auto waitingIt = std::find_if(orders.begin(), orders.end(), [](const Order& o) {
            return o.status == OrderStatus::PRODUCING && o.productionStartEpochSec == 0;
        });
        if (waitingIt != orders.end()) {
            waitingIt->productionStartEpochSec = NowEpochSec();
            m_orderRepo.Update(*waitingIt);
        }
    }

    return completed;
}

}
