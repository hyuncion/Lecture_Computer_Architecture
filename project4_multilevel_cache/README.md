# Project 4: Multilevel Cache Simulator

본 프로젝트는 **L1 / L2 멀티레벨 캐시 구조**를 소프트웨어적으로 시뮬레이션한 것입니다.  
Cache의 용량, 연관도, 블록 크기 및 교체 정책에 따른 동작 차이를 분석하며  
캐시 메모리의 성능 특성을 이해하는 것을 목표로 하였습니다.

---

## 🎯 Objectives
- 캐시 메모리의 기본 구조 및 동작 원리 이해
- Multilevel Cache (L1 / L2) 구조 학습
- Cache parameter 변화에 따른 성능 특성 분석

---

## ⚙️ Key Features
- L1 / L2 멀티레벨 캐시 구조 구현
- Cache parameter 설정 가능
  - Capacity
  - Associativity
  - Block Size
- Cache replacement policy 지원
  - LRU (Least Recently Used)
  - Random
- Read / Write access 시 cache hit 및 miss 처리
- Cache miss rate 및 eviction 통계 수집
  - Read miss / Write miss
  - Clean eviction / Dirty eviction

---

## 🛠 Implementation Overview
- 메모리 접근 trace 파일을 기반으로 cache access 시뮬레이션 수행
- 주소를 Tag / Index / Block Offset으로 분리하여 cache 접근 처리
- Cache hit / miss 여부에 따라 L1과 L2 cache를 순차적으로 갱신
- Replacement policy에 따라 cache line 교체 로직 구현
- 각 실험 조건에 대해 cache 성능 지표를 기록

---

## 🧠 What I Learned
- 캐시 메모리 구조가 시스템 성능에 미치는 영향을 정량적으로 이해함
- Cache parameter(capacity, associativity, block size)에 따른 miss rate 변화 분석
- Multilevel cache 환경에서의 데이터 지역성(locality) 개념을 명확히 체득
- 하드웨어 개념을 소프트웨어 시뮬레이션으로 모델링하는 경험 축적

---

## 📎 Notes
- 본 프로젝트는 학부 컴퓨터구조 강의 과제로 수행되었습니다.
- 교육 목적의 시뮬레이터로, 실제 하드웨어 캐시 구현과는 일부 차이가 있을 수 있습니다.
