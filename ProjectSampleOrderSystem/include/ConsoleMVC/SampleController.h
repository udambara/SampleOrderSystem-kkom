#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"

namespace cmvc {

class SampleController {
public:
    // Runs the sample management submenu. Stub in this phase — see docs/phases/phase2.md.
    void Run();

private:
    dp::SampleRepository m_sampleRepo;
    ConsoleView m_view;
};

}
