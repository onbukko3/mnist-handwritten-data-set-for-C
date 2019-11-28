package team3.a3.coref;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import gate.Annotation;
import gate.AnnotationSet;
import gate.Resource;
import gate.Utils;
import gate.creole.AbstractLanguageAnalyser;
import gate.creole.ExecutionException;
import gate.creole.ResourceInstantiationException;
import gate.creole.metadata.CreoleResource;
import gate.util.OffsetComparator;

/**
 * @author Robin Lamberti
 *
 */
@CreoleResource(name = "Stanford Parser (Stanford CoreNLP)", comment = "Parsing with Stanford CoreNLP")
public class Plugin extends AbstractLanguageAnalyser {
	private static final long serialVersionUID = -7921061482940070912L;

	@Override
	public Resource init() throws ResourceInstantiationException {
		super.init();

		return this;
	}

	@Override
	public void execute() throws ExecutionException {
		if (document == null) {
			throw new ExecutionException("No document to process!");
		}

		/*
		 * getting the input anotation set, which contains all annotations in the
		 * document (as long as we do not define different input and output annotation
		 * sets in our plugins)
		 */
		AnnotationSet inputAS = document.getAnnotations();

		List<Annotation> sentenceAnnotations = Utils.inDocumentOrder(inputAS.get("Sentence"));

		List<Node> trees = new ArrayList<>();
		for (Annotation s : sentenceAnnotations) {
			// extract tokens from sentence
			List<Annotation> sortedTokens = Utils.getContainedAnnotations(inputAS, s, "Token").stream()
					.sorted(new OffsetComparator()).collect(Collectors.toList());

			AnnotationSet parses = Utils.getContainedAnnotations(inputAS, s, "Parse");

			Node root = new TreeBuilder(parses, sortedTokens).buildTree();
			trees.add(root);
			System.out.println(print(root));
			
			List<Node> pronouns = new BreathFirstSearch().execute(root).stream()
					.filter(n -> n.getData().getFeatures().get("type").toString().equals("PRP"))
					.collect(Collectors.toList());
			
			
		}
	}

	private String print(Node n) {
		return "(" + n.getData().getFeatures().get(n.isLeaf() ? "string" : "type")
				+ n.getChildren().stream().map(this::print).collect(Collectors.joining(" ")) + ")";
	}
}
