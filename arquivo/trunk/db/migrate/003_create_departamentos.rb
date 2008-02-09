class CreateDepartamentos < ActiveRecord::Migration
  def self.up
    create_table :departamentos do |t|
      t.string :nome, :null => false
      t.references :empresa, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :departamentos
  end
end
