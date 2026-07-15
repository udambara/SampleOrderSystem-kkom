# Phase 8 — 더미데이터 생성기

## 목표

테스트/데모를 위해 임의의 Sample/Order 데이터를 생성해 저장소(JSON)에 직접 추가하는 도구를 만든다. 메인 프로그램의 기능이 아니라 별도 실행 유틸리티로 제공한다.

## 데이터

- 추가 필드 없음. 기존 `Sample`/`Order` 구조를 그대로 사용해 임의값 생성

## 동작

1. **Dummy Sample 생성**
   - 시료ID, 이름, 평균생산시간, 수율을 임의값으로 생성해 `SampleRepository`에 등록
   - 생성 개수를 지정 가능 (예: N개)
2. **Dummy Order 생성**
   - 이미 등록된 시료 중에서 임의의 시료ID, 임의의 고객명, 임의의 주문수량으로 주문 생성
   - 상태는 `RESERVED`부터 시작 (승인/생산/출고 흐름은 본 프로그램에서 처리)
   - 생성 개수를 지정 가능 (예: M개)
3. **실행 방식**
   - 메인 프로그램과 별도의 콘솔 실행 파일(또는 실행 시 커맨드라인 인자로 개수 지정)로 제공
   - 생성한 데이터는 메인 프로그램이 사용하는 동일한 `data/samples.json`, `data/orders.json`에 직접 추가

## 구현 파일

- `include/DummyDataGenerator/DummyDataGenerator.h` / `src/DummyDataGenerator/DummyDataGenerator.cpp` — `GenerateSamples(int count)`, `GenerateOrders(int count)` 구현
- 별도 진입점 `src/DummyDataGeneratorMain.cpp` (또는 커맨드라인 인자 처리 방식 확정 후 결정)

## 완료 조건

- 실행 후 `data/samples.json`/`data/orders.json`에 지정한 개수만큼 데이터가 추가됨
- 생성된 Order의 `sampleId`는 항상 실제 존재하는 Sample을 참조함
- 메인 프로그램 실행 시 더미로 생성된 데이터가 정상적으로 조회/주문/승인 흐름에 사용 가능함
