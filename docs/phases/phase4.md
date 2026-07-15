# Phase 4 — 시료승인/거절

## 목표

접수된(`RESERVED`) 주문을 재고 상황에 따라 승인(`CONFIRMED`/`PRODUCING`)하거나 거절(`REJECTED`)한다.

## 데이터

- `Order`의 `productionStartEpochSec`(0=대기, >0=생산시작시각), `totalProductionSeconds`, `shortageQty` 필드는 Phase 1에서 이미 정의됨 — 이 단계에서 실제로 값을 채운다
- `actualProductionQty` 필드 추가 — `ceil(shortageQty / yield)`. 생산 완료 시(Phase 5) 재고에 반영되는 값은 `shortageQty`가 아니라 이 값이다 (수율만큼 손실되지 않고 실생산량 전량이 재고로 들어감)

## 동작

1. **접수된 주문 목록**
   - `RESERVED` 상태 주문만 필터링해 표시 (주문번호/시료명/고객명/수량)
2. **주문승인**
   - 목록에서 특정 주문 선택
   - 대상 시료의 현재 재고 조회
   - 재고 ≥ 주문수량 → 즉시 상태 `CONFIRMED`로 전환
   - 재고 < 주문수량 →
     - `shortageQty` = 주문수량 − 재고
     - `actualProductionQty`(실생산량) = `ceil(shortageQty / yield)` — 이 값을 `Order.actualProductionQty`에 저장
     - `totalProductionSeconds` = 평균생산시간(분) × 실생산량 (분 단위 수치를 초 단위로 그대로 사용 — 데모 속도를 위한 60배 축소)
     - 상태 `PRODUCING`으로 전환
     - 생산라인이 비어 있으면 즉시 생산 시작(`productionStartEpochSec = now()`), 아니면 대기(0으로 유지 → Phase 5의 생산큐에서 대기)
   - `Logger::LogOrderTransition(orderNo, "RESERVED", "CONFIRMED"|"PRODUCING")` 호출
3. **주문거절**
   - 목록에서 특정 주문 선택 → 즉시 상태 `REJECTED`로 전환
   - `Logger::LogOrderTransition(orderNo, "RESERVED", "REJECTED")` 호출

## 구현 파일

- `include/DataPersistence/OrderWorkflow.h` / `src/DataPersistence/OrderWorkflow.cpp` — `Approve(Order)`, `Reject(Order)` 구현 (재고 조회는 `SampleRepository`, 주문 상태 갱신은 `OrderRepository::Update`)
- `include/ConsoleMVC/ApprovalController.h` / `src/ConsoleMVC/ApprovalController.cpp` — 시료승인/거절 서브메뉴 구현 (`OrderWorkflow` 호출)

## 완료 조건

- 재고가 충분한 주문 승인 시 즉시 `CONFIRMED`로 바뀌고 재고는 차감되지 않음(차감은 출고 시점)
- 재고가 부족한 주문 승인 시 `PRODUCING`으로 바뀌고 `shortageQty`/`actualProductionQty`/`totalProductionSeconds`가 올바르게 계산됨
- 생산라인이 비어 있을 때 승인하면 `productionStartEpochSec`가 즉시 채워짐 (Phase 5에서 검증)
- 거절한 주문은 이후 목록(접수된 주문 목록)에 다시 나타나지 않음
