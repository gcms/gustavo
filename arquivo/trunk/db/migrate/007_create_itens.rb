class CreateItens < ActiveRecord::Migration
  def self.up
    create_table :itens do |t|
      t.integer :qtd, :null => false
      t.references :material, :null => false
      t.references :requisicao, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :itens
  end
end
