#pragma once

#include "ConsoleMVC/ConsoleView.h"
#include "DataPersistence/SampleRepository.h"

namespace cmvc {

class SampleController {
public:
    // Runs the sample management submenu loop until the user chooses to go back.
    void Run();

private:
    dp::SampleRepository m_sampleRepo;
    ConsoleView m_view;

    void HandleRegister();
    void HandleList();
    void HandleSearch();
};

}
