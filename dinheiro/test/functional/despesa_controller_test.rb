require File.dirname(__FILE__) + '/../test_helper'
require 'despesa_controller'

# Re-raise errors caught by the controller.
class DespesaController; def rescue_action(e) raise e end; end

class DespesaControllerTest < Test::Unit::TestCase
  def setup
    @controller = DespesaController.new
    @request    = ActionController::TestRequest.new
    @response   = ActionController::TestResponse.new
  end

  # Replace this with your real tests.
  def test_truth
    assert true
  end
end
