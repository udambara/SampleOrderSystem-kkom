# Phase 6 — 출고처리

## 목표

재고가 확보된 `CONFIRMED` 주문에 대해 출고를 실행한다.

## 데이터

- `Order`의 `releasedAtEpochSec`, `releasedQty` 필드는 Phase 1에서 이미 정의됨 — 이 단계에서 값을 채운다

## 동작

1. **CONFIRMED 주문 목록 표시**
   - 번호 / 주문번호 / 고객명 / 시료명 / 수량
2. **출고실행**
   - 목록에서 주문 선택 — 단일 번호(`1`) 또는 여러 건을 한 번에 선택 가능(`1,3` 콤마 구분, `1-3`/`1~3` 범위 지정)
   - 선택된 주문을 순서대로 각각 처리:
     - `stockDeductedAtApproval == true`(승인 시 재고 충분으로 이미 차감됨)면 재고를 다시 차감하지 않음
     - `false`(생산을 거쳐 CONFIRMED된 경우)면 대상 시료 재고 ≥ 주문수량 확인 후 차감 (부족하면 해당 건만 에러 처리하고 다음 선택 건을 계속 처리 — 정상 흐름에서는 생산 완료 시점에 재고가 보장되어야 하므로 방어적 체크)
   - 상태 `RELEASE`로 전환, `releasedAtEpochSec`/`releasedQty` 기록
   - `Logger::LogOrderTransition(orderNo, "CONFIRMED", "RELEASE")` 호출
   - 처리된 건마다 결과 화면 출력 (주문번호 / 출고수량 / 처리일시 / 상태변경: CONFIRMED → RELEASE)

## 구현 파일

- `include/DataPersistence/Order.h` — 필드는 이미 존재, 변경 없음
- `include/DataPersistence/OrderRepository.h` / `src/DataPersistence/OrderRepository.cpp` — `Update` 재사용
- `include/ConsoleMVC/ShippingController.h` / `src/ConsoleMVC/ShippingController.cpp` — 출고처리 서브메뉴 구현

## 완료 조건

- 재고가 부족한 상태에서 출고 시도 시 에러 처리되고 상태가 변경되지 않음
- 출고 성공 시 재고가 정확히 차감되고 상태가 `RELEASE`로 바뀜
- 출고된 주문은 이후 CONFIRMED 목록에 다시 나타나지 않음
