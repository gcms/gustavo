dataSource {
    pooled = true
    driverClassName = "org.hsqldb.jdbcDriver"
    username = "sa"
    password = ""
}
hibernate {
    cache.use_second_level_cache = true
    cache.use_query_cache = true
    cache.provider_class = 'net.sf.ehcache.hibernate.EhCacheProvider'
}
// environment specific settings
environments {
    development {
        dataSource {
//            dbCreate = "create-drop" // one of 'create', 'create-drop','update'
//            url = "jdbc:hsqldb:mem:devDB"

            dbCreate = 'update'
            driverClassName = "org.postgresql.Driver"
            url = 'jdbc:postgresql://localhost/ambulancia'
            username = 'ambulancia'
            password = 'ambulancia'
        }
    }
    test {
        dataSource {
            dbCreate = "create-drop"
            url = "jdbc:hsqldb:mem:testDb"
        }
    }
    production {
        dataSource {
            dbCreate = "update"
//            url = "jdbc:hsqldb:file:prodDb;shutdown=true"

//            driverClassName = "org.postgresql.Driver"
//            url = 'jdbc:postgresql://localhost/ambulancia'
//            username = 'ambulancia'
//            password = 'ambulancia'
            jndiName = "java:/comp/env/jdbc/ambulancia"
        }
    }
}
