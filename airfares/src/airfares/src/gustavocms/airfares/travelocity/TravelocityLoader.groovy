package gustavocms.airfares.travelocity

import gustavocms.airfares.cache.Cache
import gustavocms.airfares.impl.CachedJsonLoader

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 21:58
 * To change this template use File | Settings | File Templates.
 */
class TravelocityLoader extends CachedJsonLoader {
    TravelocityLoader() {
        super(Cache.DEFAULT, 'travelocity')
    }
}
