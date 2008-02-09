class Empresa < ActiveRecord::Base
  has_many :usuarios
  has_many :departamentos
  has_many :unidades

  has_many :requisicoes

  validates_presence_of :nome
  validates_uniqueness_of :nome

  def codigo
    sprintf("%0.3d", id)
  end
end
