# This file is autogenerated. Instead of editing this file, please use the
# migrations feature of ActiveRecord to incrementally modify your database, and
# then regenerate this schema definition.

ActiveRecord::Schema.define(:version => 4) do

# Could not dump table "despesas" because of following StandardError
#   Unknown type 'real' for column 'valor'

  create_table "tags", :force => true do |t|
    t.column "nome",      :string, :null => false
    t.column "descricao", :string
  end

  create_table "vendedores", :force => true do |t|
    t.column "nome",      :string, :null => false
    t.column "descricao", :string
    t.column "endereco",  :string
  end

end
