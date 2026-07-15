#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace cmvc {

class ProductionController {
public:
    // Runs the production-line submenu. Stub in this phase — see docs/phases/phase5.md.
    void Run();

private:
    ConsoleView m_view;
};

}
