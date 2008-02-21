class CreateEventos < ActiveRecord::Migration
  def self.up
    create_table :eventos do |t|
      t.string :tipo, :null => false
      t.datetime :data, :null => false
      t.references :usuario, :null => false
      t.references :requisicao, :null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :eventos
  end
end
