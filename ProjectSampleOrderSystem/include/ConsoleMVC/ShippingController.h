#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"
#include "DataPersistence/OrderWorkflow.h"
#include "Logging/Logger.h"

namespace cmvc {

class ShippingController {
public:
    // Runs the shipping submenu loop until the user chooses to go back.
    void Run();

private:
    dp::SampleRepository m_sampleRepo;
    dp::OrderRepository m_orderRepo;
    dp::OrderWorkflow m_workflow{m_sampleRepo, m_orderRepo};
    logging::Logger m_logger;
    ConsoleView m_view;

    void HandleList();
    void HandleRelease();
};

}
