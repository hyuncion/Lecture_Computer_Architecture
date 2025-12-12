# Project 2: MIPS Instruction Simulator

본 프로젝트는 MIPS 머신 코드를 입력으로 받아  
명령어를 순차적으로 실행하는 **MIPS Instruction-Level Simulator**를 C++로 구현한 것입니다.  
단일 사이클 CPU 모델을 기반으로 PC, 레지스터, 메모리의 동작을 직접 시뮬레이션하며  
명령어 실행 흐름에 대한 이해를 목표로 하였습니다.

---

## 🎯 Objectives
- 단일 사이클 기반 CPU 동작 흐름 이해
- Instruction Fetch → Decode → Execute 과정 구현
- PC, Register File, Memory 구조 직접 관리

---

## ⚙️ Key Features
- MIPS 머신 코드(hexadecimal) 입력 처리
- Program Counter(PC) 기반 명령어 fetch
- Register File 및 Memory 상태 관리
- 주요 MIPS 명령어 실행 지원
  - Arithmetic / Logical instructions
  - Branch / Jump instructions
  - Load / Store instructions
- 옵션에 따른 레지스터 및 메모리 상태 출력 기능 제공

---

## 🛠 Implementation Overview
- 머신 코드를 메모리에 적재한 후 PC를 기준으로 명령어를 fetch
- Binary decoding을 통해 opcode 및 operand 추출
- Instruction type에 따라 각 명령어의 동작을 함수 단위로 구현
- 명령어 실행에 따라 PC, 레지스터, 메모리 상태를 갱신

---

## 🧠 What I Learned
- 단일 사이클 CPU의 명령어 실행 흐름을 코드 수준에서 명확히 이해함
- 레지스터와 메모리 간의 데이터 이동 과정을 직접 구현하며 동작 원리 체득
- 이후 파이프라인 CPU 및 캐시 시뮬레이터 구현을 위한 기반 역량 확보

---

## 📎 Notes
- 본 프로젝트는 학부 컴퓨터구조 강의 과제로 수행되었습니다.
- 교육 목적의 시뮬레이터로, 실제 하드웨어와는 일부 차이가 있을 수 있습니다.
