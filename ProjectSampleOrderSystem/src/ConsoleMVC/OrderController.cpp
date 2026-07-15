#include "ConsoleMVC/OrderController.h"

namespace cmvc {

void OrderController::Run() {
    while (true) {
        m_view.PrintOrderMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleReserve(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void OrderController::HandleReserve() {
    NewOrderInput input = m_view.ReadNewOrderInput();

    if (!m_sampleRepo.FindById(input.sampleId).has_value()) {
        m_view.PrintError("존재하지 않는 시료ID입니다: " + input.sampleId);
        return;
    }
    if (input.quantity < 1) {
        m_view.PrintError("주문수량은 1 이상이어야 합니다.");
        return;
    }

    dp::Order order;
    order.orderNo = m_orderRepo.GenerateOrderNo();
    order.id = order.orderNo;
    order.sampleId = input.sampleId;
    order.customerName = input.customerName;
    order.quantity = input.quantity;
    order.status = dp::OrderStatus::RESERVED;

    std::string errorMessage;
    if (!m_orderRepo.Add(order, errorMessage)) {
        m_view.PrintError(errorMessage);
        return;
    }

    m_logger.LogOrderTransition(order.orderNo, "-", "RESERVED");
    m_view.PrintMessage("주문이 접수되었습니다: " + order.orderNo);
}

}
