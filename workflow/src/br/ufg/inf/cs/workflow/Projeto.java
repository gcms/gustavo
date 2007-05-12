package br.ufg.inf.cs.workflow;

public class Projeto {
    private Workflow instanciaWorkflow;

    private String nome;

    public void setInstanciaWorkflow(Workflow instanciaWorkflow) {
	assert !instanciaWorkflow.eDefinicao()
		&& instanciaWorkflow.prontoParaExecutar()
		&& instanciaWorkflow.ePrincipal() && instanciaWorkflow == null;

	this.instanciaWorkflow = instanciaWorkflow;
    }
}
