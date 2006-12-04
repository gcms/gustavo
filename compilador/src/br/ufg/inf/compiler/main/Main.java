package br.ufg.inf.compiler.main;

import java.io.FileNotFoundException;

import br.ufg.inf.compiler.xml.DOMBuilder;
import jargs.gnu.CmdLineParser;

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
		System.err.println("Usage: java -jar compilador.jar [{-v,--verbose}] "
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

		if (grammarFile == null) {
			printUsage();
			System.exit(2);
		}

		if (inputFile == null) {
			if (remaining == null || remaining.length == 0) {
				printUsage();
				System.exit(2);
			} else {
				inputFile = remaining[0];
			}
		}

		if (outputFile == null) {
			outputFile = inputFile + ".o";
		}

		DOMBuilder builder = new DOMBuilder();

		builder.parse(grammarFile);

		Compiler c = new Compiler(builder.getLexerSpec(), builder.getGrammar());

		c.run(inputFile, outputFile);
	}
}
