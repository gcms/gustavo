class CreateDespesasTags < ActiveRecord::Migration
  def self.up
    create_table :despesas_tags do |t|
      t.column :despesa_id, :integer, :null => false, :references => :despesas
      t.column :tag_id, :integer, :null => false, :references => :tags
    end
  end

  def self.down
    drop_table :despesas_tags
  end
end
