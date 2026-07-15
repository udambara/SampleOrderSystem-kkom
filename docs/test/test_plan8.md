# Test Plan — Phase 8 (더미데이터 생성기)

## 사전조건

- Phase 1의 `SampleRepository`/`OrderRepository`가 사용하는 `data/samples.json`, `data/orders.json` 경로와 동일한 경로를 더미데이터 생성기가 사용

## 테스트 시나리오

### TC8-1. Dummy Sample 생성

1. 더미데이터 생성기 실행, 생성 개수 N=10 지정
2. 예상 결과: `data/samples.json`에 임의값(ID/이름/평균생산시간/수율)을 가진 시료 10개가 추가됨, 각 시료ID는 중복되지 않음

### TC8-2. Dummy Order 생성

1. TC8-1 이후 더미데이터 생성기로 주문 M=20건 생성
2. 예상 결과: `data/orders.json`에 주문 20건이 추가되고, 각 주문의 `sampleId`는 모두 실제 존재하는 시료를 참조함, 상태는 모두 `RESERVED`

### TC8-3. 생성 개수 0 또는 미지정

1. 생성 개수를 0으로 지정하거나 인자 없이 실행
2. 예상 결과: 에러 없이 아무 데이터도 추가되지 않음 (또는 사용법 안내 출력)

### TC8-4. 메인 프로그램과의 연동 확인

1. 더미데이터 생성 후 메인 프로그램(ProjectSampleOrderSystem) 실행
2. 예상 결과: 시료관리(조회)에서 생성된 시료가 보이고, 생성된 주문에 대해 승인/생산/출고 흐름을 정상적으로 진행할 수 있음

## 완료 기준

- 위 4개 시나리오 모두 통과
