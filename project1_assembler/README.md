# Project 1: MIPS Assembler

본 프로젝트는 MIPS 어셈블리 코드를 입력으로 받아  
머신 코드(hexadecimal)로 변환하는 **MIPS Assembler**를 C++로 구현한 것입니다.  
컴퓨터구조 강의의 첫 프로젝트로, 명령어 인코딩과 메모리 구조에 대한 이해를 목표로 하였습니다.

---

## 🎯 Objectives
- MIPS instruction format (R / I / J) 구조 이해
- Assembly → Machine Code 변환 과정 구현
- Text section과 Data section의 역할 및 구성 방식 학습

---

## ⚙️ Key Features
- MIPS 어셈블리 코드 파싱
- Text section size 계산
- Data section size 계산
- 주요 MIPS 명령어 인코딩 지원
  - Arithmetic / Logical instructions
  - Branch / Jump instructions
  - Load / Store instructions
- Binary instruction을 hexadecimal 형식으로 변환하여 출력
- `.word` directive 처리

---

## 🛠 Implementation Overview
- 문자열 기반 파싱을 통해 instruction과 operand 추출
- Instruction type에 따라 opcode, register, immediate field 구성
- Binary instruction을 내부적으로 생성한 뒤 hexadecimal 형식으로 변환
- Text section과 Data section을 분리하여 처리

---

## 🧠 What I Learned
- MIPS instruction encoding 과정을 직접 구현하며 어셈블러의 내부 동작을 명확히 이해함
- 명령어 형식과 메모리 주소 계산 방식에 대한 기초 역량 확보
- 이후 CPU 시뮬레이터 및 파이프라인 구현을 위한 기반 지식 습득

---

## 📎 Notes
- 본 프로젝트는 학부 컴퓨터구조 강의 과제로 수행되었습니다.
- 교육 목적의 구현으로, 실제 상용 MIPS assembler와는 일부 차이가 있을 수 있습니다.
