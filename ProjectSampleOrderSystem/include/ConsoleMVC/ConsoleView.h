#pragma once

#include <string>

namespace cmvc {

class ConsoleView {
public:
    void PrintMainMenu() const;
    void PrintNotImplemented(const std::string& menuName) const;
    void PrintMessage(const std::string& message) const;
    void PrintError(const std::string& message) const;

    // Reads an integer menu choice; re-prompts on invalid (non-numeric) input.
    int ReadMenuChoice(const std::string& prompt) const;
};

}
