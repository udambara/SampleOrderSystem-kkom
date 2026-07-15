#include "DataPersistence/Sample.h"

namespace dp {

void to_json(nlohmann::json& j, const Sample& s) {
    j = nlohmann::json{
        {"id", s.id},
        {"name", s.name},
        {"avgProductionTimeMinutes", s.avgProductionTimeMinutes},
        {"yield", s.yield},
        {"stock", s.stock}
    };
}

void from_json(const nlohmann::json& j, Sample& s) {
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("avgProductionTimeMinutes").get_to(s.avgProductionTimeMinutes);
    j.at("yield").get_to(s.yield);
    j.at("stock").get_to(s.stock);
}

}
