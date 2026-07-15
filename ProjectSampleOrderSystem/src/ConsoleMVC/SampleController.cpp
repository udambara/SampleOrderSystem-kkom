#include "ConsoleMVC/SampleController.h"

namespace cmvc {

void SampleController::Run() {
    while (true) {
        m_view.PrintSampleMenu();
        int choice = m_view.ReadMenuChoice("선택: ");
        switch (choice) {
            case 1: HandleRegister(); break;
            case 2: HandleList(); break;
            case 3: HandleSearch(); break;
            case 0: return;
            default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
        }
    }
}

void SampleController::HandleRegister() {
    dp::Sample sample = m_view.ReadNewSample();
    std::string errorMessage;
    if (m_sampleRepo.Add(sample, errorMessage)) {
        m_view.PrintMessage("시료가 등록되었습니다: " + sample.id);
    } else {
        m_view.PrintError(errorMessage);
    }
}

void SampleController::HandleList() {
    m_view.PrintSampleList(m_sampleRepo.GetAll());
}

void SampleController::HandleSearch() {
    m_view.PrintSampleSearchMenu();
    int choice = m_view.ReadMenuChoice("선택: ");
    switch (choice) {
        case 1: {
            std::string id = m_view.ReadSearchKeyword("시료ID");
            std::vector<dp::Sample> results;
            std::optional<dp::Sample> sample = m_sampleRepo.FindById(id);
            if (sample.has_value()) {
                results.push_back(*sample);
            }
            m_view.PrintSampleList(results);
            break;
        }
        case 2: {
            std::string keyword = m_view.ReadSearchKeyword("이름");
            m_view.PrintSampleList(m_sampleRepo.FindByName(keyword));
            break;
        }
        case 0: break;
        default: m_view.PrintError("올바른 메뉴 번호를 선택해주세요."); break;
    }
}

}
