#include "ConsoleMVC/AppController.h"

namespace cmvc {

void AppController::Run() {
    while (true) {
        std::optional<dp::Order> completed = m_workflow.Tick();
        if (completed.has_value()) {
            m_logger.LogOrderTransition(completed->orderNo, "PRODUCING", "CONFIRMED");
        }

        m_view.PrintMainMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: m_sampleController.Run(); break;
            case 2: m_orderController.Run(); break;
            case 3: m_approvalController.Run(); break;
            case 4: m_monitoringController.Run(); break;
            case 5: m_shippingController.Run(); break;
            case 6: m_productionController.Run(); break;
            case 7: return;
            // 8(관리자모드)은 메인 메뉴에서 비활성화됨 — AdminController는 유지하되 라우팅만 제거.
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

}
