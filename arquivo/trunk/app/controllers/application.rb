# Filters added to this controller apply to all controllers in the application.
# Likewise, all the methods added will be available for all controllers.

class ApplicationController < ActionController::Base
  helper :all # include all helpers, all the time

  before_filter :autorizar

  # See ActionController::RequestForgeryProtection for details
  # Uncomment the :secret if you're not using the cookie session store
  protect_from_forgery  :secret => '6bfb92be7f914c0dc7451695b7938d2a'

  private

  # verifica se o usuário está logado
  # carrega @usuario e @empresa
  def autorizar
    @usuario = Usuario.find_by_id(session[:usuario_id])
    unless @usuario
      session[:original_uri] = request.request_uri

      flash[:notice] = "Por favor entre no sistema"
      redirect_to :controller => :login, :action => :login
    else
      @empresa = @usuario.empresa
    end
  end
end
