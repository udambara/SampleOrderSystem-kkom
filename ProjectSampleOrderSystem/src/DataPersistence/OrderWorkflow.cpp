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
        order.status = OrderStatus::CONFIRMED;
        m_orderRepo.Update(order);
        return order;
    }

    int shortage = order.quantity - sample->stock;
    int actualProductionQty = static_cast<int>(std::ceil(shortage / sample->yield));

    order.status = OrderStatus::PRODUCING;
    order.shortageQty = shortage;
    order.totalProductionSeconds =
        static_cast<long long>(sample->avgProductionTimeMinutes * actualProductionQty);

    bool lineBusy = IsLineBusy(m_orderRepo.GetAll());
    order.productionStartEpochSec = lineBusy ? 0 : NowEpochSec();

    m_orderRepo.Update(order);
    return order;
}

Order OrderWorkflow::Reject(Order order) {
    order.status = OrderStatus::REJECTED;
    m_orderRepo.Update(order);
    return order;
}

}
