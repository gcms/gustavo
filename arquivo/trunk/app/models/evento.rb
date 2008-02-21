class Evento < ActiveRecord::Base
  belongs_to :requisicao
  belongs_to :usuario

  validates_presence_of :tipo, :data, :usuario_id, :requisicao_id
  symbolize :tipo
end
