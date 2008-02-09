require File.dirname(__FILE__) + '/../test_helper'

class RequisicoesControllerTest < ActionController::TestCase
  def test_should_get_index
    get :index
    assert_response :success
    assert_not_nil assigns(:requisicoes)
  end

  def test_should_get_new
    get :new
    assert_response :success
  end

  def test_should_create_requisicao
    assert_difference('Requisicao.count') do
      post :create, :requisicao => { }
    end

    assert_redirected_to requisicao_path(assigns(:requisicao))
  end

  def test_should_show_requisicao
    get :show, :id => requisicoes(:one).id
    assert_response :success
  end

  def test_should_get_edit
    get :edit, :id => requisicoes(:one).id
    assert_response :success
  end

  def test_should_update_requisicao
    put :update, :id => requisicoes(:one).id, :requisicao => { }
    assert_redirected_to requisicao_path(assigns(:requisicao))
  end

  def test_should_destroy_requisicao
    assert_difference('Requisicao.count', -1) do
      delete :destroy, :id => requisicoes(:one).id
    end

    assert_redirected_to requisicoes_path
  end
end
