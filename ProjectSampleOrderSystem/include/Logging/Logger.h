#pragma once

#include <string>
#include <vector>

namespace logging {

// Appends order status-transition events to a log file. fromStatus is "-" for
// newly-created orders that had no prior status.
class Logger {
public:
    explicit Logger(std::string filePath = "logs/order_events.log");

    void LogOrderTransition(const std::string& orderNo,
                             const std::string& fromStatus,
                             const std::string& toStatus) const;

    // Returns up to `count` most recent log lines, newest first. Empty if the
    // log file doesn't exist yet or has no entries.
    std::vector<std::string> ReadRecentTransitions(int count) const;

private:
    std::string m_filePath;
};

}
