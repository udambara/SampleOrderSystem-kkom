#include "DataPersistence/SampleRepository.h"
#include "third_party/nlohmann/json.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>

namespace dp {

namespace {

std::string ToLower(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

}

SampleRepository::SampleRepository(std::string filePath) : m_filePath(std::move(filePath)) {
    if (!std::filesystem::exists(m_filePath)) {
        Save({});
    }
}

std::vector<Sample> SampleRepository::Load() const {
    std::vector<Sample> samples;
    std::ifstream in(m_filePath);
    if (!in.is_open()) {
        return samples;
    }
    nlohmann::json j;
    in >> j;
    for (const auto& item : j) {
        samples.push_back(item.get<Sample>());
    }
    return samples;
}

void SampleRepository::Save(const std::vector<Sample>& samples) const {
    std::filesystem::path path(m_filePath);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    nlohmann::json j = samples;
    std::ofstream out(m_filePath);
    out << j.dump(2);
}

bool SampleRepository::Add(const Sample& sample, std::string& errorMessage) {
    std::vector<Sample> samples = Load();
    if (std::any_of(samples.begin(), samples.end(),
                     [&](const Sample& s) { return s.id == sample.id; })) {
        errorMessage = "이미 등록된 시료ID입니다: " + sample.id;
        return false;
    }
    samples.push_back(sample);
    Save(samples);
    return true;
}

std::vector<Sample> SampleRepository::GetAll() const {
    return Load();
}

std::vector<Sample> SampleRepository::FindByName(const std::string& keyword) const {
    std::vector<Sample> result;
    std::string loweredKeyword = ToLower(keyword);
    for (const auto& sample : Load()) {
        if (ToLower(sample.name).find(loweredKeyword) != std::string::npos) {
            result.push_back(sample);
        }
    }
    return result;
}

std::optional<Sample> SampleRepository::FindById(const std::string& id) const {
    std::vector<Sample> samples = Load();
    auto it = std::find_if(samples.begin(), samples.end(),
                            [&](const Sample& s) { return s.id == id; });
    if (it == samples.end()) {
        return std::nullopt;
    }
    return *it;
}

}
