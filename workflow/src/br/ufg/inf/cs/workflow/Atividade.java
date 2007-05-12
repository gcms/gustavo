package br.ufg.inf.cs.workflow;

import java.util.List;
import java.util.Set;

import br.ufg.inf.cs.estimativas.Esforco;
import br.ufg.inf.cs.login.Usuario;
import br.ufg.inf.cs.outros.ValidationException;

public class Atividade {
    private String nome;

    private Workflow workflow;

    /* Entradas e sa�das devem ser pr�-cadastradas. */
    private Set<Produto> entradas;

    private Set<Produto> saidas;

    private static class PapelDoUsuario {
	private Papel papel;

	private Set<Usuario> executores;
    }

    private PapelDoUsuario responsavel;

    private List<Acao> procedimentos;

    private Set<PapelDoUsuario> colaboradores;

    private Set<Recurso> recursos;

    private Set<Atributo> atributos;

    private Esforco previsto, realizado, restante;

    public String getNome() {
	return nome;
    }

    public void addAtributo(String nome, String tipo, Object valor) {
	addAtributo(nome, tipo).setValor(valor);
    }

    public Atributo addAtributo(String nome, String tipo) {
	if (!eDefinicao()) {
	    throw new ValidationException(
		    "Opera��o n�o pode ser realizada durante a execu��o.");
	}

	try {
	    Atributo at = new Atributo(nome, Class.forName(tipo));
	    atributos.add(at);
	    return at;
	} catch (ClassNotFoundException e) {
	    throw new IllegalArgumentException(tipo + " n�o � um tipo v�lido");
	}
    }

    public void setEsforcoPrevisto(Esforco previsto) {
	if (!eDefinicao()) {
	    throw new ValidationException(
		    "Opera��o n�o pode ser realizada durante a execu��o.");
	}
	this.previsto = previsto;
    }

    public void setEsforcoRealizado(Esforco realizado) {
	if (eDefinicao()) {
	    throw new ValidationException(
		    "Opera��o n�o pode ser realizada durante a defini��o.");
	}

	this.realizado = realizado;
    }

    public void setEsforcoRestante(Esforco realizado) {
	if (eDefinicao()) {
	    throw new ValidationException(
		    "Opera��o n�o pode ser realizada durante a defini��o.");
	}

	this.realizado = realizado;
    }

    public boolean eDefinicao() {
	return workflow.eDefinicao();
    }

    public boolean prontoParaExecutar() {
	assert !eDefinicao();

	return nome != null && entradas != null && saidas != null
		&& responsavel != null && procedimentos != null
		&& previsto != null;
    }
}
