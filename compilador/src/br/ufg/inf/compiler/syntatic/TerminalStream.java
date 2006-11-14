package br.ufg.inf.compiler.syntatic;

public interface TerminalStream {
	public Terminal getTerminal();
	public void advance();
}
