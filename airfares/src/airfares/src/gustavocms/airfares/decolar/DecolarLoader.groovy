package gustavocms.airfares.decolar

import gustavocms.airfares.cache.Cache
import gustavocms.airfares.impl.CachedJsonLoader

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 21:58
 * To change this template use File | Settings | File Templates.
 */
class DecolarLoader extends CachedJsonLoader {
    public DecolarLoader() {
        super(Cache.DEFAULT, 'decolar')
    }
}
