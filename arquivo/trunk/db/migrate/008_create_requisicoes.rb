class CreateRequisicoes < ActiveRecord::Migration
  def self.up
    create_table :requisicoes do |t|
      t.string :pedido, :null => false
      t.datetime :data, :null => false

      t.string :estado, :null => false

      t.boolean :emergencial, :null => false
      t.boolean :transporte, :null => false

      t.references :unidade
      t.references :usuario, :null => false
      t.references :empresa, :null => false
      t.references :departamento

      t.timestamps
    end
  end

  def self.down
    drop_table :requisicoes
  end
end
