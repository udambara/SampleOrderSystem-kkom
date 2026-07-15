#include "DataPersistence/OrderWorkflow.h"

namespace dp {

OrderWorkflow::OrderWorkflow(SampleRepository& sampleRepo, OrderRepository& orderRepo)
    : m_sampleRepo(sampleRepo), m_orderRepo(orderRepo) {}

}
