package br.ufg.inf.compiler.main;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;

import monq.jfa.ByteCharSource;
import monq.jfa.CharSource;
import br.ufg.inf.compiler.lexical.Lexer;
import br.ufg.inf.compiler.lexical.LexerSpec;
import br.ufg.inf.compiler.syntatic.Grammar;
import br.ufg.inf.compiler.syntatic.SyntaticTable;

/**
 * Representa um compilador, que pode ser executado passando uma fonte de
 * caracteres.
 * 
 * @author gustavo
 * 
 */
public class Compiler {
	/** Especificação léxica. */
	private LexerSpec spec;

	/** Gramática. */
	private Grammar grammar;

	/**
	 * Cria um compilador.
	 * 
	 * @param spec
	 *            especificação léxica
	 * @param grammar
	 *            gramática
	 */
	public Compiler(LexerSpec spec, Grammar grammar) {
		super();
		this.spec = spec;
		this.grammar = grammar;
	}

	/**
	 * Executa o compilador em uma fonte de caracteres.
	 * 
	 * @param src
	 *            fonte de caracteres
	 */
	public void run(CharSource src) {
		Lexer lexer = spec.getLexer(src);
		SyntaticTable table = new SyntaticTable(grammar);

		System.out.println(table.parse(lexer));
	}

	/**
	 * Executa o compilador em um arquivo.
	 * 
	 * @param filename
	 *            nome do arquivo
	 * @throws FileNotFoundException
	 */
	public void run(String filename) throws FileNotFoundException {
		run(new ByteCharSource(filename));
	}

	/**
	 * Executa o compilador.
	 * 
	 * @param inputFile
	 *            arquivo de entrada
	 * @param outputFile
	 *            arquivo de saída
	 * @throws FileNotFoundException
	 */
	public void run(String inputFile, String outputFile)
			throws FileNotFoundException {
		System.setOut(new PrintStream(new File(outputFile)));

		run(inputFile);
	}
}
