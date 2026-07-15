#include "ConsoleMVC/ApprovalController.h"

namespace cmvc {

void ApprovalController::Run() {
    while (true) {
        m_view.PrintApprovalMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleList(); break;
            case 2: HandleApprove(); break;
            case 3: HandleReject(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void ApprovalController::HandleList() {
    m_view.PrintOrderTable(m_orderRepo.GetByStatus(dp::OrderStatus::RESERVED), m_sampleRepo);
}

void ApprovalController::HandleApprove() {
    std::vector<dp::Order> orders = m_orderRepo.GetByStatus(dp::OrderStatus::RESERVED);
    m_view.PrintOrderTable(orders, m_sampleRepo);
    int index = m_view.ReadIndexChoice(static_cast<int>(orders.size()), "승인할 주문 번호");
    if (index == 0) {
        return;
    }

    dp::Order updated = m_workflow.Approve(orders[index - 1]);
    m_logger.LogOrderTransition(updated.orderNo, "RESERVED", dp::ToString(updated.status));
    m_view.PrintMessage("주문이 승인되었습니다: " + updated.orderNo + " (" + dp::ToString(updated.status) + ")");
}

void ApprovalController::HandleReject() {
    std::vector<dp::Order> orders = m_orderRepo.GetByStatus(dp::OrderStatus::RESERVED);
    m_view.PrintOrderTable(orders, m_sampleRepo);
    int index = m_view.ReadIndexChoice(static_cast<int>(orders.size()), "거절할 주문 번호");
    if (index == 0) {
        return;
    }

    dp::Order updated = m_workflow.Reject(orders[index - 1]);
    m_logger.LogOrderTransition(updated.orderNo, "RESERVED", dp::ToString(updated.status));
    m_view.PrintMessage("주문이 거절되었습니다: " + updated.orderNo);
}

}
