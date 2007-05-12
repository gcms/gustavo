package br.ufg.inf.cs.workflow;

import java.util.Set;

import br.ufg.inf.cs.outros.ValidationException;

public class Workflow {
    private Workflow definicao;

    private Workflow pai;

    private Set<Workflow> subWorkflows;

    private Set<Atividade> atividades;

    private String nome;

    public String getNome() {
	return nome;
    }

    public boolean eDefinicao() {
	return definicao == null;
    }

    public boolean eSubWorfklow() {
	return pai != null;
    }

    public Set<Atividade> getAtividades() {
	return atividades;
    }

    public Workflow getDefinicao() {
	return definicao;
    }

    public Set<Workflow> getSubWorkflows() {
	return subWorkflows;
    }

    public boolean ePrincipal() {
	return pai == null;
    }

    public Workflow getPai() {
	return pai;
    }

    public void addSubWorkflow(Workflow subWorkflow) {
	subWorkflow.pai = this;
	subWorkflows.add(subWorkflow);
    }

    public void addAtividade(Atividade atividade) {
	for (Atividade a : atividades) {
	    if (a.getNome() == atividade.getNome()) {
		throw new ValidationException(
			"Nome da atividade deve ser único");
	    }
	}

	/* set atividades.workflow */
	atividades.add(atividade);
    }

    public boolean prontoParaExecutar() {
	assert !eDefinicao();

	for (Workflow wf : subWorkflows) {
	    if (!wf.prontoParaExecutar()) {
		return false;
	    }
	}

	for (Atividade a : atividades) {
	    if (!a.prontoParaExecutar()) {
		return false;
	    }
	}

	return true;
    }

    public boolean valido() {
	return false;
    }
}