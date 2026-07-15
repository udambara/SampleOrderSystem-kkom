#pragma once

#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace mon {

// Prints order counts grouped by status (RESERVED/CONFIRMED/PRODUCING/RELEASE).
// REJECTED orders are intentionally excluded, since they are not valid orders.
void PrintOrderStatusTable(const dp::OrderRepository& orderRepo);

// Prints current stock per sample along with a status derived from stock vs.
// outstanding demand (the summed quantity of that sample's RESERVED/CONFIRMED/
// PRODUCING orders — anything not yet REJECTED or RELEASEd):
//   stock == 0                  -> 고갈
//   stock - outstanding demand >= 0 -> 여유 (enough to cover everything owed)
//   otherwise                   -> 부족 (short by (demand - stock))
void PrintInventoryTable(const dp::SampleRepository& sampleRepo, const dp::OrderRepository& orderRepo);

}
