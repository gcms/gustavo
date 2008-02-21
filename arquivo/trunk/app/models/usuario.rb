class Usuario < ActiveRecord::Base
  belongs_to :empresa

  NIVEIS = { :admin => 'Administração', :user => 'Usuário'}

  symbolize :nivel
  validates_inclusion_of :nivel, :in => NIVEIS.keys

  validates_presence_of :nome, :senha
  validates_uniqueness_of :nome

  def self.autenticar(nome, senha)
    usuario = find_by_nome(nome)

    if usuario && usuario.senha == senha
      usuario
    else
      nil
    end
  end

  def admin?
    nivel == :admin
  end

  def nivel_s
    NIVEIS[nivel]
  end

  def codigo
    sprintf("%0.4d", id)
  end
end
