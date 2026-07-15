#pragma once

#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace dp {

// Encapsulates the approve/reject decision and the production-line advancement
// (a demo-speed simulation: minutes of average production time are treated as seconds).
// Tick is filled in during Phase 5 (see docs/phases/phase5.md).
class OrderWorkflow {
public:
    OrderWorkflow(SampleRepository& sampleRepo, OrderRepository& orderRepo);

    // order must currently be RESERVED. Returns the order with its status updated
    // to CONFIRMED (enough stock) or PRODUCING (queued/started for production).
    Order Approve(Order order);

    // order must currently be RESERVED. Returns the order with status REJECTED.
    Order Reject(Order order);

    // True if a production job is currently running.
    bool IsLineBusy(const std::vector<Order>& orders) const;

private:
    SampleRepository& m_sampleRepo;
    OrderRepository& m_orderRepo;
};

}
