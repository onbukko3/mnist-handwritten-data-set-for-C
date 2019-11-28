package team3.a3.coref;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import gate.Annotation;
import gate.AnnotationSet;
import gate.util.OffsetComparator;

/**
 * @author Robin Lamberti
 *
 */
public class TreeBuilder {
	private AnnotationSet annotations;
	private List<Annotation> tokens;
	private OffsetComparator comp = new OffsetComparator();

	public TreeBuilder(AnnotationSet annotations, List<Annotation> tokens) {
		this.annotations = annotations;
		this.tokens = tokens;
	}

	public Node buildTree() {
		final Integer maxLevel = annotations.stream()
				.mapToInt(a -> Integer.parseInt(a.getFeatures().get("level").toString())).max()
				.orElseThrow(() -> new IllegalStateException("No parse annotations present"));

		Map<Integer, List<Node>> nodes = annotations.stream().map(NodeImpl::new).collect(
				Collectors.groupingBy(n -> Integer.parseInt(n.getData().getFeatures().get("level").toString())));

		nodes.forEach((k, v) -> v.sort((n1, n2) -> comp.compare(n1.getData(), n2.getData())));

		List<Node> leaves = nodes.get(maxLevel);

		assert leaves.size() == tokens.size();

		for (int i = 0; i < leaves.size(); i++) {
			Node leaf = leaves.get(i);
			leaf.addChild(new NodeImpl(tokens.get(i), leaf));
		}

		List<Node> potentialChildren = leaves;
		for (Integer currentLevel = maxLevel - 1; currentLevel >= 0; currentLevel--) {
			List<Node> potentialParents = nodes.get(currentLevel);
			for (Node n : potentialParents) {
				List<Node> children = getContainedAnnotations(n, potentialChildren);
				assert !children.isEmpty();
				
				n.addChildren(children);
				potentialChildren.removeAll(children);
				potentialChildren.add(n);
			}
		}
		
		return nodes.get(0).get(0);
	}

	private List<Node> getContainedAnnotations(Node parent, Collection<Node> nodes) {
		Long start = parent.getData().getStartNode().getOffset();
		Long end = parent.getData().getEndNode().getOffset();
		return nodes.stream().filter(n -> n.getData().getStartNode().getOffset() >= start)
				.filter(n -> n.getData().getEndNode().getOffset() <= end)
				.sorted((n1, n2) -> comp.compare(n1.getData(), n2.getData())).collect(Collectors.toList());
	}
}
