#include "ConsoleMVC/MonitoringController.h"
#include "DataMonitor/DataMonitor.h"

namespace cmvc {

void MonitoringController::Run() {
    while (true) {
        std::optional<dp::Order> completed = m_workflow.Tick();
        if (completed.has_value()) {
            m_logger.LogOrderTransition(completed->orderNo, "PRODUCING", "CONFIRMED");
        }

        m_view.PrintMonitoringMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleOrderStatus(); break;
            case 2: HandleInventory(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void MonitoringController::HandleOrderStatus() {
    mon::PrintOrderStatusTable(m_orderRepo);
}

void MonitoringController::HandleInventory() {
    mon::PrintInventoryTable(m_sampleRepo, m_orderRepo);
}

}
