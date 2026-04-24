# HoT-Based Skill Assessment – Problem 9
## DAG Optimization: Redundant Expression Detection

### Problem Statement
Directed Acyclic Graphs (DAGs) are constructed to represent expressions during
optimization. Raw node listings do not immediately reveal repeated computations.

### Task
Use C to compute `Expression_Frequency` and create a flag `Is_Common_Subexpression`
for repeated expressions across basic blocks.

### Tool
**C (GCC)**

### HOTS Goal
Detect optimization opportunities via common subexpression analysis

### How to Run
```bash
gcc -o dag_optimization dag_optimization.c
./dag_optimization
```

### Features Engineered
| Feature | Description |
|---|---|
| `Expression_Frequency` | How many times an expression appears in the same block |
| `Is_Common_Subexpression` | True if frequency > 1 |
| `Redundancy_Ratio` | Fraction of redundant nodes per block |
| `Optimization_Opportunity` | True if redundancy ratio > 0.3 |

### Sample Output
```
=================================================================
DAG NODE ANALYSIS - Expression Frequency & CSE Detection
=================================================================
Node_ID  Block_ID  Expression  Freq  Is_Common_Subexpression
-----------------------------------------------------------------
1        B1        a+b         2     True
2        B1        c*d         2     True
...
=================================================================
BLOCK-LEVEL SUMMARY
=================================================================
Block_ID  Redundancy_Ratio  Optimization_Opportunity
B1        0.8000            True
B2        0.6000            True
B3        0.4000            True
B4        0.4000            True
```
