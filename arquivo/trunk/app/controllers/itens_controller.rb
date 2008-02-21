class ItensController < ApplicationController
  before_filter :load_requisicao

  def index
    @itens = @requisicao.itens.find(:all)
    @item ||= Item.new

    render :action => "index" # necessário, pois é chamado em create()
  end

  def create
    @item = @requisicao.itens.build(params[:item])

    if @item.save
      flash[:notice] = 'Item adicionado.'
      redirect_to empresa_requisicao_itens_path(@requisicao)
    else
      index
    end
  end

  def destroy
    @item = @requisicao.itens.find(params[:id])
    @item.destroy

    redirect_to(empresa_requisicao_itens_path(@requisicao))
  end

  private
  def load_requisicao
    @requisicao = @empresa.requisicoes.find(params[:requisicao_id])
  end
end
