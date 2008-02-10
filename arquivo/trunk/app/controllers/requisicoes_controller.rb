class RequisicoesController < ApplicationController
  # GET /requisicoes
  def index
    @requisicoes = @empresa.requisicoes.find(:all,
      :conditions => { :estado => :pendente })
  end

  # GET /requisicoes/new
  def new
    @requisicao = @empresa.requisicoes.build
  end

  # GET /requisicoes/1/edit
  def edit
    @requisicao = @empresa.requisicoes.find(params[:id])
  end

  # POST /requisicoes
  def create
    @requisicao = @empresa.requisicoes.build(params[:requisicao])
    @requisicao.usuario = @usuario

    if @requisicao.save
      flash[:notice] = 'Requisicão criada.'
      redirect_to(empresa_requisicao_itens_path(@requisicao))
    else
      logger.info @requisicao.errors.inspect
      logger.info "estado is '#{@requisicao.estado}'"
      render :action => :new
    end
  end

  # GET /requisicoes/1
  def show
    @requisicao = @empresa.requisicoes.find(params[:id])
  end

  # PUT /requisicoes/1
  def update
    @requisicao = @empresa.requisicoes.find(params[:id])

    if @requisicao.update_attributes(params[:requisicao])
      flash[:notice] = 'Requisicao was successfully updated.'
      redirect_to([:edit, :empresa, @requisicao])
    else
      render :action => "edit"
    end
  end

  # DELETE /requisicoes/1
  def destroy
    requisicao = @empresa.requisicoes.find(params[:id])
    requisicao.destroy

    redirect_to(empresa_requisicoes_url)
  end

  def confirm
    @requisicao = @empresa.requisicoes.find(params[:id])

    @requisicao.estado = :confirmado

    if @requisicao.save
      redirect_to :action => :ordem, :id => @requisicao
    else
      render :action => :show
    end
  end

  def ordem
    @requisicao = @empresa.requisicoes.find(params[:id])

    # Verifica se pendente
    if @requisicao.pendente?
      redirect_to empresa_requisicao_path(@requisicao)
    end
  end

  def transporte_changed
    if request.xhr?

      render :update do |page|
        # check_box() default true value is '1'
        page['requisicao_unidade_id'].disabled = params[:transporte] != '1'
      end
    end
  end

  def get_endereco
    unless params[:id].blank?
      unidade = @empresa.unidades.find_by_id(params[:id])
      @endereco = unidade && unidade.endereco
    end

    render :partial => 'endereco'
  end

end
