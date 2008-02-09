class Requisicao < ActiveRecord::Base
  has_many :itens, :dependent => :destroy
  belongs_to :endereco
  belongs_to :unidade
  belongs_to :usuario
  belongs_to :empresa
  belongs_to :departamento

  ESTADOS = {
    :novo => 'Novo',
    :pendente => 'Pendente',
    :confirmado => 'Confirmado',
    :enviado => 'Enviado',
    :recebido => 'Recebido'
  }

  symbolize :estado
  validates_inclusion_of :estado, :in => ESTADOS.keys
  validates_uniqueness_of :pedido

  def before_validation_on_create
    self.data = Time.now
    self.pedido = empresa.codigo + usuario.codigo \
      + data.strftime('%d%m%Y%H%M%S')
    self.estado = :pendente
  end

#  def add_item(novo_item)
#    itens.each do |item|
#      if item.material == novo_item.material
#        item.qtd += novo_item.qtd
#        return
#      end
#    end
#
#    itens << novo_item
#  end
end
