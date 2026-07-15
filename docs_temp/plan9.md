# Plan 9 — 관리자모드

## 구현 목표

- 메인 메뉴 8번: 승인 절차 없이 테스트 주문을 즉시 생산중 상태로 생성(가속평가)하고, 주문 상태전환 로그를 조회

## 구현 체크리스트

- [ ] `AdminController` — 테스트 주문 생성 / 로그 조회 서브메뉴
- [ ] 테스트 주문 생성: 시료ID(존재 검증)/고객명/수량/생산시간(초) 입력 → `status=PRODUCING`, `shortageQty=수량`, `totalProductionSeconds=입력값`으로 즉시 생성
- [ ] `OrderWorkflow`의 큐잉 로직(Phase 5 `IsLineBusy` 등) 재사용해 생산라인 비었으면 즉시 시작, 아니면 대기열 등록
- [ ] `Logger::LogOrderTransition(orderNo, "-", "PRODUCING")` 호출
- [ ] 로그 조회: `Logger::ReadRecentTransitions(count)`로 최근 로그를 콘솔에 출력 (없으면 안내 메시지)
- [ ] `AppController` 메인 메뉴에 8번 연결

## 완료 조건

- 생산라인 비었을 때 즉시 시작, 사용 중일 때 대기열 FIFO 등록 — 정상 승인 플로우와 큐 공유하며 충돌 없음
- 지정 생산시간 경과 후 자동으로 CONFIRMED 전환 (Phase 5 Tick과 동일 동작)
- 로그 조회 시 관리자모드 포함 전체 전환 기록이 최근 순으로 표시
