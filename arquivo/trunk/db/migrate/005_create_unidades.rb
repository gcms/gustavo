class CreateUnidades < ActiveRecord::Migration
  def self.up
    create_table :unidades do |t|
      t.string :nome
      t.references :empresa
      t.references :endereco

      t.timestamps
    end
  end

  def self.down
    drop_table :unidades
  end
end
