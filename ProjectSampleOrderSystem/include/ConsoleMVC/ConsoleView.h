#pragma once

#include <optional>
#include <string>
#include <vector>
#include "DataPersistence/Sample.h"
#include "DataPersistence/Order.h"
#include "DataPersistence/SampleRepository.h"

namespace cmvc {

struct NewOrderInput {
    std::string sampleId;
    std::string customerName;
    int quantity = 0;
};

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

    void PrintOrderMenu() const;
    NewOrderInput ReadNewOrderInput() const;

    void PrintApprovalMenu() const;

    // Prints a numbered order table: 번호/주문번호/고객명/시료명/수량/상태.
    void PrintOrderTable(const std::vector<dp::Order>& orders, const dp::SampleRepository& sampleRepo) const;

    // Reads a 1-based selection index; returns 0 if the user cancels.
    int ReadIndexChoice(int count, const std::string& prompt) const;

    void PrintProductionMenu() const;
    void PrintProductionStatus(const std::optional<dp::Order>& current, const dp::SampleRepository& sampleRepo) const;
    void PrintWaitingQueue(const std::vector<dp::Order>& waiting, const dp::SampleRepository& sampleRepo) const;
};

}
