module Admin::RequisicoesHelper
  def empresa_options
    options_for_select([['Todas', nil]]) + options_from_collection_for_select(Empresa.find(:all), :id, :nome)
  end

  def options_for_select_from_collection(collection, value_method, text_method,
                                         options = {})
    blank_option(options[:include_blank]) \
        + options_from_collection_for_select(collection,
                                             value_method, text_method,
                                             options[:selected])
  end

  private
  def blank_option(prompt)
    prompt ? options_for_select([[prompt, nil]]) : ''
  end
end
