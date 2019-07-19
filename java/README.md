
<h2>Randomized Heuristic for the Maximum Clique Problem</h2>

Author: Shalin Shah

A simple random search algorithm for the maximum clique problem. A clique of a graph is a set of vertices in which each pair in the set have an edge between them i.e. it is a complete subgraph. A clique of maximum  size is called the maximum clique. Finding the maximum clique of a graph is an NP-complete problem, and it it not possible to approximate the problem within a constant factor of the optimal.

This algorithm performs the following steps:

	1) Create an initial clique using a greedy algorithm based on non-increasing degrees of the nodes
	and call it gBest
	2) Randomly remove two vertics from the clique created in step one.
	3) Add vertices to the incomplete clique returned by step two in order of non-increasing degrees.
	4) If the complete clique formed in step 3 is better than gBest, gBest = (3).
	5) Continue from step 2 till some termination criteria (Number of Iterations)

Instances are available at: http://www.nlsde.buaa.edu.cn/~kexu/benchmarks/graph-benchmarks.htm

Usage:
	- Change Constants.FILE to the file on your disk that represents the graph
	- Change Constants.ANNEALING_ITERATIONS to the number of iterations the algorithm should run for
