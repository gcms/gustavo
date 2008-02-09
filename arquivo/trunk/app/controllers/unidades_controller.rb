class UnidadesController < ApplicationController
  # GET /unidades
  def index
    @unidades = @empresa.unidades.find(:all)
  end

  # GET /unidades/1
  def show
    @unidade = @empresa.unidades.find(params[:id])
  end

  # GET /unidades/new
  def new
    @unidade = @empresa.unidades.build
    @endereco = Endereco.new
  end

  # GET /unidades/1/edit
  def edit
    @unidade = @empresa.unidades.find(params[:id])
    @endereco = @unidade.endereco
  end

  # POST /unidades
  # POST /unidades.xml
  def create
    @unidade = @empresa.unidades.build(params[:unidade])
    @unidade.endereco = Endereco.new(params[:endereco])

    Unidade.transaction do
      if @unidade.save && @unidade.endereco.save
        flash[:notice] = 'Unidade was successfully created.'
        redirect_to([:empresa, @unidade])
      else
        render :action => "new"
      end
    end
  end

  # PUT /unidades/1
  # PUT /unidades/1.xml
  def update
    @unidade = @empresa.unidades.find(params[:id])

    respond_to do |format|
      Unidade.transaction do
        if @unidade.update_attributes(params[:unidade]) \
          && @unidade.endereco.update_attributes(params[:endereco])
          flash[:notice] = 'Unidade was successfully updated.'
          format.html { redirect_to([@empresa, @unidade]) }
          format.xml  { head :ok }
        else
          format.html { render :action => "edit" }
          format.xml  { render :xml => @unidade.errors, :status => :unprocessable_entity }
        end
      end
    end
  end

  # DELETE /unidades/1
  def destroy
    @unidade = @empresa.unidades.find(params[:id])
    @unidade.destroy

    format.html { redirect_to(edit_empresa_path) }
  end
end
