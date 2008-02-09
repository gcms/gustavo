require File.dirname(__FILE__) + '/../test_helper'

class UnidadesControllerTest < ActionController::TestCase
  def test_should_get_index
    get :index
    assert_response :success
    assert_not_nil assigns(:unidades)
  end

  def test_should_get_new
    get :new
    assert_response :success
  end

  def test_should_create_unidade
    assert_difference('Unidade.count') do
      post :create, :unidade => { }
    end

    assert_redirected_to unidade_path(assigns(:unidade))
  end

  def test_should_show_unidade
    get :show, :id => unidades(:one).id
    assert_response :success
  end

  def test_should_get_edit
    get :edit, :id => unidades(:one).id
    assert_response :success
  end

  def test_should_update_unidade
    put :update, :id => unidades(:one).id, :unidade => { }
    assert_redirected_to unidade_path(assigns(:unidade))
  end

  def test_should_destroy_unidade
    assert_difference('Unidade.count', -1) do
      delete :destroy, :id => unidades(:one).id
    end

    assert_redirected_to unidades_path
  end
end
