class Requisicao < ActiveRecord::Base
  has_many :itens, :dependent => :destroy
  has_many :eventos

  belongs_to :unidade
  belongs_to :usuario
  belongs_to :empresa
  belongs_to :departamento

  ESTADOS = {
    :pendente => 'Pendente',
    :confirmado => 'Confirmado',
    :enviado => 'Enviado',
    :recebido => 'Recebido'
  }

  # valores são criados em before_create(), apos a validação,
  # por isso essa  validação deve ocorrer apenas no update
  validates_presence_of :pedido, :data, :estado, :on => :update

  # valor boolean
  validates_inclusion_of :emergencial, :transporte, :in => [ true, false ]

  # quando definido transporte a unidade deve ser definida
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
    self.pedido = data.strftime('%d%m%Y%H%M%S') \
        + empresa.codigo + usuario.codigo
    self.estado = :pendente

    eventos.build(:tipo => :criacao, :data => data,
                  :usuario => usuario)
  end

  def pendente?
    estado == :pendente
  end

  def confirmar(usuario)
    if pendente?
      Requisicao.transaction do
        self.estado = :confirmado
        e = eventos.build(:tipo => :confirmacao, :data => Time.now,
                          :usuario => usuario)
        e.save && save
      end
    else
      confirmado?
    end
  end

  def confirmado?
    estado == :confirmado
  end

  def resumo_qtd
    itens.group_by { |item| item.material }.collect do |material, item_array|
      [ material, item_array.sum { |item| item.qtd } ]
    end.sort_by { |pair| pair.first.id } #ordena pelo material_id
  end

  def method_missing(name, *params, &block)
    eventos.find_by_tipo(name) || super
  end
end
