class DepartamentosController < ApplicationController
  # GET /departamentos

  def index
    @departamentos = @empresa.departamentos.find(:all)
  end

  # GET /departamentos/1
  def show
    @departamento = @empresa.departamentos.find(params[:id])
  end

  # GET /departamentos/new
  def new
    @departamento = @empresa.departamentos.build
  end

  # GET /departamentos/1/edit
  def edit
    @departamento = @empresa.departamentos.find(params[:id])
  end

  # POST /departamentos
  def create
    @departamento = @empresa.departamentos.build(params[:departamento])

    if @departamento.save
      flash[:notice] = 'Departamento was successfully created.'
      redirect_to([:empresa, @departamento])
    else
      render :action => "new"
    end
  end

  # PUT /departamentos/1
  def update
    @departamento = @empresa.departamentos.find(params[:id])

    if @departamento.update_attributes(params[:departamento])
      flash[:notice] = 'Departamento was successfully updated.'
      redirect_to([@empresa, @departamento])
    else
      render :action => "edit"
    end
  end

  # DELETE /departamentos/1
  def destroy
    @departamento = @empresa.departamentos.find(params[:id])
    @departamento.destroy

    redirect_to(edit_empresa_path)
  end
end
