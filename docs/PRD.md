# SampleOrderSystem PRD

## 개요

반도체 시료(Sample) 관리 프로그램. 고객의 시료 주문이 들어오면 재고를 확인해 필요한 만큼 생산한 뒤 출고하는 콘솔 기반 애플리케이션이다.

기존에 PoC 형태로 아래 5개 저장소로 나누어 검증했던 구현을 참고하여, 하나의 완전한 프로그램으로 재구성한다.

- `poc-SampleOrderSystem` — 전체 개념 증명 (서브모듈 통합 버전)
- `ConsoleMVC-kkom` — 콘솔 UI (MVC 패턴)
- `DataPersistence-kkom` — 데이터 영속성 (JSON)
- `DataMonitor-kkom` — 상태 모니터링 도구
- `DummyDataGenerator-kkom` — 테스트용 더미 데이터 생성기

## 목표 사용자 / 시나리오

- 주문 담당자: 고객 주문을 접수한다.
- 생산라인 담당자: 접수된 주문을 승인/거절하고 생산 현황을 확인한다.
- 관리자: 전체 주문/재고 상태를 모니터링한다.
- 출고 담당자: 승인된 주문을 출고 처리한다.

## 메인 메뉴

1. **시료관리** — 시료 등록 / 목록 조회 / 검색
2. **시료주문** — 고객 주문 접수 (예약)
3. **시료승인/거절** — 접수된 주문에 대한 승인·거절 처리
4. **모니터링** — 상태별 주문 수 및 시료별 재고 현황 확인
5. **출고처리** — 승인된 주문의 출고 실행
6. **생산라인** — 생산 현황 및 대기 주문(FIFO) 확인
7. **종료**
8. **관리자모드** — 테스트용 주문을 즉시 생산중 상태로 생성(가속평가)하고, 주문 상태전환 로그를 조회

## 도메인 모델

### Sample (시료)

| 필드 | 타입 | 설명 |
|---|---|---|
| id | string | 시료 고유 ID |
| name | string | 시료 이름 |
| avgProductionTimeMinutes | double | 평균 생산시간(분) |
| yield | double | 수율 = 정상 생산 수 / 총 생산 수 (예: 0.9) |
| stock | int | 현재 재고 수량 |

### Order (주문)

| 필드 | 타입 | 설명 |
|---|---|---|
| id | string | 주문 내부 ID |
| orderNo | string | 표시용 주문번호 `ORD-YYYYMMDD-NNNN` |
| sampleId | string | 주문 대상 시료 ID |
| customerName | string | 고객명 |
| quantity | int | 주문 수량 |
| status | enum | `RESERVED` / `CONFIRMED` / `PRODUCING` / `RELEASE` / `REJECTED` |
| productionStartEpochSec | long long | 생산 시작 시각 (0 = 대기중) — `PRODUCING` 상태에서만 의미 있음 |
| totalProductionSeconds | long long | 총 생산 소요 시간(초) |
| shortageQty | int | 승인 시점의 부족분(주문수량 − 재고), 참고용 |
| actualProductionQty | int | 실생산량 = `ceil(shortageQty / yield)`. 생산 완료 시 재고에 더해지는 값 |
| stockDeductedAtApproval | bool | 승인 시점에 재고가 이미 차감되었는지 여부 (재고 충분으로 즉시 CONFIRMED된 경우 true) — 출고 시 이중 차감을 막는 데 사용 |
| releasedAtEpochSec | long long | 출고 처리 시각 — `RELEASE` 상태에서만 의미 있음 |
| releasedQty | int | 출고 수량 |

## 기능 요구사항

### 1. 시료관리

- **시료등록**: 시료ID, 이름, 평균생산시간, 수율을 입력받아 신규 등록
- **시료조회**: 등록된 모든 시료 목록과 현재 재고 수량 표시
- **시료검색**: 이름 등 속성으로 시료 검색

### 2. 시료주문

- **시료예약**: 시료ID/고객명/주문수량 입력
  - 시료ID가 등록되어 있는지 검증 (없으면 에러)
  - orderNo 자동 발급 (`ORD-YYYYMMDD-` + 당일 기존 주문 수+1을 4자리 zero-pad)
  - 상태 `RESERVED`로 저장, 등록 결과(주문번호 포함) 출력

### 3. 시료승인/거절

- **접수된 주문 목록**: `RESERVED` 상태 주문만 표시
- **주문승인**: 특정 주문 선택
  - 재고 ≥ 주문수량 → **재고에서 주문수량만큼 즉시 차감**하고 `CONFIRMED`로 전환 (`stockDeductedAtApproval = true` 기록 — 이후 출고 시 다시 차감하지 않음)
  - 재고 < 주문수량 →
    - `shortageQty` = 주문수량 − 재고 (부족분, 참고용)
    - `actualProductionQty`(실생산량) = `ceil(shortageQty / yield)` — 생산라인은 수율 손실을 감안해 이만큼을 투입하지만, **생산이 끝나면 투입한 실생산량 전량이 그대로 재고에 반영된다** (수율만큼 다시 걸러내지 않음). 따라서 생산 완료 후에는 부족분보다 재고가 더 늘어날 수 있다 (예: 재고 50, 주문 100, 수율 50% → 실생산량 100 전량이 재고에 더해져 재고 150, 이 중 100을 출고하면 50이 남음)
    - `totalProductionSeconds` = 평균생산시간(분) × 실생산량 (데모 속도를 위해 분을 초로 그대로 사용, 60배 축소)
    - 상태 `PRODUCING`으로 전환
    - 생산라인이 비어 있으면 즉시 생산 시작(`productionStartEpochSec` = now), 아니면 대기(0 → 생산큐 대기)
- **주문거절**: 특정 주문 선택 → 즉시 `REJECTED`

### 4. 모니터링

- **주문량확인**: 상태별(`RESERVED`/`CONFIRMED`/`PRODUCING`/`RELEASE`) 주문 수 표시. `REJECTED`는 유효한 주문이 아니므로 집계에서 제외
- **재고량확인**: 시료별 현재 재고 수량과 상태 표기. 상태는 고정 임계값이 아니라 **재고 대비 미출고 주문 수요**로 판단
  - 미출고 수요 = 해당 시료의 `RESERVED`+`PRODUCING` 주문과, 생산을 거쳐 `CONFIRMED`가 된 주문(`stockDeductedAtApproval == false`)의 수량 합. 재고 충분으로 승인 즉시 `CONFIRMED`된 주문(`stockDeductedAtApproval == true`)은 이미 재고에서 차감되었으므로 제외. `REJECTED`/`RELEASE`도 제외
  - 재고 0 → 고갈
  - 재고 − 미출고 수요 ≥ 0 → 여유 (지금 있는 모든 주문을 감당하고도 남음)
  - 그 외(재고 < 미출고 수요) → 부족
  - 예: 재고 170, CONFIRMED 130(승인 즉시 확정, 이미 차감 반영되어 재고가 40으로 표시) → 여유. 여기에 RESERVED 90이 추가로 들어오면 재고 40 − 수요 90 = -50 → 부족(50개 추가 생산 필요)

### 5. 출고처리

- **출고실행**: `CONFIRMED` 상태 주문 목록에서 선택
  - `stockDeductedAtApproval`이 true면(승인 시 이미 차감됨) 재고를 다시 차감하지 않음. false면(생산을 거쳐 CONFIRMED된 경우) 재고 ≥ 주문수량 확인 후 차감 (부족하면 에러)
  - 상태 `RELEASE`로 전환, `releasedAtEpochSec`/`releasedQty` 기록
  - 결과 화면 출력 (주문번호/출고수량/처리일시/상태변경 내역)

### 6. 생산라인

- **Tick 처리**: 메뉴 진입마다 1회 호출
  - 현재 생산중인 주문의 경과시간이 `totalProductionSeconds` 이상이면 → 재고에 `actualProductionQty`(실생산량 전량) 반영, 상태 `CONFIRMED`로 전환
  - 생산라인이 비어 있고 대기중인 주문(`productionStartEpochSec == 0`)이 있으면 다음 주문을 꺼내 생산 시작
- **생산현황표기**: 현재 생산중인 주문의 주문번호/시료명/경과시간/남은시간 표시 (없으면 "생산중인 주문 없음")
- **대기주문확인**: 생산큐를 FIFO 순서로 표시 (주문번호/시료명/수량)

### 8. 관리자모드

테스트/시연을 빠르게 진행하기 위한 관리자 전용 메뉴. 메인 메뉴 8번으로 진입한다.

- **테스트 주문 생성(가속평가)**: 시료ID/고객명/주문수량과 함께 `totalProductionSeconds`를 직접 지정해 주문 생성
  - 승인 절차 없이 즉시 상태 `PRODUCING`으로 생성, `shortageQty`/`actualProductionQty` 모두 주문수량으로 설정 (Tick 완료 시 이 값이 재고에 반영됨)
  - 생산라인이 비어 있으면 즉시 생산 시작, 아니면 기존 대기열 뒤에 FIFO로 등록 (Phase 5 로직 재사용)
  - 실제 승인 대기시간 없이 곧바로 생산 완료 → 재고 반영까지의 흐름을 짧은 시간 안에 확인 가능
- **로그 조회**: 주문 상태전환 로그(아래 로깅 요구사항 참고)를 최근 순으로 콘솔에 출력

## 아키텍처 / 모듈 구성

기존 PoC는 4개 서브모듈(ConsoleMVC / DataPersistence / DataMonitor / DummyDataGenerator)로 분리되어 있었다. 새 구현에서도 아래 계층 구분을 유지한다.

| 모듈 | 책임 |
|---|---|
| **ConsoleMVC** | Model / Controller / View 구조의 콘솔 UI. View는 입출력만 담당, Controller가 입력 검증과 DataPersistence/DataMonitor 호출을 담당 |
| **DataPersistence** | Sample/Order를 JSON 파일로 저장·조회하는 Repository 계층 + `OrderWorkflow`(승인/거절/생산 tick 로직) |
| **DataMonitor** | DataPersistence가 저장한 데이터를 읽어 상태별 주문 수, 재고 현황, 생산 현황을 콘솔에 표시 (데이터를 직접 수정하지 않음) |
| **DummyDataGenerator** | 테스트용 더미 Sample/Order를 생성해 DataPersistence 저장소에 직접 추가. 생성 개수를 지정 가능 |
| **Logging** | 주문 상태전환 이벤트를 파일에 기록하는 공통 유틸리티. ConsoleMVC/DataPersistence 전 영역에서 상태전환 시점에 호출 |

## 비기능 요구사항

- 언어/환경: C++20, Visual Studio 프로젝트(.vcxproj), Windows 콘솔 애플리케이션
- 데이터 저장: JSON 파일 (nlohmann::json 사용), 단일 프로세스 기준 — 동시성은 고려하지 않음
- 생산 시뮬레이션은 데모 속도를 위해 실제 분 단위 시간을 초 단위로 축소해서 사용
- **로깅**: 모든 주문 상태전환(RESERVED 생성 포함 CONFIRMED/PRODUCING/REJECTED/RELEASE 전환, 관리자모드에서 생성된 주문 포함)을 `logs/order_events.log`에 시각/주문번호/이전상태/새상태 형식으로 기록

## 참고 / 미결 사항

- 기존 PoC는 4개 저장소(서브모듈)로 분리되어 있었으나, 새 구현을 단일 저장소/프로젝트로 통합할지 여부는 별도 설계 단계에서 결정한다.
- ~~재고 상태 임계값(여유/부족/고갈 기준 수량)은 PoC의 예시 값(100)을 그대로 쓸지 조정할지 설계 단계에서 확정한다.~~ → Phase 7 리뷰에서 고정 임계값(100) 대신 "재고 대비 미출고 주문 수요" 기준으로 확정함 (위 4.모니터링 참고)
- 동시 다중 생산 라인 지원 여부(현재는 단일 생산라인, FIFO 큐 가정)는 확장 검토 대상이다.
