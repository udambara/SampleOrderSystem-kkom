# Plan 8 — 더미데이터 생성기

## 구현 목표

- 테스트용 Sample/Order를 대량 생성해 메인 프로그램과 동일한 저장소(JSON)에 추가하는 별도 실행 도구

## 구현 체크리스트

- [ ] `DummyDataGenerator::GenerateSamples(int count)` — 임의 ID/이름/평균생산시간/수율로 시료 생성
- [ ] `DummyDataGenerator::GenerateOrders(int count)` — 기존 시료 중 임의 선택 + 임의 고객명/수량으로 RESERVED 주문 생성
- [ ] 별도 진입점(실행파일 또는 커맨드라인 인자 처리)에서 생성 개수 지정 가능하도록 구현
- [ ] 메인 프로그램과 동일한 `data/samples.json`, `data/orders.json` 경로 사용

## 완료 조건

- 지정 개수만큼 데이터 추가, 생성된 Order의 sampleId는 항상 실제 존재하는 Sample 참조
- 메인 프로그램에서 생성된 데이터로 정상 흐름(조회/주문/승인) 수행 가능
