require File.dirname(__FILE__) + '/../test_helper'

class UnidadeTest < ActiveSupport::TestCase
  fixtures :empresas

  # Replace this with your real tests.
  def test_truth
    assert true
  end

  def test_unidade_endereco_create
    assert Unidade.count == 0
    assert Endereco.count == 0

    u = empresas(:one).unidades.build(:nome => 'Unidade 1')
    assert !u.save

    assert Unidade.count == 0
    assert Endereco.count == 0

    u.endereco = Endereco.new
    assert !u.save

    assert Unidade.count == 0
    assert Endereco.count == 0

    u.nome = nil
    u.endereco.logradouro = 'logradouro'
    assert !u.save

    assert Unidade.count == 0
    assert Endereco.count == 0

    u.nome = 'Unidade 3'
    assert u.save

    assert Unidade.count == 1
    assert Endereco.count == 1

    u.destroy

    assert Unidade.count == 0
    assert Endereco.count == 0
  end

  def test_unidade_endereco_update
    u = empresas(:one).unidades.build(:nome => 'Unidade 1')
    u.endereco = Endereco.new(:logradouro => 'logradouro')
    assert u.save

    u2 = Unidade.find_by_nome('Unidade 1')
    assert u.id == u2.id
    assert u.endereco.id = u2.endereco.id

    u2.endereco.logradouro = 'log'
    assert u2.save

    u3 = Unidade.find_by_nome('Unidade 1')
    assert u3.id == u2.id

    assert u3.endereco.logradouro == 'log'

    u3.destroy

    assert !Unidade.find_by_nome('Unidade 1')

  end
end
