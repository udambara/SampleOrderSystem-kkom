#include "DummyDataGenerator/DummyDataGenerator.h"

#include <iomanip>
#include <iterator>
#include <random>
#include <sstream>

namespace ddg {

namespace {

std::mt19937& Rng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

std::string RandomSampleId(int attempt) {
    std::ostringstream oss;
    oss << "DUMMY-" << std::setw(4) << std::setfill('0') << attempt;
    return oss.str();
}

const char* kNamePool[] = {
    "실리콘웨이퍼", "텅스텐카바이드", "알루미늄합금", "구리기판", "세라믹코어", "실리콘카바이드"
};

const char* kCustomerPool[] = {
    "ACME", "Globex", "Initech", "삼성", "하이닉스", "엔비디아"
};

}

void GenerateSamples(dp::SampleRepository& sampleRepo, int count) {
    if (count <= 0) {
        return;
    }

    std::uniform_real_distribution<double> avgTimeDist(1.0, 30.0);
    std::uniform_real_distribution<double> yieldDist(0.5, 1.0);
    std::uniform_int_distribution<int> nameDist(0, static_cast<int>(std::size(kNamePool)) - 1);

    int generated = 0;
    int attempt = 0;
    while (generated < count && attempt < count * 10) {
        ++attempt;
        dp::Sample sample;
        sample.id = RandomSampleId(attempt);
        sample.name = kNamePool[nameDist(Rng())];
        sample.avgProductionTimeMinutes = avgTimeDist(Rng());
        sample.yield = yieldDist(Rng());
        sample.stock = 0;

        std::string errorMessage;
        if (sampleRepo.Add(sample, errorMessage)) {
            ++generated;
        }
    }
}

void GenerateOrders(dp::SampleRepository& sampleRepo, dp::OrderRepository& orderRepo, int count) {
    if (count <= 0) {
        return;
    }

    std::vector<dp::Sample> samples = sampleRepo.GetAll();
    if (samples.empty()) {
        return;
    }

    std::uniform_int_distribution<size_t> sampleDist(0, samples.size() - 1);
    std::uniform_int_distribution<int> customerDist(0, static_cast<int>(std::size(kCustomerPool)) - 1);
    std::uniform_int_distribution<int> qtyDist(1, 100);

    for (int i = 0; i < count; ++i) {
        const dp::Sample& sample = samples[sampleDist(Rng())];

        dp::Order order;
        order.orderNo = orderRepo.GenerateOrderNo();
        order.id = order.orderNo;
        order.sampleId = sample.id;
        order.customerName = kCustomerPool[customerDist(Rng())];
        order.quantity = qtyDist(Rng());
        order.status = dp::OrderStatus::RESERVED;

        std::string errorMessage;
        orderRepo.Add(order, errorMessage);
    }
}

}
