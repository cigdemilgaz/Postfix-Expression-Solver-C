# Postfix-Expression-Solver-C

This project is an advanced Postfix Expression Solver that can determine unknown characters (variables or operators) in a given mathematical expression. It utilizes Expression Trees and Recursive Brute-Force search to find all possible combinations that satisfy a target equation.

## Key Capabilities

- **Expression Tree Construction:** Converts raw postfix strings into a structured binary tree format.
- **Recursive Evaluation:** Performs post-order traversal to calculate the value of the tree.
- **Brute-Force Permutation:** Systematically tests combinations for each variable to find solutions where the total expression equals zero.
- **Constraint Enforcement:** Ensures all division operations result in integers and handles zero-division errors.

## Technical Implementation

### Data Structures
- **Stack:** A linked-list based stack used for building the Expression Tree from postfix notation.
- **Binary Expression Tree:** Nodes represent either operands (integers) or operators, with recursive links to children.

### Algorithm Flow
- **Parsing:** Identifies variables and constants in the input.
- **Recursive Search (solveT):** For each unknown character, the algorithm tries every operator and a range of integer values.
- **Tree Building (buildTree):** For every combination, a unique tree is constructed.
- **Validation (postorder):** The tree is evaluated. If the result is 0, the combination is printed as a valid solution.

## Setup and Usage

### Prerequisites
* A C compiler (e.g., `gcc`) or an IDE like Dev-C++.
* An `input.txt` file in the same directory.

### Input Format
The input (input.txt) should be a postfix expression ending with = 0. Unknowns are represented by lowercase letters.
- Example: ``` 3 1 + 2 3 2 ^ a 2 / 4 3 - b - c * 2 4 1 + ^ 3 4 * 2 2 * - / 6 2 * - + = 0 ```

### Output Example
The program will display all valid combinations for the unknowns found in the expression:PlaintextPossible combination values for (a, b, c):
```
Possible value combinations for (a, b, c)
(*, 8, +)
(20, -, -)
(20, -, +)
(18, *, +)
(18, *, -)
(16, +, +)
(16, +, -)
(16, *, *)
(16, *, /)
(16, /, *)
(16, /, /)
(18, -, *)
(18, -, /)
(14, +, *)
(14, +, /)
```
