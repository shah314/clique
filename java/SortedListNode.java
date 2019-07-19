
/* 
 * A node in the sorted list based on the degrees of the graph nodes
 * in the induced subgraph of possible additions (PA)
 *
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class SortedListNode implements Comparable 
{
    public int node = -1;
    public int reach = 0;
    
    public int compareTo(Object obj) 
    {
        SortedListNode n = (SortedListNode)obj;
        if(n.reach > this.reach) 
        {
            return 1;
        } 
        else if(n.reach < this.reach) 
        {
            return -1;
        } 
        else 
        {
            return 0;
        }
    }
}