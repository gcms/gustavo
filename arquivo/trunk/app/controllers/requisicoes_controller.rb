class RequisicoesController < ApplicationController
  # GET /requisicoes
  def index
    @requisicoes = @empresa.requisicoes.find(:all)
  end

  # GET /requisicoes/new

  # Cria novo objeto não salvo, para ser utilizado por edit_new
  def new
    requisicao = @empresa.requisicoes.build
    requisicao.endereco = Endereco.new
    requisicao.usuario = @usuario

    session[:requisicao] = requisicao
    redirect_to :action => :edit_new
  end

  # GET /requisicoes/1/edit

  # Carrega um objeto do banco para ser utilizado por edit_new
  def edit
    requisicao = @empresa.requisicoes.find(params[:id])

    session[:requisicao] = requisicao
    redirect_to :action => :edit_new
  end


  def edit_new
    # Coloca o objeto na sessão pronto para ser editado

    @requisicao = session[:requisicao]
    if @requisicao
      @endereco = @requisicao.endereco
    else
      redirect_to :action => :new
    end
  end

  # POST /requisicoes
  def create
    # Salva o objeto da sessão, utilizado por edit_new para
    # criar uma nova requisição
    requisicao = session[:requisicao]

    # estudar transacoes
    Requisicao.transaction do
      if requisicao.endereco.update_attributes(params[:endereco]) \
          && requisicao.update_attributes(params[:requisicao])
        flash[:notice] = 'Requisicao was successfully created.'
        session[:requisicao] = nil
        redirect_to(empresa_requisicoes_path)
      else
        redirect_to :action => :edit_new
      end
    end
  end

  # GET /requisicoes/1
  def show
    @requisicao = @empresa.requisicoes.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.xml  { render :xml => @requisicao }
    end
  end

  # PUT /requisicoes/1
  def update
    @requisicao = Requisicao.find(params[:id])

    if @requisicao.update_attributes(params[:requisicao])
      flash[:notice] = 'Requisicao was successfully updated.'
      redirect_to([:empresa, @requisicao])
    else
      render :action => "edit"
    end
  end

  # DELETE /requisicoes/1
  # DELETE /requisicoes/1.xml
  def destroy
    requisicao = @empresa.requisicoes.find(params[:id])
    requisicao.destroy

    redirect_to(empresa_requisicoes_url)
  end

  def get_endereco
    unless params[:id].blank?
      unidade = Unidade.find_by_id(params[:id])
      if unidade && unidade.empresa == @empresa
        @endereco = unidade.endereco
      end
    end

    render :partial => 'endereco'
  end

  def add_item
    @requisicao = session[:requisicao]

    if @requisicao
      logger.info @requisicao.itens(false).size

#      item = @requisicao.itens(false).build(params[:item])
#      item.requisicao = @requisicao

      item = Item.new(params[:item])
      @requisicao.itens(false) << item
      item.requisicao = @requisicao

      logger.info @requisicao.itens(false).size

      render :update do |page|
        page.replace_html 'itens-div', :partial => 'itens'
      end
    end
  end

  def delete_item
    @requisicao = session[:requisicao]

    if @requisicao && (item = @requisicao.itens[params[:id].to_i])
      @requisicao.itens.delete item

      render :update do |page|
        page.replace_html 'itens-div', :partial => 'itens'
      end
    end
  end

  def confirm
    @requisicao = @empresa.requisicoes.find(params[:id])
  end
end
