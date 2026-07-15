#include "DataMonitor/DataMonitor.h"

#include <iomanip>
#include <iostream>

namespace mon {

void PrintOrderStatusTable(const dp::OrderRepository& orderRepo) {
    std::map<dp::OrderStatus, int> counts = orderRepo.CountByStatus();

    std::cout << "\n[주문량확인]\n";
    std::cout << std::left << std::setw(12) << "상태" << std::right << std::setw(6) << "건수" << "\n";
    std::cout << std::string(18, '-') << "\n";
    std::cout << std::left << std::setw(12) << "RESERVED" << std::right << std::setw(6) << counts[dp::OrderStatus::RESERVED] << "\n";
    std::cout << std::left << std::setw(12) << "CONFIRMED" << std::right << std::setw(6) << counts[dp::OrderStatus::CONFIRMED] << "\n";
    std::cout << std::left << std::setw(12) << "PRODUCING" << std::right << std::setw(6) << counts[dp::OrderStatus::PRODUCING] << "\n";
    std::cout << std::left << std::setw(12) << "RELEASE" << std::right << std::setw(6) << counts[dp::OrderStatus::RELEASE] << "\n";
}

namespace {

std::string InventoryStatusLabel(int stock, int outstandingDemand) {
    if (stock == 0) return "고갈";
    if (stock - outstandingDemand >= 0) return "여유";
    return "부족";
}

// Sum of quantities for orders against this sample that still owe stock:
// RESERVED (may yet be approved), CONFIRMED that reached that status via
// production (awaiting shipment), and PRODUCING (awaiting shipment once
// production finishes). REJECTED and RELEASE orders no longer draw on
// inventory, and neither do CONFIRMED orders whose stock was already
// deducted at approval time (stock.stock already reflects them).
int OutstandingDemand(const std::string& sampleId, const std::vector<dp::Order>& orders) {
    int demand = 0;
    for (const dp::Order& order : orders) {
        if (order.sampleId != sampleId) continue;
        if (order.status == dp::OrderStatus::REJECTED || order.status == dp::OrderStatus::RELEASE) continue;
        if (order.status == dp::OrderStatus::CONFIRMED && order.stockDeductedAtApproval) continue;
        demand += order.quantity;
    }
    return demand;
}

}

void PrintInventoryTable(const dp::SampleRepository& sampleRepo, const dp::OrderRepository& orderRepo) {
    std::vector<dp::Sample> samples = sampleRepo.GetAll();
    std::vector<dp::Order> orders = orderRepo.GetAll();

    std::cout << "\n[재고량확인]\n";
    if (samples.empty()) {
        std::cout << "등록된 시료가 없습니다.\n";
        return;
    }

    std::cout << std::left << std::setw(8) << "시료ID"
               << std::setw(22) << "이름"
               << std::right << std::setw(8) << "재고"
               << "  " << "상태" << "\n";
    std::cout << std::string(48, '-') << "\n";
    for (const dp::Sample& sample : samples) {
        int demand = OutstandingDemand(sample.id, orders);
        std::cout << std::left << std::setw(8) << sample.id
                   << std::setw(22) << sample.name
                   << std::right << std::setw(6) << sample.stock << "ea"
                   << "  " << InventoryStatusLabel(sample.stock, demand) << "\n";
    }
}

}
