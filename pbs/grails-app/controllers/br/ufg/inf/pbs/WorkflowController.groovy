package br.ufg.inf.pbs

class WorkflowController {

    def view = {
        [workflow: Workflow.get(params.id)]
    }
}
