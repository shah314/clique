---
title: 'GCLIQUE: A Genetic Algorithm for the Maximum Clique Problem in C++'
tags:
  - C++
  - maximum clique
  - genetic algorithm
  - solver
  - heuristic
  - approximation algorithm
  - open source
authors:
    - name: Shalin Shah
      orcid: 0000-0002-3770-1391
      affiliation: 1
affiliations:
   - name: Johns Hopkins University
     index: 1
date: 9 May 2020
bibliography: paper.bib
---

![Clique.](clique.jpg)

# Abstract
A clique in a graph is a set of vertices that are all connected to each other. A maximum clique is a clique of maximum size. A graph may have more than one maximum cliques. The problem of finding a maximum clique is a strongly hard NP-hard problem. It is not possible to find an approximation algorithm which finds a maximum clique that is a constant factor of the optimum solution. In this work, we present a genetic algorithm for the maximum clique problem that is able to find optimum or close to optimum solutions to most DIMACS graphs. The genetic algorithm uses new crossover mechanisms which are able to find reasonably good cliques which can then be used in other applications downstream. We also provide C++ code for our algorithm. Results show that our algorithm is able to find maximum cliques for most DIMACS instances, and if not, close to optimum solutions for the other instances.

Keywords: maximum clique, randomized algorithm, genetic algorithm, approximation algorithm, k-opt moves, local search, open source, C++, intersection crossover

# Methods

This work implements a genetic algorithm for the maximum clique problem. The maximum clique problem has been well studied in the past. [@pardalos1994maximum] and [@bomze1999maximum] are surveys on the problem which describe the various algorithms and provable approximations on the problem. This work [@OSTERGARD2002197] presents a fast algorithm for the maximum clique problem. A survey on maximum clique algorithms can be found in [@WU2015693].

This genetic algorithm uses new types of crossover mechanisms to find reasonably good solutions. The intersection crossover that generates a new offspring based on an intersection of the two clique solutions in the two parents is inspired by [@dharwadker2006clique].

The genetic algorithm uses a population of individuals (solutions) and performs crossover and mutation on them. It initializes the population using random solutions. If in consecutive 10 generations, there is no improvement, it does a random restart of the population. In each generation, the algorithm keeps a cache of the global best solution, and propagates the global best individual to the next generation.

For the entire population, the algorithm performs the intersection crossover and mutation. After each step, it performs 2-opt local improvement. Mutation happens only if the fitness (clique size) of the offspring is less than or equal to at least one of the parent solutions.

# Results

In most of the DIMACS instances, the algorithm is able to find the optimum or close to optimum clique (except the san1000 instance). The results are shown in the following table. The git repository can be found at \url{https://github.com/shah314/clique}.

|DataSet|Vertices|Edges|Best Known|This Algorithm|Duration (Seconds)
|--- |--- |--- |--- |--- |--- |
|brock200_1|200|14834|21|21|9
|C125.9|125|6963|34|34|0
|c-fat200-1|200|1534|12|12|0
|DSJC500.5|500|125248|13|13|3
|gen200_p0.9_44|200|17910|44|40|5
|hamming6-2|64|1824|32|32|0
|johnson32-2-4|496|107880|16|16|0
|keller4|171|9435|11|11|0
|MANN_a27|378|70551|126|125|1
|p_hat300-1|300|70551|8|8|0
|san200_0.7_1|200|13930|30|30|0
|san1000|1000|250500|15|10|1

The DIMACS problem instances are available at [@dimacsinstances].

If a faster and simple solution in Java is desired, the work in \url{https://github.com/shah314/clique2} could be useful. 

# References
