class UsuariosController < ApplicationController
  # GET /usuarios
  # GET /usuarios.xml
#  def index
#    @usuarios = Usuario.find(:all)

#    respond_to do |format|
#      format.html # index.html.erb
#      format.xml  { render :xml => @usuarios }
#    end
#  end

  # GET /usuarios/1
  # GET /usuarios/1.xml
#  def show
#    @usuario = Usuario.find(params[:id])

#    respond_to do |format|
#      format.html # show.html.erb
#      format.xml  { render :xml => @usuario }
#    end
#  end

  # GET /usuarios/new
  def new
    @usuario = @empresa.usuarios.build
  end

  # GET /usuarios/1/edit
  def edit
    @usuario = @empresa.usuarios.find(params[:id])
  end

  # POST /usuarios
  def create
    @usuario = @empresa.usuarios.build(params[:usuario])

    if @usuario.save
      flash[:notice] = 'Usuario was successfully created.'
      redirect_to edit_empresa_path
#      redirect_to([:empresa, @usuario])
    else
      render :action => "new"
    end
  end

  # PUT /usuarios/1
  # PUT /usuarios/1.xml
  def update
    @usuario = @empresa.usuarios.find(params[:id])

    respond_to do |format|
      if @usuario.update_attributes(params[:usuario])
        flash[:notice] = 'Usuario was successfully updated.'
        format.html { redirect_to([@empresa, @departamento]) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @usuario.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /usuarios/1
  def destroy
    @usuario = @empresa.usuarios.find(params[:id])
    @usuario.destroy

    redirect_to(edit_empresa_path)
  end
end
