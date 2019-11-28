package team3.a3.coref;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * 
 * @author Robin Lamberti
 *
 */
public class BreadthFirstSearch {
	public List<Node> execute(Node start) {
		List<Node> result = new ArrayList<>();
		Queue<Node> queue = new LinkedBlockingQueue<>();
		queue.add(start);
		while (!queue.isEmpty()) {
			Node n = queue.poll();
			result.add(n);
			result.addAll(n.getChildren());
		}
		return result;
	}
}
