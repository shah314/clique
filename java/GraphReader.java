
import java.util.*;
import java.io.*;

/**
 * Read the graph from Constants.FILE
 * See the following URL for instances:
 * http://www.nlsde.buaa.edu.cn/~kexu/benchmarks/graph-benchmarks.htm
 * 
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class GraphReader {
    
    /** Creates a new instance of GraphReader */
    public GraphReader() {
    }
 
    public static Graph readGraph() throws Exception
    {
        BufferedReader reader = new BufferedReader(new FileReader(new File(Constants.FILE)));
        String line = reader.readLine();
        StringTokenizer token = new StringTokenizer(line, " ");
        token.nextToken();
        token.nextToken();
        Constants.NUMBER_NODES = Integer.parseInt(token.nextToken().trim());
        Graph graph = new Graph();
        line = reader.readLine();
        while(line != null)
        {
            token = new StringTokenizer(line, " ");
            token.nextToken();
            int sv = Integer.parseInt(token.nextToken().trim());
            int ev = Integer.parseInt(token.nextToken().trim());
            sv--;
            ev--;
            graph.addEdge(sv, ev);
            line = reader.readLine();
        }
        
        return graph;
    }
}
