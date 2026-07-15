# Test Plan — Phase 4 (시료승인/거절)

## 사전조건

- Phase 3 완료
- 시료 `S001`(실리콘웨이퍼, 평균생산시간=10분, 수율=0.9, 재고=0) 등록되어 있음
- 주문 A: `S001` / 수량 30 (RESERVED)
- 주문 B: `S001` / 수량 20 (RESERVED)

## 테스트 시나리오

### TC4-1. 접수된 주문 목록 표시

1. 시료승인/거절 → 접수된 주문 목록 진입
2. 예상 결과: RESERVED 상태인 주문 A, 주문 B가 목록에 표시됨

### TC4-2. 재고 부족 상태에서 첫 주문 승인 → 생산 즉시 시작

1. 주문 A(수량 30) 승인
2. 예상 결과:
   - `shortageQty = 30 - 0 = 30`
   - `actualProductionQty = ceil(30 / 0.9) = 34` (생산 완료 시 재고에 반영될 값)
   - `totalProductionSeconds = 10 * 34 = 340`
   - 상태 `PRODUCING`으로 전환
   - 생산라인이 비어 있었으므로 `productionStartEpochSec`가 즉시 채워짐 (생산현황표기에서 확인, Phase 5 연계)

### TC4-3. 생산라인이 이미 사용 중일 때 두 번째 주문 승인 → 대기열 등록

1. TC4-2 직후, 주문 B(수량 20) 승인
2. 예상 결과:
   - `shortageQty`, `actualProductionQty`, `totalProductionSeconds` 계산은 동일 방식으로 수행
   - 상태 `PRODUCING`으로 전환되지만 `productionStartEpochSec = 0` (대기, Phase 5의 대기주문확인에서 확인)

### TC4-4. 재고가 충분한 경우 즉시 CONFIRMED

1. 별도 시료 `S002`(재고 100)에 대해 수량 10 주문 생성 후 승인
2. 예상 결과: 즉시 `CONFIRMED` 상태로 전환 (생산 관련 필드는 채워지지 않음)

### TC4-5. 주문거절

1. RESERVED 상태의 새 주문 C 생성 후 거절 처리
2. 예상 결과: 상태 `REJECTED`로 전환, 이후 접수된 주문 목록에 나타나지 않음

### TC4-6. 상태전환 로그 기록

1. TC4-2(승인), TC4-5(거절) 이후 `logs/order_events.log` 확인
2. 예상 결과: 주문 A는 `RESERVED`→`PRODUCING`, 주문 C는 `RESERVED`→`REJECTED` 전환 기록이 각각 추가됨

## 완료 기준

- 위 6개 시나리오 모두 통과
- `data/orders.json`에서 각 주문의 상태/필드 값이 시나리오별 예상과 일치
