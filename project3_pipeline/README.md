# Project 3: MIPS Pipeline CPU Simulator

본 프로젝트는 MIPS 기반 **5-stage Pipeline CPU**를 소프트웨어적으로 시뮬레이션한 것입니다.  
IF, ID, EX, MEM, WB 단계로 구성된 파이프라인 구조를 구현하고,  
Data Hazard와 Control Hazard를 고려한 실행 흐름을 C++로 설계하였습니다.

---

## 🎯 Objectives
- 5-stage Pipeline CPU 구조(IF / ID / EX / MEM / WB) 이해
- Pipeline 환경에서의 명령어 병렬 실행 흐름 구현
- Data Hazard 및 Control Hazard 처리 기법 학습

---

## ⚙️ Key Features
- 5-stage Pipeline CPU 시뮬레이션
- Pipeline State Register 구현
  - IF/ID, ID/EX, EX/MEM, MEM/WB
- Data Hazard 처리
  - Stall insertion
  - Data Forwarding
- Control Hazard 처리
  - Branch instruction 처리
  - Branch prediction 방식 지원
    - Always Taken
    - Always Not Taken
- Pipeline 진행 상태 출력 기능 지원

---

## 🛠 Implementation Overview
- 각 파이프라인 단계를 독립적인 함수로 구현
- Pipeline state register를 통해 stage 간 데이터 전달
- Data Hazard 발생 시 stall 및 forwarding 로직 적용
- Branch instruction 실행 시 control hazard를 감지하고 flush 수행
- 실제 하드웨어 파이프라인 동작을 순차적 코드 흐름으로 시뮬레이션

---

## 🧠 What I Learned
- 단일 사이클 CPU와 파이프라인 CPU의 구조적 차이를 명확히 이해함
- Data Hazard와 Control Hazard가 발생하는 원인과 해결 기법을 코드 수준에서 체득
- 복잡한 하드웨어 동작을 소프트웨어적으로 모델링하는 경험을 통해  
  컴퓨터 구조 전반에 대한 이해도를 크게 향상시킴

---

## 📎 Notes
- 본 프로젝트는 학부 컴퓨터구조 강의 과제로 수행되었습니다.
- 교육 목적의 시뮬레이터로, 실제 하드웨어 구현과는 일부 차이가 있을 수 있습니다.
