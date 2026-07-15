#include "ConsoleMVC/AdminController.h"

namespace cmvc {

namespace {
constexpr int kRecentLogCount = 20;
}

void AdminController::Run() {
    while (true) {
        std::optional<dp::Order> completed = m_workflow.Tick();
        if (completed.has_value()) {
            m_logger.LogOrderTransition(completed->orderNo, "PRODUCING", "CONFIRMED");
        }

        m_view.PrintAdminMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleCreateTestOrder(); break;
            case 2: HandleViewLogs(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void AdminController::HandleCreateTestOrder() {
    TestOrderInput input = m_view.ReadTestOrderInput();

    if (!m_sampleRepo.FindById(input.sampleId).has_value()) {
        m_view.PrintError("존재하지 않는 시료ID입니다: " + input.sampleId);
        return;
    }
    if (input.quantity < 1) {
        m_view.PrintError("주문수량은 1 이상이어야 합니다.");
        return;
    }
    if (input.totalProductionSeconds < 1) {
        m_view.PrintError("생산시간(초)은 1 이상이어야 합니다.");
        return;
    }

    dp::Order order;
    order.orderNo = m_orderRepo.GenerateOrderNo();
    order.id = order.orderNo;
    order.sampleId = input.sampleId;
    order.customerName = input.customerName;
    order.quantity = input.quantity;
    order.status = dp::OrderStatus::PRODUCING;
    order.shortageQty = input.quantity;
    order.actualProductionQty = input.quantity;
    order.totalProductionSeconds = input.totalProductionSeconds;

    dp::Order enqueued = m_workflow.EnqueueTestOrder(order);
    m_logger.LogOrderTransition(enqueued.orderNo, "-", "PRODUCING");
    m_view.PrintTestOrderResult(enqueued, enqueued.productionStartEpochSec > 0);
}

void AdminController::HandleViewLogs() {
    m_view.PrintLogEntries(m_logger.ReadRecentTransitions(kRecentLogCount));
}

}
