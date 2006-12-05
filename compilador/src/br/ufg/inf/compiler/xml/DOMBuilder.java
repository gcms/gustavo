package br.ufg.inf.compiler.xml;

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
import br.ufg.inf.compiler.main.SetupException;
import br.ufg.inf.compiler.syntatic.Grammar;
import br.ufg.inf.compiler.syntatic.NonTerminal;
import br.ufg.inf.compiler.syntatic.Sentence;
import br.ufg.inf.compiler.syntatic.Symbol;
import br.ufg.inf.compiler.syntatic.SyntaticTable;
import br.ufg.inf.compiler.syntatic.Terminal;

import com.sun.org.apache.xerces.internal.parsers.DOMParser;

/**
 * Um construtor de especifica��o l�xica, e gram�tica, a partir de um arquivo
 * xml, usando DOM. A classe permite que seja especificado um arquivo para ser
 * analisado, e que se obtenha em seguida a especifica��o l�xica e a gram�tica
 * correspondente ao arquivo.
 * 
 * O formato do arquivo deve seguir a seguinte conven��o:
 * 
 * @author gustavo
 * 
 */
public class DOMBuilder {

	/** Especifica��o l�xica, a ser gerada. */
	private LexerSpec spec;

	/** Gram�tica a ser gerada. */
	private Grammar grammar;

	/**
	 * Obt�m a expecifica��o l�xica gerada.
	 * 
	 * @return especifica��o l�xica
	 */
	public LexerSpec getLexerSpec() {
		return spec;
	}

	/**
	 * Obt�m a gram�tica gerada.
	 * 
	 * @return gram�tica
	 */
	public Grammar getGrammar() {
		return grammar;
	}

	/**
	 * Analisa um documento, gerando a especifica��o l�xica, e gram�tica.
	 * 
	 * @param document
	 *            documento a ser processado
	 */
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

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
	}

	/**
	 * Analisa um documento, gerando a especifica��o l�xica, e gram�tica.
	 * 
	 * @param filename
	 *            nome do arquivo xml
	 */
	public void parse(String filename) {
		DOMParser parser = new DOMParser();

		try {
			parser.parse(filename);

		} catch (SAXException se) {
			se.printStackTrace();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}

		parse(parser.getDocument());
	}

	/**
	 * Analisa o elemento raiz do documento.
	 * 
	 * @param parent
	 */
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

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
	}

	/**
	 * Analisa elemento lexico.
	 * 
	 * @param parent
	 */
	private void parseLexical(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("token-spec")) {
				parseTokenSpec(n);
				return;
			}
		}

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
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
					throw new SetupException(
							"DOMBuilder: Erro ao ler gram�tica");
				}
				return;
			}
		}

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
	}

	/**
	 * Analisa elemento sint�tico.
	 * 
	 * @param parent
	 */
	private void parseSyntatic(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("grammar")) {
				parseGrammar(n);
				return;
			}
		}

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
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

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
	}

	private void parseRules(Node parent) {
		NodeList l = parent.getChildNodes();

		for (int i = 0; i < l.getLength(); i++) {
			Node n = l.item(i);

			if (n.getNodeName().equals("production")) {
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
			} else if (n.getNodeName().equals("right-side")) {
				String rightSideString = n.getTextContent();
				List<Symbol> symbols = new ArrayList<Symbol>();

				String[] items = rightSideString.split("[ \r\n\t]");

				for (String s : items) {
					if (s.length() == 0) {
					} else if (spec.isToken(s)) {
						symbols.add(new Terminal(s));
					} else {
						symbols.add(new NonTerminal(s));
					}

				}

				rightSide = new Sentence(symbols);
			}

			if (leftSide != null && rightSide != null) {
				grammar.addProduction(leftSide, rightSide);
				return;
			}
		}

		throw new SetupException("DOMBuilder: Erro ao ler gram�tica");
	}

	/**
	 * Teste.
	 * 
	 * @param args
	 */
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
				.getLexer(new CharSequenceCharSource("ibac"))));

	}

}
