# Phase 5 — 생산라인

## 목표

`PRODUCING` 상태 주문의 생산 진행을 시뮬레이션하고, 생산 현황과 대기(FIFO) 큐를 조회할 수 있게 한다.

## 데이터

- 추가 필드 없음 (Phase 1/4에서 정의한 `productionStartEpochSec`, `totalProductionSeconds`, `shortageQty`, `actualProductionQty` 사용)

## 동작

1. **OrderWorkflow::Tick()**
   - 앱의 모든 메뉴 진입 시마다 1회 호출 (`AppController`의 메인 루프에서 매 반복 호출)
   - 현재 생산중인 주문(`productionStartEpochSec > 0`)의 경과시간이 `totalProductionSeconds` 이상이면:
     - 대상 시료 재고에 `actualProductionQty`(실생산량 전량)만큼 증가 — `shortageQty`가 아님에 주의. 수율 손실 없이 생산라인에 투입한 수량이 그대로 재고에 들어가므로, 부족분보다 재고가 더 늘어날 수 있다
     - 주문 상태 `PRODUCING` → `CONFIRMED`로 전환
     - `Logger::LogOrderTransition(orderNo, "PRODUCING", "CONFIRMED")` 호출
   - 생산라인이 비어 있고 대기중인 주문(`productionStartEpochSec == 0`, 상태 `PRODUCING`)이 있으면 가장 먼저 대기열에 들어온 주문을 꺼내 생산 시작(`productionStartEpochSec = now()`)
2. **생산현황표기**
   - 현재 생산중인 주문의 주문번호/시료명/경과시간/남은시간 표시
   - 생산중인 주문이 없으면 "생산중인 주문 없음" 출력
3. **대기주문확인**
   - 대기중인 주문(`productionStartEpochSec == 0`, 상태 `PRODUCING`)을 FIFO 순서(생성 순서 기준)로 목록 표시 (주문번호/시료명/수량)

## 구현 파일

- `include/DataPersistence/OrderWorkflow.h` / `src/DataPersistence/OrderWorkflow.cpp` — `Tick()`, `IsLineBusy()` 구현
- `include/ConsoleMVC/ProductionController.h` / `src/ConsoleMVC/ProductionController.cpp` — 생산현황표기/대기주문확인 서브메뉴 구현
- `include/ConsoleMVC/AppController.h` / `src/ConsoleMVC/AppController.cpp` — 메인 루프에 `OrderWorkflow::Tick()` 호출 추가

## 완료 조건

- 생산 시간이 경과한 주문은 다음 메뉴 진입 시 자동으로 `CONFIRMED`로 전환되고 재고가 반영됨
- 생산라인이 비었을 때 대기 중인 다음 주문이 자동으로 생산 시작됨
- 생산현황표기/대기주문확인 출력이 실제 상태와 일치함
