package gustavocms.airfares.cache

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 10/09/13
 * Time: 08:50
 * To change this template use File | Settings | File Templates.
 */
class MemoryCache implements Cache {
    private Map cache = [:]
    @Override
    Object get(String key) {
        cache[key]
    }

    @Override
    void put(String key, Object map) {
        cache[key] = map
    }

    @Override
    void flush() {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    void close() {
        cache.clear()
    }
}
