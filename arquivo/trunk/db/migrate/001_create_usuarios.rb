class CreateUsuarios < ActiveRecord::Migration
  def self.up
    create_table :usuarios do |t|
      t.string :nome, :senha, :null => false
      t.references :empresa
      t.string :nivel, :null => false
      t.timestamps
    end
  end

  def self.down
    drop_table :usuarios
  end
end
