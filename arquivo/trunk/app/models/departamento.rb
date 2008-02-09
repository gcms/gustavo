class Departamento < ActiveRecord::Base
  belongs_to :empresa

  validates_presence_of :nome, :empresa_id
  validates_uniqueness_of :nome, :scope => :empresa_id
end
