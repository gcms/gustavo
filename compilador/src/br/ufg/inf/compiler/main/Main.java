package br.ufg.inf.compiler.main;

import jargs.gnu.CmdLineParser;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

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

		Boolean isVerbose = (Boolean) parser.getOptionValue(verbose);
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

		PrintStream sysout = System.out;
		if (isVerbose == null || !isVerbose) {
			System.setOut(new PrintStream(new OutputStream() {
				@Override
				public void write(int arg0) throws IOException {
					// TODO Auto-generated method stub

				}
			}));
		}

		DOMBuilder builder = new DOMBuilder();

		builder.parse(grammarFile);

		// Lexer l = builder.getLexerSpec()
		// .getLexer(new ByteCharSource(inputFile));
		// Lexeme lexeme;
		//
		// while ((lexeme = l.nextLexeme()) != null) {
		// System.out.println(lexeme);
		// }

		Compiler c = new Compiler(builder.getLexerSpec(), builder.getGrammar());
		boolean result = c.run(inputFile);

		sysout.println(result);
	}
}
