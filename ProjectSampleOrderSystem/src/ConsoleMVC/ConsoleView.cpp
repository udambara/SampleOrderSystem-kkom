#include "ConsoleMVC/ConsoleView.h"

#include <iostream>
#include <limits>

namespace cmvc {

void ConsoleView::PrintMainMenu() const {
    std::cout << "\n===== 반도체 시료 관리 프로그램 =====\n"
               << "1. 시료관리\n"
               << "2. 시료주문\n"
               << "3. 시료승인/거절\n"
               << "4. 모니터링\n"
               << "5. 출고처리\n"
               << "6. 생산라인\n"
               << "7. 종료\n"
               << "8. 관리자모드\n";
}

void ConsoleView::PrintNotImplemented(const std::string& menuName) const {
    std::cout << "[" << menuName << "] 메뉴는 아직 구현되지 않았습니다.\n";
}

void ConsoleView::PrintMessage(const std::string& message) const {
    std::cout << message << "\n";
}

void ConsoleView::PrintError(const std::string& message) const {
    std::cout << "[오류] " << message << "\n";
}

int ConsoleView::ReadMenuChoice(const std::string& prompt) const {
    std::cout << prompt;
    int choice = 0;
    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "숫자를 입력해주세요: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

}
