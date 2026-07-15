#include "ConsoleMVC/AppController.h"

namespace cmvc {

void AppController::Run() {
    while (true) {
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
            case 8: m_adminController.Run(); break;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

}
