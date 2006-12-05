package br.ufg.inf.compiler.main;

import jargs.gnu.CmdLineParser;

import java.io.FileNotFoundException;
import java.io.IOException;

import monq.jfa.ByteCharSource;
import monq.jfa.CharSequenceCharSource;
import monq.jfa.CharSource;
import br.ufg.inf.compiler.lexical.Lexeme;
import br.ufg.inf.compiler.lexical.Lexer;
import br.ufg.inf.compiler.syntatic.SyntaticTable;
import br.ufg.inf.compiler.xml.DOMBuilder;

/**
 * Classe principal, ponto de entrada do programa.
 * 
 * @author gustavo
 * 
 */
public class Main {
	/**
	 * Mostra mensagem de erro de argumentos.
	 * 
	 */
	private static void printUsage() {
		System.err
				.println("\nUsage: java -jar compilador.jar [{-v,--verbose}] "
						+ "{-g,--grammar} grammar\n[(-i,--input)] input "
						+ "[(-o,--output) output]");
	}

	/**
	 * Programa principal.
	 * 
	 * @param args
	 *            argumentos do programa
	 * @throws FileNotFoundException
	 */
	public static void main(String[] args) throws FileNotFoundException {
		CmdLineParser parser = new CmdLineParser();

		CmdLineParser.Option verbose = parser.addBooleanOption('v', "verbose");
		CmdLineParser.Option grammar = parser.addStringOption('g', "grammar");
		CmdLineParser.Option input = parser.addStringOption('i', "input");
		CmdLineParser.Option output = parser.addStringOption('o', "output");

		try {
			parser.parse(args);
		} catch (CmdLineParser.OptionException e) {
			System.err.println(e.getMessage());
			printUsage();
			System.exit(2);
		}

		String grammarFile = (String) parser.getOptionValue(grammar);
		String inputFile = (String) parser.getOptionValue(input);
		String outputFile = (String) parser.getOptionValue(output);

		String[] remaining = parser.getRemainingArgs();

		// if (grammarFile == null) {
		// printUsage();
		// System.exit(2);
		// }
		//
		// if (inputFile == null) {
		// if (remaining == null || remaining.length == 0) {
		// printUsage();
		// System.exit(2);
		// } else {
		// inputFile = remaining[0];
		// }
		// }
		//
		// if (outputFile == null) {
		// outputFile = inputFile + ".o";
		// }

		DOMBuilder builder = new DOMBuilder();

		builder.parse("data/grammar3.xml");

		SyntaticTable table = new SyntaticTable(builder.getGrammar());

		System.out.println("\n");
		System.out.println(table);

		Lexer lexer = builder.getLexerSpec().getLexer(
				new ByteCharSource("data/exemplo1.txt"));
		
//		Lexeme lexeme;
//		while ((lexeme = lexer.nextToken()) != null) {
//			System.out.println(lexeme);
//		}
		
//		System.out.println(table.parse(lexer));

		// Compiler c = new Compiler(builder.getLexerSpec(),
		// builder.getGrammar());
		//
		// CharSource src = new ByteCharSource(inputFile);
		// int data;
		// try {
		// while ((data = src.read()) != -1) {
		// System.out.println("'" + (char) data + "'" + "\t" + data);
		// }
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// System.out.println("EOF");
		//
		// c.run(new CharSequenceCharSource("a+ba+aa+b"));
		// c.run("data/exemplo.src");
	}
}
