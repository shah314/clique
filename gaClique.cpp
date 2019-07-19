/*
Genetic Algorithm - Maximum Clique Problem
------------------------------------------

Author: Shalin Shah
Email: shalin@alumni.usc.edu

For an explanation of the code see http://shah.freeshell.org/clique/.

** Requirements **
	The program requires a data.CLQ file in the current directory.
   Change the file name in processData() function.

*/

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
//#include <stdlib.h>
//#include <conio.h>
#include <map>

using namespace std;

int NUMBER_NODES;     // populated automatically by processData
int NUMBER_EDGES;		 // populated automatically by processData
const int POPULATION = 10; // size of the population
const int LOCAL_IMPROVEMENT = 10; // number of local improvements
const int GENERATIONS = 100; // number of generations to run the algorithm
const int MUTATIONS = 10; 	// How many vertices to remove randomly in the mutate() function
const int UNIQUE_ITERATIONS = 100;	// Used by localImprovement() to prevent a stall for very small cliques
const int SHUFFLE_TOLERANCE = 500; // Generate a fresh population after a stall

/* A Node in the graph */
class Node
{
	public:
   	int value;
      int degree;
      vector<int> edges;

   public:
   	Node()
      {
      	value = 0;
         degree = 0;
      }

      Node(int v)
      {
      	value = v;
         degree = 0;
      }

      void addEdge(int v)
      {
      	edges.push_back(v);
      }
};

/* Used for sorting nodes according to degrees in the induced subgraph of possible additions (PA) */
class SortedListNode
{
   public:
		int node;
   	int reach;

   	SortedListNode()
      {
      	node = -1;
         reach = 0;
      }
};

/* A functor to sort nodes in non-increasing order of degrees */
struct DescendingNode
{
	public:
	bool operator()(Node *& node1, Node *& node2)
   {
   	return node1->degree > node2->degree;
   }
};

/* A functor to sort nodes in non-increasing order of degrees
in the induced subgraph of possible additions (PA) */
struct DescendingSortedListNode
{
	public:
	bool operator()(SortedListNode & node1, SortedListNode & node2)
   {
   	return node1.reach > node2.reach;
   }
};

/* The graph read from data.CLQ. A global instance of this class is populated
by the processData() function */
class Graph
{
	public:
  		Node ** nodes;
      vector<Node*> sortedNodes;
      int ** aMatrix;

   public:
   	Graph()
      {
      	aMatrix = new int*[NUMBER_NODES];
      	for(int i=0; i<NUMBER_NODES;i++)
         {
         	aMatrix[i] = new int[NUMBER_NODES];
          	for(int j=0; j<NUMBER_NODES; j++)
            {
            	aMatrix[i][j] = 0;
            }
         }
			nodes = new Node*[NUMBER_NODES];

         for(int i=0; i<NUMBER_NODES; i++)
         {
         	nodes[i] = NULL;
         }
      }

      ~Graph()
      {
			for(int i=0; i<NUMBER_NODES; i++)
         {
         	delete nodes[i];
            delete aMatrix[i];
         }

         delete [] nodes;
         delete [] aMatrix;

         // no need to delete the contents of sortedNodes as they have already been deleted
         // by the above loop 
      }

   	void addEdge(int sv, int ev)
      {
      	aMatrix[sv][ev] = 1;
         aMatrix[ev][sv] = 1;

         Node * node = nodes[sv];
         if(node == NULL)
         {
             node = new Node(sv);
             nodes[sv] = node;
             node->addEdge(ev);
             sortedNodes.push_back(node);
             node->degree++;
         }
         else
         {
         	node->addEdge(ev);
            node->degree++;
         }

         node = nodes[ev];
         if(node == NULL)
         {
         	node = new Node(ev);
            nodes[ev] = node;
            node->addEdge(sv);
            sortedNodes.push_back(node);
            node->degree++;
         }
         else
         {
         	node->addEdge(sv);
            node->degree++;
         }
      }

      void sortList()
      {
      	sort(sortedNodes.begin(), sortedNodes.end(), DescendingNode());
      }
};

/* A global instance of the Graph class used througout the code */
Graph * GRAPH = NULL;

/* Abstraction of a clique - a solution to the problem */
class Clique
{
	public:
   	vector<int> clique;
      vector<int> pa;
      map<int, bool, less<int> > mapPA;
      map<int, bool, less<int> > mapClique;

      Clique()
      {
      }

   	Clique(int firstVertex)
      {
      	clique.reserve(NUMBER_NODES);
         pa.reserve(NUMBER_NODES);
         
  			clique.push_back(firstVertex);
         mapClique.insert(map<int, bool, less<int> >::value_type(firstVertex, true));

         for(int i=0; i<NUMBER_NODES; i++)
         {
         	if(i == firstVertex)
            {
            	continue;
            }
            else
            {
               if(GRAPH->aMatrix[i][firstVertex] == 1)
               {
                	pa.push_back(i);
                  mapPA.insert(map<int, bool, less<int> >::value_type(i, true));
               }
            }
         }
      }

      void addVertex(int vertex)
      {
      	if(containsInClique(vertex))
         	return;

			clique.push_back(vertex);
         mapClique.insert(map<int, bool, less<int> >::value_type(vertex, true));

         eraseFromPA(vertex);
         vector<int> erasedNodes;
         for(int i=0; i<pa.size(); i++)
         {
         	int pavertex = pa.at(i);
            if(GRAPH->aMatrix[pavertex][vertex] == 0)
            {
            	erasedNodes.push_back(pavertex);
            }
         }

         for(int i=0; i<erasedNodes.size(); i++)
         {
         	eraseFromPA(erasedNodes.at(i));
         }
      }

      void removeVertex(int vertex)
      {
          if(!containsInClique(vertex))
          {
           	return;
          }

          eraseFromClique(vertex);
          for(int i=0; i<NUMBER_NODES; i++)
          {
          	if(containsInClique(i))
            {
            	continue;
            }
            else
            {
            	bool flag = true;

            	for(int n=0; n<clique.size(); n++)
               {
               	int ver = clique.at(n);
                  if(GRAPH->aMatrix[i][ver] == 0)
                  {
               		flag = false;
                     break;
                  }
               }

               if(flag)
               {
               	if(!containsInPA(i))
                  {
                  	pa.push_back(i);
                     mapPA.insert(map<int, bool, less<int> >::value_type(i, true));
                  }
               }
            }
          }
      }

      void eraseFromPA(int vertex)
      {
         mapPA.erase(vertex);
      	bool flag = false;
      	std::vector<int>::iterator it;
         for(it = pa.begin(); it != pa.end(); it++)
         {
         	int ver = *it;
            if(ver == vertex)
            {
            	flag = true;
            	break;
            }
         }

         if(flag)
         	pa.erase(it);
      }

      bool containsInPA(int vertex)
      {
			map<int, bool, less<int> >::iterator it = mapPA.find(vertex);
         if(it == mapPA.end())
         {
         	return false;
         }

         pair<int, bool> p = *it;

         if(p.second)
         	return true;

      	return false;
	}

      void eraseFromClique(int vertex)
      {
      	mapClique.erase(vertex);
      	bool flag = false;
      	std::vector<int>::iterator it;
         for(it = clique.begin(); it != clique.end(); it++)
         {
         	int ver = *it;
            if(ver == vertex)
            {
            	flag = true;
            	break;
            }
         }
         
			if(flag)
         	clique.erase(it);
      }

      bool containsInClique(int vertex)
      {
      	map<int, bool, less<int> >::iterator it = mapClique.find(vertex);
         if(it == mapClique.end())
         {
         	return false;
         }

         pair<int, bool> p = *it;

         if(p.second)
         	return true;
	
	return false;	
	}

      vector<SortedListNode> computeSortedList()
      {
      	vector<SortedListNode> sortedList;

  			for(int i=0; i<pa.size(); i++)
         {
         	int node1 = pa.at(i);
            int reach = 0;
         	for(int j=0; j<pa.size(); j++)
            {
            	if(i == j)
               	continue;
               
            	int node2 = pa.at(j);
               if(GRAPH->aMatrix[node1][node2] == 1)
               {
               	reach++;
               }
            }

            SortedListNode n;
            n.reach = reach;
            n.node = node1;
            sortedList.push_back(n);
         }

         sort(sortedList.begin(), sortedList.end(), DescendingSortedListNode());

         return sortedList;
      }

      Clique clone()
      {
      	vector<int> cpa;
         vector<int> cclique;
         for(int i=0; i<pa.size(); i++)
         {
         	cpa.push_back(pa.at(i));
         }

         for(int i=0; i<clique.size(); i++)
         {
         	cclique.push_back(clique.at(i));
         }

         map<int, bool, less<int> > cMapPA;
         map<int, bool, less<int> >::iterator it = mapPA.begin();
         while(it != mapPA.end())
         {
         	pair<int, bool> p = *it;
            int i = p.first;
            bool j = p.second;
            cMapPA.insert(map<int, bool, less<int> >::value_type(i, j));
            it++;
         }

         map<int, bool, less<int> > cMapClique;
         map<int, bool, less<int> >::iterator itt = mapClique.begin();
         while(itt != mapClique.end())
         {
         	pair<int, bool> p = *itt;
            int i = p.first;
            bool j = p.second;
            cMapClique.insert(map<int, bool, less<int> >::value_type(i, j));
            itt++;
         }

         Clique clone;
			clone.clique = cclique;
         clone.pa = cpa;
         clone.mapPA = cMapPA;
         clone.mapClique = cMapClique;
         return clone;
      }

};

/* A functor used to sort individuals of the population in non-increasing order
of the size of cliques */
struct DescendingClique
{
	public:
	bool operator()(Clique &c1, Clique &c2)
   {
   	return c1.clique.size() > c2.clique.size();
   }
};

/* Generate a random number in [min, max] */
inline int generateRandomNumber(int min, int max)
{
	int r;
	double randd = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
	r = (int) (randd * (double)(max-min+1)) + min;
	return r;
}

/* Generate a random number in [0, 1) */
double generateDoubleRandomNumber()
{
	return ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
}

/* Process the data from the data.CLQ file */
void processData()
{
	FILE * file;
	file = fopen("data.CLQ", "r");
   if(file == NULL)
   {
   	printf("data.CLQ File Not Found in Current Directory.");
      exit(1);
   }

	char * line = new char [1000];
	fgets(line, 1000, file);
   while(line[0] == 'c')
   {
   	fgets(line, 1000, file);
   }

   char * tok = NULL;
   if(line[0] == 'p')
   {
		tok = strtok(line, " ");
   	tok = strtok(NULL, " ");
   	tok = strtok(NULL, " ");
		NUMBER_NODES = atoi(tok);
		tok = strtok(NULL, " ");
		NUMBER_EDGES = atoi(tok);
   }
   else
   {
   	printf("\n\nInput File Format Not Understood.\n");
      exit(0);
   }

	//printf("%d\n", NUMBER_NODES);
	//printf("%d\n", NUMBER_EDGES);
	int i = 0;

   GRAPH = new Graph();

   for(i=0; i<NUMBER_EDGES; i++)
   {
   	//printf("\n%d",i);

   	fgets(line, 1000, file);
      if(line[0] != 'e' && line[0] != 'E')
      {
      	printf("Input File Format Not Understood.\n");
         exit(0);
      }
      tok = strtok(line, " ");
      tok = strtok(NULL, " ");
      int sv = atoi(tok);
		sv--;
      tok = strtok(NULL, " ");
      int ev = atoi(tok);
      ev--;
      GRAPH->addEdge(sv, ev);
   }

	delete [] line;
}

/* Generate the population for the genetic algorithm randomly */
vector<Clique> generateRandomPopulation()
{
	printf("\nGenerating Population...");
	vector<Clique> population;
   population.reserve(POPULATION);
   bool * flags = new bool[NUMBER_NODES];
   for(int i=0; i<NUMBER_NODES; i++)
   {
   	flags[i] = false;
   }
	for(int i=0; i<POPULATION-1; i++)
   {
      int rand = generateRandomNumber(0, NUMBER_NODES-1);
      int cntt = 0;
      while(flags[rand])
      {
      	cntt++;
         if(cntt > NUMBER_NODES)
         {
         	break;
         }
      	rand = generateRandomNumber(0, NUMBER_NODES-1);
      }
      flags[rand] = true;

      Clique clique(rand);
      vector<SortedListNode> sortedList = clique.computeSortedList();
      int cnt = 0;
      while(clique.pa.size() > 0)
      {
      	int node = sortedList.at(cnt).node;
         cnt++;
         if(clique.containsInPA(node))
         {
         	clique.addVertex(node);
         }
      }

      population.push_back(clique);
   }

   int node = GRAPH->sortedNodes.at(0)->value;
   Clique clique(node);
   vector<SortedListNode> sortedList = clique.computeSortedList();
   int count = 0;
   while(clique.pa.size() > 0)
   {
   	node = sortedList.at(count).node;
      count++;
      if(clique.containsInPA(node))
      {
      	clique.addVertex(node);
      }
   }

   population.push_back(clique);

   delete [] flags;
   
   return population;
}

/* Crossover - Creates an optimized child from the two parents.
	1. It takes all vertices present in the two parents and creates an induced subgraph.
   2. Then it calculates the degrees of these vertices in the same induced subgraph and orders
   then in non-increasing order.
   3. Then, it adds vertices to the clique in the order given by step 2.
   4. In the end, of the possible additions is non-empty, it adds the remaining vertices to the clique
   in non-increasing order of degrees in the induced subgraph of possible additions (PA).
*/
inline Clique greedyCrossover(Clique &c1, Clique &c2)
{
	vector<int> vec;
	bool * flags = new bool[NUMBER_NODES];
   for(int i=0; i<NUMBER_NODES; i++)
   {
   	flags[i] = false;
   }

   /* Take all vertices from c1 and add them to the vector */
   for(int i=0; i<c1.clique.size(); i++)
   {
      int vertex = c1.clique.at(i);
   	if(!flags[vertex])
      {
      	vec.push_back(vertex);
         flags[vertex] = true;
      }
   }

   /* Take all vertices from c2 and add them to the vector */
   for(int i=0; i<c2.clique.size();i++)
   {
   	int vertex = c2.clique.at(i);
      if(!flags[vertex])
      {
      	vec.push_back(vertex);
         flags[vertex] = true;
      }
   }

   /* Create a vector of vertices from c1 and c2 and sort them in non-increasing
    order of degrees in the induced subgraph of this created vector */
   vector<SortedListNode> sortedList;
   for(int i=0; i<vec.size(); i++)
   {
   	int node1 = vec.at(i);
      int reach = 0;
      for(int j=0; j<vec.size(); j++)
      {
      	if(i == j)
         	continue;

         int node2 = vec.at(j);
         if(GRAPH->aMatrix[node1][node2] == 1)
         {
         	reach++;
         }
      }

      SortedListNode sNode;
      sNode.reach = reach;
      sNode.node = node1;
      sortedList.push_back(sNode);
   }

	sort(sortedList.begin(), sortedList.end(), DescendingSortedListNode());
   int firstVertex = sortedList.at(0).node;
   Clique clique(firstVertex);
   int count = 1;
   while(count < sortedList.size())
   {
   	int node = sortedList.at(count).node;
      if(clique.containsInPA(node))
      {
      	clique.addVertex(node);
      }
      count++;
   }

   while(clique.pa.size() > 0)
   {
   	int node = clique.pa.at(0);
      clique.addVertex(node);
   }

   return clique;
}

/* Take the common nodes from the two cliques and construct
one offspring sequentially */
Clique intersectionCrossover(Clique &c1, Clique&c2)
{
	vector<int> intersect;
   intersect.reserve(NUMBER_NODES);
   bool *flags = new bool[NUMBER_NODES];
   for(int i=0; i<NUMBER_NODES; i++)
   {
    	flags[i] = false;
   }

   for(int i=0; i<c2.clique.size(); i++)
   {
   	int vertex = c2.clique.at(i);
      flags[vertex] = true;
   }

	for(int i=0; i<c1.clique.size(); i++)
   {
   	int ver1 = c1.clique.at(i);
      if(flags[ver1])
      {
      	intersect.push_back(ver1);
      }
   }

   if(intersect.size() == 0)
   {
   	delete [] flags;
   	return greedyCrossover(c1, c2);
   }

   int vertex = intersect.at(0);
   Clique clique(vertex);
   for(int i=1; i<intersect.size(); i++)
   {
   	vertex = intersect.at(i);
      if(clique.containsInPA(vertex))
      {
      	clique.addVertex(vertex);
      }
   }

   if(clique.pa.size() > 0)
   {
    	vector<SortedListNode> sortedList = clique.computeSortedList();
      int cnt = 0;
      while(clique.pa.size() > 0)
      {
      	int node = sortedList.at(cnt).node;
         cnt++;
         if(clique.containsInPA(node))
         {
         	clique.addVertex(node);
         }
      }
   }

   delete [] flags;
   return clique;
}



/* Randomly select parents for crossover and mutation */
inline vector<Clique> randomSelection(vector<Clique> &population)
{
	vector<Clique> parents;
   parents.reserve(2);
   int rand1 = generateRandomNumber(0, POPULATION-1);
	int rand2 = generateRandomNumber(0, POPULATION-1);
   while(rand1 == rand2)
   {
   	rand1 = generateRandomNumber(0, POPULATION-1);
		rand2 = generateRandomNumber(0, POPULATION-1);
   }

   Clique p1 = population.at(rand1);
   Clique p2 = population.at(rand2);
   parents.push_back(p1);
   parents.push_back(p2);

   return parents;
}

/* Improve the passed in clique locally */
inline void localImprovement(Clique &clique)
{
	Clique gBest = clique.clone();
	for(int i=0; i<LOCAL_IMPROVEMENT; i++)
   {
   	int rand1 = generateRandomNumber(0, clique.clique.size()-1);
      int rand2 = generateRandomNumber(0, clique.clique.size()-1);
      int countt = 0;
      while(rand1 == rand2)
      {
      	countt++;
         if(countt > UNIQUE_ITERATIONS)
         {
         	/* Prevent Stall for very small cliques */
         	break;
         }
      	rand1 = generateRandomNumber(0, clique.clique.size()-1);
      	rand2 = generateRandomNumber(0, clique.clique.size()-1);
      }

      int vertex1 = clique.clique.at(rand1);
      int vertex2 = clique.clique.at(rand2);
      clique.removeVertex(vertex1);
      clique.removeVertex(vertex2);
      vector<SortedListNode> sortedList = clique.computeSortedList();
      int count = 0;
      while(clique.pa.size() > 0)
      {
      	int node = sortedList.at(count).node;
         count++;
         if(node >= NUMBER_NODES)
         {

         	printf("**Node greater: %d**", node);
            exit(0);

         }
         if(clique.containsInPA(node))
         {
         	clique.addVertex(node);
         }
      }

      if(gBest.clique.size() < clique.clique.size())
      {
       	gBest = clique.clone();
      }
   }

   clique = gBest;
}

/* Mutation */
inline void mutate(Clique &clique)
{
	bool * flags = new bool[NUMBER_NODES];
   for(int i=0; i<NUMBER_NODES; i++)
   {
   	flags[i] = false;
   }

   /* Remove vertices from the clique */
	for(int i=0; i<MUTATIONS; i++)
   {
		int rand = generateRandomNumber(0, clique.clique.size()-1);
      int count = 0;
      while(flags[rand])
      {
			rand = generateRandomNumber(0, clique.clique.size()-1);
			count++;
         if(count > UNIQUE_ITERATIONS)
         {
         	break;
         }
      }

      flags[rand] = true;
      int vertex = clique.clique.at(rand);
      clique.removeVertex(vertex);
   }

   double rand = generateDoubleRandomNumber();
   if(rand < 0.5)
   {
   	vector<SortedListNode> sortedList = clique.computeSortedList();
      int cnt = 0;
      while(clique.pa.size() > 0)
      {
      	int node = sortedList.at(cnt).node;
         cnt++;
         if(clique.containsInPA(node))
         {
         	clique.addVertex(node);
         }
      }
   }
   else
   {
	   /* Add vertices to the incomplete clique randomly */
	   while(clique.pa.size() > 0)
   	{
   		int rand = generateRandomNumber(0, clique.pa.size()-1);
	      int vertex = clique.pa.at(rand);
   	   clique.addVertex(vertex);
	   }
   }

   delete [] flags;
}

/* Main */
int main (void)
{
	srand(time(NULL));
   processData();
   GRAPH->sortList();
   vector<Clique> population;
   population.reserve(POPULATION);
	population = generateRandomPopulation();
   sort(population.begin(), population.end(), DescendingClique());
   /*for(int i=0; i<population.size(); i++)
   {
   	Clique clique = population.at(i);
      printf("\n%d", clique.clique.size());
	} */

   //exit(0);
   Clique gBest = population.at(0).clone();
   int prevBest = gBest.clique.size();
   int cnt = 0;
   for(int n=0; n<GENERATIONS; n++)
   {
   	if(prevBest == gBest.clique.size())
      {
      	cnt++;
         if(cnt > SHUFFLE_TOLERANCE)
         {
         	vector<Clique> pop = generateRandomPopulation();
            population.swap(pop);
            pop.erase(pop.begin(), pop.end());
            srand(time(NULL));
            cnt = 0;
         }
      }
      else
      {
      	prevBest = gBest.clique.size();
         cnt = 0;
      }
   	vector<Clique> newPopulation;
      newPopulation.reserve(POPULATION);
      sort(population.begin(), population.end(), DescendingClique());

      /* Elitism */
      Clique localBest = population.at(0);
		if(gBest.clique.size() < localBest.clique.size())
      {
      	gBest = localBest.clone();
      }
      
      localImprovement(gBest);
      newPopulation.push_back(gBest);
      printf("\n%d:%lu",n, gBest.clique.size());
      /*for(int i=0; i<POPULATION; i++)
      {
      	printf("%d ", population.at(i).clique.size());
      }
      getch();*/
      
      for(int i=0; i<POPULATION-1; i++)
      {
         vector<Clique> parents = randomSelection(population);
         Clique offspring = intersectionCrossover(parents.at(0), parents.at(1));
         localImprovement(offspring);
         if(offspring.clique.size() <= parents.at(0).clique.size() || offspring.clique.size() <= parents.at(1).clique.size())
         {
         	mutate(offspring);
         }
         newPopulation.push_back(offspring);
      }

      population.swap(newPopulation);
      newPopulation.erase(newPopulation.begin(), newPopulation.end());
   }

   printf("\nVertices in the Clique:\n");
   for(int i=0; i<gBest.clique.size(); i++)
   {
   	printf("%d ", gBest.clique.at(i)+1);
   }

   delete GRAPH;
	return 0;
}
