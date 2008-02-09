class DepartamentosController < ApplicationController
  # GET /departamentos

  def index
    @departamentos = @empresa.departamentos.find(:all)
  end

  # GET /departamentos/1
  # GET /departamentos/1.xml
  def show
    @departamento = @empresa.departamentos.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @departamento }
    end
  end

  # GET /departamentos/new
  # GET /departamentos/new.xml
  def new
    @departamento = @empresa.departamentos.build

    respond_to do |format|
      format.html # new.html.erb
      format.xml  { render :xml => @departamento }
    end
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
  # PUT /departamentos/1.xml
  def update
    @departamento = @empresa.departamentos.find(params[:id])

    respond_to do |format|
      if @departamento.update_attributes(params[:departamento])
        flash[:notice] = 'Departamento was successfully updated.'
        format.html { redirect_to([@empresa, @departamento]) }
        format.xml  { head :ok }
      else
        format.html { render :action => "edit" }
        format.xml  { render :xml => @departamento.errors, :status => :unprocessable_entity }
      end
    end
  end

  # DELETE /departamentos/1
  # DELETE /departamentos/1.xml
  def destroy
    @departamento = @empresa.departamentos.find(params[:id])
    @departamento.destroy

    redirect_to(edit_empresa_path)
  end
end
