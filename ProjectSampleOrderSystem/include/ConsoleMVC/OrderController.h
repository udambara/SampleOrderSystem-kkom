#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace cmvc {

class OrderController {
public:
    // Runs the sample-order submenu. Stub in this phase — see docs/phases/phase3.md.
    void Run();

private:
    dp::SampleRepository m_sampleRepo;
    dp::OrderRepository m_orderRepo;
    ConsoleView m_view;
};

}
