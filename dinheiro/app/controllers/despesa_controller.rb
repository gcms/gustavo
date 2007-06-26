require_dependency 'sparklines'

class DespesaController < ApplicationController
  helper :sparklines
  
  def index
    today = Time.now.beginning_of_month.to_date
    @despesas = Despesa.find(:all, :order => 'data DESC, id DESC',
                             :conditions => ['data >= ?', today.to_s(:db)])
    @despesa = Despesa.new(:data => Date.today)
    @vendedores = Vendedor.find(:all, :order => 'nome ASC')

    @inicio = Date.today
    @fim = Date.today

    session[:despesas] = @despesas
  end


  require 'date_from_params'
  def filter
    @inicio = Date.from_params(params[:inicio])
    @fim = Date.from_params(params[:fim])

    @despesas = Despesa.find(:all,
                             :conditions => [ 'data >= ? AND data <= ?',
                                              @inicio.to_s(:db), @fim.to_s(:db)],
                             :order => 'data DESC, id DESC')

    session[:despesas] = @despesas
  end

  def despesa
    @despesa = Despesa.find(params[:id])
  end

  def vendedor
  end

  def despesa_nova
    if not params[:novo_vendedor].blank?
      vendedor = Vendedor.new(:nome => params[:novo_vendedor])
      if vendedor.save
        index
        @despesa = Despesa.new(params[:despesa])
        @despesa.vendedor = vendedor
      end

      render :action => :index
    else
      params[:despesa][:data] = string_to_date(params[:despesa][:data])
      @despesa = Despesa.new(params[:despesa])
      params[:tags].split(/\s*,\s*/).collect {|s| s.downcase}.each do |tag_nome|
        tag = Tag.find_by_nome(tag_nome)
        if tag.nil?
          tag = Tag.new(:nome => tag_nome)
          tag.save
        end

        @despesa.tags << tag
      end

      if @despesa.save
        flash[:notice] = 'Adicionado'
        redirect_to :action => :index
      else
        render :action => :index
      end
    end
  end

  def tag
    @tag = Tag.find(params[:id])
    @valor = valor(@tag)
    @valor_total = valor_total
    @percentual = sprintf("%.2f %", @valor / @valor_total * 100.00)

    @primeiros = @tag.despesas.group_by {|d| d.vendedor}.sort {|a, b|
      b[1].length <=> a[1].length
    }.collect {|k, v| [k, v.length]}.first(3)

    @primeiros_por_custo = @tag.despesas.group_by {|d| d.vendedor}.sort {|a, b|
      b[1].sum {|d| d.valor.to_d} <=> a[1].sum {|d| d.valor.to_d}
    }.collect {|k, v| [k, v.sum {|d| d.valor.to_d}]}.first(3)

  end

  def tag_modificar
    tag = Tag.find(params[:id])

    if tag.update_attributes(params[:tag])
      flash[:notice] = 'Modificada.'
      redirect_to :action => :tag, :id => tag
    else
      raise "Não foi possível modificar."
    end
  end

  def despesa_remover
    if request.post?
      despesa = Despesa.find(params[:id])
      despesa.destroy
      redirect_to :action => :index
    end
  end

  private


  def string_to_date(string)
    dateArray = string.match(/(\d{2})\/(\d{2})\/(\d{4})/) \
    .to_a[1, 3].collect {|s| s.to_i}.reverse
    Date.civil(*dateArray)
  end


  def valor(tag)
    valor_despesas(session[:despesas].select {|d| d.tags.member?(tag)} )
  end

  def valor_total
    valor_despesas(session[:despesas])
#      valor(Despesa.find(:all))
  end

  def valor_despesas(despesas)
    despesas.collect {|despesa| despesa.valor}.sum {|valor| valor.to_d}
  end
end
