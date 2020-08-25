import java.io.*;
import java.util.*;

public class TestClique {

	public static void main(String [] args) throws Exception
	{
		String graphfile = "/Users/shah/Documents/git/clique/instances/brock200_1.clq";
		String clique = "104 32 180 103 144 26 122 4 199 138 132 48 83 46 107 120 175 100 137 41 191";
		BufferedReader graphsb = new BufferedReader(new FileReader(new File(graphfile)));
		String line = graphsb.readLine().trim().toLowerCase();
		Map<String, Set<String>> edges = new HashMap<String, Set<String>>();
		while(line != null)
		{
			if(line.charAt(0) == 'e')
			{
				String [] split = line.split(" ");
				if(edges.containsKey(split[1]))
				{
					edges.get(split[1].trim()).add(split[2].trim());
				}
				else
				{
					Set<String> set = new HashSet<String>();
					set.add(split[2].trim());
					edges.put(split[1].trim(), set);
				}

				if(edges.containsKey(split[2]))
				{
					edges.get(split[2].trim()).add(split[1].trim());
				}
				else
				{
					Set<String> set = new HashSet<String>();
					set.add(split[1].trim());
					edges.put(split[2].trim(), set);
				}
			}

			line = graphsb.readLine();
			if(line != null)
			{
				line = line.trim().toLowerCase();
			}
		}

		graphsb.close();

		String [] split = clique.split(" ");
		for(int i=0; i<split.length; i++)
		{
			String begin = split[i];
			for(int j=i+1; j<split.length; j++)
			{
				String end = split[j];
				if(edges.get(begin).contains(end) || edges.get(end).contains(begin))
				{
					//System.out.println("Match!");
				}
				else
				{
					System.out.println("No Match!");
					System.out.println(begin + ":" + end);
				}
			}
		}
	}
}
