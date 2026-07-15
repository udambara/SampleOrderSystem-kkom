#pragma once

#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace dp {

// Encapsulates the approve/reject decision and the production-line advancement.
// Approve/Reject/Tick are filled in during later phases (see docs/phases/phase4.md,
// docs/phases/phase5.md) — this phase only wires up construction.
class OrderWorkflow {
public:
    OrderWorkflow(SampleRepository& sampleRepo, OrderRepository& orderRepo);

private:
    SampleRepository& m_sampleRepo;
    OrderRepository& m_orderRepo;
};

}
