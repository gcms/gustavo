module DespesaHelper
  def data_valor_tag(despesa)
    "#{data_tag(despesa.data)} #{valor_tag(despesa.valor)}"
  end

  def data_tag(data)
    data.strftime("%d/%m/%Y")
  end

  def valor_tag(valor)
    number_to_currency(valor, :unit => 'R$')
  end
end
