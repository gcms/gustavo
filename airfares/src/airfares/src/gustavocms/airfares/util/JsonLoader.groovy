package gustavocms.airfares.util

import org.apache.http.client.methods.HttpUriRequest

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 16/09/13
 * Time: 12:49
 * To change this template use File | Settings | File Templates.
 */
public interface JsonLoader {
    Map getMap(URL url)
    Map getMap(HttpUriRequest request)
}