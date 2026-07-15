# Plan 4 — 시료승인/거절

## 구현 목표

- `RESERVED` 주문을 재고 상황에 따라 승인(`CONFIRMED`/`PRODUCING`)하거나 거절(`REJECTED`)

## 구현 체크리스트

- [ ] 접수된 주문 목록 (RESERVED 필터링)
- [ ] `OrderWorkflow::Approve` — 재고 충분 시 즉시 CONFIRMED
- [ ] `OrderWorkflow::Approve` — 재고 부족 시 shortageQty/actualProductionQty(ceil)/totalProductionSeconds 계산 후 PRODUCING 전환, 생산라인 상태에 따라 즉시 시작 또는 대기 (재고 반영은 shortageQty가 아니라 actualProductionQty 기준, Phase 5 참고)
- [ ] `OrderWorkflow::Reject` — 즉시 REJECTED
- [ ] 승인/거절 각각 `Logger::LogOrderTransition` 호출

## 완료 조건

- 재고 충분/부족 두 분기 모두 정확한 필드값으로 전환
- 거절된 주문은 접수 목록에서 사라짐
- 로그에 전환 기록 남음
