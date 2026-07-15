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
   - 재고 상태 표기:
     - 재고 0 → 고갈
     - 재고 100 미만 → 부족
     - 그 외 → 여유
     - (임계값 100은 PRD의 예시값을 그대로 사용, 필요시 조정)

## 구현 파일

- `include/DataMonitor/DataMonitor.h` / `src/DataMonitor/DataMonitor.cpp` — `PrintOrderStatusTable(const OrderRepository&)`, `PrintInventoryTable(const SampleRepository&)` 구현
- `include/ConsoleMVC/MonitoringController.h` / `src/ConsoleMVC/MonitoringController.cpp` — 모니터링 서브메뉴에서 위 함수 호출

## 완료 조건

- 각 상태별 주문 수가 실제 `orders.json` 내용과 일치
- REJECTED 주문이 존재해도 주문량확인 집계에 포함되지 않음
- 재고 0/100 미만/이상 케이스 각각에 대해 고갈/부족/여유 표기가 올바르게 나타남
