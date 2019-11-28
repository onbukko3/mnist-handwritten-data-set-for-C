package team3.a3.coref;

import java.util.Collection;
import java.util.List;

import gate.Annotation;

/**
 * 
 * @author Robin Lamberti
 *
 */
public interface Node {
	List<Node> getChildren();
	
	void addChild(Node node);
	
	default void addChildren(Collection<Node> children) {
		children.forEach(this::addChild);
	}
	
	Node getParent();
	
	void setParent(Node parent);
	
	Annotation getData();
	
	default boolean isLeaf() {
		return getChildren().isEmpty();
	}
	
	default boolean isRoot() {
		return getParent() == null;
	}
}
