# Review — Phase 2

## 피드백

시료 검색 시 시료ID나 시료 이름 중 어떤 기준으로 검색할지 선택할 수 있는 메뉴를 추가해달라는 요청. 기존 구현은 이름 부분 일치 검색만 지원했음.

## 조치

- `ConsoleView::PrintSampleSearchMenu()` 추가 — "1. 시료ID로 검색 / 2. 이름으로 검색 / 0. 취소" 서브메뉴
- `ConsoleView::ReadSearchKeyword()`를 `ReadSearchKeyword(const std::string& label)`로 변경해 프롬프트에 검색 기준을 표시 (`검색어(시료ID): ` / `검색어(이름): `)
- `SampleController::HandleSearch()`에서 선택한 기준에 따라 분기:
  - 시료ID: `SampleRepository::FindById`로 정확히 일치하는 시료 1건(또는 없음)만 표시
  - 이름: 기존과 동일하게 `SampleRepository::FindByName`으로 부분 일치 검색
- `docs/phases/phase2.md`, `docs/test/test_plan2.md`를 변경사항에 맞춰 갱신 (TC2-6, TC2-7 추가: 시료ID 검색 일치/불일치 케이스)

## 검증

- 재빌드 성공 (경고/에러 없음)
- 시나리오 직접 실행 확인:
  - 시료ID로 검색 → 등록된 ID(`S001`) 입력 시 해당 시료 1건 표시
  - 시료ID로 검색 → 존재하지 않는 ID(`S999`) 입력 시 "등록된 시료가 없습니다" 출력
  - 이름으로 검색 → 부분 일치(`실리콘`) 시 정상 표시, 불일치(`텅스텐카바이드`) 시 "등록된 시료가 없습니다" 출력
