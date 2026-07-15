#pragma once

#include "DataPersistence/SampleRepository.h"
#include "DataPersistence/OrderRepository.h"

namespace ddg {

// Generates `count` random samples and adds them to sampleRepo.
void GenerateSamples(dp::SampleRepository& sampleRepo, int count);

// Generates `count` random orders (RESERVED) against samples already in
// sampleRepo, and adds them to orderRepo. No-op if sampleRepo is empty.
void GenerateOrders(dp::SampleRepository& sampleRepo, dp::OrderRepository& orderRepo, int count);

}
