module RequisicoesHelper
  def unidade_vazia
    [ Unidade.new(:nome => "", :id => 0) ]
  end
end
