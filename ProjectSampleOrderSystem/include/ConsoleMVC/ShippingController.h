#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace cmvc {

class ShippingController {
public:
    // Runs the shipping submenu. Stub in this phase — see docs/phases/phase6.md.
    void Run();

private:
    ConsoleView m_view;
};

}
