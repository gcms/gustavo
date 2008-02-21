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
  def create
    @unidade = @empresa.unidades.build(params[:unidade])
    @endereco = @unidade.endereco = Endereco.new(params[:endereco])

    if @unidade.save
      flash[:notice] = 'Unidade was successfully created.'
      redirect_to([:empresa, @unidade])
    else
      render :action => "new"
    end
  end

  # PUT /unidades/1
  def update
    @unidade = @empresa.unidades.find(params[:id])
    @endereco = @unidade.endereco


    if @unidade.update_attributes(params[:unidade]) \
        & @unidade.endereco.update_attributes(params[:endereco])
      flash[:notice] = 'Unidade was successfully updated.'
      redirect_to([@empresa, @unidade])
    else
      render :action => "edit"
    end
  end

  # DELETE /unidades/1
  def destroy
    @unidade = @empresa.unidades.find(params[:id])
    @unidade.destroy

    redirect_to(edit_empresa_path)
  end
end
