ActionController::Routing::Routes.draw do |map|
  map.root :controller => 'home'

#  map.resource :empresa, :has_many => :requisicoes

#  map.resource :empresa, :has_many => :usuarios
#  map.resource :empresa, :has_many => :unidades
#  map.resource :empresa, :has_many => :departamentos

  map.resource :empresa do |empresa|
    empresa.resources :usuarios
    empresa.resources :unidades
    empresa.resources :departamentos

    empresa.resources :requisicoes,
        :member => { :confirm => :post, :ordem => :get } do |requisicao|
      requisicao.resources :itens
    end
  end


#  map.resources :empresas, :has_many => :usuarios
#  map.resources :empresas, :has_many => :departamentos
#  map.resources :empresas, :has_many => :unidades
#  map.resources :empresas, :has_many => :requisicoes

#  map.resources :requisicoes, :has_many => :itens

#  map.resources :requisicoes, :has_many => :itens

#  map.resources :usuario

  # The priority is based upon order of creation: first created -> highest priority.

  # Sample of regular route:
  #   map.connect 'products/:id', :controller => 'catalog', :action => 'view'
  # Keep in mind you can assign values other than :controller and :action

  # Sample of named route:
  #   map.purchase 'products/:id/purchase', :controller => 'catalog', :action => 'purchase'
  # This route can be invoked with purchase_url(:id => product.id)

  # Sample resource route (maps HTTP verbs to controller actions automatically):
  #   map.resources :products

  # Sample resource route with options:
  #   map.resources :products, :member => { :short => :get, :toggle => :post }, :collection => { :sold => :get }

  # Sample resource route with sub-resources:
  #   map.resources :products, :has_many => [ :comments, :sales ], :has_one => :seller

  # Sample resource route within a namespace:
  #   map.namespace :admin do |admin|
  #     # Directs /admin/products/* to Admin::ProductsController (app/controllers/admin/products_controller.rb)
  #     admin.resources :products
  #   end

  # You can have the root of your site routed with map.root -- just remember to delete public/index.html.
  # map.root :controller => "welcome"

  # See how all your routes lay out with "rake routes"

  # Install the default routes as the lowest priority.
  map.connect ':controller/:action/:id'
  map.connect ':controller/:action/:id.:format'
end
