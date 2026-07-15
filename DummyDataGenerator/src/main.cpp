#include "DummyDataGenerator/DummyDataGenerator.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc < 3) {
        std::cout << "사용법: DummyDataGenerator.exe <생성할 시료 수> <생성할 주문 수>\n";
        return 0;
    }

    int sampleCount = std::atoi(argv[1]);
    int orderCount = std::atoi(argv[2]);

    if (sampleCount <= 0 && orderCount <= 0) {
        std::cout << "생성 개수가 0이라 아무것도 생성하지 않습니다.\n";
        return 0;
    }

    dp::SampleRepository sampleRepo;
    dp::OrderRepository orderRepo;

    if (sampleCount > 0) {
        ddg::GenerateSamples(sampleRepo, sampleCount);
        std::cout << "시료 " << sampleCount << "건 생성 완료\n";
    }
    if (orderCount > 0) {
        ddg::GenerateOrders(sampleRepo, orderRepo, orderCount);
        std::cout << "주문 " << orderCount << "건 생성 완료\n";
    }

    return 0;
}
