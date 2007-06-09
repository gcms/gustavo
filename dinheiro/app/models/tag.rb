class Tag < ActiveRecord::Base
  validates_presence_of :nome
  validates_uniqueness_of :nome

  has_and_belongs_to_many :despesas
end
