#include "ConsoleMVC/ProductionController.h"

#include <algorithm>

namespace cmvc {

void ProductionController::Run() {
    while (true) {
        m_view.PrintProductionMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleStatus(); break;
            case 2: HandleWaitingQueue(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void ProductionController::HandleStatus() {
    std::vector<dp::Order> orders = m_orderRepo.GetAll();
    auto it = std::find_if(orders.begin(), orders.end(), [](const dp::Order& o) {
        return o.status == dp::OrderStatus::PRODUCING && o.productionStartEpochSec > 0;
    });
    std::optional<dp::Order> current;
    if (it != orders.end()) {
        current = *it;
    }
    m_view.PrintProductionStatus(current, m_sampleRepo);
}

void ProductionController::HandleWaitingQueue() {
    std::vector<dp::Order> waiting;
    for (const dp::Order& order : m_orderRepo.GetAll()) {
        if (order.status == dp::OrderStatus::PRODUCING && order.productionStartEpochSec == 0) {
            waiting.push_back(order);
        }
    }
    m_view.PrintWaitingQueue(waiting, m_sampleRepo);
}

}
