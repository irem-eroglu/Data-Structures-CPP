# Data Structures in C++

This repository contains a collection of comprehensive C++ projects focusing on fundamental data structures, manual memory management, and object-oriented design. 

> **Academic Disclaimer:** These projects were developed as part of the BBM203 Software Laboratory I course at Hacettepe University. The architectural skeletons (header definitions) and the problem statements/scenarios were provided by the course instructors. All core algorithmic implementations, custom data structures, and memory management logic were developed entirely by me.

## Repository Structure

The projects in this repository are built strictly using **C++11** without relying on the Standard Template Library (STL). All data structures (Linked Lists, Stacks, Queues, Trees) were implemented from scratch.

### 1. [PA1: Rail Marshal System](./PA1-Rail-Marshal-System)
A rule-based freight network simulation managing train assembly and dispatch.
* **Key Topics:** Custom Doubly Linked Lists, 2D Matrix Classification, Dynamic Memory Allocation.
* **Highlight:** Splitting linked lists dynamically based on physical weight thresholds (Coupler Overload logic) and merging lists in $O(1)$ time.

### 2. [PA2: QuakeAssist System](./PA2-QuakeAssist-System)
An emergency response coordination system handling concurrent supply and rescue operations.
* **Key Topics:** Circular Queues, Dynamic Stacks, LIFO/FIFO Operations.
* **Highlight:** Engineered a custom rollback algorithm that securely pops overloaded tasks from a team's mission stack and returns them to their original queues without losing priority order.

### 3. [PA3: Artifact Stewardship System](./PA3-Artifact-Stewardship-System)
A complex simulation managing historical artifacts and researcher assignments using advanced self-balancing trees.
* **Key Topics:** Red-Black Trees, AVL Trees, Relational Node Synchronization.
* **Highlight:** From-scratch implementation of Red-Black Tree `deleteFixup` algorithms and AVL multi-directional rotations to maintain strict algorithmic efficiency.

## Technical Focus
* **Memory Management:** Zero-leak policy enforced via strict destructor implementation and raw pointer handling.
* **Algorithmic Efficiency:** Prioritized $O(1)$ and $O(\log n)$ complexities for data retrieval and manipulation.
* **Architecture:** Modular object-oriented design keeping business logic separated from data models.

---
*Developed by İrem Eroğlu as part of the Artificial Intelligence Engineering curriculum.*
