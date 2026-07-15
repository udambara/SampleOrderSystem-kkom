# Phase 9 — 관리자모드

## 목표

테스트/시연 시 정상 승인 절차와 실제 생산시간을 기다리지 않고도 생산라인(Phase 5) 및 출고(Phase 6) 흐름을 빠르게 검증할 수 있도록, 메인 메뉴 8번에 관리자 전용 메뉴를 추가한다.

## 데이터

- 추가 필드 없음. 기존 `Order` 필드(`productionStartEpochSec`, `totalProductionSeconds`, `shortageQty`)를 관리자모드에서 직접 채워 생성

## 동작

1. **테스트 주문 생성 (가속평가)**
   - 시료ID / 고객명 / 주문수량 입력 (시료ID는 Phase 2와 동일하게 존재 검증)
   - 생산시간(초) 직접 입력받음 (`totalProductionSeconds` 오버라이드 — 예: 5초)
   - 승인 절차를 거치지 않고 주문을 다음 값으로 즉시 생성:
     - `status = PRODUCING`
     - `shortageQty = 주문수량` (테스트 주문은 전량을 생산 대상으로 취급)
     - `totalProductionSeconds = 입력값`
     - 생산라인이 비어 있으면 `productionStartEpochSec = now()`로 즉시 생산 시작, 아니면 0으로 두고 기존 대기열 뒤에 등록 (Phase 5의 `OrderWorkflow::Tick`/`IsLineBusy` 로직을 그대로 재사용)
   - `Logger::LogOrderTransition(orderNo, "-", "PRODUCING")` 호출
   - 생성 결과(주문번호, 지정한 생산시간, 대기열 위치) 출력
2. **로그 조회**
   - `logs/order_events.log`를 읽어 최근 순으로 콘솔에 출력 (시각 / 주문번호 / 이전상태 / 새상태)
   - 로그 파일이 없거나 비어 있으면 "기록된 로그 없음" 출력

## 구현 파일

- `include/ConsoleMVC/AdminController.h` / `src/ConsoleMVC/AdminController.cpp` — 관리자모드 서브메뉴(테스트 주문 생성 / 로그 조회) 구현
- `include/DataPersistence/OrderWorkflow.h` — 테스트 주문을 큐에 등록하는 헬퍼(예: `EnqueueTestOrder`) 추가 (Approve와 큐잉 로직을 공유하도록 리팩터링)
- `include/Logging/Logger.h` — 로그 파일을 읽어 최근 N건을 반환하는 조회 함수 추가 (예: `ReadRecentTransitions(int count)`)
- `include/ConsoleMVC/AppController.h` / `src/ConsoleMVC/AppController.cpp` — 메인 메뉴에 8번 관리자모드 진입 추가

## 완료 조건

- 관리자모드로 생성한 주문이 생산라인이 비어 있으면 즉시 생산 시작되고, 지정한 초(예: 5초) 경과 후 다음 메뉴 진입 시 자동으로 `CONFIRMED`로 전환됨 (Phase 5 Tick과 동일하게 동작)
- 생산라인이 사용 중일 때 관리자모드로 주문을 생성하면 기존 대기열 뒤에 FIFO로 등록됨
- 로그 조회 시 지금까지 발생한 모든 주문 상태전환(관리자모드 포함)이 최근 순으로 표시됨
- 정상 승인 플로우(Phase 4)와 관리자모드 생성 플로우가 동일한 생산큐를 공유해 서로 간섭 없이 동작함
