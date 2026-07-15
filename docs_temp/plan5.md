# Plan 5 — 생산라인

## 구현 목표

- `PRODUCING` 주문의 생산 진행 시뮬레이션(Tick) 및 생산현황/대기열 조회

## 구현 체크리스트

- [ ] `OrderWorkflow::Tick()` — 경과시간 ≥ totalProductionSeconds인 생산중 주문 완료 처리(재고에 actualProductionQty 반영 — shortageQty 아님, CONFIRMED 전환, 로그)
- [ ] `OrderWorkflow::Tick()` — 생산라인이 비면 대기중인 다음 주문 자동 시작
- [ ] `AppController` 메인 루프에서 매 반복 `Tick()` 호출
- [ ] 생산현황표기: 생산중 주문의 주문번호/시료명/경과/남은시간 (없으면 안내 메시지)
- [ ] 대기주문확인: 대기열을 FIFO 순서로 표시

## 완료 조건

- 생산시간 경과 시 자동 CONFIRMED 전환 + 재고 반영 + 다음 주문 자동 시작
- 생산현황/대기열 출력이 실제 상태와 일치
- 로그에 PRODUCING→CONFIRMED 전환 기록 남음
