dataSource {
	pooled = true
	driverClassName = "org.hsqldb.jdbcDriver"
	username = "sa"
	password = ""
}
hibernate {
    cache.use_second_level_cache=true
    cache.use_query_cache=true
    cache.provider_class='net.sf.ehcache.hibernate.EhCacheProvider'
}
// environment specific settings
environments {
	development {
		dataSource {
            dbCreate = "update"
			url = "jdbc:hsqldb:file:devDb;shutdown=true"
            
//			dbCreate = "create-drop" // one of 'create', 'create-drop','update'
//			url = "jdbc:hsqldb:mem:devDB"
		}
	}
	test {
		dataSource {
			dbCreate = "update"
			url = "jdbc:hsqldb:mem:testDb"
		}
	}
	production {
		dataSource {
            driverClassName = "org.gjt.mm.mysql.Driver"
            dialect = "org.hibernate.dialect.MySQLDialect"
            url = "jdbc:mysql://localhost:3306/refman"
            dbCreate =  "update"
            username = "root"
            password = "donline"

		}
	}
}