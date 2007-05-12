package br.ufg.inf.cs.workflow;

import java.util.Set;

public interface WorkflowManager {
    public Set<Workflow> findAll();

    public Set<Workflow> findAllDefinicao();

    public Set<Workflow> findAllInstanciasByDefinicao(Workflow definicao);
}
