# Test Plan — Phase 5 (생산라인)

## 사전조건

- Phase 4의 TC4-2, TC4-3 상태 재현: 주문 A(PRODUCING, 생산 진행중, totalProductionSeconds=340), 주문 B(PRODUCING, 대기중)

## 테스트 시나리오

### TC5-1. 생산현황표기 — 생산 진행중

1. 생산라인 → 생산현황표기 진입 (주문 A 생산 시작 직후)
2. 예상 결과: 주문 A의 주문번호/시료명/경과시간(0에 가까움)/남은시간(340초에 가까움) 표시

### TC5-2. 대기주문확인 — FIFO 순서

1. 생산라인 → 대기주문확인 진입
2. 예상 결과: 주문 B가 대기 목록에 표시됨 (주문번호/시료명/수량)

### TC5-3. 생산 완료 후 자동 전환

1. 주문 A의 `totalProductionSeconds`(340초) 경과 후 임의의 메뉴에 진입 (Tick 트리거)
2. 예상 결과:
   - 주문 A 상태가 `PRODUCING` → `CONFIRMED`로 전환
   - 시료 `S001` 재고가 `shortageQty`(30)만큼 증가
   - 생산라인이 비었으므로 대기중이던 주문 B가 자동으로 생산 시작 (`productionStartEpochSec` 채워짐)

### TC5-4. 생산중인 주문 없음 표시

1. 모든 주문이 생산 완료된 상태에서 생산현황표기 진입
2. 예상 결과: "생산중인 주문 없음" 출력

### TC5-5. 상태전환 로그 기록

1. TC5-3 완료 후 `logs/order_events.log` 확인
2. 예상 결과: 주문 A에 대해 `PRODUCING`→`CONFIRMED` 전환 기록이 추가됨

## 완료 기준

- 위 5개 시나리오 모두 통과
- 시간 경과 시뮬레이션을 위해 테스트 중 `totalProductionSeconds`를 짧게(예: 5~10초) 설정해 실제 대기 후 확인 가능해야 함
