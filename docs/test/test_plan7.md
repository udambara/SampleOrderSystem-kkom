# Test Plan — Phase 7 (모니터링)

## 사전조건

- Phase 6까지 완료된 상태에서 RESERVED/CONFIRMED/PRODUCING/RELEASE/REJECTED 각 상태의 주문이 최소 1건씩 존재

## 테스트 시나리오

### TC7-1. 주문량확인 — 상태별 집계

1. 모니터링 → 주문량확인 진입
2. 예상 결과: RESERVED/CONFIRMED/PRODUCING/RELEASE 각각의 건수가 실제 `data/orders.json`과 일치하도록 표시됨

### TC7-2. 주문량확인 — REJECTED 제외

1. TC7-1 화면에서 REJECTED 주문 존재 여부 확인
2. 예상 결과: REJECTED 건수는 집계/표시에 포함되지 않음

### TC7-3. 재고량확인 — 상태 표기

1. 시료별 재고를 각각 0개(고갈 케이스), 50개(부족 케이스, 임계값 100 미만), 200개(여유 케이스)로 준비
2. 모니터링 → 재고량확인 진입
3. 예상 결과: 각 시료가 재고 0 → "고갈", 재고 100 미만 → "부족", 그 외 → "여유"로 정확히 표기됨

## 완료 기준

- 위 3개 시나리오 모두 통과
