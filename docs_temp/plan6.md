# Plan 6 — 출고처리

## 구현 목표

- `CONFIRMED` 주문의 출고 처리(`RELEASE` 전환)

## 구현 체크리스트

- [ ] CONFIRMED 주문 목록 표시
- [ ] 출고실행: 재고 ≥ 주문수량 검증 (부족 시 에러)
- [ ] 재고 차감, `RELEASE` 전환, `releasedAtEpochSec`/`releasedQty` 기록
- [ ] `Logger::LogOrderTransition` 호출
- [ ] 결과 화면 출력(주문번호/출고수량/처리일시/상태변경)

## 완료 조건

- 재고 부족 시 출고 거부, 정상 출고 시 재고 차감 및 상태 전환 정확
- 출고 완료 주문은 CONFIRMED 목록에서 제외
- 등록→주문→승인→생산→출고 전체 흐름 1회 이상 재현 가능
