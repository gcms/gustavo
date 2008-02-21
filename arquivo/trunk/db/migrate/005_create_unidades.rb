class CreateUnidades < ActiveRecord::Migration
  def self.up
    create_table :unidades do |t|
      t.string :nome, :null => false
      t.references :empresa, :null => false
      t.references :endereco, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :unidades
  end
end
