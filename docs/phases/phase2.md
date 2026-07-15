# Phase 2 — 시료관리

## 목표

시료(Sample)를 등록하고 조회/검색할 수 있게 한다. 이후 모든 주문 관련 기능은 여기서 등록된 시료를 전제로 한다.

## 데이터

- Phase 1의 `Sample` 구조체를 그대로 사용 (필드 추가 없음)

## 동작

1. **시료등록**
   - 시료ID, 이름, 평균생산시간(분), 수율(0~1) 입력
   - 이미 존재하는 시료ID면 에러 처리 후 재입력 또는 취소
   - 초기 재고(`stock`)는 0으로 등록
   - `SampleRepository::Add` 호출 후 결과 출력
2. **시료조회**
   - 등록된 모든 시료를 목록으로 표시 (ID / 이름 / 평균생산시간 / 수율 / 현재 재고)
3. **시료검색**
   - 검색 기준(시료ID / 이름)을 먼저 선택하는 서브메뉴 표시
   - 시료ID로 검색: 정확히 일치하는 시료 1건(또는 없음) 표시
   - 이름으로 검색: 부분 일치로 시료 검색, 결과 목록 표시 (없으면 "검색 결과 없음")

## 구현 파일

- `include/DataPersistence/SampleRepository.h` — `FindByName` (부분 일치 검색) 추가
- `src/DataPersistence/SampleRepository.cpp` — 위 메서드 구현
- `include/ConsoleMVC/ConsoleView.h` / `src/ConsoleMVC/ConsoleView.cpp` — 검색 기준 선택 서브메뉴(`PrintSampleSearchMenu`), 검색어 입력(`ReadSearchKeyword(label)`)
- `include/ConsoleMVC/SampleController.h` / `src/ConsoleMVC/SampleController.cpp` — 시료관리 서브메뉴(등록/조회/검색) 구현, 검색은 ID/이름 선택 후 분기

## 완료 조건

- 동일 시료ID 중복 등록 시도 시 에러 메시지 출력 및 등록 거부
- 등록 직후 시료조회 목록에 해당 시료가 재고 0으로 표시됨
- 시료ID로 검색 시 정확히 일치하는 시료만 표시되고, 존재하지 않는 ID면 "등록된 시료가 없습니다" 출력
- 이름 일부만 입력해도 검색 결과에 포함됨
