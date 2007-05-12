package br.ufg.inf.cs.login;

import br.ufg.inf.cs.workflow.Papel;

public interface FiltroUsuario {
    public Usuario[] findUsuarios();

    public Usuario[] findUsuariosByPapel(Papel papel);

    public Papel[] findPapeisByUsuario(Usuario usuario);
}
