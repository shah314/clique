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

Keywords:maximum clique, randomized algorithm, genetic algorithm, approximation algorithm, k-opt moves, local search, open source, C++, intersection crossover

# Introduction

# Results

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

The method uses the following steps:


The problem instances are available here:
<http://iridia.ulb.ac.be/~fmascia/maximum_clique/DIMACS-benchmark>.

# References
