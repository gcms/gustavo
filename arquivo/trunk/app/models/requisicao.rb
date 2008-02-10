class Requisicao < ActiveRecord::Base
  has_many :itens, :dependent => :destroy

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

  # criado em before_create, por isso validacao apensa no update
  validates_presence_of :pedido, :data, :estado, :on => :update
  validates_inclusion_of :emergencial, :transporte, :in => [ true, false ]
  validates_presence_of :unidade_id, :if => :transporte?
  validates_presence_of :usuario_id, :empresa_id


  symbolize :estado
  validates_inclusion_of :estado, :in => ESTADOS.keys, :on => :update

  validates_uniqueness_of :pedido
#  validates_format_of

  def validate_on_update
    if estado != :pendente && itens.empty?
      errors.add 'itens', 'não foram adicionados'
    end
  end

  def before_create
    self.data = Time.now
    self.pedido = empresa.codigo + usuario.codigo \
      + data.strftime('%d%m%Y%H%M%S')
    self.estado = :pendente

    logger.info "self.estado = #{self.estado}"
  end


  def pendente?
    estado == :pendente
  end

  def resumo_qtd
    itens.group_by { |item| item.material }.collect do |material, item_array|
      [ material, item_array.sum { |item| item.qtd } ]
    end.sort { |a, b| a[0].id <=> b[0].id } #ordena pelo material_id
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
