#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace cmvc {

class MonitoringController {
public:
    // Runs the monitoring submenu. Stub in this phase — see docs/phases/phase7.md.
    void Run();

private:
    ConsoleView m_view;
};

}
