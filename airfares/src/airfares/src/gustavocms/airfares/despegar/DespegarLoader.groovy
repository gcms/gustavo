package gustavocms.airfares.despegar

import gustavocms.airfares.cache.Cache
import gustavocms.airfares.impl.CachedJsonLoader

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 21:58
 * To change this template use File | Settings | File Templates.
 */
class DespegarLoader extends CachedJsonLoader {
    public DespegarLoader() {
        super(Cache.DEFAULT, 'decolar')
        Cache.DEFAULT.expireTime = 1000 * 60 * 30
    }
}
