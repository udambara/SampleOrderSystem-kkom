#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace cmvc {

class ProductionController {
public:
    // Runs the production-line submenu loop until the user chooses to go back.
    void Run();

private:
    dp::SampleRepository m_sampleRepo;
    dp::OrderRepository m_orderRepo;
    ConsoleView m_view;

    void HandleStatus();
    void HandleWaitingQueue();
};

}
