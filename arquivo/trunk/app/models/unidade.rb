class Unidade < ActiveRecord::Base
  belongs_to :empresa

  belongs_to :endereco

  # A idéia da validação é verificar  a presença de um endereço
  # associado, mesmo que não esteja persistido, a persistencia é feita
  # logo após a validação em save_endereco()
#  validates_associated :endereco
  validates_presence_of :endereco

  validates_presence_of :nome, :empresa_id
  validates_uniqueness_of :nome, :scope => :empresa_id

  before_save :save_endereco
  after_destroy :destroy_endereco

  protected
  def save_endereco
    endereco && endereco.save
  end

  def destroy_endereco
    endereco.destroy
  end

end
