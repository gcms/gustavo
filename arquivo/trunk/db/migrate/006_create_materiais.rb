class CreateMateriais < ActiveRecord::Migration
  def self.up
    create_table :materiais do |t|
      t.string :nome, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :materiais
  end
end
