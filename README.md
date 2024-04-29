# Graph Analysis for Facebook Dataset

## Overview
This repository contains code for analyzing the Facebook dataset obtained from SNAP (Stanford Network Analysis Project). The dataset represents a social network, where nodes represent Facebook users and edges represent friendships between users.

## Files
1. `graph_fb.c`: This file contains the code to read the Facebook dataset from a file and create a graph data structure.
2. `diffusion_degree.c`: Implements the calculation of diffusion degree scores for nodes in the graph.
3. `mid.c`: Implements the calculation of maximum influence degree scores using the Facebook dataset from `facebook_combined.txt`.
4. `modularity.c`: Calculates the modularity of a partitioning of the graph.
5. `modularity_on_greeding.c`: Implements the greedy algorithm for maximizing modularity.
6. `partitioning.ipynb`: Jupyter notebook containing the analysis of partitioning algorithms. Calculates deficiency and measures time complexity of the partitioning algorithm, finding \( O(n^2 \log n) \) time complexity.
7. `partition_with_mid.ipynb`: Jupyter notebook continuing the analysis, focusing on time complexity, which is found to be \( O(n \log n) \).

## Instructions
1. Compile the C files using an appropriate compiler (e.g., gcc).
2. Run the compiled executables to perform various analyses on the Facebook dataset.
3. Refer to the Jupyter notebooks for a detailed analysis, including time complexity measurements and comparisons between different algorithms.

## Dependencies
- C Compiler (e.g., gcc)
- Jupyter Notebook
- Python 3.x
- Libraries: matplotlib, numpy, scipy

## Notes
- Ensure the dataset files (`facebook_combined.txt`, `facebook_cpmbined.txt`) are present in the same directory as the code files.
- Results, visualizations, and conclusions are provided within the Jupyter notebooks for each analysis.
