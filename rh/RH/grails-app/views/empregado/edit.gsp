<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 09/04/13
  Time: 11:04
  To change this template use File | Settings | File Templates.
--%>

<!DOCTYPE html>

<%@ page import="rh.Sexo; rh.Cargo; rh.Escolaridade" contentType="text/html;charset=UTF-8" %>
<html>
<head>
    <title></title>
    <meta name="pessoa.viewport" content="width=device-width, initial-scale=1.0">

    <!-- JQUERY -->
    <link rel="stylesheet" href="http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css"/>
    %{--<script src="http://code.jquery.com/jquery-1.9.1.js"></script>--}%
    <script src="//ajax.googleapis.com/ajax/libs/jquery/2.0.0/jquery.js"></script>
    %{--<script src="http://code.jquery.com/ui/1.10.3/jquery-ui.js"></script>--}%
    <script src="//ajax.googleapis.com/ajax/libs/jqueryui/1.10.3/jquery-ui.js"></script>

    <!-- BOOTSTRAP -->
    <link rel="stylesheet" href="${resource(dir: 'bootstrap/css', file: 'bootstrap.css')}" type="text/css"
          media="screen">

    <style type="text/css">
    body {
        padding-top: 60px;
        padding-bottom: 40px;
    }

    .sidebar-nav {
        padding: 9px 0;
    }

    @media (max-width: 980px) {
        /* Enable use of floated navbar text */
        .navbar-text.pull-right {
            float: none;
            padding-left: 5px;
            padding-right: 5px;
        }
    }
    </style>

    %{--<link rel="stylesheet" href="${resource(dir: 'bootstrap/css', file: 'bootstrap-resposiveness.css')}" type="text/css"--}%
    %{--media="screen"/>--}%

</head>

<body>

<div class="navbar navbar-inverse navbar-fixed-top">
    <div class="navbar-inner">
        <div class="container-fluid">
            <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="brand" href="#">RH</a>

            <div class="nav-collapse collapse">
                <p class="navbar-text pull-right">
                    Logado em horas como <a href="#" class="navbar-link">Username</a>
                </p>
                <ul class="nav">
                    %{--<li class="active"><a href="#">Home</a></li>--}%
                    %{--<li><a href="#about">About</a></li>--}%
                    <li><a href="#logout">Logout</a></li>
                </ul>
            </div><!--/.nav-collapse -->
        </div>
    </div>
</div>


<div class="container-fluid">
    <div class="row-fluid">
        <div class="span3"></div>

        <div class="span9">
            <div class="span6"><input type="text" style="float: left; width: 100%;"/></div>

            <div class="btn-group pull-right">
                %{--<a class="btn dropdown-toggle" data-toggle="dropdown" href="#">--}%
                %{--Action--}%
                %{--<span class="caret"></span>--}%
                %{--</a>--}%
                %{--<ul class="dropdown-menu">--}%
                %{--<!-- dropdown menu links -->--}%
                %{--</ul>--}%
                %{--<button class="btn">Left</button>--}%
                %{--<button class="btn">Middle</button>--}%
                <script type="text/javascript">
                    function salvar() {
                        document.forms[0].submit();
                    }
                </script>
                <button class="btn" onclick="salvar()">Salvar</button>
            </div>

        </div>

    </div>
</div>

<div class="container-fluid">
<div class="row-fluid">
<div class="span3">
    <g:render template="../comum/menu"/>

</div><!--/span-->
<div class="span9">
<div class="hero-unit" style="padding: 10px;">

%{--<input type="text" value="Novo empregado" style="font-weight: bolder; font-size: 20pt; height: 25pt; width: 25em;"/>--}%

    <g:form class="empregado-form" action="save" method="POST">

        <div id="form-tabs" class="tabbable"><!-- Only required for left/right tabs -->
            <ul class="nav nav-tabs">
                <li class="active"><a href="#tab1" data-toggle="tab">Pessoal</a></li>
                <li><a href="#tab2" data-toggle="tab">Profissional</a></li>
                <li><a href="#tab3" data-toggle="tab">Contrato</a></li>
            </ul>

            <div class="tab-content">
                <div class="tab-pane active" id="tab1">

                    <fieldset>

                        <g:hiddenField name="id" value="${empregado.id}"/>
                        <g:hiddenField name="pessoa-id" value="${empregado.pessoa.id}"/>

                        <div class="pessoa-header-info-nome">
                            <label for="pessoa.nome">Nome</label>
                            <input id="pessoa-nome" type="text" name="pessoa.nome" value="${empregado.pessoa.nome}"/>
                            <script type="text/javascript">
                                $(function () {
                                    $('#pessoa-nome').autocomplete({
                                        source: 'pessoas',
                                        minLength: 5,
                                        select: function (event, ui) {
                                            loadPessoa(ui.item);
                                        }
                                    });
                                });
                            </script>
                        </div>

                        <div class="pessoa-header-info-dataNascimento">
                            <label for="pessoa.dataNascimento">Data nascimento</label>
                            <g:datePicker precision="day" name="pessoa.dataNascimento"
                                          value="${empregado.pessoa.dataNascimento}"/>
                        </div>

                        <div class="pessoa-header-info-nomeMae">
                            <label for="pessoa.nomeMae">Nome da mãe</label>
                            <input id="pessoa-nomeMae" type="text" name="pessoa.nomeMae"
                                   value="${empregado.pessoa.nomeMae}"/>
                        </div>

                        <div class="pessoa-header-info-sexo">
                            <label for="pessoa.sexo">Sexo</label>
                            <g:radioGroup id="pessoa-sexo" values="${Sexo.values()}" labels="${Sexo.values()}"
                                          name="pessoa.sexo"
                                          value="${empregado.pessoa.sexo}">
                                ${it.label} ${it.radio}&nbsp;&nbsp;&nbsp;
                            </g:radioGroup>

                        </div>

                        <div class="pessoa-header-info-nomePai">
                            <label for="pessoa.nomePai">Nome do pai</label>
                            <input id="pessoa-nomePai" type="text" name="pessoa.nomePai"
                                   value="${empregado.pessoa.nomePai}"/>
                        </div>

                        <div class="pessoa-header-info-pne">
                            <label for="pessoa.pne">PNE</label>
                            <g:checkBox name="pessoa.pne" value="${empregado.pessoa.pne}"/>
                        </div>

                        <div class="pessoa-info-naturalidade">
                            <label for="pessoa.naturalidade">Naturalidade</label>
                            <input type="text" name="pessoa.naturalidade"
                                   value="${empregado.pessoa.codigoNaturalidade}"/>
                        </div>

                        <div class="pessoa-info-estadoCivil">
                            <label for="pessoa.estadoCivil">estadoCivil</label>
                            <input type="text" name="pessoa.estadoCivil"
                                   value="${empregado.pessoa.estadoCivil}"/>
                        </div>

                        <div class="pessoa-info-cpf">
                            <label for="pessoa.cpf">cpf</label>
                            <input type="text" name="pessoa.cpf" value="${empregado.pessoa.cpf}"/>
                        </div>

                        <div class="pessoa-info-rg">
                            <label for="pessoa.rg">rg</label>
                            <input type="text" name="pessoa.rg" value="${empregado.pessoa.rg}"/>
                        </div>

                        <div class="pessoa-info-pis">
                            <label for="pis">pis</label>
                            <input type="text" name="pis" value="${empregado.pis}"/>
                        </div>

                    </fieldset>
                </div>

                <div class="tab-pane" id="tab2">
                    <fieldset>
                        <g:hiddenField name="formacao.id" value="${empregado.formacao.id}"/>
                        <div>
                            <label for="formacao.descricao">Formacao</label>
                            <g:textField name="formacao.descricao" value="${empregado.formacao.descricao}"/>
                            %{--<input type="text" name="pessoa.nome" value="${empregado.pessoa.nome}"/>--}%
                        </div>

                        <div>
                            <label for="formacao.nivel">Nível</label>
                            <g:select name="formacao.nivel" from="${Escolaridade.values()}"
                                      value="${empregado.formacao.nivel}" noSelection="['': '- Escolha o nível -']"/>
                        </div>
                    </fieldset>
                </div>

                <div class="tab-pane" id="tab3">
                    <fieldset>
                        <div>
                            <label for="contrato.dataAdmissao">Data admissão</label>
                            <g:datePicker precision="day" name="contrato.dataAdmissao"
                                          value="${empregado.contrato.dataAdmissao}"/>
                        </div>

                        <div>
                            <label for="contrato.dataRegistro">Data registro</label>
                            <g:datePicker precision="day" name="contrato.dataRegistro"
                                          value="${empregado.contrato.dataRegistro}"/>
                        </div>

                        <div>
                            <label for="contrato.dataDemissao">Data demissão</label>
                            <g:datePicker precision="day" name="contrato.dataDemissao"
                                          value="${empregado.contrato.dataDemissao}"/>
                        </div>

                        <div>
                            <label for="contrato.cargaHoraria">Carga horária semanal</label>
                            <g:textField name="contrato.cargaHoraria" value="${empregado.contrato.cargaHoraria}"/>
                        </div>

                        <div>
                            <label for="contrato.salarioBase">Salário base</label>
                            <g:textField name="contrato.salarioBase" value="${empregado.contrato.salarioBase}"/>
                        </div>

                        <div>
                            <label for="contrato.cargo.id">Cargo</label>
                            <g:select name="contrato.cargo.id" from="${Cargo.list()}" optionValue="nome" optionKey="id"
                                      value="${empregado.contrato.cargo?.id}"
                                      noSelection="['': '- Escolha o cargo -']"/>
                        </div>

                    </fieldset>
                </div>
            </div>
        </div>

    </g:form>

%{--<h1>Hello, world!</h1>--}%

%{--<p>This is a template for a simple marketing or informational website. It includes a large callout called the hero unit and three supporting pieces of content. Use it as a starting point to create something more unique.</p>--}%

%{--<p><a href="#" class="btn btn-primary btn-large">Learn more &raquo;</a></p>--}%
</div>

%{--<div class="row-fluid">--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--</div><!--/row-->--}%
%{--<div class="row-fluid">--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--<div class="span4">--}%
%{--<h2>Heading</h2>--}%

%{--<p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>--}%

%{--<p><a class="btn" href="#">View details &raquo;</a></p>--}%
%{--</div><!--/span-->--}%
%{--</div><!--/row-->--}%
</div><!--/span-->
</div><!--/row-->

<hr>

<footer>
    %{--<p>&copy; Company 2013</p>--}%
</footer>

</div>


%{--<script src="http://code.jquery.com/jquery.js"></script>--}%
<script type="text/javascript" src="${resource(dir: 'bootstrap/js', file: 'bootstrap.js')}"></script>

<script type="text/javascript">
    function loadPessoa(pessoa) {
        $("input[name='pessoa.id']").val(pessoa.id);
        $("input[name='pessoa.nomeMae']").val(pessoa.nomeMae);
        $("input[name='pessoa.nomePai']").val(pessoa.nomePai);
        $("input[name='pessoa.sexo']").val(pessoa.sexo);
    }
</script>

</body>
</html>