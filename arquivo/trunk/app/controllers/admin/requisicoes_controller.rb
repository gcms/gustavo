class Admin::RequisicoesController < ApplicationController
  def index
    @empresa_id = params[:empresa_id].to_i
    @emergencial = params[:emergencial]

    conditions = build_conditions(params).merge({ :estado => :confirmado })

    @requisicoes = Requisicao.find(:all, :conditions => conditions,
                                   :order => 'data ASC')
  end

  def show
    @requisicao = Requisicao.find(params[:id])
  end

  private

  def build_conditions(params)
    # obtém um hash com os valores de :empresa_id e :emergencial, se passados
    params.delete_if do |k, v|
      v.blank? || ![:empresa_id, :emergencial].member?(k.to_sym)
    end
  end
end
