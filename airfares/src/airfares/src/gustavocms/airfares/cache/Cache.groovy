package gustavocms.airfares.cache
/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 04/09/13
 * Time: 15:17
 * To change this template use File | Settings | File Templates.
 */
public interface Cache {
    public static final DEFAULT = JsonFileCache.INSTANCE

    public Object get(String key)
    public void put(String key, Object map)
    public boolean remove(String key)
    void flush()

    void close()
}