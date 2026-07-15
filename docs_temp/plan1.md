# Plan 1 — 기반 구축

## 구현 목표

- 도메인 모델(`Sample`, `Order`)과 JSON 영속성 계층(`SampleRepository`, `OrderRepository`) 뼈대 구현
- `Logger` 유틸리티(주문 상태전환 로그 인터페이스) 구현
- 콘솔 MVC 뼈대(`ConsoleView`, `AppController`, 각 기능 Controller 스텁) 구현
- 메인 메뉴(1~8) 표시 및 선택 시 스텁 컨트롤러로 진입/복귀되는 루프 완성

## 구현 체크리스트

- [ ] `Sample`/`Order` 구조체 + `to_json`/`from_json`
- [ ] `SampleRepository`/`OrderRepository` — Add/GetAll/FindById(+Update/GenerateOrderNo)
- [ ] `data/` 경로에 JSON 파일 자동 생성
- [ ] `Logger::LogOrderTransition` 인터페이스 + `logs/order_events.log` append
- [ ] `ConsoleView`, `AppController`, 7개 Controller 스텁
- [ ] `main.cpp`에서 전체 조립 후 `AppController::Run()` 호출

## 완료 조건

- 빌드 성공, 메인 메뉴(1~8) 표시 및 스텁 진입/복귀 정상 동작
- `data/*.json` 최초 실행 시 자동 생성
