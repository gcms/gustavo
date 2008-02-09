class Unidade < ActiveRecord::Base
  belongs_to :empresa

  belongs_to :endereco

  validates_presence_of :nome

  validates_uniqueness_of :nome, :scope => :empresa_id

end
