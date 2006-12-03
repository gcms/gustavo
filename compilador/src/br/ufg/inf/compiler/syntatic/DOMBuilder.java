package br.ufg.inf.compiler.syntatic;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import monq.jfa.CharSequenceCharSource;
import monq.jfa.ReSyntaxException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import br.ufg.inf.compiler.lexical.LexerSpec;
import br.ufg.inf.compiler.syntatic.Grammar;

import com.sun.org.apache.xerces.internal.parsers.DOMParser;

public class DOMBuilder {

	private LexerSpec spec;

	private Grammar grammar;
	
	public LexerSpec getLexerSpec() {
		return spec;
	}
	
	public Grammar getGrammar() {
		return grammar;
	}

	public void parse(Document document) {
		spec = null;
		grammar = null;

		NodeList l = document.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("compiler")) {
				parseCompiler(n);
				return;
			}
		}

		throw new RuntimeException();
	}

	private void parseCompiler(Node parent) {
		boolean readLexical = false, readSyntatic = false;

		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("lexical")) {
				parseLexical(n);
				readLexical = true;
			} else if (n.getNodeName().equals("syntatic")) {
				parseSyntatic(n);
				readSyntatic = true;
			}

			if (readLexical && readSyntatic)
				return;
		}

		throw new RuntimeException();
	}

	private void parseLexical(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("token-spec")) {
				parseTokenSpec(n);
				return;
			}
		}

		throw new RuntimeException();
	}

	private void parseTokenSpec(Node parent) {
		spec = new LexerSpec();

		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("token")) {
				parseToken(n);
			}
		}
	}

	private void parseToken(Node parent) {
		String name = null, regex = null;

		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("name")) {
				name = n.getTextContent();
			} else if (n.getNodeName().equals("regex")) {
				regex = n.getTextContent();
			}

			if (name != null && regex != null) {
				try {
					spec.addTokenRule(name, regex);
				} catch (ReSyntaxException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					throw new RuntimeException();
				}
				return;
			}
		}

		throw new RuntimeException();
	}

	private void parseSyntatic(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("grammar")) {
				parseGrammar(n);
				return;
			}
		}

		throw new RuntimeException();
	}

	private void parseGrammar(Node parent) {
		grammar = new Grammar();
		boolean startSymbol = false, rules = false;

		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("start-symbol")) {
				grammar.setStartSymbol(new NonTerminal(n.getTextContent()));
				startSymbol = true;
			} else if (n.getNodeName().equals("rules")) {
				parseRules(n);
				rules = true;
			}

			if (startSymbol && rules) {
				return;
			}
		}

		throw new RuntimeException();
	}

	private void parseRules(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("production")) {
				System.out.println("production>\n\t" + n.getTextContent());
				parseProduction(n);
			}
		}

	}

	private void parseProduction(Node parent) {
		NonTerminal leftSide = null;
		Sentence rightSide = null;

		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("left-side")) {
				leftSide = new NonTerminal(n.getTextContent());
				System.out.println(leftSide + "->");
			} else if (n.getNodeName().equals("right-side")) {
				String rightSideString = n.getTextContent();
				List<Symbol> symbols = new ArrayList<Symbol>();

				String[] items = rightSideString.split(" ");

				for (String s : items) {
					if (s.length() == 0) {
					} else if (spec.isToken(s)) {
						symbols.add(spec.getTerminal(s));
					} else {
						symbols.add(new NonTerminal(s));
					}

					System.out.println("\t\"" + s + "\".isToken*()? = "
							+ spec.isToken(s));
				}

				rightSide = new Sentence(symbols);
			}

			if (leftSide != null && rightSide != null) {
				grammar.addProduction(leftSide, rightSide);
				return;
			}
		}

		throw new RuntimeException();
	}

	public static void main(String[] args) {

		String xmlFile = "file:///C:/Documents and Settings/gustavo/Desktop/newgrammar.xml";

		DOMParser parser = new DOMParser();

		try {
			parser.parse(xmlFile);

		} catch (SAXException se) {
			se.printStackTrace();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}

		Document document = parser.getDocument();

		DOMBuilder test = new DOMBuilder();

		test.parse(document);

		System.out.println(test.grammar);

		SyntaticTable table = new SyntaticTable(test.grammar);

		System.out.println(table.parse(test.spec
				.getLexer(new CharSequenceCharSource("bac"))));

	}

}
