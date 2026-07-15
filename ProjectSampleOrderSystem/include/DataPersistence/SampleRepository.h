#pragma once

#include <optional>
#include <string>
#include <vector>
#include "DataPersistence/Sample.h"

namespace dp {

class SampleRepository {
public:
    explicit SampleRepository(std::string filePath = "data/samples.json");

    // Returns false and sets errorMessage if a sample with the same id already exists.
    bool Add(const Sample& sample, std::string& errorMessage);

    std::vector<Sample> GetAll() const;

    // Case-insensitive substring match against sample name.
    std::vector<Sample> FindByName(const std::string& keyword) const;

    std::optional<Sample> FindById(const std::string& id) const;

private:
    std::string m_filePath;

    std::vector<Sample> Load() const;
    void Save(const std::vector<Sample>& samples) const;
};

}
