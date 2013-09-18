package gustavocms.airfares.impl

import groovy.json.JsonSlurper
import gustavocms.airfares.cache.Cache
import gustavocms.airfares.util.JsonLoader
import org.apache.http.HttpEntity
import org.apache.http.HttpResponse
import org.apache.http.client.HttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.client.methods.HttpUriRequest
import org.apache.http.impl.client.HttpClients

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 16/09/13
 * Time: 12:19
 * To change this template use File | Settings | File Templates.
 */
class CachedJsonLoader implements JsonLoader {
    private Cache cache
    private String key
    private JsonSlurper slurper = new JsonSlurper()

    public CachedJsonLoader(Cache cache, String key) {
        this.cache = cache
        this.key = key
    }

    public Map getMap(URL url) {
        getMap(new HttpGet(url.toURI()))
    }

    Map getMap(HttpUriRequest request) {
//        println "-- getting map for ${request.requestLine.uri}"
        String key = getKey(request.requestLine.uri)
        getFromCache(key) ?: getFromRequest(key, request)
    }

    public String getKey(String url) {
        key + new Date().format('yyyyMMdd') + url
    }

    private Map getFromCache(String key) {
        try {
            Map result = cache.get(key)
            return result && result.key == key ? result.content : null
        } catch (Exception ignored) {
            return null
        }
    }

    private Map getFromRequest(String key, HttpUriRequest request) {
//        println "-- Retrieving ${url}"
        try {
            String content = fetch(request) //url.newReader().text
            print "#"

            Map result = slurper.parseText(content) as Map
            cache.put(key, [key: key, content: result])

            return result
        } catch (Exception ignored) {
            return null
        }
    }

    private static String fetch(HttpUriRequest request) {
        HttpClient client = HttpClients.createDefault()
        HttpResponse response = client.execute(request)
        HttpEntity entity = response.entity
        entity.content.newReader('UTF-8').text
    }
}
