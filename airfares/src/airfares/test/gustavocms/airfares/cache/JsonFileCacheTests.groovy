package gustavocms.airfares.cache

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 22:08
 * To change this template use File | Settings | File Templates.
 */
class JsonFileCacheTests extends GroovyTestCase {
    public void testSaveAndLoad() {
        File dir = new File('testdir')

        if (dir.exists()) {
            dir.listFiles().each { it.delete() }
            dir.deleteDir()
        }

        dir.mkdir()

        JsonFileCache cache = new JsonFileCache('testdir')
        cache.put('chave1', 'valor1')
        assertEquals('valor1', cache.get('chave1'))

        cache.put('chave2', 'valor2')
        assertEquals('valor2', cache.get('chave2'))

        cache.put('chave3', 'valor3')
        assertEquals('valor3', cache.get('chave3'))

        cache.put('chave1', 'valor1')
        assertEquals('valor1', cache.get('chave1'))

        List threads = []
        for (int i = 0; i < 50; i++) {
            final int v = i;
            threads << Thread.start {
                cache.put(v.toString(), [key: v, value: v.toString()])
            }
        }

        threads.each { it.join() }
//        Thread.sleep(1000*5)

        cache.put('mapa', [inteiro: 1, texto: 'texto'])

        cache.flush()
        cache.close()
        
        
        JsonFileCache cache5 = new JsonFileCache('testdir')

        cache5.put('chave1', 'valor1')
        assertEquals('valor1', cache5.get('chave1'))

        cache5.put('chave2', 'valor2')
        assertEquals('valor2', cache5.get('chave2'))

        cache5.put('chave3', 'valor3')
        assertEquals('valor3', cache5.get('chave3'))

        cache5.put('chave1', 'valor1')
        assertEquals('valor1', cache5.get('chave1'))

        for (int i = 0; i < 50; i++) {
            assertEquals(i, cache.get(i.toString()).key)
            assertEquals(i.toString(), cache.get(i.toString()).value.toString())
            cache.put(i.toString(), [key: i, value: i])
        }

        assertEquals 1, cache.get('mapa').inteiro
        assertEquals 'texto', cache.get('mapa').texto
    }
}
