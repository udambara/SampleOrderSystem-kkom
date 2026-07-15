# Phase 7 — 모니터링

## 목표

담당자가 현재 주문/재고 상태를 한눈에 파악할 수 있는 조회 전용 기능을 제공한다. 데이터를 직접 수정하지 않는다.

## 데이터

- 추가 필드 없음. 기존 `Sample`/`Order` 데이터를 읽기 전용으로 집계

## 동작

1. **주문량확인**
   - 상태별(`RESERVED`/`CONFIRMED`/`PRODUCING`/`RELEASE`) 주문 수를 표 형태로 표시
   - `REJECTED`는 유효한 주문이 아니므로 집계에서 제외
2. **재고량확인**
   - 시료별 현재 재고 수량을 표시
   - 미출고 수요(demand) = 해당 시료의 `RESERVED`+`PRODUCING` 주문과, `stockDeductedAtApproval == false`인 `CONFIRMED` 주문(생산을 거쳐 확정된 주문)의 수량 합. `stockDeductedAtApproval == true`인 `CONFIRMED` 주문(승인 시 재고에서 이미 차감됨)과 `REJECTED`/`RELEASE`는 제외
   - 재고 상태 표기:
     - 재고 0 → 고갈
     - 재고 − 미출고 수요 ≥ 0 → 여유
     - 그 외 → 부족
   - 예: 재고 170에서 승인 즉시 CONFIRMED된 주문 130을 처리하면 그 시점에 재고가 40으로 차감되어 표시되고(이미 반영되었으므로 수요에는 미포함), 상태는 여유. 여기에 RESERVED 90이 추가되면 재고 40 − 수요 90 = -50 → 부족

## 구현 파일

- `include/DataMonitor/DataMonitor.h` / `src/DataMonitor/DataMonitor.cpp` — `PrintOrderStatusTable(const OrderRepository&)`, `PrintInventoryTable(const SampleRepository&, const OrderRepository&)` 구현 (재고량확인은 주문 데이터도 필요)
- `include/ConsoleMVC/MonitoringController.h` / `src/ConsoleMVC/MonitoringController.cpp` — 모니터링 서브메뉴에서 위 함수 호출

## 완료 조건

- 각 상태별 주문 수가 실제 `orders.json` 내용과 일치
- REJECTED 주문이 존재해도 주문량확인 집계에 포함되지 않음
- 재고 0 케이스에서 고갈 표기, 미출고 수요를 감당 가능/불가능한 케이스에서 각각 여유/부족 표기가 올바르게 나타남 (단순 재고 절대값이 아니라 수요 대비로 판단됨)
