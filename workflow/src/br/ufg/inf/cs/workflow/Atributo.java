package br.ufg.inf.cs.workflow;

import br.ufg.inf.cs.outros.ValidationException;

public class Atributo {
    private String nome;

    private Class tipo;

    private Object valor;

    public Atributo(String nome, Class tipo, Object valor) {
	this(nome, tipo);
	setValor(valor);
    }

    public void setValor(Object valor) {
	if (valor.getClass() != tipo)
	    throw new ValidationException(valor + " deve ser do tipo " + tipo);

	this.valor = valor;
    }

    public Atributo(String nome, Class tipo) {
	super();
	this.nome = nome;
	this.tipo = tipo;

    }
}
