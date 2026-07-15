#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"
#include "DataPersistence/OrderWorkflow.h"

namespace cmvc {

class ApprovalController {
public:
    // Runs the approval/rejection submenu. Stub in this phase — see docs/phases/phase4.md.
    void Run();

private:
    ConsoleView m_view;
};

}
