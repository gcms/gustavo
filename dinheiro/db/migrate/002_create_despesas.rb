class CreateDespesas < ActiveRecord::Migration
  def self.up
    create_table :despesas do |t|
      t.column :nome, :string
      t.column :descricao, :string
      t.column :vendedor_id, :integer, :references => :vendedor
      t.column :valor, :real, :null => false
      t.column :data, :date, :null => false
    end
  end

  def self.down
    drop_table :despesas
  end
end
