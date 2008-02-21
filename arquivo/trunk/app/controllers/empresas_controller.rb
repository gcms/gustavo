class EmpresasController < ApplicationController
  before_filter :autoriza_admin_empresa

  # GET /empresa/edit
  def edit
  end

  # PUT /empresas/1
  def update
    if @empresa.update_attributes(params[:empresa])
      flash[:notice] = 'Empresa was successfully updated.'
      redirect_to :action => :edit
    else
      render :action => "edit"
    end
  end

  private
  def autoriza_admin_empresa
    logger.info "#{@usuario.empresa.id} != #{@empresa.id}"
    if @usuario.empresa != @empresa; raise 'Erro de logica'; end

    unless @usuario.admin?
      redirect_to root_path
    end
  end
end
