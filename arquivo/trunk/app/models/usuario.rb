class Usuario < ActiveRecord::Base
  belongs_to :empresa

  NIVEIS = { :admin => 'Administração', :user => 'Usuário'}

  symbolize :nivel
  validates_inclusion_of :nivel, :in => NIVEIS.keys + [ :super_admin ]

  validates_presence_of :nome, :senha
  validates_uniqueness_of :nome

  validates_presence_of :empresa_id, :if => Proc.new { |u| !u.super_admin? }

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

  def super_admin?
    nivel == :super_admin
  end

  def nivel_s
    NIVEIS[nivel]
  end

  def codigo
    sprintf("%0.4d", id)
  end
end
