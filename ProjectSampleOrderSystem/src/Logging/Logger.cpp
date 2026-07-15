#include "Logging/Logger.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace logging {

Logger::Logger(std::string filePath) : m_filePath(std::move(filePath)) {}

void Logger::LogOrderTransition(const std::string& orderNo,
                                 const std::string& fromStatus,
                                 const std::string& toStatus) const {
    std::filesystem::path path(m_filePath);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    std::ofstream out(m_filePath, std::ios::app);
    out << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << " "
        << orderNo << " " << fromStatus << " -> " << toStatus << "\n";
}

std::vector<std::string> Logger::ReadRecentTransitions(int count) const {
    std::vector<std::string> lines;
    std::ifstream in(m_filePath);
    if (!in.is_open()) {
        return lines;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    std::vector<std::string> recent;
    int start = static_cast<int>(lines.size()) - count;
    if (start < 0) {
        start = 0;
    }
    for (int i = static_cast<int>(lines.size()) - 1; i >= start; --i) {
        recent.push_back(lines[i]);
    }
    return recent;
}

}
