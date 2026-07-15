# Review — Phase 7

## 이슈 1: 생산현황 서브메뉴에 머무르면 생산 완료가 반영되지 않음

### 피드백

생산라인 → 생산현황표기를 반복 조회했는데, 경과시간이 총생산시간을 넘어(예: 20초/총 16초, 남은시간 0초) 있는데도 계속 `PRODUCING` 상태로 표시되며 완료 처리가 되지 않았다.

### 원인

`OrderWorkflow::Tick()`은 `AppController`의 메인 루프 상단에서만 호출되었다. 각 서브메뉴(`ProductionController` 등)는 자신만의 `while(true)` 루프를 가지고 있어서, 사용자가 메인 메뉴로 돌아가지 않고 같은 서브메뉴 안에서 계속 선택을 반복하면 `Tick()`이 전혀 호출되지 않아 실제로는 생산이 끝났어도 화면과 데이터 모두 갱신되지 않았다.

### 조치

`ApprovalController`, `MonitoringController`, `ShippingController`, `ProductionController`가 각자 `OrderWorkflow` 인스턴스를 갖도록 하고, 자신의 서브메뉴 루프 상단에서도 `Tick()`을 호출하고 완료 시 로그를 남기도록 수정 (`AppController`와 동일한 패턴). `docs/phases/phase5.md`, `docs/test/test_plan5.md`(TC5-6 추가)에 반영.

### 검증

재빌드 성공. 생산현황표기 서브메뉴 안에만 머무른 상태로 생산시간을 경과시킨 뒤, 메인 메뉴로 나가지 않고도 주문 상태가 `PRODUCING` → `CONFIRMED`로 정상 전환되는 것을 데이터 파일로 확인함.

## 이슈 2: 승인 시점에 재고가 반영되지 않음 (설계 변경)

### 피드백

재고가 충분해서 승인 즉시 `CONFIRMED`로 전환된 주문에 대해, 재고 숫자가 승인 시점에는 바뀌지 않고 출고해야만 바뀌는 게 이상하다는 지적. "승인 시점에서 차감해줘"라는 명시적 요청.

### 조치

- `Order`에 `stockDeductedAtApproval`(bool) 필드 추가
- `OrderWorkflow::Approve`에서 재고 ≥ 주문수량인 즉시-CONFIRMED 분기일 때, 그 자리에서 `SampleRepository::AdjustStock`으로 재고를 차감하고 `stockDeductedAtApproval = true`로 기록
- `ShippingController::HandleRelease`는 `stockDeductedAtApproval`이 true인 주문은 재고를 다시 차감하지 않고 상태만 `RELEASE`로 전환 (이중 차감 방지). false인 주문(생산을 거쳐 CONFIRMED된 경우)은 기존대로 출고 시점에 차감
- `DataMonitor`의 재고량확인 "미출고 수요" 계산에서도 `stockDeductedAtApproval == true`인 CONFIRMED 주문은 제외 (이미 재고에 반영되었으므로 중복 계산 방지)
- `docs/PRD.md`, `docs/phases/phase4.md`/`phase6.md`/`phase7.md`, `docs/test/test_plan4.md`(TC4-4 갱신)/`test_plan6.md`(TC6-7 추가)/`test_plan7.md`(TC7-3 갱신) 반영

### 검증

- 재빌드 성공
- 재고 234에 수량 220 주문을 승인 → 즉시 재고가 14로 차감됨을 확인 (사용자가 보여준 예시와 동일 수치)
- 이후 해당 주문을 출고 → 재고가 14에서 변하지 않음(이중 차감 없음)을 확인, 상태는 정상적으로 `RELEASE`로 전환됨
