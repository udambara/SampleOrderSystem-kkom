# Phase 1 — 기반 구축 (도메인 모델 / 영속성 / 콘솔 MVC 뼈대)

## 목표

이후 모든 기능(Phase 2~9)이 그 위에서 동작할 수 있는 최소 골격을 만든다. 이 단계가 끝나면 프로그램을 실행했을 때 메인 메뉴가 표시되고 선택은 가능하지만, 각 메뉴의 실제 동작은 아직 없는 상태(스텁)여도 된다.

## 데이터 모델

- `Sample` : `id`(string), `name`(string), `avgProductionTimeMinutes`(double), `yield`(double), `stock`(int)
- `Order` : `id`, `orderNo`, `sampleId`, `customerName`, `quantity`, `status`(enum: RESERVED/CONFIRMED/PRODUCING/RELEASE/REJECTED), `productionStartEpochSec`, `totalProductionSeconds`, `shortageQty`, `releasedAtEpochSec`, `releasedQty`
- 두 구조체 모두 nlohmann::json 기반 `to_json`/`from_json` 제공

## 동작

1. **JSON 영속성 계층**
   - `SampleRepository`: 파일(`samples.json`) 기반 CRUD 뼈대 — Add / GetAll / FindById (검색은 Phase 2에서 구현)
   - `OrderRepository`: 파일(`orders.json`) 기반 CRUD 뼈대 — Add / GetAll / FindById / Update / GenerateOrderNo
   - 저장 파일 경로는 실행 파일 기준 상대경로(`data/`)로 고정
2. **콘솔 MVC 뼈대**
   - `ConsoleView` : 메뉴 출력, 입력값 읽기 등 콘솔 입출력 유틸리티
   - `AppController` : 메인 메뉴(시료관리/시료주문/시료승인거절/모니터링/출고처리/생산라인/종료/관리자모드)를 표시하고 선택에 따라 각 Controller로 위임하는 루프
   - 각 기능 Controller(`SampleController`, `OrderController`, `ApprovalController`, `MonitoringController`, `ShippingController`, `ProductionController`, `AdminController`)는 이 단계에서 빈 스텁(메뉴 진입만 되고 "미구현" 출력)으로 생성
3. **로깅 유틸리티**
   - `Logger` : 주문 상태전환 이벤트를 `logs/order_events.log`에 append (시각 / 주문번호 / 이전상태 / 새상태)
   - Phase 3~6, Phase 9(관리자모드)에서 주문 상태가 바뀌는 모든 지점에서 호출할 수 있도록 이 단계에서 인터페이스를 먼저 만든다 (`LogOrderTransition(orderNo, fromStatus, toStatus)`)
4. **main 진입점**
   - `SampleRepository`/`OrderRepository`/`OrderWorkflow`/`Logger` 인스턴스를 생성해 `AppController`에 주입, `Run()` 호출

## 구현 파일 (예상)

- `include/DataPersistence/{Sample.h, Order.h, SampleRepository.h, OrderRepository.h, OrderWorkflow.h}`
- `include/third_party/nlohmann/json.hpp` (외부 라이브러리, PoC의 DataPersistence-kkom에서 그대로 가져옴)
- `src/DataPersistence/{Sample.cpp, Order.cpp, SampleRepository.cpp, OrderRepository.cpp, OrderWorkflow.cpp}`
- `include/Logging/Logger.h` / `src/Logging/Logger.cpp`
- `include/ConsoleMVC/{ConsoleView.h, AppController.h, SampleController.h, OrderController.h, ApprovalController.h, MonitoringController.h, ShippingController.h, ProductionController.h, AdminController.h}`
- `src/ConsoleMVC/{ConsoleView.cpp, AppController.cpp, ...Controller.cpp}`
- `src/main.cpp`

## 완료 조건

- 빌드 성공 (Visual Studio, x64/Debug 기준)
- 프로그램 실행 시 메인 메뉴(1~8)가 표시되고, 각 메뉴 선택 시 스텁 컨트롤러로 진입/복귀가 정상 동작
- `data/samples.json`, `data/orders.json` 파일이 최초 실행 시 자동 생성됨 (빈 배열)
- `Logger` 호출 시 `logs/order_events.log` 파일이 자동 생성되고 append 방식으로 기록됨 (호출부는 이후 단계에서 채워짐)
