package gustavocms.airfares.cache

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 03/09/13
 * Time: 07:41
 * To change this template use File | Settings | File Templates.
 */
class CacheEntry extends LinkedHashMap implements Comparable<CacheEntry> {
    @Override
    int compareTo(CacheEntry o) {
//        println "\${Thread.currentThread()}compareTo(${this.entrySet()}, ${o?.entrySet()})".replaceAll('\\s+', ' ')
        println get('loadTime')
        println "${Thread.currentThread()} ${o}"
        get('loadTime') - o.get('loadTime')
    }
}
