# Rail Marshal System: Freight Network Simulation

This project is a C++ simulation of a freight train classification and marshaling yard. It was developed to model how real-world wagons are sorted by destination and cargo type, assembled into block trains, and dispatched according to specific safety and logistical rules.

> **Academic Note & Disclaimer:** This project was developed as part of the BBM203 Software Laboratory I course. The architectural skeleton (header definitions and basic file structures) and the problem statement were provided by the course instructors. The core algorithmic implementations, including custom data structures and memory management, were developed entirely by me.

## Technical Highlights & Skills Demonstrated

This project is built using strictly C++11 without the use of standard template library (STL) containers like std::vector, std::list, or std::map. All data structures were implemented from scratch.

* **Custom Doubly Linked Lists (WagonList):** Designed a robust doubly linked list to manage wagons. Includes features like:
  * Insertion in descending order of weight.
  * Splitting lists natively when coupler weight limits are exceeded.
  * O(1) appending and merging operations via move semantics (&&).
* **Dynamic Memory Management:** Careful manipulation of raw pointers and destructors to ensure zero memory leaks during the continuous creation, merging, and destruction of Train and Wagon objects.
* **2D Matrix Classification:** Used a matrix of linked lists to categorize incoming wagons dynamically based on destination and cargo type.
* **Rule-Based Algorithm Design:** * Implemented logic to keep hazardous cargo furthest away from the locomotive.
  * Handled threshold-based automatic dispatching algorithms.

## Simulation Mechanics

The system operates via a controller (RailMarshal) that reads commands and manages two main yards:

1. **Classification Yard:** A temporary holding area where incoming wagons are sorted. Wagons are automatically placed in descending order of weight to maintain train balance.
2. **Departure Yard:** Operates as an array of tracks (one for each destination). Formed trains wait in a FIFO-like queue until they are manually or automatically dispatched.

### Core Business Rules Enforced
* **Coupler Overload Protection:** The system calculates cumulative weight from the rear. If the trailing weight exceeds a wagon's maxCouplerLoad, the train automatically splits into multiple safe trains.
* **Cargo Grouping:** When assembling trains, wagons of the same cargo type remain grouped, but groups are ordered by the weight of their heaviest wagon.

---
*Developed by İrem Eroğlu as part of the Artificial Intelligence Engineering curriculum.*