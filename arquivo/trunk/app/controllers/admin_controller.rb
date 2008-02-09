class AdminController < ApplicationController

  # Verifica se o usuario é administrador
  def administracao
    unless usuario_logado && usuario_logado.admin?
      flash[:notice] = 'Acesso não autorizado'
      redirect_to uri_principal
    end
  end

  before_filter :autorizar, :administracao

  def index
    @empresa = usuario_logado.empresa
  end

  def edit_usuario
    @usuario = Usuario.find(params[:id])
  end
end
