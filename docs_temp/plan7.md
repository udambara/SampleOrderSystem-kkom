# Plan 7 — 모니터링

## 구현 목표

- 상태별 주문 수 및 시료별 재고 현황을 조회 전용으로 표시 (데이터 수정 없음)

## 구현 체크리스트

- [ ] `PrintOrderStatusTable` — RESERVED/CONFIRMED/PRODUCING/RELEASE 건수 집계 (REJECTED 제외)
- [ ] `PrintInventoryTable` — 시료별 재고 + 상태(여유/부족/고갈, 임계값 100)
- [ ] `MonitoringController`에서 위 함수 호출해 서브메뉴 연결

## 완료 조건

- 집계 결과가 실제 데이터와 일치, REJECTED 미포함
- 재고 0/100 미만/이상 케이스별 상태 표기 정확
