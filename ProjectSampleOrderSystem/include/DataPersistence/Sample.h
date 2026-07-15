#pragma once

#include <string>
#include "third_party/nlohmann/json.hpp"

namespace dp {

struct Sample {
    std::string id;
    std::string name;
    double avgProductionTimeMinutes = 0.0;
    double yield = 1.0;
    int stock = 0;
};

void to_json(nlohmann::json& j, const Sample& s);
void from_json(const nlohmann::json& j, Sample& s);

}
