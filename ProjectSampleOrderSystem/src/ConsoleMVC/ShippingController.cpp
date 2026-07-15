#include "ConsoleMVC/ShippingController.h"

#include <ctime>

namespace cmvc {

void ShippingController::Run() {
    while (true) {
        m_view.PrintShippingMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleList(); break;
            case 2: HandleRelease(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void ShippingController::HandleList() {
    m_view.PrintOrderTable(m_orderRepo.GetByStatus(dp::OrderStatus::CONFIRMED), m_sampleRepo);
}

void ShippingController::HandleRelease() {
    std::vector<dp::Order> orders = m_orderRepo.GetByStatus(dp::OrderStatus::CONFIRMED);
    m_view.PrintOrderTable(orders, m_sampleRepo);
    std::vector<int> indices = m_view.ReadIndexChoices(static_cast<int>(orders.size()), "출고할 주문 번호");
    if (indices.empty()) {
        return;
    }

    for (int index : indices) {
        dp::Order order = orders[index - 1];
        std::string errorMessage;
        if (!m_sampleRepo.AdjustStock(order.sampleId, -order.quantity, errorMessage)) {
            m_view.PrintError(order.orderNo + ": " + errorMessage);
            continue;
        }

        order.status = dp::OrderStatus::RELEASE;
        order.releasedAtEpochSec = static_cast<long long>(std::time(nullptr));
        order.releasedQty = order.quantity;
        m_orderRepo.Update(order);

        m_logger.LogOrderTransition(order.orderNo, "CONFIRMED", "RELEASE");
        m_view.PrintShippingResult(order);
    }
}

}
