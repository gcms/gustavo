# This file is auto-generated from the current state of the database. Instead of editing this file, 
# please use the migrations feature of ActiveRecord to incrementally modify your database, and
# then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your database schema. If you need
# to create the application database on another system, you should be using db:schema:load, not running
# all the migrations from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended to check this file into your version control system.

ActiveRecord::Schema.define(:version => 8) do

  create_table "departamentos", :force => true do |t|
    t.string   "nome",       :null => false
    t.integer  "empresa_id", :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "empresas", :force => true do |t|
    t.string   "nome"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "enderecos", :force => true do |t|
    t.text     "logradouro", :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "itens", :force => true do |t|
    t.integer  "qtd",           :null => false
    t.integer  "material_id",   :null => false
    t.integer  "requisicao_id", :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "materiais", :force => true do |t|
    t.string   "nome",       :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "requisicoes", :force => true do |t|
    t.string   "pedido",          :null => false
    t.datetime "data",            :null => false
    t.string   "estado",          :null => false
    t.boolean  "emergencial",     :null => false
    t.boolean  "transporte",      :null => false
    t.integer  "usuario_id",      :null => false
    t.integer  "empresa_id",      :null => false
    t.integer  "departamento_id"
    t.integer  "endereco_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "sessions", :force => true do |t|
    t.string   "session_id", :null => false
    t.text     "data"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "sessions", ["updated_at"], :name => "index_sessions_on_updated_at"
  add_index "sessions", ["session_id"], :name => "index_sessions_on_session_id"

  create_table "unidades", :force => true do |t|
    t.string   "nome"
    t.integer  "empresa_id"
    t.integer  "endereco_id"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "usuarios", :force => true do |t|
    t.string   "nome",       :null => false
    t.string   "senha",      :null => false
    t.integer  "empresa_id", :null => false
    t.string   "nivel",      :null => false
    t.datetime "created_at"
    t.datetime "updated_at"
  end

end
