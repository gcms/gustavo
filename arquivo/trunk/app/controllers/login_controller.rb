class LoginController < ApplicationController
  skip_before_filter :autorizar

  def login
    # if session[:usuario_id]; raise 'usuario_id deve ser nil'; end
    session[:usuario_id] = nil

    if request.post?
      usuario = Usuario.autenticar(params[:nome], params[:senha])

      if usuario
        session[:usuario_id] = usuario.id

        uri = session[:original_uri]
        session[:original_uri] = nil
        redirect_to (uri || root_path)
      else
        flash[:notice] = "Autenticação falhou"
      end
    end
  end

  def logout
    session[:usuario_id] = nil

    redirect_to :action => :login
    # redirect_to uri_principal
  end
end
