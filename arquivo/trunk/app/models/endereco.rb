class Endereco < ActiveRecord::Base
  validates_presence_of :logradouro
end
