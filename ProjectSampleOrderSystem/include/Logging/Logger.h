#pragma once

#include <string>

namespace logging {

// Appends order status-transition events to a log file. fromStatus is "-" for
// newly-created orders that had no prior status.
class Logger {
public:
    explicit Logger(std::string filePath = "logs/order_events.log");

    void LogOrderTransition(const std::string& orderNo,
                             const std::string& fromStatus,
                             const std::string& toStatus) const;

private:
    std::string m_filePath;
};

}
