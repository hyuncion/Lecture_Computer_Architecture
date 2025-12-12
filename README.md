# Lecture: Computer Architecture

컴퓨터구조 학부 강의에서 수행한 4개의 프로젝트를 정리한 저장소입니다.  
MIPS 어셈블러 구현부터 명령어 실행 시뮬레이터, 5-stage 파이프라인 CPU,  
그리고 L1/L2 멀티레벨 캐시 시뮬레이터까지 컴퓨터 시스템의 동작 원리를  
단계적으로 설계하고 C++로 구현하였습니다.

---

## 📁 Repository Structure
```text
Lecture_Computer_Architecture/
├─ project1/
│  └─ mips_assembler.cpp
├─ project2/
│  └─ mips_instruction_simulator.cpp
├─ project3/
│  └─ mips_pipeline_simulator.cpp
├─ project4/
│  └─ multilevel_cache_simulator.cpp
└─ README.md
```

---

## 📌 Project Overview

### 🔹 Project 1. MIPS Assembler
- MIPS 어셈블리 코드를 입력으로 받아 머신 코드(hex)로 변환
- Text / Data section size 계산
- 주요 MIPS instruction 인코딩 구현 (R / I / J format)

📄 `project1/mips_assembler.cpp`

---

### 🔹 Project 2. MIPS Instruction Simulator
- 단일 사이클 기반 MIPS 명령어 실행 시뮬레이터
- PC, Register File, Memory 구조 구현
- 옵션에 따른 레지스터 및 메모리 상태 출력 지원

📄 `project2/mips_instruction_simulator.cpp`

---

### 🔹 Project 3. MIPS Pipeline CPU Simulator
- 5-stage pipeline 구조 구현 (IF / ID / EX / MEM / WB)
- Data Hazard 및 Control Hazard 처리
- Forwarding, Stall, Flush 로직 구현
- Branch prediction (Always Taken / Always Not Taken) 지원

📄 `project3/mips_pipeline_simulator.cpp`

---

### 🔹 Project 4. Multilevel Cache Simulator
- L1 / L2 멀티레벨 캐시 구조 시뮬레이션
- Capacity, Associativity, Block Size 설정 가능
- LRU / Random replacement 정책 비교
- Cache miss rate 및 eviction 통계 분석

📄 `project4/multilevel_cache_simulator.cpp`

---

## 🛠 Development Environment
- Language: C++
- Compiler: g++ 9.4.0
- OS: Ubuntu 20.04

---

## 📎 Notes
- 본 저장소는 학부 컴퓨터구조 강의 과제로 수행된 프로젝트들을 정리한 것입니다.
- 교육 목적의 구현으로, 실제 하드웨어 동작과는 일부 차이가 있을 수 있습니다.
