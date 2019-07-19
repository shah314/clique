
import java.util.*;

/**
 * Abstraction of a Clique - A solution to the clique problem.
 *
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class Clique 
{    
    /* Possible additions */
    public LinkedHashSet pa;
    
    /* The vertices that make up the clique */
    public LinkedHashSet clique;
    
    /* A TreeMap of possible additions sorted according to their degrees */
    public TreeMap sortedPA;
    
    /* The graph on which the algorithm runs */
    public Graph graph;
    
    /* A random access list of the vertices that make up the clique */
    public List cliqueList;
    
    /** Creates a new instance of Clique */
    public Clique(int firstVertex, Graph graph) 
    {
        int [][] aMatrix = graph.aMatrix;
        pa = new LinkedHashSet();
        clique = new LinkedHashSet();
        sortedPA = new TreeMap();
        cliqueList = new ArrayList();
        
        clique.add(new Integer(firstVertex));
        cliqueList.add(new Integer(firstVertex));
        this.graph = graph;
        for(int i=0; i<Constants.NUMBER_NODES; i++)
        {
            if(i == firstVertex)
            {
                continue;
            }
            
            if(aMatrix[i][firstVertex] == 1)
            {
                pa.add(new Integer(i));
                sortedPA.put(graph.nodes[i], graph.nodes[i]);
            }
        }   
    }
    
    /* 
     * Add a vertex to the clique and update the possible additions list.
     */
    public void addVertex(int vertex, int [][] aMatrix)
    {
        clique.add(new Integer(vertex));
        cliqueList.add(new Integer(vertex));
        removeFromSortedPA(graph.nodes[vertex]);
        pa.remove(new Integer(vertex));
        for(Iterator it = pa.iterator(); it.hasNext();)
        {
            int pavertex = ((Integer)it.next()).intValue();
            if(aMatrix[pavertex][vertex] == 0)
            {
                it.remove();
                removeFromSortedPA(graph.nodes[pavertex]);
            }
            else if(aMatrix[pavertex][vertex] == 1)
            {
                // do nothing
            }
            else
            {
                System.out.println("Invalid Matrix in addVertex of Clique");
                System.exit(1);
            }
        }
    }
    
    /* 
     * Remove a vertex from the clique and update the possible additions list.
     */
    public void removeVertex(int vertex, int [][]aMatrix)
    {
        if(!clique.contains(new Integer(vertex)))
            return;
        
        clique.remove(new Integer(vertex));
        cliqueList.remove(new Integer(vertex));
        for(int i=0; i<Constants.NUMBER_NODES; i++)
        {
            if(clique.contains(new Integer(i)))
            {
                continue;
            }
            else
            {
                Iterator it = clique.iterator();
                boolean flag = true;
                while(it.hasNext())
                {
                    int ver = ((Integer)it.next()).intValue();
                    if(aMatrix[i][ver] == 0)
                    {
                        flag = false;
                        break;
                    }
                }
                
                if(flag)
                {
                    pa.add(new Integer(i));
                    sortedPA.put(graph.nodes[i], graph.nodes[i]);
                }
            }
        }
    }
   
    /* Linear time remove method for TreeMap */
    private void removeFromSortedPA(Node node)
    {
        Set set = sortedPA.keySet();
        Iterator it = set.iterator();
        while(it.hasNext())
        {
            if(it.next().equals(node))
            {
                it.remove();
                break;
            }
        }
    }
    
    /* 
     * Compute a list of nodes based on degrees in the induced subgraph of 
     * possible additions (PA)
     */
    public List computeSortedList()
    {
        List sortedList = new ArrayList();
        Iterator it = pa.iterator();
        while(it.hasNext())
        {
            int node1 = ((Integer)it.next()).intValue();
            int reach = 0;
            Iterator itt = pa.iterator();
            while(itt.hasNext())
            {
                int node2 = ((Integer)itt.next()).intValue();
                if(graph.aMatrix[node1][node2] == 1)
                {
                    reach++;
                }
            }
            
            SortedListNode n = new SortedListNode();
            n.reach = reach;
            n.node = node1;
            sortedList.add(n);
        }
        
        Collections.sort(sortedList);
        return sortedList;
    }
}
