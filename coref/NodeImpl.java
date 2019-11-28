package team3.a3.coref;

import java.util.ArrayList;
import java.util.List;

import gate.Annotation;

/**
 * @author Robin Lamberti
 *
 */
public class NodeImpl implements Node {
	private final Annotation data;
	private List<Node> children = new ArrayList<>();
	private Node parent;

	public NodeImpl(Annotation data) {
		this.data = data;
		this.parent = null;
	}

	public NodeImpl(Annotation data, Node parent) {
		this.data = data;
		this.parent = parent;
	}
	
	@Override
	public void addChild(Node node) {
		children.add(node);
		node.setParent(this);
	}

	@Override
	public List<Node> getChildren() {
		return children;
	}

	@Override
	public Node getParent() {
		return parent;
	}
	
	@Override
	public void setParent(Node parent) {
		this.parent = parent;
	}

	@Override
	public Annotation getData() {
		return data;
	}

	@Override
	public String toString() {
		return "NodeImpl [" + (data != null ? "data=" + data + ", " : "")
				+ (children != null ? "children=" + children : "") + "]";
	}
}
