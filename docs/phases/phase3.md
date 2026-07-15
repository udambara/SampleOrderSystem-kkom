# Phase 3 — 시료주문 (예약)

## 목표

고객의 시료 주문을 접수해 `RESERVED` 상태로 저장한다.

## 데이터

- `Order`에 `orderNo`(표시용 주문번호, `ORD-YYYYMMDD-NNNN`) 필드는 Phase 1에서 이미 정의됨
- `OrderRepository`에 `GenerateOrderNo()` 추가 — 당일 날짜 + 당일 기존 주문 수+1을 4자리 zero-pad

## 동작

1. **시료예약**
   - 시료ID / 고객명 / 주문수량 입력
   - 시료ID가 `SampleRepository`에 존재하는지 검증 (없으면 에러 출력 후 재입력/취소)
   - 주문수량은 1 이상 검증
   - `OrderRepository::GenerateOrderNo()`로 orderNo 발급
   - 상태 `RESERVED`로 `OrderRepository::Add` 호출
   - `Logger::LogOrderTransition(orderNo, "-", "RESERVED")` 호출 (신규 생성 로그)
   - 등록 결과(주문번호 포함) 출력

## 구현 파일

- `include/DataPersistence/OrderRepository.h` / `src/DataPersistence/OrderRepository.cpp` — `GenerateOrderNo()` 추가
- `include/ConsoleMVC/OrderController.h` / `src/ConsoleMVC/OrderController.cpp` — 시료주문(예약) 서브메뉴 구현

## 완료 조건

- 존재하지 않는 시료ID로 예약 시도 시 에러 처리 및 저장되지 않음
- 같은 날 여러 건 예약 시 orderNo가 `ORD-YYYYMMDD-0001`, `0002` 순으로 증가
- 예약 직후 생성된 주문이 `RESERVED` 상태로 `orders.json`에 저장됨
