#include "ConsoleMVC/ConsoleView.h"

#include <iomanip>
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

void ConsoleView::PrintSampleMenu() const {
    std::cout << "\n--- 시료관리 ---\n"
               << "1. 시료등록\n"
               << "2. 시료조회\n"
               << "3. 시료검색\n"
               << "0. 이전 메뉴\n";
}

void ConsoleView::PrintSampleList(const std::vector<dp::Sample>& samples) const {
    if (samples.empty()) {
        std::cout << "등록된 시료가 없습니다.\n";
        return;
    }
    std::cout << std::left << std::setw(8) << "시료ID"
               << std::setw(22) << "이름"
               << std::right << std::setw(12) << "평균생산시간"
               << std::setw(8) << "수율"
               << std::setw(10) << "재고" << "\n";
    std::cout << std::string(62, '-') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const dp::Sample& sample : samples) {
        std::cout << std::left << std::setw(8) << sample.id
                   << std::setw(22) << sample.name
                   << std::right << std::setw(10) << sample.avgProductionTimeMinutes << "분"
                   << std::setw(7) << sample.yield
                   << std::setw(8) << sample.stock << "ea" << "\n";
    }
}

dp::Sample ConsoleView::ReadNewSample() const {
    dp::Sample sample;
    std::cout << "시료ID: ";
    std::getline(std::cin, sample.id);
    std::cout << "이름: ";
    std::getline(std::cin, sample.name);

    std::cout << "평균 생산시간(분): ";
    std::cin >> sample.avgProductionTimeMinutes;
    std::cout << "수율(0~1): ";
    std::cin >> sample.yield;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    sample.stock = 0;
    return sample;
}

void ConsoleView::PrintSampleSearchMenu() const {
    std::cout << "\n--- 시료검색 ---\n"
               << "1. 시료ID로 검색\n"
               << "2. 이름으로 검색\n"
               << "0. 취소\n";
}

std::string ConsoleView::ReadSearchKeyword(const std::string& label) const {
    std::cout << "검색어(" << label << "): ";
    std::string keyword;
    std::getline(std::cin, keyword);
    return keyword;
}

}
