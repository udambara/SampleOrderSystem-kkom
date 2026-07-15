#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"
#include "DataPersistence/OrderWorkflow.h"
#include "Logging/Logger.h"

namespace cmvc {

class AdminController {
public:
    // Runs the admin-mode submenu. Stub in this phase — see docs/phases/phase9.md.
    void Run();

private:
    ConsoleView m_view;
};

}
