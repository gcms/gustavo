class CreateEmpresas < ActiveRecord::Migration
  def self.up
    create_table :empresas do |t|
      t.string :nome, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :empresas
  end
end
