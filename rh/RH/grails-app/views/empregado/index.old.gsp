<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 09/04/13
  Time: 11:04
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <meta name="layout" content="main"/>
  <title></title>
  <style type="text/css">
    .empregado-form {

    }

    .pessoa-header {
      border: solid black 1px;
    }

    .pessoa-foto {
      float: left;
      width: 102px;
      padding: 10pt;
      vertical-align: middle;
    }

    .pessoa-foto img {
      vertical-align: middle;
    }

    .pessoa-header-info {
      float: left;
      width: 400px;
    }

    .pessoa-header-info label {
      display: block;
    }

    .pessoa-header-info div {
      padding: 5pt 0;
    }

    .pessoa-header-info-nome {
      float: left;
      width: 280px;
    }

    .pessoa-header-info-nome input, .pessoa-header-info-nomeMae input, .pessoa-header-info-nomePai input {
      width: 250px;
    }

    .pessoa-header-info-dataNascimento {
      float: left;
      width: 120px;
    }

    .pessoa-header-info-nomeMae {
      float: left;
      width: 280px;
    }

    .pessoa-header-info-sexo {
      float: left;
      width: 120px;
    }

    .pessoa-header-info-nomePai {
      float: left;
      width: 280px;
    }

    .pessoa-header-info-pne {
      float: left;
      width: 120px;
    }


  </style>

</head>

<body>

<form class="empregado-form">

  <div class="pessoa-header">
    <div class="pessoa-foto">
      <img src="https://www.google.com/s2/photos/private/AIbEiAIAAAAiCLzsv5WGoJ_6fBCYnfGu-NS_5JsBGO6n_o2s2fXipQEwAWu9sn9AWyn4hnsCuW1RrTyI99ek?sz=96"/>
    </div>
    <div class="pessoa-header-info">
      <div class="pessoa-header-info-nome">
        <label for="nome">Nome</label>
        <input type="text" name="nome" value="Gustavo Cipriano Mota Sousa"/>
      </div>

      <div class="pessoa-header-info-dataNascimento">
        <label for="dataNascimento">Data nascimento</label>
        <input type="text" name="dataNascimento" value="12/12/1985"/>
      </div>

      <div class="pessoa-header-info-nomeMae">
        <label for="nomeMae">Nome da mãe</label>
        <input type="text" name="nomeMae" value="Herondina Cipriano Mota"/>
      </div>

      <div class="pessoa-header-info-sexo">
        <label for="sexo">Sexo</label>
        M <input type="radio" name="sexo" value="M"/>
        F <input type="radio" name="sexo" value="F"/>
      </div>

      <div class="pessoa-header-info-nomePai">
        <label for="nomePai">Nome do pai</label>
        <input type="text" name="nomePai" value="José Amélio de Sousa"/>
      </div>

      <div class="pessoa-header-info-pne">
        <label for="pne">PNE</label>
        <input type="checkbox" name="pne" value=""/>
      </div>

    </div>
    <div class="clearfix"></div>
  </div>

  <div class="pessoa-info">
    <div class="pessoa-info-naturalidade">
      <label for="naturalidade">Naturalidade</label>
      <input type="text" name="naturalidade" value="Uruaçu - GO"/>
    </div>

    <div class="pessoa-info-estadoCivil">
      <label for="estadoCivil">estadoCivil</label>
      <input type="text" name="estadoCivil" value="Solteiro"/>
    </div>

    <div class="pessoa-info-cpf">
      <label for="cpf">cpf</label>
      <input type="text" name="cpf" value="017.664.801-10"/>
    </div>

    <div class="pessoa-info-rg">
      <label for="rg">rg</label>
      <input type="text" name="rg" value="3979087"/>
    </div>



  </div>

</form>

</body>
</html>