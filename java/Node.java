
import java.util.*;

/**
 * A node in the graph.
 *
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class Node implements Comparable
{    
    public int value;
    public int degree = 0;
    public LinkedHashSet list;
    
    /** Creates a new instance of Node */
    public Node(int v) 
    {
        value = v;
        list = new LinkedHashSet();
    }
    
    /* 
     * compareTo method of the interface Comparable 
     */
    public int compareTo(Object o)
    {
        Node obj = (Node)o;
        
        if(this.value == obj.value)
        {
            return 0;
        }
        
        if(this.degree < obj.degree)
            return 1;
        else if(this.degree > obj.degree)
            return -1;
        else
            return 0;
    }
    
    /* 
     * Is the passed in object equal to this object? 
     */
    public boolean equals(Object obj)
    {
        Node o = (Node)obj;
        if(o.value == this.value)
            return true;
        
        return false;
    }
    
    /* Add a sibling */
    public void addEdge(int ev)
    {
        list.add(new Integer(ev));
    }
    
    /* 
     * Canonical String representation of this Node
     */
    public String toString()
    {
        return "Value=" + value + " Degree=" + degree;
    }
    
    /* Hash Code */
    public int hashCode()
    {
        StringBuffer buffer = new StringBuffer("");
        buffer.append(value);
        buffer.append(" ");
        buffer.append(degree);
        return new String(buffer.toString()).hashCode();
    }
}
