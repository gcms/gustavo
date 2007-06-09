class Despesa < ActiveRecord::Base
  validates_presence_of :valor, :message => "não pode ser vazio"
  validates_presence_of :data, :message => "deve ser definida"

  belongs_to :vendedor
  has_and_belongs_to_many :tags
end
