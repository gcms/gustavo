class CreateVendedores < ActiveRecord::Migration
  def self.up
    create_table :vendedores do |t|
      t.column :nome, :string, :null => false
      t.column :descricao, :string
      t.column :endereco, :string
    end
  end

  def self.down
    drop_table :vendedores
  end
end
