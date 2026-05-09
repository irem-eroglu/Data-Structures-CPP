# Artifact Stewardship System: Advanced Tree Structures

This project is a C++ simulation of a digital modernization effort for a Historical Artifact Management System. It models the complex, real-time interactions between a catalog of historical artifacts and a highly dynamic directory of researchers. 

> **Academic Note & Disclaimer:** This project was developed as part of the BBM203 Software Laboratory I course. The architectural skeleton and the problem statement were provided by the course instructors. The core algorithmic implementations, specifically the self-balancing tree data structures and memory management, were developed entirely by me.

## Technical Highlights & Skills Demonstrated

This project showcases the implementation of complex, self-balancing binary search trees from scratch in **C++11**, completely avoiding the Standard Template Library (STL).

* **Red-Black Tree Implementation (Researcher Directory):** * Developed a fully functional Red-Black Tree to handle highly dynamic insertions and deletions of researcher records.
  * Implemented strict color-flipping logic and complex `insertFixup` and `deleteFixup` algorithms to maintain $O(\log n)$ height balance after node removals.
* **AVL Tree Implementation (Artifact Catalog):** * Implemented an AVL tree to catalog artifacts by their unique IDs, ensuring guaranteed worst-case search times.
  * Engineered dynamic height calculation and multi-directional rotations (Left, Right, Left-Right, Right-Left) to maintain strict balance factors [-1, 1].
* **Relational Node Synchronization:** * Created a system where nodes from two distinctly different tree structures (AVL and RBT) dynamically interact. When a researcher is deleted from the RBT, the system successfully traverses their assigned artifacts and updates the corresponding nodes in the AVL tree, preventing dangling pointers and maintaining data integrity.
* **Advanced Tree Traversals:** Implemented Pre-order, In-order, and Post-order recursive traversals for structured system reporting and statistical aggregation.

## Simulation Mechanics

The system operates via the `ArtifactManager` controller, enforcing strict business rules:
1. **Capacity Enforcement:** Researchers are strictly limited by their assignment capacity. The system rejects requests that exceed this limit.
2. **Dynamic Workflows:** Artifacts can be requested, assigned, and returned. Assignments increase an artifact's inherent "research value" dynamically based on its rarity level.
3. **Automated Revocation:** Using `FIRE_RESEARCHER` or `RETURN_ALL` commands triggers an automated cascade that unassigns artifacts in the AVL tree before modifying the RBT structure.

## Example System Commands

The system executes operations via parsed command-line instructions:
* `ADD_ARTIFACT <id> <name> <rarity> <value>`
* `HIRE_RESEARCHER <name> <capacity>`
* `REQUEST <researcherName> <artifactID>`
* `FIRE_RESEARCHER <name>`
* `MATCH_RARITY <minRarity>`
* `PRINT_STATS`

---
*Developed by İrem Eroğlu as part of the Artificial Intelligence Engineering curriculum.*