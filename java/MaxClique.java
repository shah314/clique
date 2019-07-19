
import java.util.*;

/**
 * Randomized Heuristic for the Maximum Clique Problem
 *
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class MaxClique 
{    
    public static void main(String [] args) throws Exception
    {
        /* The global best clique found so far */
        LinkedHashSet gBest = new LinkedHashSet();
        
        System.out.println("Reading Graph...");
        /* Read the graph from Constants.FILE */
        Graph graph = GraphReader.readGraph();
        System.out.println("Computing Clique...");

        long start = System.currentTimeMillis();
        
        /* Construct an initial Clique only based on the degrees of the nodes */
        graph.sortList();
        Node [] nodes = graph.getSortedNodes();
        Clique clique = new Clique(nodes[0].value, graph);
        List sList = clique.computeSortedList();
        Iterator ittt = sList.iterator();
        while(clique.pa.size() > 0)
        {
            SortedListNode node = (SortedListNode)ittt.next();
            if(clique.pa.contains(node.node))
            {
                clique.addVertex(node.node, graph.aMatrix);
            }
        }
        
        //System.out.println(clique.clique.size());
        gBest.addAll(clique.clique);
        int prevBest = clique.clique.size();
        int count = 0;
        int [] restarts = new int[Constants.NUMBER_NODES];
        for(int i=0; i<Constants.ANNEALING_ITERATIONS; i++)
        {
            //System.out.println(gBest.size());  
            
            if(prevBest == gBest.size())
            {
                count++;
                if(count > Constants.TOLERANCE)
                {
                    randomRestart(clique, graph, restarts);
                    count = 0;
                }
            }
            else
            {
                prevBest = gBest.size();
                count = 0;
            }
            
            /* Choose two vertices randomly and remove from the clique */
            int rand1 = (int)(Math.random() * (double)clique.clique.size());
            int vertex1 = ((Integer)clique.cliqueList.get(rand1)).intValue();
            int rand2 = (int)(Math.random() * (double)clique.clique.size());
            int vertex2 = ((Integer)clique.cliqueList.get(rand2)).intValue();
            while(rand1 == rand2)
            {
                rand2 = (int)(Math.random() * (double)clique.clique.size());
                vertex2 = ((Integer)clique.cliqueList.get(rand2)).intValue();
            }
            clique.removeVertex(vertex1, graph.aMatrix);
            clique.removeVertex(vertex2, graph.aMatrix);
            
            /* Add vertices to the clique based on the sorted possible additions TreeSet */
            if(clique.pa.size() > 0)
            {
                List sortedList = clique.computeSortedList();
                Iterator itt = sortedList.iterator();
                while(clique.pa.size() > 0)
                {
                    SortedListNode node = (SortedListNode)itt.next();
                    if(clique.pa.contains(new Integer(node.node)))
                    {
                        clique.addVertex(node.node, graph.aMatrix);
                    }
                }
            }
            else
            {
                System.out.println("Should Not Come Here!");
                System.exit(1);
            }
            
            //System.out.println("PA: " + clique.pa.size());
            
            /* If the new clique is better than the current global best, 
             * replace the global best with this clique
             */
            if(clique.clique.size() > gBest.size())
            {
                gBest = new LinkedHashSet();
                gBest.addAll(clique.clique);
            }
        }
        
        /* 
         * Finally, try to improve upon the solution 
         * considering all vertices one by one using 1-OPT moves
         */
        int maxClique = gBest.size();
        clique.clique = gBest;
        clique.cliqueList = new ArrayList();
        clique.cliqueList.addAll(clique.clique);
        //System.out.println("Randomized Algorithm Clique Size: " + maxClique);
        while(true)
        {
            boolean flag = false;
            for(int i=0; i<clique.clique.size(); i++)
            {
                int vertex = ((Integer)clique.cliqueList.get(i)).intValue();
                clique.removeVertex(vertex, graph.aMatrix);
                List sortedList = clique.computeSortedList();
                Iterator it = sortedList.iterator();
                while(clique.pa.size() > 0)
                {
                    // it should always be valid
                    SortedListNode node = (SortedListNode)it.next();
                    if(clique.pa.contains(new Integer(node.node)))
                    {
                        clique.addVertex(node.node, graph.aMatrix);
                    }
                }
                
                if(clique.clique.size() > maxClique)
                {
                    maxClique = clique.clique.size();
                    flag = true;
                    break;
                }
            }
            
            if(!flag)
            {
                break;
            }
        }
        
        gBest = new LinkedHashSet();
        gBest.addAll(clique.clique);
        
        System.out.println("Maximum Clique Size Found: " + gBest.size());
        System.out.println("Vertices in the Clique:");
        for(Iterator it = gBest.iterator(); it.hasNext();)
        {
            int vertex = ((Integer)it.next()).intValue();
            System.out.print(vertex + " ");
        }
        
        long end = System.currentTimeMillis();
        System.out.println("\n\n" + (end-start) + " milliseconds (excluding I/O).");
    }
    
    /* 
     * Random Restart the current search from a new random starting vertex 
     */
    public static void randomRestart(Clique clique, Graph graph, int [] restarts)
    {
        System.out.println("Random Restarting...");
        int rand = (int)(Math.random() * (double)Constants.NUMBER_NODES);
        while(restarts[rand] == 1)
        {
            rand = (int)(Math.random() * (double)Constants.NUMBER_NODES);
        }
        restarts[rand] = 1;
        
        Clique clq = new Clique(rand, graph);
        List sortedList = clq.computeSortedList();
        Iterator it = sortedList.iterator();
        while(clq.pa.size() > 0)
        {
            SortedListNode node  =(SortedListNode)it.next();
            if(clq.pa.contains(new Integer(node.node)))
            {
                clq.addVertex(node.node, graph.aMatrix);
            }
        }
        
        clique.clique = clq.clique;
        clique.cliqueList = clq.cliqueList;
        clique.pa = clq.pa;
        clique.sortedPA = clq.sortedPA;
    }
}
