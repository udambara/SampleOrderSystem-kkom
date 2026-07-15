#include "ConsoleMVC/ConsoleView.h"

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>

namespace cmvc {

namespace {

// Input is exhausted (e.g. redirected from a file, or Ctrl+D/Ctrl+Z) — there is
// no valid choice left to read, so exit cleanly instead of looping forever.
void ExitOnEof() {
    if (std::cin.eof()) {
        std::exit(0);
    }
}

}

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
        ExitOnEof();
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

void ConsoleView::PrintOrderMenu() const {
    std::cout << "\n--- 시료주문 ---\n"
               << "1. 시료예약\n"
               << "0. 이전 메뉴\n";
}

NewOrderInput ConsoleView::ReadNewOrderInput() const {
    NewOrderInput input;
    std::cout << "시료ID: ";
    std::getline(std::cin, input.sampleId);
    std::cout << "고객명: ";
    std::getline(std::cin, input.customerName);
    std::cout << "주문수량: ";
    std::cin >> input.quantity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

void ConsoleView::PrintApprovalMenu() const {
    std::cout << "\n--- 시료승인/거절 ---\n"
               << "1. 접수된 주문목록\n"
               << "2. 주문승인\n"
               << "3. 주문거절\n"
               << "0. 이전 메뉴\n";
}

void ConsoleView::PrintOrderTable(const std::vector<dp::Order>& orders, const dp::SampleRepository& sampleRepo) const {
    if (orders.empty()) {
        std::cout << "해당하는 주문이 없습니다.\n";
        return;
    }
    std::cout << std::left << std::setw(5) << "번호"
               << std::setw(20) << "주문번호"
               << std::setw(12) << "고객명"
               << std::setw(22) << "시료명"
               << std::right << std::setw(8) << "수량"
               << "  " << "상태" << "\n";
    std::cout << std::string(75, '-') << "\n";

    int index = 1;
    for (const dp::Order& order : orders) {
        std::optional<dp::Sample> sample = sampleRepo.FindById(order.sampleId);
        std::string sampleName = sample.has_value() ? sample->name : order.sampleId;

        std::cout << std::left << std::setw(5) << index
                   << std::setw(20) << order.orderNo
                   << std::setw(12) << order.customerName
                   << std::setw(22) << sampleName
                   << std::right << std::setw(6) << order.quantity << "ea"
                   << "  " << dp::ToString(order.status) << "\n";
        ++index;
    }
}

int ConsoleView::ReadIndexChoice(int count, const std::string& prompt) const {
    std::cout << prompt << " (0=취소, 1~" << count << "): ";
    int choice = 0;
    while (!(std::cin >> choice) || choice < 0 || choice > count) {
        ExitOnEof();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "0~" << count << " 사이의 번호를 입력해주세요: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void ConsoleView::PrintProductionMenu() const {
    std::cout << "\n--- 생산라인 ---\n"
               << "1. 생산현황표기\n"
               << "2. 대기주문확인\n"
               << "0. 이전 메뉴\n";
}

void ConsoleView::PrintProductionStatus(const std::optional<dp::Order>& current, const dp::SampleRepository& sampleRepo) const {
    std::cout << "\n[생산현황]\n";
    if (!current.has_value()) {
        std::cout << "생산중인 주문이 없습니다.\n";
        return;
    }

    std::optional<dp::Sample> sample = sampleRepo.FindById(current->sampleId);
    std::string sampleName = sample.has_value() ? sample->name : current->sampleId;

    long long elapsed = static_cast<long long>(std::time(nullptr)) - current->productionStartEpochSec;
    long long remaining = current->totalProductionSeconds - elapsed;
    if (remaining < 0) remaining = 0;

    std::cout << "주문번호: " << current->orderNo << "\n"
               << "시료명: " << sampleName << "\n"
               << "경과시간: " << elapsed << "초 / 총 " << current->totalProductionSeconds << "초\n"
               << "남은시간: " << remaining << "초\n";
}

void ConsoleView::PrintWaitingQueue(const std::vector<dp::Order>& waiting, const dp::SampleRepository& sampleRepo) const {
    std::cout << "\n[대기주문] (FIFO)\n";
    if (waiting.empty()) {
        std::cout << "대기중인 주문이 없습니다.\n";
        return;
    }
    int index = 1;
    for (const dp::Order& order : waiting) {
        std::optional<dp::Sample> sample = sampleRepo.FindById(order.sampleId);
        std::string sampleName = sample.has_value() ? sample->name : order.sampleId;
        std::cout << index << ". " << order.orderNo << " | " << sampleName
                   << " | " << order.quantity << "ea\n";
        ++index;
    }
}

}
