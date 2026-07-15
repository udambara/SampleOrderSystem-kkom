#pragma once

#include <string>
#include <vector>
#include "DataPersistence/Sample.h"

namespace cmvc {

class ConsoleView {
public:
    void PrintMainMenu() const;
    void PrintNotImplemented(const std::string& menuName) const;
    void PrintMessage(const std::string& message) const;
    void PrintError(const std::string& message) const;

    // Reads an integer menu choice; re-prompts on invalid (non-numeric) input.
    int ReadMenuChoice(const std::string& prompt) const;

    void PrintSampleMenu() const;
    void PrintSampleList(const std::vector<dp::Sample>& samples) const;
    dp::Sample ReadNewSample() const;

    void PrintSampleSearchMenu() const;
    // label is interpolated into the prompt, e.g. "검색어(이름): ".
    std::string ReadSearchKeyword(const std::string& label) const;
};

}
