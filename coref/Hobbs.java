package team3.a3.coref;


import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.ArrayList;
import gate.Annotation;
import java.util.stream.Collectors;
import java.io.FileReader;
import java.io.BufferedReader;

/**
 * @author Robin Lamberti
 *
 */
public class Hobbs {
	private Node pronoun;
	// assumption: 
	// forest contains pronoun's tree and a certain number of previous trees
	private List<Node> forest;

	// Labels for nominal heads
	private List<String> nominal_labels = {"NN", "NNS", "NNP", "NNPS", "PRP"};

	private Node X;
	private List<Node> p;
//	private int lookBack;
	
	
	public Hobbs(Node pronoun, List<Node> forest) {
		this.pronoun = pronoun;
		this.forest = forest;
	}

	public Node hobbsAlg() {
		
		// step 1 get immediately dominating nominal phrase of pronoun
		Node domNP = getDominatingNominalPhrase();
		
		// step 2 find X: first NP or S and path p
		getNextNominalPhrase(domNP);
		
		// step 3 	traverse all branches below X to the left of p, 
		// 			left-to-right breadth first
		Node potAntecedent = traverseLeftToRightCond();
		if(potAntecedent!= null) return potAntecedent ;
		
		// if antecedent is found return

//		return 
		
		// step 4 if X is the highest S in the sentence
		if(getType(X).equals("ROOT")){
			
			// iterate over previous sentences if no antecedent was found
			for(int i = forest.size()-2; i >=0; i--) {
				potAntecedent = traverseLeftToRightFindFirstNP();
				if(potAntecedent!= null) return potAntecedent ;
				
				// TODO step 4 needs to be implemented here
				
				// if X is not the highest node:
				// step 5  from X find first NP or S and path p
				getNextNominalPhrase(X);
				
				// step 6
				// If X is an NP and the path p to X came from a non-head 
				// phrase of X (a specifier or adjunct, such as a possessive,
				// PP, apposition, or relative clause), propose X as antecedent
				if(getType(X).equals("NP") //  and other conditions
						) {
					
					// propose X as antecedent
					return X;
				}
				
				// step 7 traverse all branches below X to the left of the path
				// in a left-to-right, breadth first manner
				// Propose any NP encountered as the antecedent
				potAntecedent = traverseLeftToRightFindFirstNP();
				if(potAntecedent!= null) return potAntecedent ;
				
				
			}
			
			
		}
		
		return null;
	}
	
	public Annotation getNextNominalPhrase(Node x) {
		
//		Node x = pronoun;
		List<Node> path = new ArrayList<>();
//		path.add(x);
		while (!(getType(x).equals("NP") || getType(x).equals("S") || getType(x).equals("SBARQ"))
				&& x.getParent() != null) {
				path.add(x);
				x = x.getParent();
		}
		//TODO take care of "root" annotation
		// new node X
		X=x;
		// path from
		p=path;
		return null;
	}
	
	public Node getDominatingNominalPhrase() {
		// step 1 get immediately dominating noun phrase of pronoun
		// (parent of PRP is a nominal phrase) 
		return pronoun.getParent();
		
	}
	
	public Node traverseLeftToRightFindFirstNP() {
		List<Node> result = new ArrayList<>();
		Queue<Node> queue = new LinkedBlockingQueue<>();
		// start with first node below X
		Node start = X.getChildren().get(0);
		queue.add(start);
		while (!queue.isEmpty()) {
			Node n = queue.poll();
			// return first NP
			//
			if((getType(n).equals("NP")) && match(n)) {
				return n;
			}
			// only add children left of path
			//TODO fix this! this prunes the tree below the first node of the path
			List<Node> childrenLeft = n.getChildren().stream()
					.takeWhile(node -> !p.contains(node))
					.collect(Collectors.toList()); 
			
			result.addAll(childrenLeft);
		}
		return null;
	
	}
	
	/**
	 * function for step 3
	 * Propose as antecedent any NP that has a NP or S between it and X
	 */
	public Node traverseLeftToRightCond() {
		List<Node> potNP = new ArrayList<>();
		Queue<Node> queue = new LinkedBlockingQueue<>();
		// start with first node below X
		Node start = X.getChildren().get(0);
		queue.add(start);
		while (!queue.isEmpty()) {
			Node n = queue.poll();
			//
			if((getType(n).equals("NP")|| getType(n).equals("S") || getType(n).equals("SBARQ")) && match(m)) {
				potNP.add(n);
			}
			// only add children left of path
			//TODO fix this! this prunes the tree below the first node of the path
			List<Node> childrenLeft = n.getChildren().stream()
					.takeWhile(node -> !p.contains(node))
					.collect(Collectors.toList()); 
			
			queue.addAll(childrenLeft);
		}
		int potNPs = potNP.size();
		// only if at least two NPS are found a NP/S between it and X exists
		// return the closest to X that fits this criterion
		if(potNPs >=2) return potNP.get(potNPs-2);
		
		else return null;
	
	}

	// functions for matching(gender and number)
	private boolean match(Node m) {
		if(number_match(m) && gender_match(m)) return true;

		else return false;
	}


	private boolean number_match(Node n) {
		Hashmap<String, String> map = new Hashmap();
         map.put("NN","singular");
         map.put("NNP","singular");
         map.put("he","singular");
         map.put("she","singular");
         map.put("him","singular");
         map.put("her","singular");
         map.put("it","singular");
         map.put("himself","singular");
         map.put("herself","singular");
         map.put("itself","singular");
         map.put("NNS","plural");
         map.put("NNPS","plural");
         map.put("they","plural");
         map.put("them","plural");
         map.put("themselves","plural");
		 map.put("PRP",null);
		 
		 String value = map.get(getType(n));
		 if(getTypte(n).equals(nominal_labels)){
			 if(map.get(getString(n)).equals(value)) return true;
		 };
				
		
	}

	public static List<String> fileLineRead(String name) throws IOException
	{
	 List<String> retStr = new ArrayList<String>();
	 BufferedReader in = new BufferedReader(new FileReader(name));
	 String s;
	 while ((s = in.readLine()) != null) {
	  retStr.add(s);
	 }
	 in.close();
	 return retStr;  
	}

	private boolean gender_match(Node g) {
		List<String> male_names = fileLineRead("male.txt");
		List<String> female_names = fileLineRead("female.txt");
		List<String> male_pro = {"he", "him", "himself"};
		List<String> female_pro = {"she", "her", "herself"};
		List<String> neutral_pro = {"it", "itself"};

		if(getTypte(n).equals(nominal_labels)){
			// In case of male name
			for(string name : )
			if(male_names.contains(isLeaf().get(0).toLowerCase())){
				if (getType(g).equals(female_pro)) return false;
				else if (getType(g).equals(neutral_pro)) return false;
			}
			// In case of female name
			else if(female_names.contains(isLeaf().get(0).toLowerCase())){
				if (getType(g).equals(male_pro)) return false;
				else if (getType(g).equals(neutral_pro)) return false;
			}
			
		} return true;
		
		
	}
	private String getType(Node a) {
		return a.getData().getFeatures().get("type").toString();
	}

	// To use the string to match gender and number
	private String getString(Node a) {
          return a.getData().getFeatures().get("string").toString();
    }

}
