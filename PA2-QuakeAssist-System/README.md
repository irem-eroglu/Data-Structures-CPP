# QuakeAssist: Emergency Coordination System

This project is a C++ simulation of a disaster response coordination system. It is designed to evaluate algorithms for handling and prioritizing simultaneous emergency requests (Supply vs. Rescue) and dispatching field teams efficiently without exceeding their operational capacities.

> **Academic Note & Disclaimer:** This project was developed as part of the BBM203 Software Laboratory I course. The architectural skeleton and the problem statement were provided by the course instructors. The core algorithmic implementations, including custom memory management, circular queues, and dynamic stacks, were developed entirely by me.

## Technical Highlights & Skills Demonstrated

Built completely from scratch in **C++11**, bypassing the Standard Template Library (STL) to demonstrate fundamental data structure mechanics and raw memory management.

* **Dynamic Circular Queues (`RequestQueue`):** * Designed FIFO (First-In-First-Out) queues for supply and rescue requests using dynamically allocated arrays.
  * Implemented circular wrap-around logic (`(front + i) % capacity`) to optimize memory usage and $O(1)$ operations.
  * Includes automated capacity doubling when the queue is full.
* **Dynamic Stacks & Rollback Mechanics (`MissionStack`):** * Implemented LIFO (Last-In-First-Out) stacks to track active team missions.
  * Developed a complex **Rollback Mechanism**: If assigning a new request exceeds a team's predefined workload capacity, the system triggers an overload protocol, safely popping all requests from the stack and returning them to their respective original queues without losing priority order.
* **Emergency Decision Engine:** Implemented a rule-based priority algorithm that computes an `emergencyScore` dynamically by comparing the front elements of both the supply and rescue queues to select the most critical mission at any given moment.

## Simulation Mechanics

The `QuakeAssistController` reads runtime commands to manage the disaster response flow:
1. **Queuing:** Incoming emergency requests are pushed into either the `supplyQueue` or `rescueQueue`.
2. **Assignment:** The decision engine compares queues and pushes the most critical tasks into a specific Team's `MissionStack`.
3. **Overload Protection:** If the `workloadContribution` exceeds the Team's `maxLoadCapacity`, the mission is aborted, and requests are rolled back.
4. **Dispatch:** Successfully loaded teams are dispatched, clearing their stacks for the next emergency.

## Example System Commands

The system is tested using a batch file of commands:
* `INIT_TEAMS <numTeams>`
* `SET_TEAM_CAPACITY <teamID> <maxLoadCapacity>`
* `ADD_SUPPLY <id> <city> <supplyType> <amount> <emergencyLevel>`
* `ADD_RESCUE <id> <city> <numPeople> <buildingRisk> <emergencyLevel>`
* `HANDLE_EMERGENCY <teamID> <k>` (Assigns up to *k* requests)

---
*Developed by İrem Eroğlu as part of the Artificial Intelligence Engineering curriculum.*
