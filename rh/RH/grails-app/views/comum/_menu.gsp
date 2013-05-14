<div class="well sidebar-nav">
    <ul class="nav nav-list">
        <li class="nav-header">Empregados</li>
        <li class="${params.action == 'list' ? 'active' : ''}"><g:link action="list">Todos</g:link></li>
        <li class="${params.action == 'listAtivos' ? 'active' : ''}"><g:link action="listAtivos">Ativos</g:link></li>
        <li class="${params.action == 'listInativos' ? 'active' : ''}"><g:link action="listInativos">Inativos</g:link></li>
        <li class="nav-header">Relatórios</li>
        <li><a href="#">Vencimento de férias</a></li>
        <li><a href="#">Aniversários</a></li>
        <li><a href="#">Gozo de férias</a></li>
        %{--<li class="nav-header">Sidebar</li>--}%
        %{--<li><a href="#">Link</a></li>--}%
        %{--<li><a href="#">Link</a></li>--}%
        %{--<li><a href="#">Link</a></li>--}%
    </ul>
</div><!--/.well -->