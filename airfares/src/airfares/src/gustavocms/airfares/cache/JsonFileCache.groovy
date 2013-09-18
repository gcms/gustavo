package gustavocms.airfares.cache

import groovy.json.JsonOutput
import groovy.json.JsonSlurper

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 10/09/13
 * Time: 11:04
 * To change this template use File | Settings | File Templates.
 */
class JsonFileCache extends FileCache {
    static public JsonFileCache INSTANCE = new JsonFileCache('filecache')

    JsonFileCache(String dirname) {
        super(dirname)
    }

    Object unmarshall(File file) {
        try {
            String content = readFile(file)
            JsonSlurper slurper = new JsonSlurper()
            return slurper.parseText(content).conteudo
        } catch (Exception ignored) {
            file.delete()
            return null
        }
    }

    void marshall(File file, Object object) {
        writeFile(file, JsonOutput.toJson([conteudo: object]))
    }
}
