# Plan 3 — 시료주문 (예약)

## 구현 목표

- 고객 주문을 `RESERVED` 상태로 접수하는 기능 구현

## 구현 체크리스트

- [ ] `OrderRepository::GenerateOrderNo()` — `ORD-YYYYMMDD-NNNN`
- [ ] 시료예약: 시료ID/고객명/주문수량 입력, 시료ID 존재 검증, 수량 유효성 검증
- [ ] 예약 성공 시 `RESERVED` 상태로 저장 + 주문번호 출력
- [ ] `Logger::LogOrderTransition(orderNo, "-", "RESERVED")` 호출

## 완료 조건

- 미등록 시료ID/잘못된 수량 시 저장 거부
- 같은 날 여러 건 예약 시 주문번호가 순차 증가
- 로그에 신규 생성 기록 남음
