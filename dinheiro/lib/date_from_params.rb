class Date
  def self.from_params(params)
    Time.utc(params[:year], params[:month], params[:day]).to_date
  end
end
