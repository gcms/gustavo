class UrlMappings {

	static mappings = {
		"/$controller/$action?/$id?"{
			constraints {
				// apply constraints here
			}
		}

//		"/"(view:"/index")
		"/"(controller: 'viagem', action: 'home')        
		"500"(view:'/error')
	}
}
