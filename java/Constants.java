
/**
 * Constants.
 *
 * @author Shalin Shah
 * Email: shah.shalin@gmail.com
 */
public class Constants {
    
    public static int NUMBER_NODES; // populated by GraphReader.readGraph()
    
    /* 
     * The file from which the graph has to be read.
     * The format is specified by the following URL:
     * http://www.nlsde.buaa.edu.cn/~kexu/benchmarks/graph-benchmarks.htm
     */
    public static final String FILE = "C:\\Documents and Settings\\Shah\\Desktop\\instances\\data.CLQ";
    
    /* The number of iterations */
    public static final int ANNEALING_ITERATIONS = 5000;
    
    /* Random Restart Parameters */
    public static final int TOLERANCE = 1000;
}
