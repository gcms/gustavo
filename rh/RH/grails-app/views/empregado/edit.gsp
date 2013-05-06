<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 09/04/13
  Time: 11:04
  To change this template use File | Settings | File Templates.
--%>

<!DOCTYPE html>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title></title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <link rel="stylesheet" href="${resource(dir: 'bootstrap/css', file: 'bootstrap.css')}" type="text/css" media="screen">

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
      <input type="text" style="float: left;"/>

      <div class="btn-group pull-right">
        <a class="btn dropdown-toggle" data-toggle="dropdown" href="#">
          Action
          <span class="caret"></span>
        </a>
        <ul class="dropdown-menu">
          <!-- dropdown menu links -->
        </ul>
        <button class="btn">Left</button>
        <button class="btn">Middle</button>
        <script type="text/javascript">
          function salvar() {
            document.forms[0].submit()
          }
        </script>
        <button class="btn" onclick="salvar()">Salvar</button>
      </div>
    </div>

  </div>
</div>
</div>

<div class="container-fluid">
  <div class="row-fluid">
    <div class="span3">
      <div class="well sidebar-nav">
        <ul class="nav nav-list">
          <li class="nav-header">Sidebar</li>
          <li class="active"><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li class="nav-header">Sidebar</li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li class="nav-header">Sidebar</li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
          <li><a href="#">Link</a></li>
        </ul>
      </div><!--/.well -->
    </div><!--/span-->
    <div class="span9">
      <div class="hero-unit" style="padding: 10px;">

        %{--<input type="text" value="Novo empregado" style="font-weight: bolder; font-size: 20pt; height: 25pt; width: 25em;"/>--}%

        <form class="empregado-form" action="salve" method="POST">

          <div id="form-tabs" class="tabbable"><!-- Only required for left/right tabs -->
            <ul class="nav nav-tabs">
              <li class="active"><a href="#tab1" data-toggle="tab">Pessoal</a></li>
              <li><a href="#tab2" data-toggle="tab">Profissional</a></li>
              <li><a href="#tab3" data-toggle="tab">Contrato</a></li>
            </ul>

            <div class="tab-content">
              <div class="tab-pane active" id="tab1">

                <fieldset>
                  <div class="pessoa-header-info-nome">
                    <label for="nome">Nome</label>
                    <input type="text" name="nome" value="${pessoa.nome}"/>
                  </div>

                  <div class="pessoa-header-info-dataNascimento">
                    <label for="dataNascimento">Data nascimento</label>
                    <input type="text" name="dataNascimento" value="${pessoa.dataNascimento}"/>
                  </div>

                  <div class="pessoa-header-info-nomeMae">
                    <label for="nomeMae">Nome da mãe</label>
                    <input type="text" name="nomeMae" value="${pessoa.nomeMae}"/>
                  </div>

                  <div class="pessoa-header-info-sexo">
                    <label for="sexo">Sexo</label>
                    <g:radioGroup values="['M', 'F']" name="sexo" value="${pessoa.sexo}"/>
                  </div>

                  <div class="pessoa-header-info-nomePai">
                    <label for="nomePai">Nome do pai</label>
                    <input type="text" name="nomePai" value="${pessoa.nomePai}"/>
                  </div>

                  <div class="pessoa-header-info-pne">
                    <label for="pne">PNE</label>
                    <g:checkBox name="pne" value="${pessoa.pne}"/>
                  </div>

                  <div class="pessoa-info-naturalidade">
                    <label for="naturalidade">Naturalidade</label>
                    <input type="text" name="naturalidade" value="${pessoa.codigoNaturalidade}"/>
                  </div>

                  <div class="pessoa-info-estadoCivil">
                    <label for="estadoCivil">estadoCivil</label>
                    <input type="text" name="estadoCivil" value="${pessoa.estadoCivil}"/>
                  </div>

                  <div class="pessoa-info-cpf">
                    <label for="cpf">cpf</label>
                    <input type="text" name="cpf" value="${pessoa.cpf}"/>
                  </div>

                  <div class="pessoa-info-rg">
                    <label for="rg">rg</label>
                    <input type="text" name="rg" value="${pessoa.rg}"/>
                  </div>

                </fieldset>
              </div>

              <div class="tab-pane" id="tab2">
                <p>Howdy, I'm in Section 2.</p>
              </div>

              <div class="tab-pane" id="tab3">
                <p>Howdy, I'm in Section 3.</p>
              </div>
            </div>
          </div>

        </form>

        <h1>Hello, world!</h1>

        <p>This is a template for a simple marketing or informational website. It includes a large callout called the hero unit and three supporting pieces of content. Use it as a starting point to create something more unique.</p>

        <p><a href="#" class="btn btn-primary btn-large">Learn more &raquo;</a></p>
      </div>

      <div class="row-fluid">
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
      </div><!--/row-->
      <div class="row-fluid">
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
        <div class="span4">
          <h2>Heading</h2>

          <p>Donec id elit non mi porta gravida at eget metus. Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Etiam porta sem malesuada magna mollis euismod. Donec sed odio dui.</p>

          <p><a class="btn" href="#">View details &raquo;</a></p>
        </div><!--/span-->
      </div><!--/row-->
    </div><!--/span-->
  </div><!--/row-->

  <hr>

  <footer>
    <p>&copy; Company 2013</p>
  </footer>

</div>


<script src="http://code.jquery.com/jquery.js"></script>
<script type="text/javascript" src="${resource(dir: 'bootstrap/js', file: 'bootstrap.js')}"></script>

</body>
</html>